/*!
\author eHVr
\section 功能
标签位置类，
\class CTagHPos 水平方向
\class CTagVPos 垂直方向
*/
#ifndef CTAGPOSITION_H
#define CTAGPOSITION_H
/*!
 \section CTagVPos
    标签垂直方向位置
*/

#define MIDDLE -100
#include <QDataStream>

class CTagVPos
{
public:
    enum class Position{
        Top = 0,
        Middle,
        Bottom
    };
public:
    CTagVPos():m_topOffset(5),m_bottomOffset(5),m_position(Position::Middle){}
    ~CTagVPos(){}


    void setOffset(int offset);
    int offset() const;

    Position position() const;
    void setPosition(const Position &position);
private:
    qint32        m_topOffset;      //! 顶部偏移
    qint32        m_bottomOffset;   //! 底部偏移
    Position      m_position;       //! 位置
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

/*!
 \section CTagHPos
    标签水平方向位置(相对于标签线)
*/
class CTagHPos
{
public:
    enum class Position{
        Left = 0,
        Middle,
        Right
    };

    CTagHPos():m_leftOffset(5),m_rightOffset(5),m_position(Position::Left){}
    ~CTagHPos(){}


public:
    void setOffset(int offset);
    int offset() const;

    Position position() const;
    void setPosition(const Position &position);
private:
    qint32        m_leftOffset ;    //! 左侧偏移
    qint32        m_rightOffset ;   //! 右侧偏移
    Position      m_position ;      //! 位置
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

class CTagPos
{
public:
    CTagPos(){}

    CTagPos(const CTagPos &other);
    ~CTagPos(){}

    CTagHPos hPos() const;
    void setHPos(const CTagHPos &hPos);

    CTagVPos vPos() const;
    void setVPos(const CTagVPos &vPos);

    CTagPos &operator =(const CTagPos &other);
private:
    CTagHPos m_hPos;
    CTagVPos m_vPos;

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

};
Q_DECLARE_METATYPE(CTagPos)
#endif // CTAGPOSITION_H
