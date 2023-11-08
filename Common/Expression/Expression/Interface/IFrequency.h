#ifndef IFREQUENCY_H
#define IFREQUENCY_H


class IFrequency
{
public:
    IFrequency()
    {
        m_frequency= 0;
    }

    int m_frequency;
    virtual void invoke(){m_frequency++;}
};

#endif // IFREQUENCY_H
