#include "TestorDialog.h"
#include "ui_TestorDialog.h"
#include <IMDDataArchives.h>
#include <IMDDataGroupManager.h>
#include<MDDataArchivesFactory.h>
#include <IMDDataSourceManager.h>
#include "RuntimeFilterDialog.h"

static IMDDataArchives  * pDataArchives = 0;
IMDDBQueryEngine * GetGlobalDBQueryEngine();

TestorDialog::TestorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestorDialog)
{
    ui->setupUi(this);
}

TestorDialog::~TestorDialog()
{
    delete ui;
}

void TestorDialog::on_dbSourceTreeButton_clicked()
{
    if(!pDataArchives)
    {
        //加载数据源文件 生成数据源管理对象
        pDataArchives = CMDDataArchivesFactory::createDataArchives();
        QString dataSrcFullPath =  "D:/NetSCADA6/code/DBQuery/UITestCase/case3/datasource.bin";
        pDataArchives->dataSourceManager()->load(dataSrcFullPath);
        pDataArchives->dataSourceManager()->save(dataSrcFullPath);
    }
}

void TestorDialog::on_runtimeFilterButton_clicked()
{
    RuntimeFilterDialog dlg;
    dlg.setDBQueryEngine(GetGlobalDBQueryEngine());
    dlg.exec();
}
