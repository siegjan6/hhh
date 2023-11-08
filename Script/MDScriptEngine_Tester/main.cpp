#include <QCoreApplication>

#include "MDScriptEngine.h"
#include "SampleObjs.h"
#include "SampleObjsWrapper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CMDScriptEngine scriptEngine;
    scriptEngine.init();

    CMDUserWrapperExpert uwExpert;
    uwExpert.setScriptEngine(&scriptEngine);
    uwExpert.addWrapper("NetSCADA_user", new CMDSimpleUser("NetSCADA_user", 16888));
    uwExpert.addWrapper("tangzw", new CMDSimpleUser("tangzw", 99999));
    uwExpert.addWrapper("nsVar", new CMDSimpleUser("nsVar", 1234));

    QList<CMDObjectWrapper*> userWrappers;
    uwExpert.Wrappers(userWrappers);
    scriptEngine.addObjects(userWrappers);

    QString source = CMDScriptEngine::helperSampleCode();
    scriptEngine.run(source);

    QList<QVariant> paras;
    paras.append(QVariant(1234));
    uwExpert.fireEvent("nsVar", "onChanged", paras);

    scriptEngine.release();

    return a.exec();
}
