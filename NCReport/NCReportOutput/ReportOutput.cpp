#include "ncreport.h"
#include "ncreportdef.h"
#include "ncreportxmldefreader.h"
#include "ncreportprinteroutput.h"
#include "ncreportpdfoutput.h"
#include "ncreportpreviewoutput.h"
#include "info.h"
#include "ncreportpreviewwindow.h"
#include "ncreportutils.h"
#include "ReportOutput.h"
#include "../../include/MDProjectExpert/IMDProjectExpert.h"
#include "../include/MDDBQueryUI/MDDBQueryUI.h"
//#include "MDDBQueryUI\MDDBQueryUI.h"

#include <QMessageBox>
#include <QMap>
#include <QDir>
#include <QDebug>
#include <QPen>
#include <QPainter>
#include <QMenu>
#include <QMovie>


CNCReportOutput::CNCReportOutput(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f),
      m_pDBQueryEngine(0)
{
    m_XMLfileName = "";
    m_reportbackcolor = Qt::gray;
    m_XMLfilePath ="";
    m_IDrawEnvrionment = NULL;
    palette = Qt::gray;
    m_NCReportFilePath = "";
    m_version = 0;
    strText = "";

    m_mainLayout = new QVBoxLayout();
    //m_filterManager = new CMDFilterTreeDataManager();

    m_report = new NCReport();
    m_priviewWindow = new NCReportPreviewWindow();
    // 报表预览
    m_mainLayout->addWidget(m_priviewWindow);
    m_priviewWindow->hide();
    this->setLayout(m_mainLayout);
    //窗体属性设置
    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint |Qt::FramelessWindowHint);
    //    this->installEventFilter(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotEditMenu(QPoint)));

    strText = QString(tr("..."));
    m_mvLabel = new QLabel(this);
    movie = new QMovie(":/query1.gif");
    m_mvLabel->setMovie(movie);
    movie->setScaledSize(QSize(80,80));
    m_mainLayout->addWidget(m_mvLabel,0,Qt::AlignCenter);
    movie->start();

    m_transTool = CMDMultiLanguage::createWidgetTool();
    m_transTool->init(this);

    //<添加右键菜单>
    m_pPopupMenu = new QMenu("", this);

    m_pSetQueryParameterAction = new QAction("设置参数", this);
    connect(m_pSetQueryParameterAction, SIGNAL(triggered()), this, SLOT(slot_SetQueryParameter()));
    m_pPopupMenu->addAction(m_pSetQueryParameterAction);

    m_pQueryAction = new QAction("查询", this);
    connect(m_pQueryAction, SIGNAL(triggered()), this, SLOT(slot_Query()));
    m_pPopupMenu->addAction(m_pQueryAction);
    //</添加右键菜单>
    m_pPopupMenu->setVisible(false);
}

CNCReportOutput::~CNCReportOutput()
{
    if(movie)
    {
        delete movie;
        movie = NULL;
    }
    if(m_mvLabel)
    {
        delete m_mvLabel;
        m_mvLabel = NULL;
    }
    if(m_mainLayout)
    {
        delete m_mainLayout;
        m_mainLayout = NULL;
    }
    if(m_report)
    {
        delete m_report;
        m_report = NULL;
    }
    if(m_priviewWindow)
    {
        delete m_priviewWindow;
        m_priviewWindow = NULL;
    }
}

/*!
\section 设置报表文件路径
*/
void CNCReportOutput::setReportPath(const QString &strPath)
{
    m_XMLfilePath = strPath;
}

/*!
\section 获取报表文件路径
*/
QString CNCReportOutput::getReportPath() const
{
    return m_XMLfilePath;
}

/*!
\section 设置报表文件名
*/
void CNCReportOutput::setXMLfileName(const QString &XMLfileName)
{
    m_XMLfileName = XMLfileName;
}

/*!
\section 获取报表文件名
*/
QString CNCReportOutput::getXMLfileName() const
{
    return m_XMLfileName;
}

void CNCReportOutput::setReportParameter(const QString &key, const QString &value)
{
    if ( !m_report )
    {
        return;
    }
    //m_report->reportDef()->addParameter(key, value);
    m_report->addParameter(key, value);
}

