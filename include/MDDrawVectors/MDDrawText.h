/*!
\author aGRw 2014.09.05
\section 文本控件
*/
#ifndef MDDRAWTEXT_H
#define MDDRAWTEXT_H

#include "MDDrawVector.h"

#if defined(MDDRAWVECTORS_LIBRARY)
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWVECTORSSHARED_EXPORT Q_DECL_IMPORT
#endif

class CDrawTextData;

class MDDRAWVECTORSSHARED_EXPORT CMDDrawText : public CMDDrawVector
{
public:
    CMDDrawText();
    virtual ~CMDDrawText();

private:
    CDrawTextData* m_data;
/*!
\section virtual property
*/
public:
     DrawType type() ;
     QString className(){return "CMDDrawText";}
/*!
\section property
*/
public:
    void setFont(const QFont* value);
    QFont* font();
    void setText(const QString& value);
    QString text();
    CMDBrushManager *textBrush();
    void setHAlignment(Qt::Alignment value);
    Qt::Alignment hAlignment();
    void setVAlignment(Qt::Alignment value);
    Qt::Alignment vAlignment();
    void setIsWrap(bool value);
    bool isWrap();
    void setButtonFrame(bool value);
    bool buttonFrame();
    void setDecimal(int value);
    int decimal();
    void setDateFormat(const QString& value);
    QString dateFormat();
    void setMaxLength(int value);
    int maxLength();
    void setIsDownFrame(bool value);
    bool isDownFrame();
    QString textFormatString();
/*!
\section protected function
*/
protected:
    CMDDrawObj *createSelf() ;
    void onPaint(QPainter* p) ;
    void generatePath() ;
/*!
\section public function
*/
public:
    void generateContent();
    CMDPropertyDrawObj *createPropertyDrawObj();
    void onInitialization();
    virtual void getBrushes(QList<CMDBrushManager*>& list);
/*!
\section common
*/
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
public:
    CMDDrawObj* clone() ;
/*!
\section var
*/
public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp) ;
//////////////////////////////////////////////////////////////////////////
private:
    void initTextOption();
/*!
\section undo
*/
public:
    void undoTextBrush(CMDBrushManager* value);
/*!
\section language
*/
public:
    virtual void changeLanguage();
    virtual void collectWords();
/*!
\section batch edit
*/
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);

};

#endif // MDDRAWTEXT_H





