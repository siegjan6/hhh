/*!
\author bWVpeGw=
\section 外观属性配置框
*/
#ifndef APPEARANCEFORM_H
#define APPEARANCEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "DataGroupData.h"

namespace Ui {
class CAppearanceForm;
}
/*!
\section 外观属性配置类
*/
class CAppearanceForm : public QWidget
{
    Q_OBJECT

public:
    explicit CAppearanceForm(QWidget *parent = 0);
    ~CAppearanceForm();

private:
    Ui::CAppearanceForm *ui;

/*!
\section 初始化
*/
public:
    void initialize();
private:
    void initUI();      //! 界面初始化
    void loadFields();  //! 字段设置

/*!
\section 信号槽连接
*/
private:
    void creatConnection();

private slots:
    void slotCheckBox();
    void slotFrontColorComboBox(const QString& currentText);
    void slotBackColorComboBox(const QString& currentText);

    void slotRowFontButton();
    void slotColFontButton();
    void slotDataFontButton();

    void slotRowColorButton();
    void slotRowForeColorButton();
    void slotColColorButton();
    void slotColForeColorButton();
    void slotDataColorButton();
    void slotTabColorButton();
    void slotCornerColorButton();

    void slotRowHeaderWidthEdit();
    void slotColHeaderHeightEdit();
    void slotHorizontalBarEdit();
    void slotVerticalBarEdit();

/*!
\section 设置字段数据
*/
public:
    void setPropertyData(CDataGroupBasicData* value);
    CDataGroupBasicData* propertyData();

    void setFields(QList<CDataGroupField> *value);

private:
    CDataGroupBasicData* m_propertyData;
    QList<CDataGroupField> m_fields;

/*!
\section 属性设置函数
*/
private:
    void setFont (QFont &initialFont);
    void setColor(QPushButton &button,QColor &color);
    void setButtonColor(QPushButton &button, QColor &initialColor);
    void setLineEdit(QLineEdit &line, qint32 &value);
};

#endif // APPEARANCEFORM_H
