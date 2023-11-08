#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QPainter>
#include <QRegExp>
#include <QDebug>
#include <QJsonObject>
#include "PropertyBrowser.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_objectController(0),
    m_showController(0),
    m_controlledObject(0),
    m_test(0),
    m_subTest(0)
{
    ui->setupUi(this);
    initUI();  
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_test;
    delete m_subTest;
}

void MainWindow::initUI()
{
    m_test = new CTest;
    m_objectController = new CPropertyBrowser(this);
    m_showController = new CPropertyBrowser(this);
    m_subTest = new CSubTest;


//    m_controlledObject = m_test;

    connect(m_objectController,&CPropertyBrowser::propertyFinish,this,&MainWindow::setHide);
    m_objectController->setObject(m_test);
    m_showController->setObject(m_test);

    connect(m_test,&CTest::idChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::mileChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::nameChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::DILdemoChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::dateChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::timeChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::dateTimeChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::keySequenceChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::characterChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::localeChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::pointChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::pointFChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::sizeChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::sizeFChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::rectChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::rectFChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::colorChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::fontChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::derectionChanged,[=](){m_showController->updateProperty();});
    connect(m_test,&CTest::locationChanged,[=](){m_showController->updateProperty();});
//    connect(m_subTest,SIGNAL(salaryChange),this,SLOT(m_showController->updateProperty());
    connect(m_test,&CTest::idChanged,this,&MainWindow::setDynamicProperty);


    // enum attribute #0  set the icon
    QtIconMap enumIcons;
    enumIcons[0] = QIcon(":/new/prefix1/object/images/up.png");
    enumIcons[1] = QIcon(":/new/prefix1/object/images/down.png");
    enumIcons[2] = QIcon(":/new/prefix1/object/images/left.png");
    enumIcons[3] = QIcon(":/new/prefix1/object/images/right.png");
    m_objectController->setPropertyAttribute(QString("derection"),QString("enumIcons"),QVariant::fromValue(enumIcons));

    // id attribute #1
    m_objectController->setPropertyAttribute(QString("id"),QString("maximum"),21);
    m_objectController->setPropertyAttribute(QString("id"),QString("minimum"),3);
    m_objectController->setPropertyAttribute(QString("id"),QString("singleStep"),3);
    // double attribute #2
    m_objectController->setPropertyAttribute(QString("mile"),QString("maximum"),45);
    m_objectController->setPropertyAttribute(QString("mile"),QString("minimum"),5);
    m_objectController->setPropertyAttribute(QString("mile"),QString("singleStep"),5);
    m_objectController->setPropertyAttribute(QString("mile"),QString("decimals"),3);
    // string attribute #3  match strings without whitespace
    m_objectController->setPropertyAttribute(QString("name"),QString("regExp"),QRegExp("^\\S+$"));
    // date attribute #4
    m_objectController->setPropertyAttribute(QString("date"),QString("maximum"),QDate(2014,12,24));
    m_objectController->setPropertyAttribute(QString("date"),QString("minimum"),QDate(2000,12,24));
    // pointF attribute #5
    m_objectController->setPropertyAttribute(QString("pointF"),QString("decimals"),3);
    // size attribute #6
    m_objectController->setPropertyAttribute(QString("size"),QString("maximum"),QSize(100,100));
    m_objectController->setPropertyAttribute(QString("size"),QString("minimum"),QSize(10,10));
    // sizeF attribute #7
    m_objectController->setPropertyAttribute(QString("sizeF"),QString("minimum"),QSizeF(10,10));
    m_objectController->setPropertyAttribute(QString("sizeF"),QString("maximum"),QSizeF(100,100));
    m_objectController->setPropertyAttribute(QString("sizeF"),QString("decimals"),3);
    // rect attribute #8
    m_objectController->setPropertyAttribute(QString("rect"),QString("constraint"), QRect(1,2,3,4));
    // rectF attribute #9
    m_objectController->setPropertyAttribute(QString("rectF"),QString("decimals"),3);
    m_objectController->setPropertyAttribute(QString("rectF"),QString("constraint"),QRectF(1,2,3,4));

    createDock();

}

void MainWindow::createDock()
{
    ui->dockWidget_3->setWidget(m_objectController);
    ui->dockWidget_3->setWindowTitle(tr("属性栏"));
    ui->dockWidget_4->setWidget(m_showController);
    ui->dockWidget_4->setWindowTitle(tr("显示属性更改"));
}

