#ifndef CEXPCOMPILE_H
#define CEXPCOMPILE_H

#include <qmap.h>
#include "FunctionBase.h"
#include<qmutex.h>
class CExpCompile:public QObject
{
    Q_OBJECT
public:
    CExpCompile();
    virtual ~CExpCompile();
    static CExpCompile *Instance();
    bool compile(CFunctionBase*& root,QString exp);
private:
    QList<QString> m_operatorList; //操作符列表

    QMap<QString,CFunctionBase*> m_funFactoryDic;//函数
    void initialize();
    void release();
    int getOptrKeyWordEndPos(const QString& str, int p);

    bool isKeyWord(QString key);
    int isAvalableParameter(QString str);
    CFunctionBase* getFunction(QString keyWord);

    bool computeOperator(QString exp,int&start,int& index,int& endPos,CFunctionBase*& rootFun,CFunctionBase*& currentFun,CFunctionBase*& currentFunParent,bool* bContinue);
    bool computeOperator(QString exp,int&start,int& index,int& endPos,CFunctionBase*& rootFun,CFunctionBase*& currentFun,CFunctionBase*& currentFunParent);
    bool computeFunction(QString& exp, QString& str, int& start, int& index, CFunctionBase*& rootFun, CFunctionBase*& currentFun, CFunctionBase*& currentFunParent, bool &isok);

    int findEndBracketsPos(QString exp, int start, bool &isok);
    int findNextOptr(QString str, int p);

    bool getParameters(QString exp,QList<QString>* list);
    int appendFun(CFunctionBase *&currentFun, CFunctionBase *&rootFun, CFunctionBase *tmp, CFunctionBase *&currentFunParent, int start, int index, QString exp, bool &isok);

    void AddVariable(const QString& varName);
};

#endif // CEXPCOMPILE_H
