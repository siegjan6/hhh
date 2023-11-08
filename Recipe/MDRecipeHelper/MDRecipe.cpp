#include "MDRecipe.h"
#include "MDRecipeData.h"
#include <QObject>

CMDRecipe::CMDRecipe()
{
    m_data = new CMDRecipeData;
}

CMDRecipe::~CMDRecipe()
{
    if(m_data)
        delete m_data;
}

CMDRecipe::CMDRecipe(const CMDRecipe& obj)
{
    m_data = new CMDRecipeData();

    copy(&obj);
}

CMDRecipe& CMDRecipe::operator = (const CMDRecipe& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

void CMDRecipe::copy(const CMDRecipe* source)
{
    m_data->m_configProperty = source->m_data->m_configProperty;
//    m_data->m_runtimeProperty = source->m_data->m_runtimeProperty;
}

QString CMDRecipe::name() const
{
    return m_data->m_configProperty.head()->name();
}
DownloadMode CMDRecipe::downloadMode() const
{
    return m_data->m_configProperty.head()->downloadMode();
}

void CMDRecipe::clear()
{
    m_data->m_configProperty.items()->deleteAll();
}

CMDRecipeProperty_Config* CMDRecipe::configProperty() const
{
    return (m_data ? &m_data->m_configProperty : NULL);
}

CMDRecipeProperty_Runtime* CMDRecipe::runtimeProperty() const
{
    return (m_data ? &m_data->m_runtimeProperty : NULL);
}

/*!
\section 辅助函数
*/

// 下载模式
void CMDRecipe::enumDownloadModes(QStringList& modeList)
{
   modeList.append(modeToString(DownloadMode::general));
   modeList.append(modeToString(DownloadMode::quick));
}

const QString STRING_SUB_PACKAGE = QObject::tr("SubPackage");
const QString STRING_QUICK = QObject::tr("Quick");
QString CMDRecipe::modeToString(DownloadMode mode)
{
   QString str;
   switch (mode) {
   case DownloadMode::general: str = STRING_SUB_PACKAGE;
       break;
   case DownloadMode::quick: str = STRING_QUICK;
       break;
   default:
       break;
   }
   return str;
}

DownloadMode CMDRecipe::stringToMode(const QString& str)
{
   DownloadMode m = DownloadMode::general;
   if(0 == str.compare(STRING_SUB_PACKAGE))
       m = DownloadMode::general;
   else if(0 == str.compare(STRING_QUICK))
       m = DownloadMode::quick;
   else{};
   return m;
}
