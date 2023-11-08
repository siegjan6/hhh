#include "MDSysLogUIImp.h"
#include "MDSysLoggerDialog.h"
#include "MDSysLogger.h"
#include "MDStringResource.h"

CMDSysLogUIImp::CMDSysLogUIImp()
{
}

CMDSysLogUIImp::~CMDSysLogUIImp()
{
    closeAll();
}

QDialog* CMDSysLogUIImp::createModaless(CMDSysLogger* logger, const QString& title, QWidget *parent)
{
    CMDSysLoggerDialog* dialog = new CMDSysLoggerDialog(parent);
    if(dialog)
    {
        dialog->setCloseDialog(false);
        dialog->setModal(false);
        dialog->move(600, 500);

        QString realTitle = (title.isEmpty() ? logger->para().comment() : title);
        dialog->setWindowTitle(realTitle);
        dialog->setlogger(logger);
        m_loggerDialogList.append(dialog);
    }
    return dialog;
}

bool CMDSysLogUIImp::destroyModaless(QDialog* loggerDialog)
{
    int index = m_loggerDialogList.indexOf(loggerDialog);
    if(index != -1)
    {
        ((CMDSysLoggerDialog*)loggerDialog)->setCloseDialog(true);

        delete loggerDialog;
        //loggerWidget->deleteLater();

        m_loggerDialogList.removeAt(index);
    }

    return (index != -1);
}

void CMDSysLogUIImp::showModal(CMDSysLogger* logger, const QString& title, QWidget *parent)
{
    CMDSysLoggerDialog dialog(parent);
    QString realTitle = (title.isEmpty() ? logger->para().comment() : title);
    dialog.setlogger(logger);
    dialog.setWindowTitle(realTitle);
    dialog.move(600, 500);
    dialog.exec();
}

void CMDSysLogUIImp::showModaless(CMDSysLogger* logger, const QString& title, QWidget *parent)
{
    CMDSysLoggerDialog* dialog = (CMDSysLoggerDialog*)createModaless(logger, title, parent);
    dialog->show();
}

void CMDSysLogUIImp::closeAll()
{
    int size = m_loggerDialogList.size();
    for(int i = 0; i < size; i++)
    {
        CMDSysLoggerDialog* loggerDialog =  (CMDSysLoggerDialog*)m_loggerDialogList.at(i);
        loggerDialog->setCloseDialog(true);

        delete loggerDialog;
    }
    m_loggerDialogList.clear();
}
