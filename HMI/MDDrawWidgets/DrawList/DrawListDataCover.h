//by hdp 2015.03.09
#ifndef CDRAWLISTDATACOVER_H
#define CDRAWLISTDATACOVER_H
//////////////////////////////////////////////////////////////////////////
#include <QListWidget>
#include <QColor>
#include <QFont>
#include <QObject>
#include <QEvent>

//////////////////////////////////////////////////////////////////////////
class CDrawListData;
class CMDDrawList;
/**
 * @brief 提供对底层数据的封装
 * 功能:为了能够实现对list控件的事件过滤和自定义事件的发出
 *
 */
class CDrawListDataCover :public QObject
{
    Q_OBJECT
public:
    CDrawListDataCover(QObject *parent = 0);
    ~CDrawListDataCover();

public:
    QListWidget *widget() const;
    void setWidget(QListWidget *widget);

    qint32 version() const;
    void setVersion(const qint32 &version);

    /*!
     \brief 控件是否显示滚动条
            目前仅设置垂直方向滚动条
    */
    bool isShowScrollBar() const;
    void setIsShowScrollBar(bool isShowScrollBar);

    /*!
     \brief 控件启用时背景色属性设置
    */
    QColor backColor() const;
    void setBackColor(const QColor &backColor);

    /*!
     \brief 控件不启用时背景色属性设置
    */
    QColor disableBackColor() const;
    void setDisableBackColor(const QColor &disableBackColor);

    /*!
     \brief 控件字体设置
    */
    QFont font() const;
    void setFont(const QFont &font);

    /*!
     \brief 控件外形属性值设置
    */
    qint32 borderStyle() const;
    void setBorderStyle(const qint32 &borderStyle);

    /*!
     \brief 控件子项的选择模式设置
    */
    qint32 selectionMode() const;
    void setSelectionMode(const qint32 &selectionMode);

    /*!
     \brief 控件阴影属性设置
    */
    qint32 frameShadow() const;
    void setFrameShadow(const qint32 &frameShadow);


    /*!
     \brief 控件子项设置
    */
    QStringList items() const;
    void setItems(const QStringList &items);
public:

    /*!
     \brief 控件子项的相关操作,添加,删除,清理,
       查询(从指定索引开始查找字符串,错误返回-1)
    */
    void addItem(int index, const QString &str);
    void removeItem(int index);
    void cleareAllItem();
    int findItem(int index, const QString &str);
public:
    CMDDrawList *list() const;
    void setList(CMDDrawList *list);
private:
    void selChanged();
protected:

    /*!
     \brief 事件过滤器
        用于过滤控件的enable事件
    */
    bool eventFilter(QObject *obj, QEvent *event);
private:
    CDrawListData *m_data = nullptr;
    CMDDrawList *m_list = nullptr;
};

#endif // CDRAWLISTDATACOVER_H


