#ifndef SELECTFROMLISTDLG_H
#define SELECTFROMLISTDLG_H

#include <QDialog>

namespace Ui {
class SelectFromListDlg;
}

class SelectFromListDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectFromListDlg(QWidget *parent = 0);
    ~SelectFromListDlg();

    bool SelectItem( const QString & s );
    void AddItem( const QString & s, int iIndex = -1 );
    QString GetSelectedItem() const;
    int GetSelectedItemIndex() const;
    bool IsInList( const QString & s ) const;

private:
    Ui::SelectFromListDlg *ui;
};

#endif // SELECTFROMLISTDLG_H
