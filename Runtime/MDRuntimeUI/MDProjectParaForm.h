/*!

\author dGFuZ3p3

\section 功能

项目参数界面类，设置与项目有关的参数，如启用安全保护、项目安全策略等。

*/

#ifndef MDPROJECTPARAFORM_H
#define MDPROJECTPARAFORM_H

#include <QWidget>
#include "MDActionSP.h"

namespace Ui {
class CMDProjectParaForm;
}

class CMDProjectConfiger;
class QTableWidgetItem;
class CMDProjectParaForm : public QWidget
{
    Q_OBJECT

public:
    explicit CMDProjectParaForm(QWidget *parent = 0);
    ~CMDProjectParaForm();
public:
    bool initial(CMDProjectConfiger* configer);
    bool accept();
protected:
    void tableWidgetDoubleClicked(QTableWidgetItem* item);
    void updateTableWidget();
    CMDActionSPList m_destList;

private slots:
private:
    Ui::CMDProjectParaForm *ui;
    CMDProjectConfiger* m_configer;
};

#endif // MDPROJECTPARAFORM_H
