#include "___Xml.h"
#include <TongYiOpenAi.hpp>
#include <QThread>


void __Xml::__init(QString httpUrl, QString token)
{
	TongYiOpenAi::__init(httpUrl.toStdString(), token.toStdString());
}

//using Json = nlohmann::json;
bool __Xml::__read(QString filepath)
{
	//__Xml xml;
	Context context;
	Message message;
	Location location;

	if (filepath.isEmpty())
	{
		return false;
	}

	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly))
	{
		//return xml;
		return false;
	}

	QXmlStreamReader reader(&file);
	while (!reader.atEnd())
	{
		reader.readNext();
		if (reader.isStartElement())
		{
			QString tagName = reader.name().toString();
			if (tagName == "TS")
			{
				QXmlStreamAttributes attr = reader.attributes();
				if (attr.hasAttribute("version"))
				{
					__version = attr.value("version").toString();
				}
				continue;
			}

			if (tagName == "context")
			{
				//context.clear();
				continue;
			}

			if (tagName == "name")
			{
				context.name = reader.readElementText();
				continue;
			}

			if (tagName == "message")
			{
				//message.clear();
				continue;
			}

			if (tagName == "location")
			{
				QXmlStreamAttributes attr = reader.attributes();


				if (attr.hasAttribute("filename"))
				{
					//message.location.filename = attr.value("filename").toString();
					location.filename = attr.value("filename").toString();
				}

				if (attr.hasAttribute("line"))
				{
					//message.location.line = attr.value("line").toString();
					location.line = attr.value("line").toString();
				}

				message.locations.push_back(location);
				continue;
			}

			if (tagName == "source")
			{
				message.source = reader.readElementText();
				continue;
			}

			if (tagName == "translation")
			{
				QXmlStreamAttributes attrs = reader.attributes();
				if (attrs.hasAttribute("type"))
				{
					message.translation.type = attrs.value("type").toString();
				}
				//message.translation.type = 
			}

			continue;
		}

		if (reader.isEndElement())
		{
			QString tagName = reader.name().toString();
			if (tagName == "context")
			{
				//xml.contexts.push_back(context);
				__contexts.push_back(context);
				context.clear();
				continue;
			}

			if (tagName == "message")
			{
				context.messages.push_back(message);
				message.clear();
				continue;
			}


		}
	}

	file.close();
	return true;
}

void __Xml::__write(QString filepath)
{
	QFile file(filepath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		return;
	}

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();

	/// 写入ts;
	writer.writeStartElement("TS");
	writer.writeAttribute("version", __version);

	for (auto context : __contexts)
	{
		writer.writeStartElement("context");
		writer.writeTextElement("name", context.name);

		for (auto message : context.messages)
		{
			writer.writeStartElement("message");

			for (auto location : message.locations)
			{
				/// write location;
				writer.writeStartElement("location");
				writer.writeAttribute("filename", location.filename);
				writer.writeAttribute("line", location.line);
				writer.writeEndElement();
			}

			// write source;
			writer.writeTextElement("source", message.source);

			///
			writer.writeStartElement("translation");
			//writer.writeAttribute("type", message.translation.type);
			//writer.writeTextElement("translation", message.translation.translation);
			writer.writeCharacters(message.translation.translation);
			writer.writeEndElement();

			writer.writeEndElement();
		}

		writer.writeEndElement();
	}

	writer.writeEndElement();// 结束ts;

	file.close();
}


QString __Xml::__translate(QString sourceLang, QString targetLang, QString source)
{
	Json json;
	json["model"] = "qwen-mt-plus";
	json["messages"] = Json::array();

	//json["messages"].push_back();

	Json message;
	message["role"] = "user";
	message["content"] = source.toStdString();
	json["messages"].push_back(message);

	Json translationOptions;
	translationOptions["source_lang"] = sourceLang.toStdString();
	translationOptions["target_lang"] = targetLang.toStdString();

	/// Terms;
	//translationOptions["terms"] = Json::array();
	//{
	//	Json term;
	//	term["source"] = "AppBarButton";
	//	term["target"] = std::string("应用栏按钮");
	//	translationOptions["terms"].push_back(term);
	//}



	json["translation_options"] = translationOptions;

	//json["messages"] = { { {"role", "user"}, {"content", sourceLang} } };
	//json["translation_options"] = { {"source_lang", sourceLang},{"target_lang", targetLang}};


	auto _json = TongYiOpenAi::completion().create(json);
	//LOG_DEBUG << "_json:" << _json.dump().c_str();
	//return QString(_json["choices"][0]["message"]["content"]);

	// _json is empty
	if (_json.empty())
	{
		return "";
	}

	// _json has choices
	if (!_json.contains("choices") || !_json["choices"].is_array())
	{
		return "";
	}

	std::string content = _json["choices"][0]["message"]["content"];
	QString dest = QString::fromUtf8(content.c_str());
	return dest;
}