QString CNCReportOutput::getReportParameter(const QString &key)
{
    if ( !m_report )
    {
        return QString::null;
    }
//    return m_report->reportDef().getParameter(key);
    return QString::null;
}

void CNCReportOutput::setReportLanguage(const QString &langcode)
{
    if ( !m_report )
    {
        return;
    }
    m_report->setCurrentLanguage(langcode);
}

QString CNCReportOutput::getReportLanguage()
{
    if ( !m_report )
    {
        return QString::null;
    }
    return m_report->currentLanguage();
}

void CNCReportOutput::setQueryEngine(IMDDBQueryEngine * pEngine)
{
    m_pDBQueryEngine = pEngine;
}

/*!
\section 设置报表文件背景色
*/
void CNCReportOutput::setReportBackColor(const QColor &color)
{
    m_reportbackcolor = color;
    update();
}

/*!
\section 查询报表文件背景色
*/
QColor CNCReportOutput::getReportBackColor()
{
    return m_reportbackcolor;
}

/*!
\section 设置项目管理参数
*/
void CNCReportOutput::setIDrawEnvrionment(IDrawEnvrionment *IDrawEnvrionment)
{
    m_IDrawEnvrionment = IDrawEnvrionment;

    if(!m_IDrawEnvrionment->isStudioMode())
    {
        if(m_IDrawEnvrionment->projectExpert())
        {
            m_pDBQueryEngine = m_IDrawEnvrionment->projectExpert()->queryEngine();
        }
    }
}

/*!
\section 获取项目管理参数指针
*/
IDrawEnvrionment *CNCReportOutput::getIDrawEnvrionment() const
{
    return m_IDrawEnvrionment;
}


/*!
\section 运行报表输出文件
*/
void CNCReportOutput::runNCReport(QString filename)
{
    if(m_priviewWindow)
    {
        m_priviewWindow->hide();
        m_mvLabel->show();
        movie->start();
    }
    // 创建报表对象
    if ( !m_report )
    {
        return;
    }
    m_report->reset();
    // 设置报表数据源
    m_report->setReportSource(NCReportSource::File);
    m_report->clearBatch();
    m_report->setReportFile(filename);

    // 创建报表输出
    m_report->setDBQueryEngine(m_pDBQueryEngine);
    m_report->runReportToPreview();
    bool error = m_report->hasError();
    QString err = m_report->lastErrorMsg();
    // 错误处理
    if ( error )
    {
        QMessageBox::information(0, tr("Report error"), err);
    }
    else
    {
        m_priviewWindow->setOutput((NCReportPreviewOutput*)(m_report->output()));
        m_priviewWindow->setReport(m_report);
        m_priviewWindow->setWindowModality(Qt::ApplicationModal);
        m_priviewWindow->setAttribute(Qt::WA_DeleteOnClose);
        m_priviewWindow->setParent(this);
        m_priviewWindow->show();
        m_mvLabel->hide();
        movie->stop();
    }
}

void CNCReportOutput::exportTo(const QString& fileName, const QString& password/* = ""*/, const QString& perm/* = ""*/)
{
    if(m_priviewWindow)
    {
        m_priviewWindow->exportTo(fileName, password/* = ""*/, perm/* = ""*/);
    }
}

void CNCReportOutput::serialize(QJsonObject &json)
{
    QJsonObject js;

    js.insert("XMLfileName",m_XMLfileName);
    js.insert("Reportbackcolor",(qreal)m_reportbackcolor.rgba());
    json.insert("NCReportOutput",js);
}

void CNCReportOutput::deserialize(const QJsonObject &json)
{
    QJsonObject NCReportOutputJson = json.value("NCReportOutput").toObject();

    m_XMLfileName = NCReportOutputJson.value("XMLfileName").toString();
    m_reportbackcolor = QColor::fromRgba(NCReportOutputJson.value("Reportbackcolor").toDouble());
}


/*!
\section 显示事件处理
*/
void CNCReportOutput::showEvent(QShowEvent *event)
{
    if(getIDrawEnvrionment())
    {
        if(!getIDrawEnvrionment()->isStudioMode())
        {
            if(getXMLfileName().isEmpty())
            {
                return;
            }
            //   setIsRunTime(true);
        }
    }
    QWidget::showEvent(event);
}

