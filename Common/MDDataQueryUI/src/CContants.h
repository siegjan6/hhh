#ifndef CCONTANTS_H
#define CCONTANTS_H
#include <QString>
#include "MDMultiLanguage.h"

namespace Translator {
inline QString findTranslation(const QString &name){
    QString newName = CMDMultiLanguage::instance()->value(name);
    return newName.isEmpty()? name:newName;
}

}
enum class ReturnType{
    DataSource = 0,
    Filter,
    Export,
    Rejected
};
#endif // CCONTANTS_H
