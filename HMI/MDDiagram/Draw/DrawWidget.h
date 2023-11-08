/*!
\section 统计图绘图类
*/
#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H
#include <QWidget>
#include "DrawData.h"
#include "IDrawEnvironment.h"
class CDrawVernier;
class QPrinter;
namespace Ui {
class CDrawWidget;
}

class CDrawWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDrawWidget(QWidget *parent = 0);
    ~CDrawWidget();

private:
    Ui::CDrawWidget *ui;
public:
    CDrawData* m_drawData;
private:
    CDrawVernier *m_vernier;
    QRectF m_rect; //! 整个区域
    QRectF m_paintRect; //! 绘图区域
    QList<QString>m_stringList1;
    QList<QString>m_stringList2;
    QList<QString>m_stringList3;
    QList<qreal>m_valueList;
    QList<QColor>m_colorList;
    QList<QRectF>m_itemRectList; //! 绘制统计图每一类别的矩形绘图区域
    QList<QRectF>m_pillarList; //! 绘制的柱子
    /*!
     \section 三维直方图数据
    */
    QRectF m_drawRect;
    QRectF m_interRect;
    QList<QRectF>m_drawRectList;
    QList<QRectF>m_interRectList;
public:
    void setRect(const QRectF &rect); //! 设置绘图区域
    void setString1(QString string); //! 获取统计类别(一分组)
    void setString2(QString string); //! 获取统计类别(二分组)
    void setString3(QString string); //! 获取统计类别(三分组)
    void setValue(QString valueString); //! 获取统计数据
protected:
    void initialize();
    void mouseMoveEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent*);
    void  resizeEvent(QResizeEvent *e);

private:
    void drawItem(QPainter* paint);
    void drawToolBar(QPainter *paint);  //! 绘制工具栏
    void drawDescribeString(QPainter *p, QString str, QFont font,int position, int arrange); //! 绘制标题
    void drawAxis(QPainter *paint); //! 绘制坐标轴
    QRectF drawAxisRect(const QRectF &rect); //! 获取绘制坐标轴的矩形区域
    void drawClicent(QPainter *paint);

/*!
 \section 绘制图例
*/
    void drawType(QPainter* p);
    QRectF drawTypeRect(const QRectF &rect); //! 绘制图例的矩形区域
    qint32 getTypeWidth(); //! 获取图例的总宽度
    qint32 getTypeHight(); //! 获取图例的总高度

private slots:
    void on_histogramBtn_clicked();
    void on_histogram2Btn_clicked();
    void on_histogram3DBtn_clicked();
    void on_histogram3D2Btn_clicked();
    void on_preprintBtn_clicked();
    void on_printBtn_clicked();
    void on_printSetBtn_clicked();

    /*!
     \section 打印
    */
public:
    void doPrint();
    void doPrintPreview();
    void setUpPage();
    void createPdf();

public:
    CDrawWidget &operator =(const CDrawWidget &other);  //! 拷贝
    /*!
    \section 序列化
    */
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    bool openDesign();  //! 配置界面

    /*!
    \section print
    */
public:
    //!打印边框参数
    qint32 m_printTop;
    qint32 m_printBottom;
    qint32 m_printLeft;
    qint32 m_printRight;
    qint32 m_printHeader;
    qint32 m_printFooter;
    QString m_printLeftHeader;
    QString m_printMiddleHeader;
    QString m_printRightHeader;
    QString m_printLeftFooter;
    QString m_printMiddleFooter;
    QString m_printRightFooter;
    QString m_printDateFormat;
    QString m_printTimeFormat;
private:
    QString m_showPrintLeftHeader;
    QString m_showPrintMiddleHeader;
    QString m_showPrintRightHeader;
    QString m_showPrintLeftFooter;
    QString m_showPrintMiddleFooter;
    QString m_showPrintRightFooter;
    QString m_showPrintDateFormat;
    QString m_showPrintTimeFormat;
private slots:
    void print(QPrinter* printer);
private:
    void setPrintMargins(QPrinter* printer);
    void printConfig();
    void printFun(QPrinter* printer, const QRect& rect);
    //!将打印自定义字符串转换为实际字符串
    QString replacePrintString(const QString& source, int count, int page);

public:
    void changeLanguage();
    void collectWords();

};

#endif // DRAWWIDGET_H
