#include "cdialogtest.h"
#include "ui_cdialogtest.h"

CDialogTest::CDialogTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialogTest)
{
    ui->setupUi(this);
    m_nsRTDBManager = CMDRTDBManagerFactory::createRTDBManager();
}

CDialogTest::~CDialogTest()
{
    delete ui;
}

void CDialogTest::on_pushButton_clicked()
{/*/
    qint8 m_receiveData2=200;

    unsigned short dataLength = (unsigned short)m_receiveData2;
    dataLength = (unsigned short)(quint8)m_receiveData2;
    quint16 dataLength2= m_receiveData2;
    quint16 dataLength3= (quint16)m_receiveData2;
    quint16 dataLength4= (quint16)(quint8)m_receiveData2;
    dataLength4= (quint16)(quint8)m_receiveData2;
    /*/
/*/
    unsigned char m_receiveData[10] = {0};
    unsigned char m_receiveData2=200;
    m_receiveData[8] =200;
    unsigned short dataLength = (unsigned short)m_receiveData[8];
     dataLength = (unsigned short)m_receiveData[8];
    dataLength = (unsigned short)m_receiveData2;
    dataLength = (unsigned short)m_receiveData2;
     dataLength = (unsigned short)(unsigned char)m_receiveData2;
      dataLength = (unsigned short)(unsigned char)m_receiveData2;
/*/
    m_nsRTDBManager->loadProject("C:/test/");
    m_nsRTDBManager->start();

}

void CDialogTest::on_pushButton_2_clicked()
{
   // QVariant vt = m_nsRTDBManager->readVariableValue("G2.V4");
     QVariant vt = m_nsRTDBManager->readVariableValue("G1.G11.fV1");
     ui->lineEdit->setText(vt.toString());

}

void CDialogTest::on_pushButton_3_clicked()
{
    QVariant vt(ui->lineEdit->text());
    //m_nsRTDBManager->writeVariableValue("G2.V4",vt) ;
    m_nsRTDBManager->writeVariableValue("G1.G11.fV1",vt) ;
}
