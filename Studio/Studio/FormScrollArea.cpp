#include "FormScrollArea.h"
#include "BaseForm.h"

CFormScrollArea::CFormScrollArea(QWidget *parent)
    : QScrollArea(parent)
{

}
CFormScrollArea::~CFormScrollArea()
{
    CBaseForm* f = dynamic_cast<CBaseForm*>(widget());
    if(f != NULL)
        f->close();
}
void CFormScrollArea::addWidget(QWidget *w)
{
    setWidget(w);
    setWindowTitle(w->windowTitle());
    connect(w, &QWidget::windowTitleChanged, this, &CFormScrollArea::childTitleChanged);
}
void CFormScrollArea::childTitleChanged(const QString &title)
{
    setWindowTitle(title);
}
