#include "DictDialog.h"
#include "ui_DictDialog.h"
//////////////////////////////////////////////////////////////
CDictDialog::CDictDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDictDialog)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->listWidget->setCurrentRow(0);

    m_languageDialog = NULL;
    m_systemDialog = NULL;
    m_projectDialog = NULL;
    m_generateDictDialog = NULL;
    m_manager = NULL;
    m_lastRow = 0;
    m_initFinished = false;
}
CDictDialog::~CDictDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////
void CDictDialog::on_listWidget_currentRowChanged(int currentRow)
{
    if(!m_initFinished)
        return;
    switch(m_lastRow)
    {
    case 0:
        m_languageDialog->save();
        break;
    case 1:
        m_systemDialog->save();
        break;
    case 2:
        m_projectDialog->save();
        break;
    default:
        break;
    }

    m_lastRow = currentRow;

    ui->stackedWidget->setCurrentIndex(currentRow);

    if(currentRow == 1)
        m_systemDialog->refresh();
    else if(currentRow == 2)
        m_projectDialog->refresh();
}
//////////////////////////////////////////////////////////////
void CDictDialog::closeEvent(QCloseEvent *e)
{
    switch(m_lastRow)
    {
    case 0:
        m_languageDialog->save();
        break;
    case 1:
        m_systemDialog->save();
        break;
    case 2:
        m_projectDialog->save();
        break;
    default:
        break;
    }

    m_manager->save();
    m_manager->resetCurrentIndex();
}
//////////////////////////////////////////////////////////////
void CDictDialog::init(CDictionaryManager *manager)
{
    m_manager = manager;

    m_languageDialog = new CLanguageDialog(this);
    m_languageDialog->init(m_manager);
    ui->stackedWidget->widget(0)->layout()->addWidget(m_languageDialog);
    m_systemDialog = new CDictEditDialog(this);
    m_systemDialog->init(m_manager, true);
    ui->stackedWidget->widget(1)->layout()->addWidget(m_systemDialog);
    m_projectDialog = new CDictEditDialog(this);
    m_projectDialog->init(m_manager, false);
    ui->stackedWidget->widget(2)->layout()->addWidget(m_projectDialog);
    m_generateDictDialog = new CGenerateDictDialog(this);
    m_generateDictDialog->init(m_manager);
    ui->stackedWidget->widget(3)->layout()->addWidget(m_generateDictDialog);
    m_initFinished = true;
}
//////////////////////////////////////////////////////////////
