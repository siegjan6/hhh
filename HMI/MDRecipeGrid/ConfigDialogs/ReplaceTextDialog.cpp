#include "ReplaceTextDialog.h"
#include "ui_ReplaceTextDialog.h"
#include "ShareTool.h"

CReplaceTextDialog::CReplaceTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CReplaceTextDialog)
{
    ui->setupUi(this);

    CShareTool::installDialogTranslator(this);
}

CReplaceTextDialog::~CReplaceTextDialog()
{
    delete ui;
}

void CReplaceTextDialog::accept()
{
    m_src = ui->lineeditSrc->text();
    m_replace = ui->lineeditReplace->text();

    QDialog::accept();
}
