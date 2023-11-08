/*!
\author aGRw 2015.02.03
\section 用于生成控件名称
*/
#ifndef OBJNAME_H
#define OBJNAME_H

#include <QString>
#include <QObject>
#include <QMap>

#include "IObjName.h"
#include "DrawCreator.h"

class CMDDrawObj;
class CEnvironment;


class CObjName : public IObjName
{
public:
    CObjName();
/*!
\section field
*/
private:
    //!字典大小写敏感,所以全用小写
    QMap<QString, QObject*> m_nameDict;
    QList<int> m_indexs;
    CEnvironment* m_container;
/*!
\section private function
*/
private:
    void addName(CMDDrawObj* obj);
    QString getName(const QString& name, int index);
    bool containsObjName(CMDDrawObj* obj, const QString& name);
/*!
\section public function
*/
public:
    void init(CEnvironment *container);
    void resetDict(QList<CMDDrawObj*>* objList);
    void addName(const QString& name);
    bool containsName(const QString& name);
    //!判断名称是否存在;如果不存在,从字典中删除
    void dynamicContainsName(const QString& name);
    void createName(CMDDrawObj* obj, bool needChangeChild = true);
    void removeName(CMDDrawObj* obj);

};

#endif // OBJNAME_H

