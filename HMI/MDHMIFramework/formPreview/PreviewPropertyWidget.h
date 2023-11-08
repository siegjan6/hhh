/*!
\author aGRw 2016.04.05
\section 属性配置窗口
*/
#ifndef PREVIEWPROPERTYWIDGET_H
#define PREVIEWPROPERTYWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "FormObjectContainer.h"

namespace Ui {
class PreviewPropertyWidget;
}

class CPreviewPropertyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPreviewPropertyWidget(CFormObjectContainer* container, QWidget *parent = 0);
    ~CPreviewPropertyWidget();
private:
    Ui::PreviewPropertyWidget *ui;

public:

private slots:
    void on_checkBoxTransparent_clicked();
    void on_checkBoxAutoJoin_clicked();
    void on_lineEditFrameWidth_editingFinished();
    void on_lineEditFrameHeight_editingFinished();
    void on_lineEditLeft_editingFinished();
    void on_lineEditTop_editingFinished();
    void on_lineEditWidth_editingFinished();
    void on_lineEditHeight_editingFinished();

private:
    void init();
    void rectChanged();
    void formEditingFinished();
    void frameEditingFinished();

private:
    CFormObjectContainer* m_container;
/*!
\section test
*/
public:
    QLineEdit* lineEditLeft();
    QLineEdit* lineEditTop();
    QLineEdit* lineEditWidth();
    QLineEdit* lineEditHeight();

};

#endif // PREVIEWPROPERTYWIDGET_H
