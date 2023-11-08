#include "MDScriptEngineImp.h"
#include "MDObjectWrapper.h"
#include "MDObjectWrapperImp.h"
#include "MDScriptEnginePool.h"
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QCoreApplication>
//#include <QMessageBox>

CMDScriptEngineImp::CMDScriptEngineImp()
{
    m_scriptIncludeSysPath = "";
    m_scriptIncludePrjPath = "";
}

CMDScriptEngineImp::~CMDScriptEngineImp()
{
    CMDScriptEnginePoolMgr::unregFromPool(this, m_PID);
}

/*!
\section 引擎的初始化/释放（PID 是引擎分组的标识）
*/
bool CMDScriptEngineImp::init(bool newIsolate, const QString& PID)
{
    Q_UNUSED(newIsolate)

    m_PID = PID;
    CMDScriptEnginePoolMgr::regToPool(this, m_PID);
    m_scriptIncludePrjPath = m_PID + "/include";
    m_scriptIncludeSysPath = qApp->applicationDirPath() + "/include";

    // todo: js script engine system extension
    //m_qsEngine.importExtension()

    return true;
}

void CMDScriptEngineImp::setIncludeSysPath(QString path)
{
    m_scriptIncludeSysPath = path;
}

void CMDScriptEngineImp::setIncludePrjPath(QString path)
{
    m_scriptIncludePrjPath = path;
}

void CMDScriptEngineImp::release()
{
    CMDScriptEnginePoolMgr::unregFromPool(this, m_PID);

    m_qsEngine.clearExceptions();

    m_soList.clear();
}

/*!
\section 添加对象到引擎，在引擎运行期间，对象必须是有效的
*/
void CMDScriptEngineImp::addObject(CMDObjectWrapper* so)
{
    m_soList.append(so);
}

void CMDScriptEngineImp::addObjects(const QList<CMDObjectWrapper*>& soList)
{
    int count = soList.count();
    for(int i = 0; i < count; i++)
        m_soList.append(soList.at(i));
}

void CMDScriptEngineImp::addObjects(const CMDObjectWrapperMgr& wm)
{
    QList<CMDObjectWrapper*> soList;
    wm.Wrappers(soList);

    addObjects(soList);
}

void CMDScriptEngineImp::regObject()
{
    for (int i = 0; i < m_soList.size(); ++i)
    {
        CMDObjectWrapper* wrapper = m_soList.at(i);

        // 注册对象到全局范围
        QScriptValue scriptObject = m_qsEngine.newQObject(wrapper);
        QString name = wrapper->QName();
        m_qsEngine.globalObject().setProperty(name, scriptObject);
    }
}

/*!
\section 运行代码
*/
bool CMDScriptEngineImp::runFile(const QString& jsFile)
{
    QFile qf(jsFile);
    if(!qf.exists())
        return false;

    qf.open(QIODevice::ReadOnly);
    QByteArray ba = qf.readAll();
    QString sourceCode(ba);
    QString preprocessedScript;
    if (prepareScript(preprocessedScript, jsFile))
        return runWithIncludeProcessed(preprocessedScript, jsFile);
    return false;
}

bool CMDScriptEngineImp::run(const QString& sourceCode, const QString& dbgSource/* = ""*/)
{
    const QString fileName;
    QString output;
    const QString scriptDir = m_scriptIncludePrjPath;
    QString aText = sourceCode;
    int errLoc;
    if(preprocessScript( fileName, aText, output, scriptDir, errLoc )) {
        return runWithIncludeProcessed(output, dbgSource);
    }
    return false;
}

