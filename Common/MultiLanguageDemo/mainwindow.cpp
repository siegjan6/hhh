#include <QMessageBox>
#include <QToolButton>
#include <QComboBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*支持翻译的控件
QAbstractButton
QListWidget
QTreeWidget
QTableWidget
QComboBox
QLineEdit
QTextEdit
QLabel
QGroupBox
QTabWidget
QDialog
QMainWindow
QAction
*/

///////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_labelText = tr("TextLabel");

    m_dictManager = CMDMultiLanguage::createDictionaryManager(this, "/home/hdp/code");
    CMDMultiLanguage::instance()->setCurrentProject(m_dictManager);

    QMenu* menu = ui->menuBar->addMenu(tr("menu"));

    QAction* a = menu->addAction(tr("action"));
    ui->mainToolBar->addAction(a);
    ui->textEdit->setText(tr("textEdit"));

    QStringList exs;
    exs << "QComboBox" << "QLabel";
    QStringList names;
    names << "comboBox";
    QStringList severeds;
    severeds << "menuBar";


    m_tool = CMDMultiLanguage::createWidgetTool();
    m_tool->setExceptionTypes(exs);
    m_tool->setTranslateWidgetName(names);
    //m_tool->setSeveredNames(severeds);
    m_tool->init(this);

    ui->mainToolBar->addWidget(CMDMultiLanguage::instance()->toolWidget());


    m_tool->changeLanguage();
    //changeLanguage();


    //注意点1
    //如果在Menu中添加Action，Action的Parent应该为Menu
    //QMenu* m_menu = new QMenu();
    //QAction * aaaa = new QAction("aaa", m_menu);
}
MainWindow::~MainWindow()
{
    delete ui;
    CMDMultiLanguage::release();
    delete m_dictManager;
}
void MainWindow::on_pushButton_clicked()
{
    //QString str = tr("&OK");
    QMessageBox box;
    box.setText(tr("text"));
    box.setWindowTitle(tr("title"));
    box.setStandardButtons(QMessageBox::Ok);

    CMDMultiLanguage::instance()->translateWidget(&box);

    box.exec();
}
///////////////////////////////////////////////////////////////////////
void MainWindow::changeLanguage()
{
    m_tool->changeLanguage();
    ui->label->setText(CMDMultiLanguage::instance()->value(m_labelText));
}
void MainWindow::collectWords()
{
    CMDMultiLanguage::instance()->addWord("test", false);
}
///////////////////////////////////////////////////////////////////////
