/*!
\author emhhbmdsZQ==
\section  绘制控制层列表。
*/

#ifndef DRAWGAUGE_H
#define DRAWGAUGE_H
#include <QDataStream>
#include "DrawData.h"
#include "DrawItem.h"
#include "IDrawEnvironment.h"
#include "IHMIForm.h"

class CDesignDialog;
class CDrawGauge
{
public:
    CDrawGauge();
    ~CDrawGauge();
public:
/*!
\section  拷贝。
 */
    CDrawGauge *clone() const;
    CDrawGauge &operator= (const CDrawGauge &other);
/*!
\section  序列化。
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    void draw(QPainter* paint);      //!  绘制全部
    void draw(QPainter* paint, const QRect& rect, QList<CDataLayer*>* pLayerList = NULL, bool isSquare = false);  //!  绘制全部,isSquare判断是否按正方形来绘制图像
    void draw(QPainter* paint, const QRect& rect, CData* data, QList<CDataLayer*>* pLayerList = NULL);    //!  绘制一项
//! 当前值
//   double getValue(int index);

    bool openDesign();    //!  打开配置界面
    QRect getRect(const QRect& rect);  //! 根据给定矩形获取正方形Rect
    bool needRefresh();   //!  阻尼有效，需要重画
    void setClientRect(const QRect& rcClient);    //! 设置绘图区域
    QRegion* getRegion();  //! 获取绘图区域

public:
    QList<CDataLayer*> m_layer;   //!  控制层列表
    bool m_redraw;  //!  是否重画
    //是否在设计期内
//    bool m_isDesign;
    double m_proportion;  //!  m_rect的宽/高比例

private:
    /*!
      \section 绘制控制层。
    */
    void drawLayer(QPainter* painter, const QRect& rect, CDataLayer* layer = NULL, CData* data = NULL, qint32 drawMode = 0);
    /*!
         获取经控制层缩放移动的坐标。
         \param isSquare 获取的坐标是否是正方形。
         \returns 经过转换的QRectF。
     */
    QRectF getLayerRectF(const QRect& rect, CDataLayer* layer, bool isSquare = true);
    QRect getProportionRect(const QRect& rect);  //!  获取设置了宽高比的rect,输入的rect默认是正方形
    double getProportion();    //!  获取m_rect的宽高比
    void initialize();   //!  绘制初始表盘
private:
    CDrawItem m_drawItem;  //!  绘图类
    bool m_isLoad;   //!  是否首次载入
    qint32 m_count;  //!  索引数组长度
    /*!
      \section 获取的绘图区域。
    */
    QRect m_rect;
    QRegion m_rgn;
    QList<CDataValue*> m_valueList;   //!  指针和显示值数据列表
    QRegion m_moveRgn;   //!  指针等会移动的组件包含region
    bool m_bhasNeedle;  //!  判断之前是否有指针等可移动区域

public:
    void setDrawEnvrionment(IDrawEnvrionment* value);
public:
    IDrawEnvrionment* m_environment;
    QRect regionRect();
    /*!
      \section 获取父窗体。
    */
public:
    void setParent(IHMIForm *parent);
    IHMIForm * m_parent;
public:
    void setValue(qint32 index, qreal value);  //!   设置指针值
    void setInitialIndex(qint32 value);  //!  设置初始索引值
    void getExpressionList(QList<qint32>& indexs, QList<QString>& expressions);  //!  获取表达式和与之对应的索引

private:
    qint32 m_initialIndex ;  //!   初始索引
    QList<CDataValue*> m_dataValueList;  //!  指针列表

public:
    void changeLanguage();
    void collectWords();

};

#endif // DRAWGAUGE_H
