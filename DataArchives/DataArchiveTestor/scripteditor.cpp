#include "ScriptEditor.h"
#include "ui_scripteditor.h"
#include "scriptcasegroupmgr.h"
#include "JSScriptExcutor.h"

ScriptEditor::ScriptEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptEditor)
{
    ui->setupUi(this);
}

ScriptEditor::~ScriptEditor()
{
    delete ui;
}

void ScriptEditor::SetCaseName(QString caseName)
{
    m_CaseName = caseName;
    QString contentOut;

    if(!ScriptCaseGroupMgr::GetInst()->GetCase(m_CaseName, contentOut))
    {
        //TODO:终止程序
        return ;
    }

    ui->scriptEdit->setPlainText(contentOut);
}

void ScriptEditor::on_saveBtn_clicked()
{
    QString fileContent = ui->scriptEdit->toPlainText();
    ScriptCaseGroupMgr::GetInst()->SaveCase(m_CaseName, fileContent);
}

//!运行脚本
void ScriptEditor::on_runBtn_clicked()
{
    //在一个线程中执行脚本
    JSScriptExcutor::GetInst()->PushScriptCaseName(m_CaseName);

}
