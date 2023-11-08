#include "LineEdit.h"

CLineEdit::CLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this,SIGNAL(returnPressed()),this,SLOT(returnTheText()));
    connect(this,SIGNAL(editingFinished()),this,SLOT(returnTheText()));
}

void CLineEdit::returnTheText()
{
     emit returnText(text());
}
