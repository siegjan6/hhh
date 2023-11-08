/*!
\author Y2hhaWhq

\section 功能

编辑框控件数据类
*/
#ifndef DRAWTEXTEDITDATA_H
#define DRAWTEXTEDITDATA_H

#include <QTextEdit>
#include <QJsonObject>
#include "DrawObjDefine.h"

class CMDDrawTexEditEvent;

class CDrawTextEditData
{
public:
    CDrawTextEditData();
    ~CDrawTextEditData(){}

private:
    QTextEdit* m_textEdit;                      //! 编辑框
    CMDDrawTexEditEvent* m_event;               //! 事件绑定类
    bool m_horizontalScrollBar;                 //! 水平滚动条，缺省:无
    bool m_verticalScrollBar;                   //! 垂直滚动条，缺省:无
    QFrame::Shape m_borderStyle;                //! 边框样式，缺省：无
    QTextEdit::LineWrapMode m_lineWrapMode;     //! 换行模式
    bool m_readOnly;                            //! 等于true时只读
    Qt::Alignment m_alignment;                  //! 文本对齐方式，缺省：左对齐
    qint32 m_selectStart;                       //! 选中文本起始位置
    qint32 m_selectLength;                      //! 选中文本长度
    QString m_selectText;                       //! 选中文本
    QString m_text;                             //! 文本
    qint32 m_VarMaxLength;                      //! 变量值的最大长度
    qint32 m_decimal;                           //! 小数位数
    QString m_dateFormat;                       //! 日期格式
    bool m_writeVarEnable;                      //! 是否可写入变量
    bool m_enterSetVarEnable;                   //! 回车赋值
    bool m_hasPassword;                         //! 是否显示密码

/*!
\section  序列化
*/
public:
    void operator=(const CDrawTextEditData& other);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);


    QTextEdit *textEdit() const;
    void setTextEdit(QTextEdit *textEdit);

    CMDDrawTexEditEvent *event() const;
    void setEvent(CMDDrawTexEditEvent *event);


    inline bool horizontalScrollBar(){return m_horizontalScrollBar;}
    inline void setHorizontalScrollBar(bool horizontalScrollBar){m_horizontalScrollBar = horizontalScrollBar;}

    inline bool verticalScrollBar(){return m_verticalScrollBar;}
    inline void setVerticalScrollBar(bool verticalScrollBar){m_verticalScrollBar = verticalScrollBar;}

    inline QFrame::Shape borderStyle(){return m_borderStyle;}
    inline void setBorderStyle(const QFrame::Shape &borderStyle){m_borderStyle = borderStyle;}

    inline QTextEdit::LineWrapMode lineWrapMode(){return m_lineWrapMode;}
    inline void setLineWrapMode(const QTextEdit::LineWrapMode &lineWrapMode){m_lineWrapMode = lineWrapMode;}

    inline bool readOnly(){return m_readOnly;}
    inline void setReadOnly(bool readOnly){m_readOnly = readOnly;}

    inline Qt::Alignment alignment(){return m_alignment;}
    inline void setAlignment(const Qt::Alignment &alignment){m_alignment = alignment;}

    inline qint32 selectStart(){return m_selectStart;}
    inline void setSelectStart(const qint32 &selectStart){m_selectStart = selectStart;}

    inline qint32 selectLength() const{return m_selectLength;}
    inline void setSelectLength(const qint32 &selectLength){m_selectLength = selectLength;}

    inline QString selectText(){return m_selectText;}
    inline void setSelectText(const QString &selectText){m_selectText = selectText;}

    inline QString text(){return m_text;}
    inline void setText(const QString &text){m_text = text;}

    inline qint32 VarMaxLength(){return m_VarMaxLength;}
    inline void setVarMaxLength(const qint32 &VarMaxLength){m_VarMaxLength = VarMaxLength;}

    inline qint32 decimal(){return m_decimal;}
    inline void setDecimal(const qint32 &decimal){m_decimal = decimal;}

    inline QString dateFormat(){return m_dateFormat;}
    inline void setDateFormat(const QString &dateFormat){m_dateFormat = dateFormat;}

    inline bool writeVarEnable(){return m_writeVarEnable;}
    inline void setWriteVarEnable(bool writeVarEnable){m_writeVarEnable = writeVarEnable;}

    inline bool enterSetVarEnable(){return m_enterSetVarEnable;}
    inline void setEnterSetVarEnable(bool enterSetVarEnable){m_enterSetVarEnable = enterSetVarEnable;}

    inline bool hasPassword(){return m_hasPassword;}
    inline void setHasPassword(bool hasPassword){m_hasPassword = hasPassword;}
};

#endif // DRAWTEXTEDITDATA_H
