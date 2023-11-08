#include "csubtest.h"

CSubTest::CSubTest()
{
}
int CSubTest::salary() const
{
    return m_salary;
}

void CSubTest::setSalary(int salary)
{
    m_salary = salary;
}
QString CSubTest::GUID_Base() const
{
    return m_GUID_Base;
}

QString CSubTest::GUID_Pro() const
{
    return m_GUID_Pro;
}
QString CSubTest::sonname() const
{
    return m_name;
}

void CSubTest::setSonname(const QString &name)
{
    m_name = name;
}



