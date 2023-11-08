/*!
\author aGRw 2016.04.22
\section 窗体布局模块测试代码
*/
#ifndef FORMPREVIEWTEST_H
#define FORMPREVIEWTEST_H

#include <QtCore/qglobal.h>

#if defined(MDHMIFRAMEWORK_LIBRARY)
#  define MDHMIFRAMEWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFRAMEWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

class CPreviewDialog;
class CHMIFramework;

class MDHMIFRAMEWORKSHARED_EXPORT FormPreviewTest : public QObject
{
    Q_OBJECT
public:
    FormPreviewTest();
    virtual ~FormPreviewTest(){}
private Q_SLOTS:
    //!初始化
    void initTestCase();
    //!测试结束
    void cleanupTestCase();
    //!加载窗体
    void loadFormCase();
    //!鼠标操作
    void mouseCase();
    //!键盘操作
    void keyCase();
    //!属性框
    void propertyCase();
    //!保存窗体
    void saveFormCase();

public:
    void setFullName(const QString& value){m_fullName = value;}
    QString fullName(){return m_fullName;}
private:
    CPreviewDialog* m_test;
    CHMIFramework* m_framework;
    QString m_fullName;
};

#endif // FORMPREVIEWTEST_H
