/*!
\author aGRw 2017.01.10
\section 窗体树列表，用于管理运行环境下的图形窗体
*/

#ifndef FORMTREEWIDGET_H
#define FORMTREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class CHMIFramework;

namespace Ui {
class CFormTreeWidget;
}

class CFormTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CFormTreeWidget(QWidget *parent = 0);
    ~CFormTreeWidget();
private:
    Ui::CFormTreeWidget *ui;
/*!
\section enum
*/
public:
    enum class FileType { Dir, File };
/*!
\section field
*/
private:
    CHMIFramework* m_framework;
/*!
\section public function
*/
public:
    void init(CHMIFramework* framework);
/*!
\section node
*/
private:
    void initNodes(QTreeWidgetItem *root, CHMIFramework *framework);
    void initRoot(QTreeWidgetItem *root, CHMIFramework *framework);
    void initOpenedState();
    void getNodes(const QString &path, const QString &pattern, QTreeWidgetItem *parent);
    QTreeWidgetItem* createNode(const QString &fullPath, FileType type);
    void setNodeIcon(QTreeWidgetItem *node, FileType type);
    FileType getFileType(QTreeWidgetItem *node);
    QString getFilePath(QTreeWidgetItem *node);
    QTreeWidgetItem *findNode(const QString& fullName);
    void setOpenedState(QTreeWidgetItem *node, bool isOpened);
/*!
\section form
*/
private:
    void openForm(QTreeWidgetItem *node);
    void closeForm(QTreeWidgetItem *node);
/*!
\section event
*/
private:
    void itemDoubleClicked(QTreeWidgetItem *item, int column);
    void formOpened(const QString& fullName);
    void formClosed(const QString& fullName);
};

#endif // FORMTREEWIDGET_H