std::list<QString> __Xml::__translate(QString sourceLang, QString targetLang, std::list<QString> sources)
{
	QStringList ss;
	for (auto& s : sources)
	{
		ss.push_back(s);
	}

	QString source = ss.join("$$@@");
	QString dest = __translate(sourceLang, targetLang, source);
	QStringList destList = dest.split("$$@@");
	std::list<QString> result;
	for (const auto& d : destList)
	{
		result.push_back(d);
	}

	bool isNeedReTranslate = false;
	if (destList.size() != sources.size())
        isNeedReTranslate = true;
	if (destList.size() == sources.size())
	{
		for (auto iterSource = sources.begin(), iterDest = result.begin(); iterSource != sources.end(); iterSource++, iterDest++)
		{
			QString source = *iterSource;
			QString dest = *iterDest;
			if (source == dest)
			{
				isNeedReTranslate = true;
				break;
			}
		}
	}


	if (isNeedReTranslate)
	{
		/// not equal one by one translate;
		/*QThread::msleep(1500);*/
		result.clear();

		for (auto s : sources)
		{
			QString t = __translate(sourceLang, targetLang, s);
			QThread::msleep(1500);
			result.push_back(t);
			//return result;
		}

		return result;
	}

	return result;
}

void __Xml::__translate(QString sourceLang, QString targetLang)
{
	int nTotalTranslate = 0;
	int nTranslate = 0;

	int p = 0;

	for (auto& context : __contexts)
	{
		for (auto& message : context.messages)
		{
			nTotalTranslate += 1;
		}
	}

	std::list<QString> sList;
	std::list<QString> dList;
	for (auto& context : __contexts)
	{
		for (auto& message : context.messages)
		{
			sList.push_back(message.source);
		}
	}

	emit __translateStarted();
	std::list<QString> waitList;
	std::list<QString> wdList;
	for (auto& s : sList)
	{
		waitList.push_back(s);
		if (waitList.size() >= 20)
		{
			std::list<QString> tList = __translate(sourceLang, targetLang, waitList);
			for (auto& t : tList)
			{
				dList.push_back(t);
				//QThread::msleep(20);
				//emit __translateInfoChanged(nTranslate, nTotalTranslate, s, t);
			}

			///emit
			{
				auto iterS = waitList.begin();
				auto iterT = tList.begin();
				while (iterS != waitList.end() && iterT != tList.end())
				{
					nTranslate += 1;
					QThread::msleep(40);
					emit __translateInfoChanged(nTranslate, nTotalTranslate, *iterS, *iterT);
					iterS++;
					iterT++;
				}
			}

			waitList.clear();

            QThread::msleep(2000);
		}
	}

	if (!waitList.empty())
	{
		std::list<QString> tList = __translate(sourceLang, targetLang, waitList);
		for (auto& t : tList)
		{
			dList.push_back(t);
		}

		///emit
		{
			auto iterS = waitList.begin();
			auto iterT = tList.begin();
			while (iterS != waitList.end() && iterT != tList.end())
			{
				nTranslate += 1;
				QThread::msleep(40);
				emit __translateInfoChanged(nTranslate, nTotalTranslate, *iterS, *iterT);
				iterS++;
				iterT++;
			}
		}

		waitList.clear();
	}

	auto iter = dList.begin();
	for (auto& context : __contexts)
	{
		for (auto& message : context.messages)
		{
			message.translation.translation = (*iter);
			iter++;
		}
	}

	emit __translateEnded();
}
