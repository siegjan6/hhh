#include "LanguageToolWidget.h"
#include "ui_LanguageToolWidget.h"
#include "DictionaryManager.h"
#include "DictDialog.h"
#include "MDMultiLanguage.h"
/////////////////////////////////////////////////////////////////////
CLanguageToolWidget::CLanguageToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLanguageToolWidget)
{
    ui->setupUi(this);

    m_isValid = false;
    m_manager = NULL;
    m_transTool = NULL;

    QStringList types;
    types << "QComboBox";
    m_transTool = CMDMultiLanguage::createWidgetTool();
    m_transTool->setExceptionTypes(types);
    m_transTool->init(this);
}
CLanguageToolWidget::~CLanguageToolWidget()
{
    delete ui;
    delete m_transTool;
}
/////////////////////////////////////////////////////////////////////
void CLanguageToolWidget::on_comboBoxSystem_currentIndexChanged(int index)
{
    if(!m_isValid)
        return;

    setCurrentLanguage(index, true);
    changeLanguage();
}
void CLanguageToolWidget::on_comboBoxProject_currentIndexChanged(int index)
{
    if(!m_isValid)
        return;

    setCurrentLanguage(index, false);
}
void CLanguageToolWidget::on_pushButton_clicked()
{
    if(m_manager == NULL)
        return;

    CDictDialog dlg(this);
    dlg.init(m_manager);

    QStringList types;
    types << "QComboBox" << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(&dlg, &types);
    dlg.exec();

    changeLanguage();
    m_manager->innerChangeLanuage();
}
/////////////////////////////////////////////////////////////////////
void CLanguageToolWidget::init(CDictionaryManager* manager)
{
    m_manager = manager;
    changeLanguage();
}
void CLanguageToolWidget::resetWidget()
{
    m_isValid = false;

    if(m_manager == NULL)
    {
        ui->comboBoxSystem->clear();
        ui->comboBoxSystem->setEnabled(false);
        ui->comboBoxProject->clear();
        ui->comboBoxProject->setEnabled(false);
    }
    else
    {
        QStringList list = m_manager->selectLanguages();
        bool enable = m_manager->enable();
        ui->comboBoxSystem->clear();
        for(int i = 0; i < list.count(); i++)
            ui->comboBoxSystem->addItem(CMDMultiLanguage::instance()->value(list.at(i)));
        ui->comboBoxSystem->setCurrentIndex(getSelectIndex(m_manager->currentSystemIndex()));
        ui->comboBoxSystem->setEnabled(enable);
        ui->comboBoxProject->clear();
        for(int i = 0; i < list.count(); i++)
            ui->comboBoxProject->addItem(CMDMultiLanguage::instance()->value(list.at(i)));
        ui->comboBoxProject->setCurrentIndex(getSelectIndex(m_manager->currentProjectIndex()));
        ui->comboBoxProject->setEnabled(enable);
    }

    m_isValid = true;
}
void CLanguageToolWidget::changeLanguage()
{
    resetWidget();
    m_transTool->changeLanguage();
}
/////////////////////////////////////////////////////////////////////
void CLanguageToolWidget::setCurrentLanguage(int selectIndex, bool isSystem)
{
    m_manager->setCurrentLanguage(getFactIndex(selectIndex), isSystem);
}
int CLanguageToolWidget::getFactIndex(int selectIndex)
{
    QList<int> list = m_manager->selectIndexs();
    int count = list.count();
    if(selectIndex >= 0 && selectIndex < count)
        selectIndex = list.at(selectIndex);
    else
        selectIndex = -1;

    return selectIndex;
}
int CLanguageToolWidget::getSelectIndex(int factIndex)
{
    QList<int> list = m_manager->selectIndexs();
    return list.indexOf(factIndex);
}
/////////////////////////////////////////////////////////////////////
