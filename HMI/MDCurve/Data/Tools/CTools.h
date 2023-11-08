/*!
\author eHVr

\section 功能

工具类，进行 【放大】【缩小】等操作
*/
#ifndef CTOOLS_H
#define CTOOLS_H

#include <QRectF>
#include <QObject>
#include <QPushButton>
#include <QList>
#include <QMap>
#include <QDataStream>

class CToolButton;
class CXAxisManager;
class CYAxisManager;
class CCurveObjectManager;
class CPreviewCurve;

class QSpacerItem;
class QGridLayout;
class CTools:public QObject
{
    Q_OBJECT
public:
    CTools(QObject *parent  = 0);
    ~CTools();
public:
    void setIsStudioMode(bool isStudioMode);
    void setParentWidget(QWidget *parent);
    void calculate();
public:
    enum class NameId{
        Normal = 0,
        ZoomIn ,
        ZoomOut,
        ZoomSelect,
        Move,
        Run,
        Pause,
        Error
    };
/*!
\section  放大，缩小等操作
*/
public:
    bool buttonZoomIn();  //! 放大
    bool buttonZoomOut(); //! 缩小
    bool buttonNormal();  //! 常规
    void buttonSelectZoom(const QRect &select,const QRect &source);
    void buttonMove();
    void buttonRun();
private slots:
    void slotButtonClicked(int id);
signals:
    void run();
    void stop();
    void refresh();
private:
    enum {MaxIndex = 7};

    qint32  m_index;
    qint32  m_mulIndex;
    QVector<double> m_scales;
    QVector<double> m_mulScales;

public:
    CTools *clone(QWidget *parent);
    //! 序列化
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    void addButton(CToolButton *button);
    void deleteAllButtons();
/*!
\section
*/
public:
    void setXAxisManager(CXAxisManager *xs);
    void setYAxisManger(CYAxisManager *ys);
//    void setCurveManager(CCurveObjectManager *curves);
    void setManagers(CXAxisManager *xs,CYAxisManager *ys,CCurveObjectManager *curves);


    QList<QString> defaultbuttonNames() const;
    void setDefaultbuttonNames(const QList<QString> &defaultbuttonNames);

    QSize buttonSize() const;
    void setButtonSize(const QSize &buttonSize);

    QButtonGroup *buttonGroup() const;
    CToolButton *button(NameId id) const;
    CToolButton *button(const QString &name) const;

    bool isRun() const;
    void setIsRun(bool isRun);

    //! 按钮图标
    QMap<QString, QIcon> iconsMap() const;
    void setIconsMap(const QMap<QString, QIcon> &iconsMap);

private:
    CXAxisManager *m_xManager ;
    CYAxisManager *m_yManager ;
    CCurveObjectManager *m_curveManager ;

private:
    //! 按钮相关
    CToolButton *createButton(const QString &name)const;
    void createDefaultButtons(QWidget *parent = 0);
    void removeAllButtons();

    NameId nameToId(const QString &name);
    QString idToName(const NameId &id) const;

    void setLayout();
    void removeOldLayout();                  //! 移除之前的布局
    void insertDefaultButtonNames();
    void insertDefaultButtonIcons();

private:
    bool setXScale(double start,double end); //! 设置x轴放大比例
    bool setYScale(double start,double end); //! 设置Y轴放大比例
    void clearCurvePic();                    //! 清空之前的曲线图片
    void setCurveRunning(bool run);
private:
    QSize           m_buttonSize;
    QWidget        *m_parentWidget ;

    QList<QString>      m_defaultbuttonNames;
    QMap<QString,QIcon> m_iconsMap;

    QButtonGroup    *m_buttonGroup;
    QGridLayout     *m_layout;


    QSpacerItem     *m_vSpaceItem;
    QSpacerItem     *m_hSpaceItem;

private:
    bool  m_isRun;
    bool  m_isStudioMode;
    bool  m_isRealTime;
public:
    void setIsRealTime(bool isRealTime);
    void setPreviewCurve(CPreviewCurve *preview);
private:
    CPreviewCurve *m_previewCurve;

};

#endif // CTOOLS_H
