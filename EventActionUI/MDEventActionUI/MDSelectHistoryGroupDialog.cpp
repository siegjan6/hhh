#include "MDSelectHistoryGroupDialog.h"
#include "ui_MDSelectHistoryGroupDialog.h"
#include "MDProjectConfiger.h"
#include "MDDataArchivesInterface/IMDDataGroupManager.h"
#include "MDDataArchivesInterface/IMDDataArchives.h"
#include "MDDataArchivesInterface/IMDDataGroup.h"

CMDSelectHistoryGroupDialog::CMDSelectHistoryGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDSelectHistoryGroupDialog)
{
    ui->setupUi(this);
}

CMDSelectHistoryGroupDialog::~CMDSelectHistoryGroupDialog()
{
    delete ui;
}

void CMDSelectHistoryGroupDialog::initial(CMDProjectConfiger* projectConfiger)
{
    QStringList groupList;
    int count = projectConfiger->dataArchives()->dataGroupManager()->hisDataGroupCount();
    for(int i = 0; i < count; i++)
    {
        IMDDataGroup* hisGroup = projectConfiger->dataArchives()->dataGroupManager()->getHisDataGroup(i);
        groupList.append(hisGroup->name());
    }
    ui->historyGroupListWidget->addItems(groupList);

    setWindowTitle("选择历史组");
}

void CMDSelectHistoryGroupDialog::on_OKButton_clicked()
{
    QListWidgetItem* item = ui->historyGroupListWidget->currentItem();
    if(item)
        m_selectedGroup = item->text();

    setResult(QDialog::Accepted);
    accept();
}

void CMDSelectHistoryGroupDialog::on_cancelButton_clicked()
{
    setResult(QDialog::Rejected);
    reject();
}
