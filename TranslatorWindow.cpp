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


TranslatorWindow::TranslatorWindow(QWidget* parent /*= nullptr*/) : FluWindowKitWindow(parent)
{
	TongYiOpenAi::__init("https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions",
		"sk-c43463e3702d452fa7d873efb7808eae");

	__initUI();

	__connect();
}

void TranslatorWindow::__initUI()
{
	setWindowTitle("TS File Translator");
	
	/// menu;
	__initMenu();
	

	
	/// editor;
	m_editor = new FluScintilla(this);
	
	/// xml解析器;
	//auto lexer = new QsciLexerXML;
	//m_editor->setLexer(lexer);
	setCentralWidget(m_editor);
}

void TranslatorWindow::__initMenu()
{
	m_fileMenu = new FluMenu;
	m_fileMenu->setTitle("File(&F)");

	m_openFileAction = new FluAction("Open(&O)");
	m_saveFileAction = new FluAction("Save(&S)");
	m_saveAsFileAction = new FluAction("Save As");
	m_exitAction = new FluAction("Exit");
	
	m_fileMenu->addAction(m_openFileAction);
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
	connect(m_aboutAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerAbout);
	connect(m_translateAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerTranslate);
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

void TranslatorWindow::__translate(QString sourceLang, QString targetLang, QString source)
{
	Json json;
	json["model"] = "qwen-mt-plus";
	json["messages"] = { { {"role", "user"}, {"content", sourceLang} } };
	json["translation_options"] = { {"source_lang", sourceLang},{"target_lang", targetLang}};
	auto _json = TongYiOpenAi::completion().create(json);
	LOG_DEBUG << "_json:" << _json.dump().c_str();
}

void TranslatorWindow::__translate(__Xml& xml)
{
	for (auto context : xml.contexts)
	{
		for (auto message : context.messages)
		{
			QString s = message.source;
			QString t;
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
			writer.writeAttribute("type", message.translation.type);
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

void TranslatorWindow::__onTriggerAbout(bool b)
{
	FluMessageBox info("About", "TS File Translator\r\nVersion: 1.0.0", this);
	info.exec();
}

void TranslatorWindow::__onTriggerTranslate(bool b)
{
	__Xml xml = __read(m_xmlFilePath);
	__write("__write.xml", xml);
}