void MainWindow::setDynamicProperty()
{
    QStringList strList;
    strList<<"address"<<"id"<<"mile"<<"name";
    m_objectController->setPropertyReadOnly(true,strList);
}

//! 属性加载完毕即隐藏某个属性
void MainWindow::setHide()
{
    m_objectController->setSinglePropertyHide("address",true);
}

void MainWindow::on_changeBlueBtn_clicked()
{
    if(!m_objectController->object())
        return;

    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QLinearGradient line(0,0,img.width(),img.height());
    line.setColorAt(0.2,Qt::white);
    line.setColorAt(0.6,Qt::blue);
    line.setColorAt(1.0,Qt::black);

    painter.fillRect(0,0,img.width(),img.height(),QBrush(line));
    painter.end();

    QPixmap icon = QPixmap::fromImage(img);
    m_objectController->setPropertyPixmap("DILdemo",icon);
}

void MainWindow::on_changeGreenBtn_clicked()
{
    if(!m_objectController->object())
        return;
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    QLinearGradient line(0,0,img.width(),img.height());
    line.setColorAt(0.2,Qt::white);
    line.setColorAt(0.6,Qt::green);
    line.setColorAt(1.0,Qt::black);

    painter.fillRect(0,0,img.width(),img.height(),QBrush(line));
    painter.end();

    m_objectController->setPropertyPixmap("DILdemo",QPixmap::fromImage(img));
}

void MainWindow::on_setHideBtn_clicked()
{
    //! 隐藏组名则该组全隐藏
    m_objectController->setSinglePropertyHide("GUID_Pro",true);
    //! 隐藏多个属性属性
    QStringList list;
    list<<"address"<<"id"<<"mile";
    m_objectController->setPropertyHide(list,true);
    //! 隐藏单一属性
    m_objectController->setSinglePropertyHide("DILdemo",true);
}

void MainWindow::on_setShowBtn_clicked()
{
    //! 复现一个组
    m_objectController->setSinglePropertyHide("GUID_Pro",false);
    //! 复现多个属性
    QStringList list;
    list<<"address"<<"id"<<"mile";
    m_objectController->setPropertyHide(list,false);
    //! 复现一个属性
    m_objectController->setSinglePropertyHide("DILdemo",false);
}
//! 只读属性背景为灰色，去掉 "Non Writable" 的描述
void MainWindow::on_setROColorBtn_clicked()
{
    QColor color = QColor(Qt::darkGray);
    m_objectController->setReadOnlyBackColor(color);
}

//! 增加属性描述区,tip信息
void MainWindow::on_setTipBtn_clicked()
{
    QString name = "name";
    QString tip = "该属性tip已设置！";
//    m_objectController->setPropertyTip(name,tip);
}

//! 指定属性的背景色
void MainWindow::on_setBackColorBtn_clicked()
{
    QColor color = QColor(Qt::blue);
    QString str = "name";
    m_objectController->setPropertyBackColor(str,color);
}

//! 设置整体只读
void MainWindow::on_SetAllROBtn_clicked()
{
    m_objectController->setPropertyReadOnly();
}

void MainWindow::on_reSetAllROBtn_clicked()
{
    m_objectController->setPropertyReadOnly(false);
}

//! 动态特征(一个属性修改后，某些属性变为只读)
void MainWindow::on_setDynamicBtn_clicked()
{
    QStringList strList;
    strList<<"address"<<"id"<<"mile"<<"name";
    m_objectController->setPropertyReadOnly(true,strList);
}

void MainWindow::on_reSetDynamicBtn_clicked()
{
    QStringList strList;
    strList<<"address"<<"id"<<"mile"<<"name";
    m_objectController->setPropertyReadOnly(false,strList);
}
//! 动态设置枚举值
void MainWindow::on_insertEnumsBtn_clicked()
{
    QString str = "location_W";
    QStringList list;
    list<<"111"<<"222"<<"east"<<"south"<<"west"<<"north";
    m_objectController->resetEnums(str,list);
}

void MainWindow::on_expandAllBtn_clicked()
{
    m_objectController->setExpandAll();
    //! 设置属性的tip
    //    QMap<QString,QVariant>map;
    //    map.insert("address","地址属性tip已设置！");
    //    map.insert("id","编号属性tip已设置！");
    //    map.insert("name","姓名属性tip已设置！");
    //    map.insert("GUID_Base","基础属性tip已设置！");
    //    m_objectController->setPropertyTipMap(map);
}
