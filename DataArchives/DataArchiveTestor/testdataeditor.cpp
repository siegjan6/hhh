#include "TestDataEditor.h"
#include "ui_testdataeditor.h"
#include "testdatagroupmgr.h"

TestDataEditor::TestDataEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestDataEditor)
{
    ui->setupUi(this);
}

TestDataEditor::~TestDataEditor()
{
    delete ui;
}

void TestDataEditor::SetTestDataFileName(QString testDataFileName)
{
    m_TestDataFileName = testDataFileName;
    QString fileContentOut;
    if(!TestDataGroupMgr::GetInst()->GetCase(m_TestDataFileName, fileContentOut))
    {
        //TODO:终止程序
        return ;
    }

    ui->datalEdit->setPlainText(fileContentOut);
}

void TestDataEditor::on_saveBtn_clicked()
{
    QString fileContent = ui->datalEdit->toPlainText();
    TestDataGroupMgr::GetInst()->SaveCase(m_TestDataFileName, fileContent);
}
