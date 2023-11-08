#include "MDAccount.h"
#include "MDAccountData.h"

CMDAccount::CMDAccount()
{
    m_data = new CMDAccountData;
}

CMDAccount::~CMDAccount()
{
    if(m_data)
        delete m_data;
}

CMDAccountProperty_Config* CMDAccount::configProperty() const
{
    return (m_data ? &m_data->m_configProperty : NULL);
}

CMDAccountProperty_Runtime* CMDAccount::runtimeProperty() const
{
    return (m_data ? &m_data->m_runtimeProperty : NULL);
}
