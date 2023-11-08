/*!
\section 统计图属性配置框
*/
#ifndef DESIGNFORM_H
#define DESIGNFORM_H

#include <QDialog>
#include "DrawData.h"

class QListWidgetItem;
class CDrawWidget;
namespace Ui {
class CDesignForm;
}

class CDesignForm : public QDialog
{
    Q_OBJECT

public:
    explicit CDesignForm(QWidget *parent = 0);
    ~CDesignForm();
private:
    Ui::CDesignForm *ui;

private:
    CDrawData* m_designData;
public:
    CDrawWidget* m_drawWidget;
public:
    void inintDesign();
    void setDesignData(CDrawData* data);
    CDrawData* getData();
    void setDiagramStyle(qint32 style);
    void setStrPosition(qint32 position);
    void setStrArrange(qint32 arrange);

private:
    void setButtonColor(QPushButton* button,QColor color);
private slots:
    void setStackWidget(QListWidgetItem* item);
    void positionSelect(qint32 index);
    void showToolBar(qint32 index);
    void clientEdge(qint32 isVisible);
    void isSetYvalue(qint32 num);
    void diagramCategorySelect(qint32 num);
    void on_OKBtn_clicked();
    void on_cancelBtn_clicked();
    void on_applyBtn_clicked();
    void on_backColorBtn_clicked();
    void on_interColorBtn_clicked();
    void on_lineColorBtn_clicked();
    void on_diagramFontBtn_clicked();
    void on_valueFontBtn_clicked();
    void on_discribeFontBtn_clicked();
    void on_topBtn_clicked();
    void on_bottomBtn_clicked();
    void on_leftBtn_clicked();
    void on_middleBtn_clicked();
    void on_rightBtn_clicked();
    void on_histogramBtn_clicked();
    void on_histogram2Btn_clicked();
    void on_describeColorBtn_clicked();
    void on_histogram3DBtn_clicked();
    void on_histogram3D2Btn_clicked();
};

#endif // DESIGNFORM_H
