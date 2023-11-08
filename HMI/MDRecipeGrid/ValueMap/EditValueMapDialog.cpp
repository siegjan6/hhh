#include "EditValueMapDialog.h"
#include "ui_EditValueMapDialog.h"
#include "ShareTool.h"
#include "ValueMapView.h"
#include "ValueMapDelegate.h"


CEditValueMapDialog::CEditValueMapDialog(IDrawEnvrionment *environment, QString mapName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEditValueMapDialog)
{
    ui->setupUi(this);

    m_environment = environment;

    if( mapName.isEmpty() )
        mapName = "newmap1";
    else
    {
        gValueMapManager->valueMap(m_environment, mapName, m_info);
        ui->lineeditDesc->setText(m_info.description);
    }

    m_srcMapName = mapName;
    ui->lineeditName->setText(mapName);

    CShareTool::installDialogTranslator(this);
}

CEditValueMapDialog::~CEditValueMapDialog()
{
    delete ui;
}

void CEditValueMapDialog::showEvent(QShowEvent *)
{
    //! tableUI初始化
    CValueMapView* table = ui->tableviewMap;
    table->setEnvironment(m_environment);
    table->init(m_info.values);

}

void CEditValueMapDialog::on_pushbuttonOk_clicked()
{
    bool isInValid = false;

    QString mapName = ui->lineeditName->text();
    if(mapName.isEmpty())
    {
        CShareTool::trWarningMessageBox(tr("表的名称不能为空"),this);
        return;
    }
    if( m_srcMapName != mapName && gValueMapManager->isValueMapExist(m_environment, mapName) )
    {
        CShareTool::trWarningMessageBox(tr("该表已经存在，请修改表名称"),this);
        return;
    }

    CValueMapManager::ValueInfo sInfo;
    sInfo.name = mapName;
    sInfo.description = ui->lineeditDesc->text();
    CValueMapManager::Values& list = sInfo.values;
    QTableView* table = ui->tableviewMap;

    for(qint32 row = 0; row < table->model()->rowCount(); row++)
    {
        QString value = table->model()->index(row, CValueMapDelegate::itemtype::value).data().toString();
        QString desc = table->model()->index(row, CValueMapDelegate::itemtype::desc).data().toString();

        if(value.isEmpty() && desc.isEmpty())
            continue;
        if(!value.isEmpty() && !desc.isEmpty())
            list.append(CValueMapManager::ValuePair(value, desc));
        else
            isInValid = true;
    }

    if(isInValid)
    {
        table->viewport()->update();
        return;
    }

    if( list.size() > 0 )
    {
        gValueMapManager->saveValueMap(m_environment, sInfo);
        gValueMapManager->saveFile(m_environment);
    }

    m_currentMapName = mapName;
    accept();
}

void CEditValueMapDialog::on_pushbuttonCancel_clicked()
{
    reject();
}

void CEditValueMapDialog::on_pushbuttonAdd_clicked()
{
    qint32 rowCount = ui->tableviewMap->model()->rowCount();
    ui->tableviewMap->model()->insertRow(rowCount);

    ui->tableviewMap->setImageButtonItem(rowCount);
}

void CEditValueMapDialog::on_pushbuttonDelete_clicked()
{
    ui->tableviewMap->deleteSelectedRows();
}

void CEditValueMapDialog::on_pushbuttonClearAll_clicked()
{
    ui->tableviewMap->model()->removeRows(0, ui->tableviewMap->model()->rowCount());
}

void CEditValueMapDialog::on_pushbuttonDeleteMap_clicked()
{
    QString mapName = ui->lineeditName->text();
    m_currentMapName = mapName;

    if( !mapName.isEmpty() )
        gValueMapManager->deleteValueMap(m_environment, mapName);


    gValueMapManager->saveFile(m_environment);
    accept();
}
