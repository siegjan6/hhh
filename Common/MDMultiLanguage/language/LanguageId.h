/*!
\author aGRw 2015.12.25
\section 语言数据类
*/
#ifndef LANGUAGEDAT_H
#define LANGUAGEDAT_H

#include <QString>

class CLanguageId
{
public:
    CLanguageId(){}
    CLanguageId(int id, const QString& name);
public:
    int id(){return m_id;}
    QString name(){return m_name;}
    void setIsSelect(bool value){m_isSelect = value;}
    bool isSelect(){return m_isSelect;}
private:
    qint32 m_id;
    QString m_name;
    bool m_isSelect;//!是否选择
};

#endif // LANGUAGEDAT_H
