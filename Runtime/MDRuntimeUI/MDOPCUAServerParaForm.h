/*!

\author bosmutus

\section 功能

项目OPCUAServer参数界面类

*/

#ifndef CMDOPCUASERVERPARAFORM_H
#define CMDOPCUASERVERPARAFORM_H

#include <QWidget>
#include "MDActionSP.h"

namespace Ui {
class CMDOPCUAServerParaForm;
}

class CMDProjectConfiger;
class QTableWidgetItem;
class CMDOPCUAServerParaForm : public QWidget
{
    Q_OBJECT

public:
    explicit CMDOPCUAServerParaForm(QWidget *parent = 0);
    ~CMDOPCUAServerParaForm();
public:
    bool initial(CMDProjectConfiger* configer);
    bool accept();
protected:
    void tableWidgetDoubleClicked(QTableWidgetItem* item);
    void updateTableWidget();
    CMDActionSPList m_destList;

private slots:
private:
    Ui::CMDOPCUAServerParaForm *ui;
    CMDProjectConfiger* m_configer;
};

#endif // CMDOPCUASERVERPARAFORM_H
