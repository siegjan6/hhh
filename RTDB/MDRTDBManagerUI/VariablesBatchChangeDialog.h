#ifndef VARIABLESBATCHCHANGEDIALOG_H
#define VARIABLESBATCHCHANGEDIALOG_H

#include <QDialog>
class CPropertyBrowser;
class CMDRTDBBatchChangeVariable;
namespace Ui {
class CMDVariablesBatchChangeDialog;
}

class CMDVariablesBatchChangeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMDVariablesBatchChangeDialog(QWidget *parent = 0);
    ~CMDVariablesBatchChangeDialog();

    CPropertyBrowser* getPropertyBrowse();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CMDVariablesBatchChangeDialog *ui;
	
private:
    CPropertyBrowser *m_propertyBrowser;
};

#endif // VARIABLESBATCHCHANGE_H
