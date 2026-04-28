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
#include "SettingsDlg.h"


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
	setWindowTitle(tr("TS File Translator"));

   
   FluDockManager::setConfigFlag(FluDockManager::OpaqueSplitterResize, true);
   FluDockManager::setConfigFlag(FluDockManager::XmlCompressionEnabled, false);
   FluDockManager::setConfigFlag(FluDockManager::FocusHighlighting, true);

   m_dockMgr = new FluDockManager(this);

	/// menu;
	__initMenu();

		/// editor;
	m_editor = new FluScintilla(this);
	// setCentralWidget(m_editor);

	auto centralDockWidget = m_dockMgr->createDockWidget("CentralWidget", this);
	centralDockWidget->setWidget(m_editor);
	auto centralDockArea = m_dockMgr->setCentralWidget(centralDockWidget);
	centralDockArea->setAllowedAreas(ads::DockWidgetArea::OuterDockAreas);

	m_outputWindow = new OutputWindow(this);
	// set readonly


	//m_outputWindow->setMarginLineNumbers(0, false);
	//m_outputWindow->setMarginWidth(0, 0);
	auto outputDockWidget = m_dockMgr->createDockWidget("OutputWidget", this);
	outputDockWidget->setWidget(m_outputWindow);
	outputDockWidget->setMinimumSizeHintMode(ads::CDockWidget::MinimumSizeHintFromDockWidget);
	// outputDockWidget->setMaximumHeight(300);
	m_dockMgr->addDockWidget(ads::BottomDockWidgetArea, outputDockWidget);

	m_listView = new FluListView(this);
	m_listView->setNoBorder(true);
	m_listView->setContextMenuPolicy(Qt::CustomContextMenu);
	auto listDockWidget = m_dockMgr->createDockWidget("ListWidget", this);
	listDockWidget->setWidget(m_listView);
	listDockWidget->setMinimumSizeHintMode(ads::CDockWidget::MinimumSizeHintFromDockWidget);
	// listDockWidget->setMaximumWidth(300);
	m_dockMgr->addDockWidget(ads::LeftDockWidgetArea, listDockWidget);
}

void TranslatorWindow::__initMenu()
{
	m_contextMenu = new FluRoundMenu("", FluAwesomeType::None, this);
	m_openFileAction = new FluAction( FluAwesomeType::OpenFile, tr("Open"));
	m_delteFileAction = new FluAction( FluAwesomeType::Delete, tr("Delete"));
	m_settingsAction = new FluAction( FluAwesomeType::Settings, tr("Settings"));
	m_translateAction = new FluAction( FluAwesomeType::Next, tr("Translate"));
	//m_contextMenu->addAction(m_openFileAction);
	//m_contextMenu->addAction(m_delteFileAction);
	//m_contextMenu->addAction(m_settingsAction);
	//m_contextMenu->addAction(m_translateAction);
}

void TranslatorWindow::__connect()
{
	connect(m_openFileAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerOpenFile);
	connect(m_translateAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerTranslate);
    //connect(m_openConfigFileAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerOpenConfigFile);
    //connect(m_saveFileAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerSaveFile);
	//connect(m_aboutAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerAbout);
	//connect(m_translateAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerTranslate);
	//connect(m_exitAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerExit);
	connect(this, &TranslatorWindow::__translateValueChanged, this, &TranslatorWindow::__onTranslateValueChanged);
	connect(m_listView, &FluListView::itemClicked, this, &TranslatorWindow::__onItemClicked);
	connect(m_listView, &FluListView::customContextMenuRequested, this, &TranslatorWindow::__onListViewCustomContextMenuRequested);
	connect(m_settingsAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerSettings);
}

void TranslatorWindow::__log(QString text)
{
	m_outputWindow->append(text+"\n");
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
			emit __translateValueChanged(nTranslate, nTotalTranslate,  s , t);
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

//void TranslatorWindow::contextMenuEvent(QContextMenuEvent *event)
//{
//}

void TranslatorWindow::closeEvent(QCloseEvent *event)
{
	FluWindowKitWindow::closeEvent(event);
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

	/// 
	m_listView->addItem(filepath);

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

void TranslatorWindow::__onTriggerSettings(bool b)
{
	SettingsDlg settingsDlg(this);
	settingsDlg.exec();
}

void TranslatorWindow::__onTriggerTranslate(bool b)
{
	loadConfig();
	if (m_sourceLang == "" || m_targetLang == "")
	{
        //FluInfoBarMgr::showInfoBar(window(), FluShortInfoBarType::Warn, "Please set source language and target language.");
		__log("Please set source language and target language.");
		return;
	}

	if (m_translateState == TranslateState::Working)
	{
		//FluInfoBarMgr::showInfoBar(window(), FluShortInfoBarType::Warn, "Wait for task end.");
		__log("Wait for task end.");
		return;
	}

	/// 
	__log("Translate ...");
	__log("filepath: " + m_xmlFilePath);
	__log("sourceLang: " + m_sourceLang);
	__log("targetLang: " + m_targetLang);

	std::thread __thread([=]() {
		emit __translateStart();
		m_translateState = TranslateState::Working;

		__Xml xml = __read(m_xmlFilePath);
		if (xml.contexts.size() == 0)
		{
			emit __translateError("Nothing need translate.");
		}

		__translate(xml);

		QString writeFile = m_xmlFilePath + "-" + m_targetLang + ".ts";
		__write(writeFile, xml);
		__log("writeFile: " + writeFile);

		m_translateState = TranslateState::Ready;
		emit __translateEnd();

		});

	__thread.detach();
}

void TranslatorWindow::__onTriggerExit(bool b)
{
	close();
}

void TranslatorWindow::__onTranslateValueChanged(int c, int t, QString source, QString dest)
{
	QString p = QString::asprintf("Translated %d/%d: \r\n\t\t %s => %s", c, t, source.toStdString().c_str(), dest.toStdString().c_str());
	__log(p);
}

void TranslatorWindow::__onItemClicked(QListWidgetItem *item)
{
	// open file;
	QString filepath = item->text();
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	__log(QString::asprintf("Open file: %s\n", filepath.toStdString().c_str()));

	QString str = file.readAll();
	file.close();
	m_editor->setText(str);
}

void TranslatorWindow::__onListViewCustomContextMenuRequested(const QPoint &pos)
{
	/// judge clicked item;
	QListWidgetItem* item = m_listView->itemAt(pos);

	m_contextMenu->clear();
	if (item == nullptr)
	{
		m_contextMenu->addAction(m_openFileAction);
		m_contextMenu->addAction(m_settingsAction);
		m_contextMenu->exec(m_listView->mapToGlobal(pos));
		return;
	}

	m_contextMenu->addAction(m_openFileAction);
	m_contextMenu->addAction(m_delteFileAction);
	m_contextMenu->addAction(m_settingsAction);
	m_contextMenu->addAction(m_translateAction);
	m_contextMenu->exec(m_listView->mapToGlobal(pos));
}