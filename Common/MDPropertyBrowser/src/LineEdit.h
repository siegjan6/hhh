/*!

\author eHVr

\section 功能

 替代QLineEdit，相应控件失去焦点后，数据不丢失
*/
#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class CLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CLineEdit(QWidget *parent = 0);
Q_SIGNALS:
    void returnText(const QString &);
private Q_SLOTS:
    void returnTheText();
};

#endif // LINEEDIT_H
