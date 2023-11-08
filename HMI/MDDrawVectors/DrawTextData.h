/*!
\author aGRw 2015.03.04
\section 文本控件,数据类
*/
#ifndef DRAWTEXTDATA_H
#define DRAWTEXTDATA_H

#include <QFont>
#include <QString>
#include <QTextOption>

#include "DrawObjDefine.h"
#include "MDColorManager/MDBrushManager.h"

class CDrawTextData
{
public:
    CDrawTextData();
public:
    QFont m_font;    //!字体
    QString m_text;   //!文本
    CMDBrushManager m_textBrushManager;   //!文本画笔
    QTextOption m_option;//!文本配置
    Qt::Alignment m_alignment;//!布局
    QTextOption::WrapMode m_wrapMode;//!换行
    bool m_isButtonFrame;//!是否绘制按钮边框
    bool m_isDownFrame;//!按钮边框是否是按下状态
    qint32 m_decimal;//!小数位数
    QString m_dateFormat;//!日期格式
    qint32 m_maxLength;//!字符串最大显示长度

    QString m_showText;//!翻译文本
    QString m_showDateFormat;//!翻译日期格式
/*!
\section common
*/
public:
    void operator=(const CDrawTextData& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
};

#endif // DRAWTEXTDATA_H
