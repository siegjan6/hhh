#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QStylePainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter *p = new QPainter(this);
    p->setPen(m_myPenMgr.pen(ui->widget->rect()));
    QPainterPath *path = new QPainterPath;
    if((int)m_myBrushManager.brushType() == 5 && m_myBrushManager.m_position ==1)
    {
        QRectF rect = ui->widget->rect();
        path->moveTo(rect.x(),rect.y());
        path->lineTo(rect.x()+rect.width(),rect.y());
        path->lineTo(rect.x()+rect.width(),rect.y()+rect.height());
        path->lineTo(rect.x(),rect.y()+rect.height());
        path->lineTo(rect.x(),rect.y());
        m_myBrushManager.draw(p,rect,path);
        m_myPenMgr.draw(p,rect,path);
    }
    else
    {
        p->setBrush(m_myBrushManager.brush(ui->widget->rect()));
        p->drawRect(ui->widget->rect());
    }

}

void MainWindow::on_pushButton_clicked()
{
    m_myBrushManager.setProjectPath("/home/hite/NetSCADA6/HMI/MDColorManager/ColorManagerTest");
    m_myBrushManager.showDialog();
}

void MainWindow::on_pushButton_2_clicked()
{
//    CMDPenSelectDialog* dlg = new CMDPenSelectDialog();
//    dlg->setPenManager(m_myPenMgr);
//    if(dlg->exec() == QDialog::Accepted)
//    {
//        m_myPenMgr = dlg->penManager();
//        update();
//    }
//    delete dlg;

    m_myPenMgr.setProjectPath("/home/hite/NetSCADA6/HMI/MDColorManager/ColorManagerTest");
    m_myPenMgr.showDialog();
}

