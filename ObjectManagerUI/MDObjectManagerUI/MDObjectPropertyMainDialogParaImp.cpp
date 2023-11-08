#include "MDObjectPropertyMainDialogParaImp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QListWidget>
#include "MDStringResource.h"

CMDObjectPropertyMainDialogParaImp::CMDObjectPropertyMainDialogParaImp()
{
    m_configer = NULL;
    m_sourceProperty = NULL;
    m_destProperty = NULL;

    m_listWidget = NULL;
    m_stackWidget = NULL;

    okButton = NULL;
    cancleButton = NULL;

    m_twTool = NULL;
}

void CMDObjectPropertyMainDialogParaImp::initialUI(QWidget* mainDialog)
{
    m_listWidget =new QListWidget(mainDialog);

    m_stackWidget =new QStackedWidget(mainDialog);
    m_stackWidget->setFrameStyle(QFrame::Panel|QFrame::Raised);

    QHBoxLayout *mainLayout =new QHBoxLayout();
    mainLayout->setMargin(1);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(m_listWidget);
    mainLayout->addWidget(m_stackWidget);
    mainLayout->setStretchFactor(m_listWidget,2);
    mainLayout->setStretchFactor(m_stackWidget,10);

    cancleButton=new QPushButton(STRING_CANCEL);
    okButton=new QPushButton(STRING_OK);

    QHBoxLayout *BtnLayout =new QHBoxLayout();
    BtnLayout->addStretch(1);
    BtnLayout->addWidget(cancleButton);
    BtnLayout->addWidget(okButton);

    QVBoxLayout *RightLayout =new QVBoxLayout(mainDialog);
    RightLayout->setMargin(5);
    RightLayout->setSpacing(5);
    RightLayout->addLayout(mainLayout);
    RightLayout->addLayout(BtnLayout);

    mainDialog->connect(okButton, SIGNAL(clicked()), mainDialog, SLOT(on_okButton_clicked()));
    mainDialog->connect(cancleButton, SIGNAL(clicked()), mainDialog, SLOT(on_cancelButton_clicked()));
    mainDialog->connect(m_listWidget,SIGNAL(currentRowChanged(int)),m_stackWidget,SLOT(setCurrentIndex(int)));

    int width = 450+100;
    int height = 450+50;
    mainDialog->resize(width,height); //设置窗体固定大小
}
