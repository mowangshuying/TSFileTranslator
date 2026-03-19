#include "TranslatorWindow.h"
#include <FluMenu.h>
#include <FluAction.h>
#include <FluMessageBox.h>
#include <QFileDialog>
#include <qscilexer.h>
#include <qscilexerxml.h>
#include <FluLogUtils.h>
//#include <QDomDocument>
//#include <QDomComment>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <TongYiOpenAi.hpp>
//#include <QThread>
#include <thread>
#include <FluInfoBarMgr.h>
#include <FluShortInfoBar.h>
#include <QApplication>
#include <FluDockManager.h>
#include <FluLabel.h>
#include <FluLineEdit.h>
#include <FluProgressRing.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <FluComboBoxEx.h>
#include <QThread>
#include <FluUtils.h>


TranslatorWindow::TranslatorWindow(QWidget* parent /*= nullptr*/) : FluWindowKitWindow(parent)
{
	m_translateState = TranslateState::Ready;

	loadConfig();
	TongYiOpenAi::__init(m_httpurl.toStdString(), m_token.toStdString());

	__initUI();
	__connect();
}

void TranslatorWindow::loadConfig()
{
	FluConfigUtils::getUtils()->getSettings()->beginGroup("config");
	m_httpurl = FluConfigUtils::getUtils()->getSettings()->value("httpurl", "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions").toString();
	m_token = FluConfigUtils::getUtils()->getSettings()->value("token", "sk-******").toString();
	m_sourceLang = FluConfigUtils::getUtils()->getSettings()->value("sourceLang", "English").toString();
    m_targetLang = FluConfigUtils::getUtils()->getSettings()->value("targetLang", "Chinese").toString();
	FluConfigUtils::getUtils()->getSettings()->endGroup();
}

void TranslatorWindow::__initUI()
{
	setWindowTitle("TS File Translator");

	/// menu;
	__initMenu();
	
	/// editor;
	m_editor = new FluScintilla(this);
	setCentralWidget(m_editor);
}

void TranslatorWindow::__initMenu()
{
	m_fileMenu = new FluMenu;
	m_fileMenu->setTitle("File(&F)");

	m_openFileAction = new FluAction("Open(&O)");
	m_openConfigFileAction = new FluAction("Open Config File...");

	m_saveFileAction = new FluAction("Save(&S)");
	m_saveAsFileAction = new FluAction("Save As");
	m_exitAction = new FluAction("Exit");
	
	m_fileMenu->addAction(m_openFileAction);
	m_fileMenu->addAction(m_openConfigFileAction);
	m_fileMenu->addAction(m_saveFileAction);
	m_fileMenu->addAction(m_saveAsFileAction);
	m_fileMenu->addAction(m_exitAction);

	m_operMenu = new FluMenu;
	m_operMenu->setTitle("Operation(&O)");
	m_translateAction = new FluAction("Translate");

	m_operMenu->addAction(m_translateAction);

	m_helpMenu = new FluMenu;
	m_helpMenu->setTitle("Help(&H)");

	m_aboutAction = new FluAction("About TSFileTranslator.");
	m_helpMenu->addAction(m_aboutAction);

	m_menuBar->addAction(m_fileMenu->menuAction());
	m_menuBar->addAction(m_operMenu->menuAction());
	m_menuBar->addAction(m_helpMenu->menuAction());
}

void TranslatorWindow::__connect()
{
	connect(m_openFileAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerOpenFile);
    connect(m_openConfigFileAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerOpenConfigFile);
    connect(m_saveFileAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerSaveFile);
	connect(m_aboutAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerAbout);
	connect(m_translateAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerTranslate);
	connect(m_exitAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerExit);
	connect(this, &TranslatorWindow::__translateValueChanged, this, &TranslatorWindow::__onTranslateValueChanged);
}

//void TranslatorWindow::__translate()
//{
//	__Xml xml = __read(m_xmlFilePath);
//	__write("__write.xml", xml);
//}

__Xml TranslatorWindow::__read(QString filepath)
{
	__Xml xml;
	Context context;
	Message message;
	Location location;

	if (filepath.isEmpty())
	{
		return xml;
	}

	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return xml;
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
					xml.version = attr.value("version").toString();
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
				xml.contexts.push_back(context);
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
	return xml;
}

QString TranslatorWindow::__translate(QString sourceLang, QString targetLang, QString source)
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

	std::string content =  _json["choices"][0]["message"]["content"];	
	QString dest =  QString::fromUtf8(content);
	return dest;
}

