#include <QFile>
#include <qtextstream.h>
#include <QFileDialog>
#include <QDir>
#include "DatablockMonitorDialog.h"
#include "ui_DatablockMonitorDialog.h"
#include "MDMultiLanguage.h"//多语言
CDataBlockMonitorDialog::CDataBlockMonitorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDataBlockMonitorDialog)
{
    ui->setupUi(this);



    m_DataBlock = NULL;
    m_Monitoring = false;
   this->setWindowTitle(tr("数据块监控"));

    //多语言对象
    CMDMultiLanguage::instance()->translateWidget(this);
    m_translateTool = CMDMultiLanguage::createWidgetTool();
    m_translateTool->init(this);

    //Qt::WindowFlags cc = (Qt::WindowFlags)(Qt::CustomizeWindowHint |  Qt::WindowTitleHint &~ Qt::WindowCloseButtonHint );
    //setWindowFlags((Qt::WindowFlags)cc);
   //    setWindowFlags(~Qt::WindowCloseButtonHint);
//setWindowFlags(windowFlags()  | );
   // setWindowFlags(Qt::WindowTitleHint);
       //setWindowFlags(Qt::WindowCloseButtonHint); //只要关闭按钮

      // setWindowFlags(Qt::WindowFlags type)

      // Qt::FrameWindowHint:没有边框的窗口

       //Qt::WindowStaysOnTopHint:  //总在最上面的窗口

       //Qt::CustomizeWindowHint:  //自定义窗口标题栏,以下标志必须与这个标志一起使用才有效,否则窗口将有默认的标题栏

       //Qt::WindowTitleHint  //显示窗口标题栏
    //响应对话框自带关闭按钮事件
    connect(this,SIGNAL(rejected()),this,SLOT(on_closeDlgBtn_clicked()));
}

CDataBlockMonitorDialog::~CDataBlockMonitorDialog()
{
    delete ui;
}
//设置新数据块时，清除已有内容
void CDataBlockMonitorDialog::setMonitoredDataBlock(IDataBlock* dataBlock)
{
    QMutexLocker locker(&m_dataBlockMutex);
    //取消原数据块的监控标志
    if(m_DataBlock != NULL)
    {
        if(m_DataBlock != dataBlock)
        {
            m_DataBlock->setMsgWindow(this);
            m_DataBlock->setMsgMonitor(false);
            //清空显示
            ui->packetlistWidget->clear();
        }
    }

    dataBlock->setMsgWindow(this);

    dataBlock->setMsgMonitor(true);
    dataBlock->setMonitorMode(IDataBlock::MDMonitorType::Binary);
    m_DataBlock = dataBlock;
    ui->dataBlockNameEdit->setText(m_DataBlock->name());
    m_Monitoring = true;
    ui->pauseMonitorBtn->setText(tr("暂停"));
}

//修改数据包监控格式
void CDataBlockMonitorDialog::on_monitorAscii_stateChanged(int arg1)
{
    QMutexLocker locker(&m_dataBlockMutex);
    if(m_DataBlock != NULL)
    {
        if(Qt::Checked == arg1)
            m_DataBlock->setMonitorMode(IDataBlock::MDMonitorType::ASCII);
        else
            m_DataBlock->setMonitorMode(IDataBlock::MDMonitorType::Binary);
    }
}
//!收到数据包
void CDataBlockMonitorDialog::customEvent(QEvent *e)
{
    if(e->type() == QEvent::User+2000)
    {
        CPacketEvent *dataBlockEvent = static_cast<CPacketEvent *>(e);
        if(dataBlockEvent != NULL)
        {
            QString packet  = dataBlockEvent->m_DataBlockPacket;
            packet += "\n";
            QMutexLocker locker(&m_dataBlockMutex);
            QListWidgetItem* newpacket = new QListWidgetItem(packet);
            ui->packetlistWidget->addItem(newpacket);
            ui->packetlistWidget->setCurrentItem(newpacket);
        }
    }
}
void CDataBlockMonitorDialog::changeLanguage()
{
    m_translateTool->changeLanguage();
}

void CDataBlockMonitorDialog::on_clearListWidgetBtn_clicked()
{
    QMutexLocker locker(&m_dataBlockMutex);
    //清空显示
    ui->packetlistWidget->clear();
}

void CDataBlockMonitorDialog::on_exportPacketBtn_clicked()
{
    //选择对话框
    QString filename = QFileDialog::getSaveFileName(
       this, "Open Document", QDir::currentPath(),
       "csv files (*.csv )");
    if (!filename.isNull())
    {
        //用户选择了文件
        QFile filein(filename);
        if(!filein.open(QIODevice::ReadWrite|QIODevice::Append |QIODevice::Text))
            return;

        QTextStream out(&filein);
        QListWidgetItem* lineitem;
        int nLine = ui->packetlistWidget->count();
        for ( int i = 0; i<nLine; i++)
        {
            lineitem = ui->packetlistWidget->item(i);
            if(lineitem != NULL)
                out<< lineitem->text()<< "\n";
        }
        out.flush();
        filein.close();
    }
}

void CDataBlockMonitorDialog::on_pauseMonitorBtn_clicked()
{
    if(m_Monitoring)
    {
        m_DataBlock->setMsgMonitor(false);
        ui->pauseMonitorBtn->setText(tr("继续"));
        m_Monitoring = false;
    }
    else
    {
        m_DataBlock->setMsgMonitor(true);
        ui->pauseMonitorBtn->setText(tr("暂停"));
        m_Monitoring = true;
    }
}

void CDataBlockMonitorDialog::on_closeDlgBtn_clicked()
{
    if(m_DataBlock != NULL)
    {
            m_DataBlock->setMsgMonitor(false);
            m_DataBlock = NULL;
    }
    this->setResult(QDialog::Accepted);
    this->accept();
}
