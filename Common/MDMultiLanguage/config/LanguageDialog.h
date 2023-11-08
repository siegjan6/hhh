/*!
\author aGRw 2015.12.30
\section 语言配置对话框
*/
#ifndef LANGUAGEDIALOG_H
#define LANGUAGEDIALOG_H

#include <QDialog>

class CDictionaryManager;

namespace Ui {
class CLanguageDialog;
}

class CLanguageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CLanguageDialog(QWidget *parent = 0);
    virtual ~CLanguageDialog();
private:
    Ui::CLanguageDialog *ui;

public:
    void init(CDictionaryManager* manager);
    void save();
private:
    void resetSelectIndexs();
    void cellChanged(int row, int column);
    int defaultIndex(bool isSystem);
    void resetComboBoxIndex(int oldIndex, bool isSytem);
    void resetComboBoxItem();

private:
    CDictionaryManager* m_manager;
    QList<int> m_selectIndexs;
};

#endif // LANGUAGEDIALOG_H
