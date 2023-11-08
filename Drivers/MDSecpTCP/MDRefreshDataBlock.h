#ifndef MDREFRESHDATABLOCK_H
#define MDREFRESHDATABLOCK_H

#include <QObject>
#include<QTimer>

class CMDDataBlockDialog;
class MDRefreshDataBlock : public QObject
{
    Q_OBJECT
public:
    explicit MDRefreshDataBlock(CMDDataBlockDialog *pDataBlockDialog,QObject *parent = 0);
    ~MDRefreshDataBlock();
public:
    CMDDataBlockDialog *m_pDataBlockDialog;
    QTimer *pTimer;

signals:

public slots:
    void getTagTableStatu();

};

#endif // MDREFRESHDATABLOCK_H
