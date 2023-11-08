/*!

\author eHVr

\section 功能

    单一曲线特征标签属性设置界面
*/
#ifndef CFEATURETAGPROPERTYSETTINGDIALOG_H
#define CFEATURETAGPROPERTYSETTINGDIALOG_H

#include <QDialog>
#include "CTagPosition.h"

class CPoint;
class QComboBox;
class CRealFTag;
class CFeatureTag;
class CHistoryTagManager;
class CFeatureTagLine;
class CRealTimeTagManager;
class ITranslateWidgetTool;

namespace Ui {
class CFeatureTagPropertySettingDialog;
}

class CFeatureTagPropertySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFeatureTagPropertySettingDialog(QWidget *parent = 0);
    ~CFeatureTagPropertySettingDialog();

private:
    Ui::CFeatureTagPropertySettingDialog *ui;
public:
    void setData(CFeatureTag *featureTag, const CTagPos &tagPos);
    void setData(CRealFTag *realTag, const CTagPos &tagPos);

    CTagPos tagPos() const;
private:
    CFeatureTag          *m_historyTag;  //! 历史标签
    CRealFTag            *m_realTag;     //! 实时标签
    CTagPos               m_tagPos;
    bool                  m_isReal;
/*!
\section 初始化
*/
public:
    void initialize();
private:
    void initializeData();
    void initializeUI();
    void initializeHPos(const CTagHPos &hPos);
    void initializeVPos(const CTagVPos &vPos);
    void initializeLine(const CFeatureTagLine *tagLine);
    void initializeTopPoint(const CPoint *topPoint);
    void initializeBottomPoint(const CPoint *bottomPoint);
/*!
\section 应用内容更改
*/
public:
    bool applyChanges();
private:
    void applyHPosChanges(CTagHPos &pos);
    void applyVPosChanges(CTagVPos &pos);
    bool applyLineChanges(CFeatureTagLine *line);
    bool applyTopPointChanges(CPoint *point);
    bool applyBottomPointChanges(CPoint *point);

/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();

private slots:
    void slotSetLineColor();
    void slotSetTopPointColor();
    void slotSetBottomPointColor();
/*!
\section tools functions
*/
private:
    void setColor(QColor &initialColor, QPushButton &colorButton);
    void setLineStyle(QComboBox *comboBox);
    void setPointStyle(QComboBox *comboBox);
private:
    QColor m_lineColor;               //! 线颜色
    QColor m_topPointColor;           //! 顶部点颜色
    QColor m_bottomPointColor;        //! 底部点颜色
    int    m_lineStyle;               //! 线型
    int    m_topPointStyle;           //! 顶部点样式
    int    m_bottomPointStyle;        //! 底部点样式
private:
    ITranslateWidgetTool *m_transTool; //! 多语言

};


#endif // CFEATURETAGPROPERTYSETTINGDIALOG_H