bool CMDScriptEngineImp::runWithIncludeProcessed(const QString& sourceCode, const QString& dbgSource/* = ""*/)
{
    regObject();

    m_source = sourceCode;
    qDebug() << "app libraryPaths: " << qApp->libraryPaths();

    QStringList availables = m_qsEngine.availableExtensions();
    qDebug() << "script availableExtensions: " << availables;
    foreach(QString as, availables)
    {
        // Returns undefinedValue() if the extension was successfully imported
        QScriptValue rv = m_qsEngine.importExtension(as);
        if(rv.isUndefined()) {
            // import succeed
        }
        else {
            if(m_qsEngine.hasUncaughtException()){
                qDebug() << "script importedExtension fail: " << as << "( line number:" << m_qsEngine.uncaughtExceptionLineNumber() << " exception:" << m_qsEngine.uncaughtException().toString() << " backtrace:" << m_qsEngine.uncaughtExceptionBacktrace() << ")";
            }
        }
    }
    QStringList exts = m_qsEngine.importedExtensions();
    qDebug() << "script importedExtensions: " << exts;

    m_qsEngine.evaluate(m_source);

    bool hasException = m_qsEngine.hasUncaughtException();
    if(hasException)
    {
        int outputPos = m_qsEngine.uncaughtExceptionLineNumber();
        QString realPos = lookup( outputPos );
        QString errMsg = QString("script error: \"%1\" @ line %2").arg(m_qsEngine.uncaughtException().toString(), realPos);
        emit scriptDebug(errMsg, dbgSource);
        qWarning() << errMsg;
        //QScriptValue exp = m_qsEngine.uncaughtException();
        //QString errMsg = exp.toString();
        //QMessageBox::information(NULL, QObject::tr("Exception"), exp.toString());
    }
    return (false == hasException);
}

bool CMDScriptEngineImp::runDynamically(const QString& tmpCode)
{
    if (m_qsEngine.isEvaluating())
    {
        QString msg = QString("ERROR: there is already a script running, please wait until it's over.");
        emit scriptDebug(msg, "tmpCode");
        qWarning() << msg;
        return false;
    }

    // Notify that the script starts here
    emit scriptRunning();

    // run that script in a new context
    QScriptContext *context = m_qsEngine.pushContext();
    m_qsEngine.evaluate(tmpCode);
    m_qsEngine.popContext();

    bool hasException = m_qsEngine.hasUncaughtException();
    if(hasException)
    {
        int outputPos = m_qsEngine.uncaughtExceptionLineNumber();
        QString realPos = lookup( outputPos );
        QString errMsg = QString("script error: \"%1\" @ line %2").arg(m_qsEngine.uncaughtException().toString(), realPos);
        emit scriptDebug(errMsg, "tmpCode");
        qWarning() << errMsg;
//        QMessageBox::information(NULL, QObject::tr("Exception"), errMsg);
    }
    emit scriptStopped();
    Q_UNUSED(context);
    return (false == hasException);
}

bool CMDScriptEngineImp::runAgain()
{
    m_qsEngine.evaluate(m_source);

    bool hasException = m_qsEngine.hasUncaughtException();
    if(hasException)
    {
        QScriptValue exp = m_qsEngine.uncaughtException();
        QString errMsg = exp.toString();
        emit scriptDebug(errMsg, "runAgain");
        qWarning() << errMsg;
//        QMessageBox::information(NULL, QObject::tr("Exception"), exp.toString());
    }
    return (false == hasException);
}

QString CMDScriptEngineImp::getCode() const
{
    return m_source;
}

QString CMDScriptEngineImp::helperFileCode(const QString& jsFile)
{
    QFile qf(jsFile);
    if(!qf.exists())
        return "";

    qf.open(QIODevice::ReadOnly);
    QByteArray ba = qf.readAll();
    QString sourceCode(ba);
    return sourceCode;
}

