#include "DialogEdit.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include "qtpropertybrowserutils_p.h"
#include <QDebug>

/**
 * @brief setupTreeViewEditorMargin
 *         设置样式
 */
static inline void setupTreeViewEditorMargin(QLayout *lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    else
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
}
/**
 * @brief The CFileEditPrivate class
 */
class CDialogEditPrivate
{
    CDialogEdit * q_ptr;
    Q_DECLARE_PUBLIC(CDialogEdit)
public:
    CDialogEditPrivate(CDialogEdit *parent =0);
    void slotButtonClicked();
    void slotValueChanged(const QVariant &value);

private:
    QLabel          *m_valueLabel;
    QLineEdit       *m_valueEdit;
    QLabel          *m_pixmapLabel;
    QPixmap          m_pixmap;
    QToolButton     *m_button;
};
CDialogEditPrivate::CDialogEditPrivate(CDialogEdit *parent)
{
    q_ptr = parent;
}

void CDialogEditPrivate::slotButtonClicked()
{
    Q_Q(CDialogEdit);
    emit q->valueChanged(" ");
}

void CDialogEditPrivate::slotValueChanged(const QVariant &value)
{
    if(value.isNull())
        return;
    m_valueLabel->setText(value.toString());
    m_valueEdit->setText(value.toString());
}


/**
 * @brief CFileEdit::CFileEdit
 * @param parent
 */
CDialogEdit::CDialogEdit(QWidget *parent)
    : QWidget(parent)
{
    d_ptr = new CDialogEditPrivate(this);

    d_ptr->m_pixmapLabel = new QLabel(this);
    d_ptr->m_valueLabel = new QLabel(this);
    d_ptr->m_valueEdit = new QLineEdit(this);
    d_ptr->m_button = new QToolButton(this);

    d_ptr->m_button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    d_ptr->m_button->setText(QLatin1String("..."));
    d_ptr->m_button->setFixedWidth(40);

    QHBoxLayout *layout = new QHBoxLayout(this);
    setupTreeViewEditorMargin(layout);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(d_ptr->m_pixmapLabel);
    layout->addWidget(d_ptr->m_valueLabel);
    layout->addWidget(d_ptr->m_valueEdit);
    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));

    setFocusProxy(d_ptr->m_button);
    setFocusPolicy(d_ptr->m_button->focusPolicy());
    d_ptr->m_button->installEventFilter(this);
    layout->addWidget(d_ptr->m_button);

    d_ptr->m_pixmapLabel->setPixmap(d_ptr->m_pixmap);

    connect( d_ptr->m_button, SIGNAL(clicked()),
            this, SLOT(slotButtonClicked()));
}

CDialogEdit::~CDialogEdit()
{
    delete d_ptr;
}

void CDialogEdit::setValue(const QString &value)
{
    if (d_ptr->m_valueLabel->text() != value)
        d_ptr->m_valueLabel->setText(value);
    if(d_ptr->m_valueEdit->text() != value)
        d_ptr->m_valueEdit->setText(value);

}

QString CDialogEdit::value() const
{
    if(!d_ptr->m_valueEdit->text().isEmpty())
        return d_ptr->m_valueEdit->text();
    return  QString();
}

QLineEdit *CDialogEdit::lineEdit()
{
    return d_ptr->m_valueEdit;
}

QLabel *CDialogEdit::label()
{
    return d_ptr->m_valueLabel;
}

void CDialogEdit::setIcon(const QPixmap &pixmap)
{
    d_ptr->m_pixmap = pixmap;
    d_ptr->m_pixmapLabel->setPixmap(pixmap);
}

bool CDialogEdit::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj ==  d_ptr->m_button)
    {
        switch (ev->type())
        {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        { // Prevent the QToolButton from handling Enter/Escape meant control the delegate
            switch (static_cast<const QKeyEvent*>(ev)->key())
            {
            case Qt::Key_Escape:
            case Qt::Key_Enter:
            case Qt::Key_Return:
                ev->ignore();
                return true;
            default:
                break;
            }
        }
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, ev);
}

void CDialogEdit::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

#include "moc_DialogEdit.cpp"
