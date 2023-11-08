/*!
\author aGRw 2015.12.29
\section 字典配置对话框
*/
#ifndef DICTDIALOG_H
#define DICTDIALOG_H

#include <QDialog>
#include "DictEditDialog.h"
#include "GenerateDictDialog.h"
#include "LanguageDialog.h"
#include "DictionaryManager.h"

namespace Ui {
class CDictDialog;
}

class CDictDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CDictDialog(QWidget *parent = 0);
    virtual ~CDictDialog();
private slots:
    void on_listWidget_currentRowChanged(int currentRow);
private:
    Ui::CDictDialog *ui;

protected:
    void closeEvent(QCloseEvent * e);

public:
    void init(CDictionaryManager* manager);

private:
    CLanguageDialog* m_languageDialog;
    CDictEditDialog* m_systemDialog;
    CDictEditDialog* m_projectDialog;
    CGenerateDictDialog* m_generateDictDialog;
    CDictionaryManager* m_manager;
    int m_lastRow;
    bool m_initFinished;
};

#endif // DICTDIALOG_H