QString CMDScriptEngineImp::helperSampleCode()
{
    QString source(
                            "var v1 = 100;"
                            "v1 = 102;"
                            "println('v1=', 100);"

                            "if(v1 > 100)"
                            "{ println('v1=', v1)};"

                            "var message = 'Hello World!';"
                            "var x = message.length;"
                            "println('x=', message.length);"

                            "message = message.toUpperCase();"
                            "println('message=', message);"

                            "var pi_value = Math.PI;"
                            "println('pi_value=', pi_value);"
                             "var sqrt_value = Math.sqrt(15);"
                             "println('sqrt_value=', sqrt_value);"

                             "function nsVar_onChanged(value)"
                            "{"
                                "println('nsVar value=', value);"
                                "println(dGFuZ3p3.uid);"
                            "}"

                            "println(MindSCADA_user.uid);"

                            "alert('Welcome!');");
    return source;
}

/*!
\section 调用 js 函数
*/
void CMDScriptEngineImp::callFunc(const QString& funcName, QList<QVariant>& paras)
{
    QScriptValue global = m_qsEngine.globalObject();
    QScriptValue func = global.property(funcName);
    QScriptValueList args;
    if(!paras.isEmpty())
    {
        foreach (QVariant v, paras)
            args.append(toQScriptValue(v));
    }
    if(func.isNull() || !func.isValid())
        return;
     func.call(QScriptValue(), args);
}

QScriptValue CMDScriptEngineImp::toQScriptValue(const QVariant& v)
{
//    switch (v.type())
//    {
//        case QMetaType::Bool :
//        {
//            return QScriptValue(v.toBool());
//        }break;
//        case QMetaType::Char :
//        case QMetaType::Short :
//        case QMetaType::Int :
//        case QMetaType::Long :
//        {
//            return QScriptValue(v.toInt());
//       }break;
//        case QMetaType::UChar :
//        case QMetaType::QChar:
//        case QMetaType::UShort :
//        case QMetaType::UInt :
//        case QMetaType::ULong :
//         {
//            return QScriptValue(v.toUInt());
//        }break;
//        case QMetaType::LongLong :
//        case QMetaType::ULongLong :
//        {
//            return QScriptValue(v.toInt());
//       }break;
//        case QMetaType::Float :
//        case QMetaType::Double :
//        {
//           return QScriptValue(v.toDouble());
//       }break;
//        case QMetaType::QString:
//        {
//           return QScriptValue(v.toString());
//       }break;
//        case QMetaType::QDate:
//        {
//            return QScriptValue(v.toDouble());
//        }break;
//        default:
//            break;
//    }


//    Bool = QMetaType::Bool,
//    Int = QMetaType::Int,
//    UInt = QMetaType::UInt,
//    LongLong = QMetaType::LongLong,
//    ULongLong = QMetaType::ULongLong,
//    Double = QMetaType::Double,
//    Char = QMetaType::QChar,
//    Map = QMetaType::QVariantMap,
//    List = QMetaType::QVariantList,
//    String = QMetaType::QString,
//    StringList = QMetaType::QStringList,
//    ByteArray = QMetaType::QByteArray,
//    BitArray = QMetaType::QBitArray,
//    Date = QMetaType::QDate,
//    Time = QMetaType::QTime,
//    DateTime = QMetaType::QDateTime,

    switch (v.type())
    {
        case QVariant::Type::Bool :
        {
            return QScriptValue(v.toBool());
        }
        case QVariant::Type::Int :
        {
            return QScriptValue(v.toInt());
        }
        case QVariant::Type::Char :
        case QVariant::Type::UInt :
        {
            return QScriptValue(v.toUInt());
        }
        case QVariant::Type::LongLong :
        case QVariant::Type::ULongLong :
        {
            return QScriptValue(v.toInt());
        }
        case QVariant::Type::Double :
        {
           return QScriptValue(v.toDouble());
        }
        case QVariant::Type::String:
        {
           return QScriptValue(v.toString());
        }
        case QVariant::Type::Date:
        {
            return QScriptValue(v.toDouble());
        }
        default:
        break;
    }
    return QScriptValue();
}

