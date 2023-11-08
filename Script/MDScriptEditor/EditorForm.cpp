#include "EditorForm.h"
#include <QHBoxLayout>

CEditorForm::CEditorForm(QString filePath, const QList<QPair<QString, QString> > &objs, QWidget *parent) :
    CBaseForm(parent)
{
    m_filePath = filePath;
    m_webview = new CWebContainer(CWebContainer::ContentType::file, m_filePath, this);
    m_webview->initObjModel(objs);
    connect(m_webview, &CWebContainer::setModify, this, &CEditorForm::onSetModify);

    setCentralWidget(m_webview);

    m_webview->showContent();

    m_fileName = m_filePath.right(m_filePath.count()-m_filePath.lastIndexOf("/")-1);
    CBaseForm::setWindowText(m_fileName);
}

CEditorForm::CEditorForm(QString content, bool isContent, const QList<QPair<QString, QString> > &objs, QWidget *parent) :
    CBaseForm(parent)
{
    m_webview = new CWebContainer(CWebContainer::ContentType::string, content, this);
    m_webview->initObjModel(objs);
    connect(m_webview, &CWebContainer::setModify, this, &CEditorForm::onSetModify);

    setCentralWidget(m_webview);

    m_webview->showContent();
}

bool CEditorForm::save()
{
    m_webview->saveFile();
    return true;
}

void CEditorForm::closeEvent(QCloseEvent *)
{

}

void CEditorForm::onSetModify(bool modify)
{
    setModify(modify);
}

