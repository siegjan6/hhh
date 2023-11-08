#ifndef NSENUMS_H
#define NSENUMS_H
namespace CommandQueueType
{
    enum _CommandQueueType
    {
        Read=0,
        Write=1,
        Parse=2,
    };

}

namespace NSDriverLibs
{
    enum NSDriverType
    {
        Serial=0,
        TCPUDP=1,
    };
}

namespace ProtocolCommand
{
  enum ProtocolCommandType
   {
       Read = 0,
       Write = 1,
       BlockWrite = 2,
   };

   enum ProtocolCommandResult
   {
       None = 0,
       Right = 1,
       RightAndNextCmd = 2,
       SplitData = 3,
       Error = 4,
   };

}

namespace Communication
{
    enum CommunicationStatus
       {
           DisConnected = 0,//断开连接
           Connected = 1,   //已经建立连接
           Connecting = 2   //正在连接
       };
     enum CommunicationType
       {
           Serial = 0,     //串口通信
           TCP = 1,        //TCP通信
           UDP = 2,        //UDP通信
           Board = 3,      //板卡
           USB = 4,		   //USB
           Other = 5,	   //其它
       };
}

namespace NSVariable
{

    enum NSVariableDataType
    {
    BOOL = 0,
    SBYTE = 1,
    BYTE = 2,
    SHORT = 3,
    USHORT = 4,
    INT = 5,
    UINT = 6,
    LONG = 7,
    ULONG = 8,
    FLOAT = 9,
    DOUBLE = 10,
    DATETIME = 11,
    STRING = 12,
    };


}

namespace NSDataQuality
{
    enum _NSDataQuality
    {
        None = 0,
        Good = 1,
        AddressBad = 2,
        CommBad = 3,
        TimeoutError = 4,
        ProtocolError = 5,
        DataTypeError = 6,
    };
}
#endif // NSENUMS_H
