#ifndef INTERFACE_H
#define INTERFACE_H
#include <qstring.h>
class IEmpty
{
public:
    virtual ~IEmpty(){}
    virtual bool inHerits(const QString& typeName) = 0;
    virtual QString toString()
    {
        return "";
    }

    virtual qreal toFloat(bool* bOk = NULL)
    {
        QString str = toString();
        if(str == "true")
            str = "1";
        else if(str == "false")
            str = "0";
        bool isok = false;
        qreal value = str.toFloat(&isok);
        *bOk = isok;
        if(isok)
            return value;
        return 0;
    }
};

static void onError()
{

}



#endif // INTERFACE_H