QString CMDScriptEngineImp::lookup( int outputPos )
{
    // Error beyond the last line?
    if (outputPos > m_outline)
        outputPos = m_outline;

    QMap<int,QPair<QString,int>>::const_iterator i = m_num2loc.lowerBound( outputPos );
    int outputEnd = i.key();
    QString path = i.value().first;
    int inputEnd = i.value().second;
    int inputPos = outputPos - outputEnd + inputEnd;
    // qDebug() << outputPos << " maps to " << path << ":" << inputPos;
    QString msg;
    if (path.isEmpty())
        msg = QString( "%1" ).arg(inputPos);
    else
        msg = QString( "%1:%2" ).arg(path).arg(inputPos);

    return msg;
}

bool CMDScriptEngineImp::prepareScript( QString &script, const QString &fileName)
{
    QString absPath;

    // 绝对路径
    if (QFileInfo(fileName).isAbsolute())
        absPath = fileName;
    else {
        // 相对于项目脚本路径
        QFile qf(m_scriptIncludePrjPath + "/" + fileName);
        if(qf.exists())
            absPath = m_scriptIncludePrjPath + "/" + fileName;

        // 相对于系统安装脚本路径
        QFile qf2(m_scriptIncludeSysPath + "/" + fileName);
        if(qf2.exists())
            absPath = m_scriptIncludeSysPath + "/" + fileName;
    }

    if (absPath.isEmpty())
    {
        QString msg = QString("WARNING: could not find script file %1").arg(QDir::toNativeSeparators(fileName));
        emit scriptDebug(msg);
        qWarning() << msg;
        return false;
    }

    // 脚本路径所在目录为优先引用目录
    QString scriptDir = QFileInfo(absPath).dir().path();

    QFile fic(absPath);
    if (!fic.open(QIODevice::ReadOnly))
    {
        QString msg = QString("WARNING: cannot open script: %1").arg(QDir::toNativeSeparators(fileName));
        emit scriptDebug(msg);
        qWarning() << msg;
        return false;
    }

//    QDir sysdir(m_scriptIncludeSysPath);
//    if (!m_scriptIncludeSysPath.isEmpty() && sysdir.exists(m_scriptIncludeSysPath))
//        m_includeSet.insert(m_scriptIncludeSysPath);
//    QDir prjdir(m_scriptIncludePrjPath);
//    if (!m_scriptIncludeSysPath.isEmpty() && prjdir.exists(m_scriptIncludeSysPath))
//        m_includeSet.insert(m_scriptIncludeSysPath);

    bool ok = false;
    if (fileName.endsWith(".js"))
        ok = preprocessFile(absPath, fic, script, scriptDir);
    if (!ok)
    {
        return false;
    }

    return true;
}

bool CMDScriptEngineImp::preprocessFile(const QString fileName, QFile &input, QString& output, const QString& scriptDir)
{
    QString aText = QString::fromUtf8(input.readAll());
    int errLoc;
    return preprocessScript( fileName, aText, output, scriptDir, errLoc );
}

bool CMDScriptEngineImp::preprocessScript(const QString fileName, const QString &input, QString &output, const QString &scriptDir, int &errLoc )
{
    // The one and only top-level call for expand.
    m_scriptFileName = fileName;
    m_num2loc = QMap<int,QPair<QString,int>>();
    m_outline = 0;
    m_includeSet.clear();
    errLoc = -1;
    expand( fileName, input, output, scriptDir, errLoc );
    if( errLoc != -1 ){
        return false;
    } else {
        return true;
    }
}