void TranslatorWindow::__translate(__Xml& xml)
{
	int nTotalTranslate = 0;
	int nTranslate = 0;

	int p = 0;

	for (auto& context : xml.contexts)
	{
		for (auto& message : context.messages)
		{
			nTotalTranslate += 1;
		}
	}

	for (auto& context : xml.contexts)
	{
		for (auto& message : context.messages)
		{
			QString s = message.source;
			QString t = __translate(m_sourceLang, m_targetLang, s);
			QThread::msleep(1500);
			message.translation.translation = t;
			nTranslate += 1;
			emit __translateValueChanged(nTranslate, nTotalTranslate);
		}
	}
}

void TranslatorWindow::__write(QString filepath, __Xml xml)
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
	writer.writeAttribute("version", xml.version);

	for (auto context : xml.contexts)
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

void TranslatorWindow::closeEvent(QCloseEvent* event)
{
	FluMessageBox messageBox(tr("Close TSFileTranslator?"), tr("choose \"Ok\" to close. choose \"Cancel\" do nothing."), this);
	int nExec = messageBox.exec();
	if (nExec == QDialog::Rejected)
	{
		event->ignore();
		return;
	}
	else if (nExec == QDialog::Accepted)
	{
		// event->accept();
		QApplication::quit();
	}
}

void TranslatorWindow::__onTriggerOpenFile(bool b)
{
	QFileDialog qfd(this);
	qfd.setFileMode(QFileDialog::ExistingFile);
	int nExec = qfd.exec();
	if (nExec != QDialog::Accepted)
	{
		qfd.close();
		return;
	}

	QStringList sFiles = qfd.selectedFiles();
	if (sFiles.isEmpty())
	{
		return;
	}

	QString filepath = sFiles[0];
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	m_xmlFilePath = filepath;
	
	//QDomDocument doc;
	//if (doc.setContent(&file))
	//{
	//	file.close();
	//	return;
	//}

	
	QString str = file.readAll();


	file.close();
	m_editor->setText(str);
}

void TranslatorWindow::__onTriggerOpenConfigFile(bool b)
{
	// open config file;
	QString filepath = "../config/config.ini";
	m_curFilePath = filepath;
	QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QString str = file.readAll();
	m_editor->setText(str);
    file.close();
}

void TranslatorWindow::__onTriggerSaveFile(bool b)
{
	// save file;
    QFile file(m_curFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    file.write(m_editor->text().toUtf8());
    file.close();

	if (m_curFilePath == "../config/config.ini")
	{
		loadConfig();
		TongYiOpenAi::__init(m_httpurl.toStdString(), m_token.toStdString());
	}
}

void TranslatorWindow::__onTriggerAbout(bool b)
{
	FluMessageBox info("About", "TS File Translator\r\nVersion: 1.0.0", this);
	info.exec();
}

void TranslatorWindow::__onTriggerTranslate(bool b)
{
	//FluThread thread;
	//thread.start(nullptr, [=](&thread) {
	//	}, nullptr);
	if (m_sourceLang == "" || m_targetLang == "")
	{
        FluInfoBarMgr::showInfoBar(window(), FluShortInfoBarType::Warn, "Please set source language and target language.");
		return;
	}

	if (m_translateState == TranslateState::Working)
	{
		FluInfoBarMgr::showInfoBar(window(), FluShortInfoBarType::Warn, "Wait for task end.");
		return;
	}

	FluMessageBox messageBox(tr("Translate Progress."), tr(""), this);
	connect(this, &TranslatorWindow::__translateProgress, &messageBox, &FluMessageBox::setInfo);
	connect(this, &TranslatorWindow::__translateError, &messageBox, &FluMessageBox::setInfo);

	std::thread __thread([=]() {
		emit __translateStart();
		m_translateState = TranslateState::Working;

		__Xml xml = __read(m_xmlFilePath);
		if (xml.contexts.size() == 0)
		{
			emit __translateError("Nothing need translate.");
		}

		__translate(xml);
		__write("__write.xml", xml);
		

		m_translateState = TranslateState::Ready;
		emit __translateEnd();

		});

	__thread.detach();
	messageBox.exec();

}

void TranslatorWindow::__onTriggerExit(bool b)
{
	close();
}

void TranslatorWindow::__onTranslateValueChanged(int c, int t)
{
	//LOG_DEBUG << c << "/" << t;
	QString p = QString::asprintf("Translated (%d) of total (%d).", c, t);
	emit __translateProgress(p);
}
