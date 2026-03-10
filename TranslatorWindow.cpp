#include "TranslatorWindow.h"
#include <FluMenu.h>
#include <FluAction.h>
#include <FluMessageBox.h>
#include <QFileDialog>
#include <qscilexer.h>
#include <qscilexerxml.h>
#include <FluLogUtils.h>
//#include <QDomComment>

TranslatorWindow::TranslatorWindow(QWidget* parent /*= nullptr*/) : FluWindowKitWindow(parent)
{
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
	auto lexer = new QsciLexerXML;
	m_editor->setLexer(lexer);
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

	m_helpMenu = new FluMenu;
	m_helpMenu->setTitle("Help(&H)");

	m_aboutAction = new FluAction("About TSFileTranslator.");
	m_helpMenu->addAction(m_aboutAction);

	m_menuBar->addAction(m_fileMenu->menuAction());
	m_menuBar->addAction(m_helpMenu->menuAction());
}

void TranslatorWindow::__connect()
{
	connect(m_openFileAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerOpenFile);
	connect(m_aboutAction, &FluAction::triggered, this, &TranslatorWindow::__onTriggerAbout);
}

void TranslatorWindow::__translate()
{
	//// 将xml转为相应结构体;
	LOG_DEBUG << "Xml to struct.";
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

	QString str = file.readAll();
	file.close();
	m_editor->setText(str);
}

void TranslatorWindow::__onTriggerAbout(bool b)
{
	FluMessageBox info("About", "TS File Translator\r\nVersion: 1.0.0", this);
	info.exec();
}
