/*!
\author bWVpeGw=

\section 功能

编辑框控件类
*/
#ifndef MDDRAWTEXTEDIT_H
#define MDDRAWTEXTEDIT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QTextEdit>
#include <QJsonObject>
#include "MDDrawBasicWidget.h"

class CDrawTextEditData;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawTextEdit : public CMDDrawBasicWidget
{
public:
    CMDDrawTextEdit();
    virtual ~CMDDrawTextEdit();

private:
    CDrawTextEditData* m_data;
/*!
\section 属性函数
*/
public:
    //! 控件
    QTextEdit* widget();
    //! 水平滚动条
    void setHorizontalScrollBar(bool value);
    bool horizontalScrollBar();
    //! 垂直滚动条
    void setVerticalScrollBar(bool value);
    bool verticalScrollBar();
    //! 边框样式
    void setBorderStyle(QFrame::Shape value);
    QFrame::Shape borderStyle();
    //! 换行模式
    void setLineWrapMode(QTextEdit::LineWrapMode value);
    QTextEdit::LineWrapMode lineWrapMode();
    //! 只读
    void setReadOnly(bool value);
    bool readOnly();
    //! 文本对齐方式
    void setAlignment(Qt::Alignment value);
    Qt::Alignment alignment();
    //! 文本
    void setText(const QString& value);
    QString text();
    //! 选中文本起始位置
    void setSelectStart(int value);
    int selectStart();
    //! 选中文本长度
    void setSelectLength(int value);
    int selectLength();
    //! 选中文本
    void setSelectText(QString &value);
    QString selectText();
    //! 变量最大字符长度
    void setVarMaxLength(int value);
    int varMaxLength();
    //! 小数位数
    void setDecimal(int value);
    int decimal();
    //! 日期格式
    void setDateFormat(const QString& value);
    QString dateFormat();
    //! 控制点，是否可写入变量
    void setWriteVarEnable(bool enable);
    bool writeVarEnable();
    //! 回车赋值
    void setEnterSetVarEnable(bool enable);
    bool enterSetVarEnable();
    //! 是否使用密码
    void setHasPassword(bool enable);
    bool hasPassword();
    //! 格式字符串
    QString textFormatString();
    //! 返回编辑框数据类指针
    CDrawTextEditData *data() const;

private:
    QTextCursor cursor;

/*!
\section 控件对象属性
*/
public:
    //! 返回控件类型名
    DrawType type();
    QString className(){return "CMDDrawTextEdit";}
    //! 返回属性框对象
    CMDPropertyDrawObj *createPropertyDrawObj();
    //! 初始化
    void onInitialization();
protected:
    //! 创建自身
    CMDDrawObj *createSelf();
/*!
\section 序列化
*/
public:
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

/*!
\section 绑定相关变量
*/
public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);
    void writeVariable();
    void handlePassWordChar(QString &password);
    void handleHasPassword();
    void handleEventWriteVarNoPassword();
    QString getMaskString(int count);

/*!
\section batch edit
*/
public:
   virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
   //初始化类信息
   virtual void initClassInfo(QStringList &classInfo);
};

#endif // MDDRAWTEXTEDIT_H
