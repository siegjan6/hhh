/*!

\author xuk

\section 功能

测试类
*/
#ifndef CSUBTEST_H
#define CSUBTEST_H

#include "ctest.h"

class CSubTest : public CTest
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Base READ GUID_Base)
    Q_PROPERTY(int salary READ salary WRITE setSalary NOTIFY salaryChange)
    Q_PROPERTY(QString GUID_Pro READ GUID_Pro)
    Q_PROPERTY(QString sonname READ sonname WRITE setSonname)

public:
    CSubTest();
    ~CSubTest(){}

    int salary() const;
    void setSalary(int salary);

    QString sonname() const;
    void setSonname(const QString &name);

    QString GUID_Base() const;//! 分组属性
    QString GUID_Pro() const;

Q_SIGNALS:
    void salaryChange(int);

private:

    int m_salary;
    QString m_name;

    QString m_GUID_Base;
    QString m_GUID_Pro;
};

#endif // CSUBTEST_H
