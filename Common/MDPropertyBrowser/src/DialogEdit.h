/*!

\author eHVr

\section 功能

    扩展属性的编辑控件，用于显示属性图片和字符值
*/
#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QLineEdit>
#include <QLabel>
#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class CDialogEditPrivate;
class CDialogEdit : public QWidget
{
    Q_OBJECT

public:
    CDialogEdit(QWidget *parent = 0);
    ~CDialogEdit();

    void setValue(const QString &value);
    QString value() const;

    QLineEdit *lineEdit();
    QLabel    *label();

public Q_SLOTS:
    void setIcon(const QPixmap &pixmap);
Q_SIGNALS:
    void valueChanged(const QString &);
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void paintEvent(QPaintEvent *);
private:
    CDialogEditPrivate *d_ptr;
    Q_DECLARE_PRIVATE(CDialogEdit)
    Q_PRIVATE_SLOT(d_func(),void slotButtonClicked())
    Q_PRIVATE_SLOT(d_func(),void slotValueChanged(QVariant))
};


#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif
