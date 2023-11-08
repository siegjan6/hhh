#include "reader.h"
#include <QSettings>
#include <QDateTime>
#include <QCoreApplication>
Reader::Reader(QString host, int port)
{
    this->host = host;
    this->port = port;

    socket = new QTcpSocket;
    reading = false;

    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketConnectionClosed()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));    
}

Reader::~Reader()
{
     delete socket;
}

void Reader::connectHost(const QString &host, quint16 port)
{
    if (reading)
        return;

    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);//pinkie
    socket->connectToHost(host, port);
}

bool Reader::isConnected() const
{
    return reading;
}

void Reader::disconnectHost()
{
    socket->disconnectFromHost();
}

void Reader::socketConnected()
{
    reading = true;
    emit connected();
}

void Reader::socketConnectionClosed()
{
    reading = false;
    emit disconnected();
}

void Reader::socketReadyRead()
{
    //parse the package and queue to responseVector
    QByteArray btarr = socket->readAll();
    int nSize = btarr.size();
    recvbuffer.append(btarr);

    if(nSize >1 && (btarr[nSize -2] == char(13) || btarr[nSize -1] == char(10)))
    {
        ParsePackage();
    }
}

void Reader::socketError(QAbstractSocket::SocketError e)
{
    emit error(e);
}

void Reader::sendData(const QByteArray &data)
{
    if (!reading)
        return;

    QMutexLocker locker(&m_processedQueueMutex);

    //QTextStream stream(socket);
    /*
    QString test(data);
    int k;

    QChar c,next;
    QStringList parts;
    QString buffer="";
    bool open=false;
    int nSpaceNum = 0;
    for(k=0;k<test.length();k++)
    {
        c=test.at(k);
        if(open)
        {
            next=k<test.length()-1?test
                       .at(k+1):' ';
            if(c=='\\'&&next=='"')
            {
                buffer+='"';
                k++;
            }
            else if(c=='"')
                open=false;
            else
                buffer+=c;
        }
        else
        {
            if(!c.isSpace())
            {
                if(c=='"')
                    open=true;
                else
                    buffer+=c;
            }
           //解决值中有空格问题

//            else if(!buffer.isEmpty())
//            {
//                parts<<buffer;
//                buffer="";
//            }
            else
            {
                nSpaceNum++;
                if(nSpaceNum <=3)//hset myhash field1 Hello
                {
                    if(!buffer.isEmpty())
                    {
                        parts<<buffer;
                        buffer="";
                    }
                }
                else
                {
                    buffer+=c;
                }
            }
        }
    }//end for

    if(!buffer.isEmpty())
    {
        parts<<buffer;
    }

    QByteArray bin;
    bin.append(QString("*%1\r\n")
       .arg(parts.length()));
    int i;
    */
    /* pinkie
    for(i=0;i<parts.length();i++)
    {
        bin.append(QString("$%1\r\n")
           .arg(parts.at(i)
           .length()));

        bin.append(QString("%1\r\n")
           .arg(parts.at(i)));
    }*/
    /*
    for(i=0;i<parts.length();i++)
    {
        int nsize = parts.at(i).toUtf8().size();       
        bin.append(QString("$%1\r\n")
           .arg(nsize));

        bin.append(QString("%1\r\n")
           .arg(parts.at(i)));
    }

    socket->write(bin,bin.size());
    socket->flush();
    socket->waitForBytesWritten();
    */
    socket->write(data,data.size());
    socket->flush();
    socket->waitForBytesWritten();
}

void Reader::ParsePackage()
{
    QString reply = (QString)recvbuffer;

    QChar first=reply.at(0);
    QString value;
    QStringList result;
    bool bMutiMsg = false;

    if(first == '*')
    {
        int index=reply.indexOf("\r\n");
        int count=reply.mid(1,index-1)
                       .toInt();
        int i;
        int len;
        int pos=index+2;
        result << "list";

        for(i=0;i<count;i++)
        {
            index=reply.indexOf("\r\n",pos);
            if(index == -1)
                break;
            len=reply.mid(pos+1,index-pos-1)
                     .toInt();
            //check if len is all remaining bytes size
            if(len==-1)
            {
                result<<"NULL";
            }
            else
            {
                result<<reply.mid(index+2,len);
                if(i == 2 && result[1] == "message" &&  len >=  (recvbuffer.size() -2-index-2))
                {
                    bMutiMsg = true;
                    result.clear();
                    break;
                }
            }
            pos=index+2+len+2;
        }
        if(bMutiMsg)
        {
            ParseMessage();
            if(responseVector.size() > 0)
                emit subResponse();
        }
        else
        {
            emit response(result);
            recvbuffer.clear();
        }
    }
    else
    {
        if(first=='+')
        {
            value=reply.mid(1);
            value.chop(2);

            result << "string" << value;
            emit response(result);
        }
        else if(first=='-')
        {
            value=reply.mid(1);
            value.chop(2);

            result << "error" << value;
            emit response(result);
        }
        else if(first==':')
        {
            value=reply.mid(1);
            value.chop(2);

            result << "integer" << value;
            emit response(result);

        }
        else if(first=='$')
        {
            int index=reply.indexOf("\r\n");
            int len=reply.mid(1,index-1)
                         .toInt();
            if(len==-1)
                value="NULL";
            else
                value=reply.mid(index+2,len);
            result<< "bulk" << value;
            emit response(result);
        }
        else
        {
            //recvbuffer.append(bt);
        }
        recvbuffer.clear();
    }

}
void Reader::ParseMessage()
{
    QString allstr = (QString)recvbuffer;

    QByteArray temparr;
    int index=0;
    int count=0;

    int pos=0,pos1 = 0;
    int i = 0;
    int len = 0;
    bool msgok = false;
    responseVector.clear();
    temparr.append(recvbuffer);
    while(1)
    {
        msgok = false;
        QStringList result;
        index=allstr.indexOf("\r\n");
        if(index == -1)
            break;

        count=allstr.mid(1,index-1).toInt();
        pos=index+2;
        i = 0;
        for(i=0;i<count;i++)
        {
            index=allstr.indexOf("\r\n",pos);
            if(index == -1)
                break;
            len=allstr.mid(pos+1,index-pos-1).toInt();
            if(len==-1)
            {
                result<<"NULL";
            }
            else
            {
                QByteArray bttemp = recvbuffer.mid(pos1+index+2,len);
                result<<QString(bttemp);
                if(i == 2)
                {
                    if((index+2+len+2) == temparr.size())
                    {
                        msgok = true;
                    }
                }
//                if(i == 2 && ( (index+2+len+2) == temparr.size()))
//                    msgok = true;
            }
            pos=index+2+len+2;
        }
        //the above msg is ok
        if(msgok)
        {
            responseVector.append(result);
            result.clear();
            pos1 += pos;

            if(pos1 < recvbuffer.size())
            {
                temparr = recvbuffer.mid(pos1);
                //碎包
                if(temparr.size() < 8)
                {
                    recvbuffer.clear();
                    recvbuffer.append(temparr);
                    break;
                }
                else //continue to parse
                {
                    allstr = QString(temparr);
                }
            }
            else//ok,全部解析完
            {
                recvbuffer.clear();
                break;
            }
        }
        else//碎包
        {
            recvbuffer.clear();
            recvbuffer.append(temparr);
            break;
        }
    }//end while
}
