#include "EditorDialog.h"
#include "ui_EditorDialog.h"
#include <QMessageBox>
#include <QThread>
#include "MDMultiLanguage.h"
//#include "BrowserManager.h"

CEditorDialog::CEditorDialog(QString content, const QList<QPair<QString, QString> > &objs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEditorDialog),
    m_content(content),
    m_modify(false),
    m_willQuit(false),
    m_srcContent(content)
{
    ui->setupUi(this);

    m_srcContent = m_srcContent.replace("\\","\\\\");
    m_srcContent = m_srcContent.replace("\"","\\\"");
    m_webview = new CWebContainer(CWebContainer::ContentType::string, m_srcContent, this);
    m_webview->initObjModel(objs);

    connect(m_webview, &CWebContainer::setModify, this, &CEditorDialog::onSetModify);
    connect(m_webview, &CWebContainer::saveFileFinished, this, &CEditorDialog::onSaveFileFinished);

    layout()->addWidget(m_webview);

    m_webview->showContent();
}

CEditorDialog::~CEditorDialog()
{
    delete ui;
}

void CEditorDialog::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);

    m_webview->initSize();
}


void CEditorDialog::done(int res)
{
//    if( !gBrowserManager->m_errString.isEmpty() )
//    {
//        qDebug("cef invalid! quit");
//        QDialog::done(res);
//        return;
//    }

    if( m_modify )
    {
        QMessageBox dialog(this);
        dialog.setIcon(QMessageBox::Warning);
        dialog.setWindowTitle(QObject::tr("警告"));
        dialog.setText(tr("脚本已修改，需要保存么？"));
        dialog.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);
        CMDMultiLanguage::instance()->translateWidget(&dialog);
        int result = dialog.exec();
        if( QMessageBox::Discard == result )
        {
            QDialog::done(QDialog::Rejected);
            return;
        }
        else if( QMessageBox::Save == result )
        {
            m_willQuit = true;
            m_webview->saveFile();
            return;
        }
        else if( QMessageBox::Cancel == result )
        {
           return;
        }

    }
    else
        QDialog::done(QDialog::Rejected);
}


void CEditorDialog::onSetModify(bool modify)
{
    m_modify = modify;

    QString title = windowTitle();
    title.remove('*');
    if(m_modify)
        title.append('*');
    setWindowTitle(title);

}

void CEditorDialog::onSaveFileFinished()
{ 
    m_content = m_webview->m_editorModel;
    if(m_willQuit)
        QDialog::done(QDialog::Accepted);
}
