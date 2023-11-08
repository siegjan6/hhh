#include "PreviewDialog.h"
#include "ui_PreviewDialog.h"
#include "MDHMIForm.h"
#include "FormOperation.h"
#include "MDMultiLanguage.h"
//////////////////////////////////////////////////////////////////////////
CPreviewDialog::CPreviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPreviewDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);
    setWindowState(Qt::WindowMaximized);

    m_framework = NULL;
    m_node = new CPreviewNode(ui->treeWidget, this);
    m_objectWidget.init(this);
    ui->scrollArea->setWidget(&m_objectWidget);
    ui->scrollArea->installEventFilter(this);
    m_property = new CPreviewPropertyWidget(&m_objectWidget, this);
    ui->verticalLayout->addWidget(m_property);

    QStringList exs;
    exs << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &exs);
}
CPreviewDialog::~CPreviewDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CPreviewDialog::init(CHMIFramework *framework)
{
    m_framework = framework;
    QTreeWidgetItem* item = m_framework->rootItem();
    m_node->init(item);
}
void CPreviewDialog::loadForm(const QString &fullName)
{
    //已存在，则选中
    CFormObject* obj = m_objectWidget.findOpened(fullName);
    if(obj != NULL)
    {
        m_objectWidget.select(obj);
        return;
    }

    //不存在，添加
    bool hasOpened = false;
    CMDHMIForm* f = dynamic_cast<CFormOperation*>(m_framework->forms())->openBackgroundForm(fullName, hasOpened);
    if(f == NULL)
        return;

    m_objectWidget.appendForm(f);

    //!如果是后台打开
    if(!hasOpened)
    {
        f->close();
        delete f;
    }
}
void CPreviewDialog::saveForm(CFormObject* obj)
{   
    QString fullName = obj->fullName();
    bool hasOpened = false;
    CMDHMIForm* f = dynamic_cast<CFormOperation*>(m_framework->forms())->openBackgroundForm(fullName, hasOpened);
    if(f == NULL)
        return;

    f->setPropertyRect(obj->rect());

    if(!hasOpened)
    {
        f->save();
        f->close();
        delete f;
    }
}
//////////////////////////////////////////////////////////////////////////
bool CPreviewDialog::eventFilter(QObject *o, QEvent *e)
{
    if(o == ui->scrollArea)
    {
        if(e->type() == QEvent::KeyPress)
        {
            QKeyEvent* event = static_cast<QKeyEvent*>(e);
            m_objectWidget.onKeyPress(event);
            return true;
        }
    }

    return QDialog::eventFilter(o, e);
}
void CPreviewDialog::closeEvent(QCloseEvent *e)
{
    m_objectWidget.onClose(e);
    e->ignore();
    hide();
}
//////////////////////////////////////////////////////////////////////////

