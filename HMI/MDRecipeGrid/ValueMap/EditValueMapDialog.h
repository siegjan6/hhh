/*!
author Y2hvdWps 2016.10.19
section 数值映射表编辑对话框
*/
#ifndef EDITVALUEMAPDIALOG_H
#define EDITVALUEMAPDIALOG_H

#include <QDialog>
#include "IDrawEnvironment.h"
#include "ValueMapManager.h"

namespace Ui {
class CEditValueMapDialog;
}

class CEditValueMapDialog : public QDialog
{

    Q_OBJECT

public:
    explicit CEditValueMapDialog(IDrawEnvrionment *environment, QString mapName, QWidget *parent = 0);
    virtual ~CEditValueMapDialog();

    QString m_currentMapName;
protected:
    void showEvent(QShowEvent *);

signals:
    void doValidation();
private slots:
    void on_pushbuttonOk_clicked();

    void on_pushbuttonCancel_clicked();

    void on_pushbuttonAdd_clicked();

    void on_pushbuttonDelete_clicked();

    void on_pushbuttonClearAll_clicked();

    void on_pushbuttonDeleteMap_clicked();

private:
    Ui::CEditValueMapDialog *ui;
    IDrawEnvrionment *m_environment;
    CValueMapManager::ValueInfo m_info;
    QString m_srcMapName;

};

#endif // EDITVALUEMAPDIALOG_H
