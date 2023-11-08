#include "mainwindow.h"
#include <QApplication>
#include <MDDBQueryConfigure.h>
#include "ViewListDialog.h"
#include "ViewFieldsDialog.h"
#include "SourceTreeDialog.h"
#include "RuntimeSourceDialog.h"
#include "NewFieldDialog.h"
#include "NewViewDialog.h"
#include "ViewVarEnumValueDialog.h"
#include "ViewVarTableDialog.h"
#include "ViewFieldTableDialog.h"
#include <QDebug>

namespace DBQUI
{
void SetConfigFilesDir(const QString & configFilesDir);
bool GetFieldsFromView(const QString & viewName, QList<DBQ::ViewField> & fieldListOut);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if 0
    ViewFieldTableDialog dlg;
    dlg.exec();
#endif


#if 0
    ViewVarTableDialog dlg;
    QString viewVarFullPath = "D:/Mind/MindSCADA/DBQuery/ConfigureTestCase/case2/view_t_employee.json";
    dlg.LoadViewVarTable(viewVarFullPath);
    dlg.creatTableView();
    dlg.exec();
#endif

#if 0
    DBQUI::SetConfigFilesDir("D:/Mind/MindSCADA/DBQuery/ConfigureTestCase/case2");
    ViewFieldsDialog vfDlg;
    vfDlg.LoadView("D:/Mind/MindSCADA/DBQuery/ConfigureTestCase/case2", "t_employee");
    vfDlg.exec();
    QList<DBQ::ViewField *> fields;
    vfDlg.GetFields(fields);
    for(int i = 0; i < fields.count(); i++)
    {
        qDebug()<<fields.at(i)->name;
    }
#endif

#if 0
    QString dataSrcFullPath = "D:/Mind/MindSCADA/DBQuery/UITestCase/case3/datasource.bin";
    SourceTreeDialog STDialog(dataSrcFullPath);
    STDialog.exec();


    STDialog.getFieldsSelected();

    qDebug()<< STDialog.getDBSouce().dbSrcName << "DBQ::DBSource  11111111111";

    for(int i = 0 ; i < STDialog.getFieldsSelected().count() ; i++)
    {
        qDebug()<< STDialog.getFieldsSelected().at(i)->name << "Table Fields  22222222222";
    }
    qDebug()<< STDialog.getViewField().name << "selectedField  333333333333";;

#endif

#if 0
    RuntimeSourceDialog dlg;
    dlg.exec();
#endif

#if 0
    NewFieldDialog dlg;
    if(dlg.exec() == QDialog::Accepted)
    {
        if(dlg.getField())
        {
            qDebug()<< "tableName" << dlg.getField()->tableName;
            qDebug()<< "name" << dlg.getField()->name;
            qDebug()<< "alias" << dlg.getField()->alias;
            qDebug()<< "type" << (int)dlg.getField()->type;
            qDebug()<< "description" << dlg.getField()->description;
        }
    }
#endif

#if 0
    NewViewDialog dlg;
    if(dlg.exec() == QDialog::Accepted)
    {
        qDebug()<< "tableName" << dlg.getViewName();
    }

#endif

#if 0
    ViewVarEnumValueDialog dlg;

    QList<DBQ::ViewVariableEnumValue *> m_enumValueList;
    DBQ::ViewVariableEnumValue *enumvalue1 = new DBQ::ViewVariableEnumValue;
    enumvalue1->value = QString("111111");
    enumvalue1->desc = QString("aaaaaaa");
    m_enumValueList.append(enumvalue1);
    DBQ::ViewVariableEnumValue *enumvalue2 = new DBQ::ViewVariableEnumValue;
    enumvalue2->value = QString("222222");
    enumvalue2->desc = QString("bbbbbbb");
    m_enumValueList.append(enumvalue2);
    DBQ::ViewVariableEnumValue *enumvalue3 = new DBQ::ViewVariableEnumValue;
    enumvalue3->value = QString("333333");
    enumvalue3->desc = QString("bbbbbbb");
    m_enumValueList.append(enumvalue3);

    dlg.setEnumValueList(m_enumValueList);
    dlg.setEnumValueType(DBQ::VIEW_FIELD_TYPE_INT);
    dlg.exec();
#endif

    //    QList<DBQ::ViewField> fieldList;
    //    DBQUI::GetFieldsFromView("t_employee", fieldList);
    //    MainWindow w;
    //    w.show();

#if 0
    ViewListDialog vlDlg;
    vlDlg.LoadViews("D:/Mind/MindSCADA/DBQuery/ConfigureTestCase/case1");
    vlDlg.exec();
    QStringList viewNames = vlDlg.GetViewNameSelected();
    qDebug()<<viewNames;
#endif

    //    DBQUI::SetConfigFilesDir("E:/NetSCADA6/code/DBQuery//ConfigureTestCase/case2");
    //    ViewFieldsDialog vfDlg;
    //    vfDlg.LoadView("E:/NetSCADA6/code/DBQuery/ConfigureTestCase/case2", "t_employee");
    //    vfDlg.exec();
    //    QList<DBQ::ViewField *> fields;
    //    vfDlg.GetFields(fields);
    return a.exec();
}