void CMDScriptEngineImp::expand(const QString fileName, const QString &input, QString &output, const QString &scriptDir, int &errLoc)
{
    QStringList lines = input.split("\n");
    static const QRegularExpression includeRe("^include\\s*\\(\\s*\"([^\"]+)\"\\s*\\)\\s*;\\s*(//.*)?$");
    int curline = 0;
    for (const auto& line : lines)
    {
        curline++;
        QRegularExpressionMatch includeMatch=includeRe.match(line);
        if (includeMatch.hasMatch())
        {
            QString incName = includeMatch.captured(1);
            QString incPath;

            // Search for the include file.  Rules are:
            // 1. If incPath is absolute, just use that（绝对路径）
            // 2. If incPath is relative, look in scriptDir + included filename（即脚本文件本身所在目录）
            // 3. If incPath is relative, look in project scriptDir + included filename（即项目路径下的include目录）
            // 4. If incPath is relative (undefined), look in system standard scripts directory + included filename（即安装包可执行文件目录下的include目录）

            // case 1:
            if (QFileInfo(incName).isAbsolute())
                incPath = incName;
            else
            {
                if (incPath.isEmpty())
                {
                    QString fail = "";
                    QFile qf(scriptDir + "/" + incName);
                    // case 2:
                    if(qf.exists())
                        incPath = scriptDir + "/" + incName;
                    else {
                        fail = scriptDir + "/" + incName;
                        qWarning() << "WARNING: file not found! Let's check project scripts directory...";
                    }

                    QFile qf1(m_scriptIncludePrjPath + "/"  + incName);
                    // case 3:
                    if(qf1.exists())
                        incPath = m_scriptIncludePrjPath + "/"  + incName;
                    else {
                        fail = m_scriptIncludePrjPath + "/" + incName;
                        qWarning() << "WARNING: file not found! Let's check system standard scripts directory...";
                    }

                    // OK, file is not exists in project relative path; Let's check system standard scripts include directory
                    if (incPath.isEmpty())
                    {
                        QFile qf2(m_scriptIncludeSysPath + "/"  + incName);
                        // case 4:
                        if(qf2.exists())
                            incPath = m_scriptIncludeSysPath + "/"  + incName;
                        else {
                            fail += " or include/" + m_scriptIncludeSysPath + "/"  + incName;
                            emit scriptDebug(QString("WARNING: could not find system script include file: %1").arg(QDir::toNativeSeparators(incName)));
                            qWarning() << "WARNING: could not find system script include file: " << QDir::toNativeSeparators(incName);
                            if( errLoc == -1 ) errLoc = output.length();
                            output += line + " // <<< " + fail + " not found\n";
                            m_outline++;
                            continue;
                        }
                    }
                }
            }
            // Write the include line as a comment. It is end of a range.
            output += "// " + line + "\n";
            m_outline++;

            if( ! m_includeSet.contains(incPath) )
            {
                m_includeSet.insert(incPath);
                m_num2loc.insert( m_outline, QPair<QString,int>(fileName, curline) );
                QFile fic(incPath);
                bool ok = fic.open(QIODevice::ReadOnly);
                if (ok)
                {
                    qWarning() << "script include: " << QDir::toNativeSeparators(incPath);
                    QString aText = QString::fromUtf8(fic.readAll());
                    expand( incPath, aText, output, scriptDir, errLoc );
                    fic.close();
                }
                else
                {
                    emit scriptDebug(QString("WARNING: could not open script include file for reading: %1").arg(QDir::toNativeSeparators(incPath)));
                    qWarning() << "WARNING: could not open script include file for reading: " << QDir::toNativeSeparators(incPath);
                    if( errLoc == -1 ) errLoc = output.length();
                    output += line + " // <<< " + incPath + ": cannot open\n";
                    m_outline++;
                    continue;
                }
            }
        }
        else
        {
            output += line + '\n';
            m_outline++;
        }
    }
    m_num2loc.insert( m_outline, QPair<QString,int>(fileName, curline) );

    // Do we need this any more? (WL, 2020-04-10)
    if (qApp->property("verbose")==true)
    {
        // Debug to find stupid errors. The line usually reported may be off due to the preprocess stage.
        QStringList outputList=output.split('\n');
        qDebug() << "Script after preprocessing:";
        int lineIdx=0;
        for (const auto& line : outputList)
        {
            qDebug() << lineIdx << ":" << line;
            lineIdx++;
        }
    }
    return;
}
