#ifndef MDGAUGE_H
#define MDGAUGE_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include "IDrawEnvironment.h"
#include "IHMIForm.h"
class CDrawGauge;

#if defined(MDGAUGE_LIBRARY)
#  define MDGAUGE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDGAUGE_SHARED_EXPORT Q_DECL_IMPORT
#endif

class  MDGAUGE_SHARED_EXPORT CMDGauge
{
public:
    CMDGauge();
    ~CMDGauge();
public:
    CMDGauge &operator = (const CMDGauge & other);
    //设置控件框架指针
    void setDrawEnvrionment(IDrawEnvrionment* value);
    void setParent(IHMIForm *parent);

    //序列化和反序列化
public:
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(const QJsonObject& json);
    //设置大小
    void setRect(const QRect& value);
    //绘图
    void paint(QPainter* p);
    bool openDesign();
    //设置指针值
    void setValue(qint32 index, qreal value);
    //设置初始索引值
    void setInitialIndex(qint32 value);
    //获取表达式和与之对应的索引
    void getExpressionList(QList<qint32> &indexs, QList<QString>& expressions);
    //双击打开配置界面
    void onDoubleClick();
    //翻译
    void changeLanguage();
    void collectWords();
private:
    CDrawGauge *m_gauge;
};

#endif // MDGAUGE_H