/*!
\section 绘制报表输出控件背景
*/
void CNCReportOutput::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen     pen;
    QFont    font;
    font.setFamily("normal");
    font.setPointSize(12);
    font.setBold(false);
    pen.setColor(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setFont(font);
    //    painter.drawText(rect(), Qt::AlignCenter,QString(strText));
    painter.drawText(rect(), Qt::AlignCenter,CMDMultiLanguage::instance()->value(strText));
    //painter.drawPixmap(rect(), movie->currentPixmap());
    painter.drawRect(QRect(0,0,this->width(),this->height()));

    this->setAutoFillBackground(true);
    this->palette.setColor(QPalette::Window, QColor(getReportBackColor()));
    this->setPalette(palette);

    QWidget::paintEvent(event);
}

/*!
\section 获取报表文件目录
*/
QString CNCReportOutput::getNCReportFilePath()
{
    if (!m_NCReportFilePath.isEmpty())
    {
        return m_NCReportFilePath;
    }
    QString projectPath = getIDrawEnvrionment()->projectPath();
    if(projectPath.isEmpty())
    {
        qDebug()<<"getIDrawEnvrionment()->projectPath()"<<getIDrawEnvrionment()->projectPath();
        return m_NCReportFilePath;
    }
    m_NCReportFilePath += projectPath;
    if(m_NCReportFilePath.right(0) != "/")
    {
        m_NCReportFilePath += "/";
    }
    m_NCReportFilePath += "Report";
    m_NCReportFilePath += "/";
    //    m_NCReportFilePath = m_NCReportFilePath+QString("BatchReport.xml");
    m_NCReportFilePath = m_NCReportFilePath+m_XMLfileName/*QString("test.xml")*/;

    return m_NCReportFilePath;
}

void CNCReportOutput::loadFilter(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QDataStream ar(&file);
    int count = 0;
    ar >> count;
    for (int i = 0; i < count; ++i)
    {
        //m_filterManager->load(&ar);
    }
}

void CNCReportOutput::slotEditMenu(const QPoint &pos)
{
    if(getIDrawEnvrionment())
    {
        if(!getIDrawEnvrionment()->isStudioMode())
        {
            //m_pPopupMenu->exec(mapToGlobal(pos));
//            //鼠标右击事件
//            m_report->reportSource().setReportDef(m_report->reportDef());
//            m_report->reportDef()->reset( false );

//            m_report->setReportSource(NCReportSource::File);
//            QString filename = getNCReportFilePath();
//            if(!filename.right(4).endsWith(".xml",Qt::CaseSensitive))
//            {
//                qDebug()<<"Load ReportFile Failed!";
//                return;
//            }
//            qDebug()<<"Load ReportFile Succeed!";
//            m_report->setReportFile(filename);
//            runNCReport(getNCReportFilePath());
        }
    }
}

void CNCReportOutput::slot_SetQueryParameter()
{
    DBQUI::CallStandardRumtimeParmatersConfigUI(m_pDBQueryEngine);
}

void CNCReportOutput::slot_Query()
{
    if(!getIDrawEnvrionment())
        return ;

        if(getIDrawEnvrionment()->isStudioMode())
            return ;

        //鼠标右击事件
        m_report->reportSource().setReportDef(m_report->reportDef());
        m_report->reportDef()->reset(false);

        m_report->setReportSource(NCReportSource::File);
        QString filename = getNCReportFilePath();
        if(!filename.right(4).endsWith(".xml",Qt::CaseSensitive))
        {
            //qDebug()<<"Load ReportFile Failed!";
            return;
        }
        //qDebug()<<"Load ReportFile Succeed!";
        m_report->setReportFile(filename);

        runNCReport(getNCReportFilePath());
}

void CNCReportOutput::changeLanguage()
{
    m_transTool->changeLanguage();
    CMDMultiLanguage::instance()->value(strText, false);
}

void CNCReportOutput::collectWords()
{
    CMDMultiLanguage::instance()->addWord(strText, false);
}
