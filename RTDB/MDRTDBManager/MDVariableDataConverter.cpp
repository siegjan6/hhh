#include "MDVariableDataConverter.h"
#include <QtCore/qmath.h>
CMDVariableDataConverter::CMDVariableDataConverter()
{
}

//! 由原始值获取工程值，分别调用4种类型转换
bool CMDVariableDataConverter::getEngineeringValue(const MDVariableDataConversionParameter& dataConversionParameter,
                                                    const MDDataType &originalDataType,const MDDataType & dataType,const int & precision,
                                                    const QVariant &originalValue,QVariant &engineeringValue )
{
    bool ret = true;
    if(dataConversionParameter.m_conversionType == MDVariableDataConversionType::None ||
           dataConversionParameter.m_enableLinearConversion == false)
    {
        if(dataType == MDDataType::String || dataType == MDDataType::DateTime)
            engineeringValue = originalValue;
        else
            ret = convertDbToVariant(originalValue.toDouble(),dataType,engineeringValue);
        return ret;
    }

    double beforevalue = originalValue.toDouble();
    //先类型转换
    switch (dataConversionParameter.m_conversionType)
    {
    case MDVariableDataConversionType::BCD3:
        {
            ret = BCD3ToDecimal(dataConversionParameter,
                                originalValue,
                                dataType,
                                beforevalue);
        }
        break;
    case MDVariableDataConversionType::BCD4:
        {
            ret = BCD4ToDecimal(dataConversionParameter,
                                originalValue,
                                dataType,
                                beforevalue);
        }
        break;
    case MDVariableDataConversionType::BCD8:
        {
            ret = BCD8ToDecimal(dataConversionParameter,
                                originalValue,
                                dataType,
                                beforevalue);
        }
        break;
    case MDVariableDataConversionType::OMRONBCD4:
        {
            ret = OmronBCD4ToDecimal(dataConversionParameter,
                                     originalValue,
                                     dataType,
                                     beforevalue);
        }
        break;
    case MDVariableDataConversionType::SiemensBCD4:
        {
            ret = SiemensBCD4ToDecimal( dataConversionParameter,
                                        originalValue,
                                        dataType,
                                        beforevalue);
        }
        break;
    case MDVariableDataConversionType::ByteOrderChange:
        {
            ret = byteOrderChange(dataConversionParameter,
                                  originalDataType,
                                     originalValue,
                                     dataType,
                                     beforevalue);
        }
        break;
    case MDVariableDataConversionType::Bit8BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit8BinaryWithAlarm:
        {
            ret = binary8ToDecimal(dataConversionParameter,
                                   originalValue,
                                   dataType,
                                   beforevalue);
        }
        break;
    case MDVariableDataConversionType::Bit12BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit12BinaryWithAlarm:
        {
            ret = binary12ToDecimal(dataConversionParameter,
                                   originalValue,
                                   dataType,
                                   beforevalue);
        }
        break;
    case MDVariableDataConversionType::Bit13BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit13BinaryWithAlarm:
        {
            ret = binary13ToDecimal(dataConversionParameter,
                                   originalValue,
                                   dataType,
                                   beforevalue);
        }
        break;
    case MDVariableDataConversionType::Bit15BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit15BinaryWithAlarm:
        {
            ret = binary15ToDecimal(dataConversionParameter,
                                   originalValue,
                                   dataType,
                                   beforevalue);
        }
        break;
    default:
        break;
    }
    //再量程转换
    if(ret)
        ret = getY(dataConversionParameter,beforevalue,dataType,engineeringValue);
    return ret;
}
//! 由工程值获取原始值
bool CMDVariableDataConverter::getOriginalValue(const MDVariableDataConversionParameter& dataConversionParameter,
                                                      const MDDataType &originalDataType,const MDDataType & dataType,const int & precision,
                                                      const QVariant &engineeringValue,QVariant &originalValue )
{
    bool ret = true;
    if(dataConversionParameter.m_conversionType == MDVariableDataConversionType::None ||
           dataConversionParameter.m_enableLinearConversion == false)
    {
        convertDbToVariant(engineeringValue.toDouble(),originalDataType,originalValue);
        return ret;
    }
    //先量程反转换
    double x;
    getX(dataConversionParameter,engineeringValue.toDouble(),x);

    //再类型反转换
    switch (dataConversionParameter.m_conversionType)
    {
    case MDVariableDataConversionType::BCD3:
        {
               ret = decimalToBCD3(   dataConversionParameter,
                                      originalDataType,
                                      originalValue,
                                      x);
        }
        break;
    case MDVariableDataConversionType::BCD4:
        {
               ret = decimalToBCD4(   dataConversionParameter,
                                      originalDataType,
                                      originalValue,
                                      x);
        }
        break;
    case MDVariableDataConversionType::BCD8:
        {
               ret = decimalToBCD8(   dataConversionParameter,
                                      originalDataType,
                                      originalValue,
                                      x);
        }
        break;
    case MDVariableDataConversionType::OMRONBCD4:
        {
              ret = decimalToOmronBCD4(   dataConversionParameter,
                                          originalDataType,
                                          originalValue,
                                          x);
        }
        break;
    case MDVariableDataConversionType::SiemensBCD4:
        {
              ret = decimalToSiemensBCD4(   dataConversionParameter,
                                            originalDataType,
                                            originalValue,
                                            x);
        }
        break;
    case MDVariableDataConversionType::ByteOrderChange:
        {
            ret = ReverseByteOrderChange(dataConversionParameter,
                                         originalDataType,
                                         originalValue,
                                         dataType,
                                         x);
        }
        break;
    case MDVariableDataConversionType::Bit8BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit8BinaryWithAlarm:
        {
            ret = decimalToBinary8(dataConversionParameter,
                                   originalDataType,
                                   originalValue,
                                   x);
        }
        break;
    case MDVariableDataConversionType::Bit12BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit12BinaryWithAlarm:
        {
            ret = decimalToBinary12(dataConversionParameter,
                                   originalDataType,
                                   originalValue,
                                   x);
        }
        break;
    case MDVariableDataConversionType::Bit13BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit13BinaryWithAlarm:
        {
            ret = decimalToBinary13(dataConversionParameter,
                                   originalDataType,
                                   originalValue,
                                   x);
        }
        break;
    case MDVariableDataConversionType::Bit15BinaryWithoutAlarm:
    case MDVariableDataConversionType::Bit15BinaryWithAlarm:
        {
            ret = decimalToBinary15(dataConversionParameter,
                                   originalDataType,
                                   originalValue,
                                   x);
        }
        break;
    default:
        break;
    }
    if(ret)
        ret = convertDbToVariant(x,originalDataType,originalValue);
     return ret;
}

//线性转换
//正向线性转换(原始值转工程值）
bool CMDVariableDataConverter::forwardLinearConversion(const MDVariableDataConversionParameter& dataConversionParameter,
                                                    const MDDataType &originalDataType,const MDDataType & dataType,const int & precision,
                                                    const QVariant &originalValue,QVariant &engineeringValue )
{
    bool ret = false;
    /*
    if(originalDataType == MDDataType::Bool||
        originalDataType == MDDataType::DateTime||
        originalDataType == MDDataType::String)
    {
        engineeringValue= originalValue;
        ret = true;
        return ret;
    }
    double diff0 = dataConversionParameter.m_originalMaxValue - dataConversionParameter.m_originalMinValue ;
    double diff = dataConversionParameter.m_maxValue - dataConversionParameter.m_minValue ;
    if(diff0!=0)
    {
      double k=diff/diff0;
      double dbEngineeringValue = k*(originalValue.toDouble()-dataConversionParameter.m_originalMinValue)+dataConversionParameter.m_minValue;
      ret = convertDbToVariant(dbEngineeringValue,dataType,engineeringValue);
    }*/
    ret = getY(dataConversionParameter,originalValue.toDouble(),dataType,engineeringValue);
    return ret;
}

//反向线性转换
bool CMDVariableDataConverter::reverseLinearConversion(const MDVariableDataConversionParameter& dataConversionParameter,
                                                const MDDataType &originalDataType,
                                                const MDDataType & dataType,
                                                const int & precision,
                                                const QVariant &engineeringValue,
                                                QVariant &originalValue )
{
    bool ret = false;
    /*
    if(dataType == MDDataType::Bool||
        dataType == MDDataType::DateTime||
        dataType == MDDataType::String)
    {
        originalValue = engineeringValue;
        ret = true;
        return ret;
    }

    double diff0 = dataConversionParameter.m_originalMaxValue - dataConversionParameter.m_originalMinValue ;
    double diff = dataConversionParameter.m_maxValue - dataConversionParameter.m_minValue ;
    if(diff!=0)
    {
      double k=diff0/diff;
      double dbOriginalValue = k*(engineeringValue.toDouble()-dataConversionParameter.m_minValue)+dataConversionParameter.m_originalMinValue;
      ret = convertDbToVariant(dbOriginalValue,originalDataType,originalValue);
    }*/
    double x = 0;
    ret = getX(dataConversionParameter,engineeringValue.toDouble(),x);
    ret = convertDbToVariant(x,originalDataType,originalValue);
    return ret;
}
/*
 * 关于BCD码的编码和解码
把十进制数的每一位分别写成二进制形式的编码，称为二进制编码的十进制数，
即二到十进制编码或BCD（Binary Coded Decimal）编码。

BCD码编码方法很多，通常采用8421编码，这种编码方法最自然简单。
其方法使用四位二进制数表示一位十进制数，从左到右每一位对应的权分别是
23、22、21、20，即8、4、2、1。例如十进制数1975的8421码可以这样得出
1975（D）=0001 1001 0111 0101（BCD）
用四位二进制表示一位十进制会多出6种状态，这些多余状态码称为BCD码中的非法码。
BCD码与二进制之间的转换不是直接进行的，
当需要将BCD码转换成二进制码时，要先将BCD码转换成十进制码，然后再转换成二进制码；
当需要将二进制转换成BCD码时，要先将二进制转换成十进制码，然后再转换成BCD码。


压缩与非压缩：由于1字节有8bit(8个0或1)如果用一字节存储4位BCD码(其余补0)就是非压缩,如十进制的123：00000001 00000010 0011,如果用一字节存储8位BCD码就是压缩,如 十进制的123：0001 0010 0011



编码过程，将数字69进行BCD编码（注：BCD编码低位在前，后面将不再注释）。

1.       将6，9分别转换成二进制表示：6（00000110）9（00001001），大家可以看到，最大的数字9也只要4个位，在传输过程中白白浪费了4个位；

2.       将69合并为一个字节，分别取6，9二进制编码的低4位，按照低位在前的原则，将9的低四位放前面6的低四位放后面得出新的字节二进制编码是10010110；

3.       完成编码过程，69的BCD编码结果为10010110。

解码过程：将69的BCD码10010110进行解码。

1.       将10010110的高4位与低4位拆分开，得到两个二进制数1001和0110；

2.       分别将1001和0110的前面补充4位0000得到两个8位的二进制数00001001，00000110；

3.       因为编码时低位在前，所以我们将两个二进制数编排顺序为00000110 000010001；



1.BCD码如何表示一个有符号数，如一个负数用BCD如何表示？
 在STEP7中的16位BCD码格式是1位符号位+3位BCD码数值范围-999至+999；32位BCD码格式是1位符号位+7位BCD码数值范围-9999999至+9999999；
 例如16位BCD码存储形式：
 0000，0001，0010，0011；其对应符号位为+,百位为1，十位为2，个位为3.计算公式：
 十进制=符号位（BCD码的百位x100+BCD码的十位x10+BCD码的个位）
 32位BCD码格式同理。
 STEP7中有专用指令：BTI,ITB,BTD,DTB来实现3位或7为的BCD码与10进制数之间的转换。

*/

bool CMDVariableDataConverter::BCD3ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                                             const QVariant &originalValue,
                                             const MDDataType &dataType,
                                             double&engineeringValue)
{
    bool ret = false;
    quint16 originalValueTemp = quint16(originalValue.toUInt());
    originalValueTemp &= 0xFFF;
    if(originalValueTemp > quint16(0x999))
        return ret;

    double dbVal = 0;
    switch (dataType)
    {
      case MDDataType::Byte:
      case MDDataType::SByte:
          {
              quint8 BCDV2 =(originalValueTemp>>4)&0x0F;
              quint8 BCDV1 =(originalValueTemp)&0x0F;
              if(BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::Int16:
      case MDDataType::UInt32:
      case MDDataType::Int32:
      case MDDataType::UInt64:
      case MDDataType::Int64:
      case MDDataType::Float:
      case MDDataType::Double:
          {
              quint8 BCDV3 =(originalValueTemp>>8)&0x000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x000F;
              quint8 BCDV1 =(originalValueTemp)&0x000F;
              if(BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:      
      default:
          break;
    }//end switch
    if(ret)
        engineeringValue = dbVal;
    return ret;
}
//4位BCD转十进制
bool CMDVariableDataConverter::BCD4ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,const QVariant &originalValue, const MDDataType &dataType,double&engineeringValue)
{
    bool ret = false;
    double dbVal = 0;
    quint16 originalValueTemp = quint16(originalValue.toUInt());
    if(originalValueTemp > quint16(0x9999))
        return ret;
    switch (dataType)
    {
      case MDDataType::Byte:
      case MDDataType::SByte:
          {
              quint8 BCDV2 =(originalValueTemp>>4)&0x0F;
              quint8 BCDV1 =(originalValueTemp)&0x0F;
              if(BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::Int16:
      case MDDataType::UInt32:
      case MDDataType::Int32:
      case MDDataType::UInt64:
      case MDDataType::Int64:
      case MDDataType::Float:
      case MDDataType::Double:
          {
              quint8 BCDV4 =(originalValueTemp>>12)&0x000F;
              quint8 BCDV3 =(originalValueTemp>>8)&0x000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x000F;
              quint8 BCDV1 =(originalValueTemp)&0x000F;
              if(BCDV4>9||BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV4*qPow(10,3)+ BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1);
              ret = true;
          }
          break;

      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:     
      default:
          break;
    }//end switch
    if(ret)
        engineeringValue = dbVal;
    return ret;
}

//8位BCD转十进制
bool CMDVariableDataConverter::BCD8ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,const QVariant &originalValue, const MDDataType &dataType,double&engineeringValue)
{
    bool ret = false;
    quint32 originalValueTemp = (quint32)(originalValue.toUInt());
    if(originalValueTemp > quint32(0x99999999))
        return ret;

    double dbVal = 0;
    switch (dataType)
    {
      case MDDataType::Byte:
      case MDDataType::SByte:
          {
              quint8 BCDV2 =(originalValueTemp>>4)&0x0F;
              quint8 BCDV1 =(originalValueTemp)&0x0F;
              if(BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::Int16:
          {
              quint8 BCDV4 =(originalValueTemp>>12)&0x000F;
              quint8 BCDV3 =(originalValueTemp>>8)&0x000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x000F;
              quint8 BCDV1 =(originalValueTemp)&0x000F;
              if(BCDV4>9||BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV4*qPow(10,3)+ BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::UInt32:
      case MDDataType::Int32:
      case MDDataType::UInt64:
      case MDDataType::Int64:
      case MDDataType::Float:
      case MDDataType::Double:
          {
              quint8 BCDV8 =(originalValueTemp>>28)&0x0000000F;
              quint8 BCDV7 =(originalValueTemp>>24)&0x0000000F;
              quint8 BCDV6 =(originalValueTemp>>20)&0x0000000F;
              quint8 BCDV5 =(originalValueTemp>>16)&0x0000000F;
              quint8 BCDV4 =(originalValueTemp>>12)&0x0000000F;
              quint8 BCDV3 =(originalValueTemp>>8)&0x0000000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x0000000F;
              quint8 BCDV1 =(originalValueTemp)&0x0000000F;
              if(BCDV8>9||BCDV7>9||BCDV6>9||BCDV5>9
               ||BCDV4>9||BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV8*(10,8)+ BCDV7*qPow(10,6)+ BCDV6*qPow(10,5)+ BCDV5*qPow(10,4)+BCDV4*qPow(10,3)+ BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:      
      default:
          break;
    }//end switch
    if(ret)
        engineeringValue = dbVal;
    return ret;
}
//Omron 4BCD：有符号BCD码最高1位二进制位为符号位
bool CMDVariableDataConverter::OmronBCD4ToDecimal(  const MDVariableDataConversionParameter& dataConversionParameter,
                                                    const QVariant &originalValue,
                                                    const MDDataType &dataType,
                                                    double &engineeringValue )
{
    bool ret = false;
    quint16 originalValueTemp = quint16(originalValue.toUInt());
    if(originalValueTemp > quint16(0x9999))
        return ret;
    double dbVal = 0;
    switch (dataType)
    {
      case MDDataType::Byte:
          {
              quint8 BCDV2 =(originalValueTemp>>4)&0x0F;
              quint8 BCDV1 =(originalValueTemp)&0x0F;
              if(BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::UInt32:
      case MDDataType::UInt64:
          {
              quint8 BCDV4 =(originalValueTemp>>12)&0x000F;
              quint8 BCDV3 =(originalValueTemp>>8)&0x000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x000F;
              quint8 BCDV1 =(originalValueTemp)&0x000F;
              if(BCDV4>9||BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal= double(BCDV4*qPow(10,3)+ BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::SByte:
          {
              quint8 BCDV2 =(originalValueTemp>>4)&0x0F;
              quint8 BCDV1 =(originalValueTemp)&0x0F;
              quint8 symbol = (((BCDV2) >> (4)) & (0x01));
              BCDV2 = (((BCDV2) >> (3)) & (0x01));

              if(BCDV2>7||BCDV1>9)
              {
                 return ret;
              }
              if(symbol>0)
              {
                  dbVal= double((BCDV2*10+BCDV1)*(-1));
              }
              else
              {
                  dbVal= double(BCDV2*10+BCDV1);
              }
              ret = true;
          }
          break;
      case MDDataType::Int16:
      case MDDataType::Int32:
      case MDDataType::Int64:
      case MDDataType::Float:
      case MDDataType::Double:
          {
              quint8 BCDV4 =(originalValueTemp>>12)&0x000F;
              quint8 BCDV3 =(originalValueTemp>>8)&0x000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x000F;
              quint8 BCDV1 =(originalValueTemp)&0x000F;
              quint8 symbol = (((BCDV4) >> (4)) & (0x01));
              BCDV4 = (((BCDV4) >> (3)) & (0x01));

              if(BCDV4>7||BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              if(symbol>0)
              {
                  dbVal= double((BCDV4*qPow(10,3)+BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1)*(-1));
              }
              else
              {
                  dbVal= double((BCDV4*qPow(10,3)+BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1));
              }
              ret = true;
          }
          break;
    case MDDataType::Bool:
    case MDDataType::DateTime:
    case MDDataType::String:    
    default:
        break;
    }
    if(ret)
        engineeringValue = dbVal;
    return ret;
}

//Siemens 4BCD：有符号BCD码最高4位二进制位为符号位
bool CMDVariableDataConverter::SiemensBCD4ToDecimal(  const MDVariableDataConversionParameter& dataConversionParameter,
                                                      const QVariant &originalValue,
                                                      const MDDataType &dataType,
                                                      double &engineeringValue)
{
    bool ret = false;
    quint16 originalValueTemp = quint16(originalValue.toUInt());
    if(originalValueTemp > quint16(0x9999))
        return ret;
    double dbVal = 0;
    switch (dataType)
    {
      case MDDataType::Byte:
          {
              quint8 BCDV2 =(originalValueTemp>>4)&0x0F;
              quint8 BCDV1 =(originalValueTemp)&0x0F;
              if(BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal = double(BCDV2*10+ BCDV1);
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::UInt32:
      case MDDataType::UInt64:
          {
              quint8 BCDV4 =(originalValueTemp>>12)&0x000F;
              quint8 BCDV3 =(originalValueTemp>>8)&0x000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x000F;
              quint8 BCDV1 =(originalValueTemp)&0x000F;
              if(BCDV4>9||BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              dbVal = double(BCDV4*qPow(10,3)+ BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1);
              ret = true;
          }
          break;      
      case MDDataType::SByte:
          {
              quint8 BCDV2 =(originalValueTemp>>4)&0x0F;
              quint8 BCDV1 =(originalValueTemp)&0x0F;
              if(BCDV1>9)
              {
                 return ret;
              }
              if(BCDV2>0)
              {
                 dbVal = double(BCDV1*(-1));
              }
              else
              {
                  dbVal = double(BCDV1);
              }
              ret = true;
          }
          break;
      case MDDataType::Int16:
      case MDDataType::Int32:
      case MDDataType::Int64:        
      case MDDataType::Float:
      case MDDataType::Double:
          {
              quint8 BCDV4 =(originalValueTemp>>12)&0x000F;
              quint8 BCDV3 =(originalValueTemp>>8)&0x000F;
              quint8 BCDV2 =(originalValueTemp>>4)&0x000F;
              quint8 BCDV1 =(originalValueTemp)&0x000F;
              if(BCDV3>9||BCDV2>9||BCDV1>9)
              {
                 return ret;
              }
              if(BCDV4>0)
              {
                  dbVal = double((BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1)*(-1));
              }
              else
              {
                  dbVal = double(BCDV3*qPow(10,2)+ BCDV2*10+ BCDV1);
              }
              ret = true;
          }
          break;
        case MDDataType::Bool:
        case MDDataType::DateTime:
        case MDDataType::String:
        default:
            break;
    }
    if(ret)
        engineeringValue = dbVal;
    return ret;
}
//!数据字节顺序转换
bool CMDVariableDataConverter::byteOrderChange(  const MDVariableDataConversionParameter& dataConversionParameter,
                                                 const MDDataType &originalDataType,
                                                 const QVariant &originalValue,
                                                 const MDDataType &dataType,
                                                 double &engineeringValue)
{
    bool ret = false;
    QStringList strList = dataConversionParameter.m_byteOrder.split("-");
    if(strList.count() < 2)//at least 0,1,short
        return ret;
    double dbvalue;
    switch (originalDataType)
    {
      case MDDataType::UInt16:
      case MDDataType::Int16:
            {
                if(strList.count() != 2 || (dataType != MDDataType::UInt16 && dataType != MDDataType::Int16))//at least 0,1,short
                    return ret;
                if(strList.at(0) == "1")
                {
                    quint16 data = (quint16)(originalValue.toUInt());
                    quint8 temp[2] = {0};
                    quint8 temp1[2] = {0};
                    memcpy(temp,&data,2);
                    temp1[0] = temp[1];
                    temp1[1] = temp[0];
                    memcpy(&data,temp1,2);
                    dbvalue = double(data);
                }
                else
                {
                    dbvalue = double(quint16(originalValue.toUInt()));
                }
                ret = true;
            }
            break;
      case MDDataType::UInt32:
      case MDDataType::Int32:
            {
                if(dataType != MDDataType::UInt32 &&
                   dataType != MDDataType::Int32 &&
                   dataType != MDDataType::Float)
                    return ret;
                if(strList.count() != 4)
                    return ret;

                quint32 data = (quint32)(originalValue.toUInt());
                quint8 temp[4] = {0};
                quint8 temp1[4] = {0};
                memcpy(temp,&data,4);
                //按顺序放数据
                int ntemp = 0;
                for(int i = 0; i<4; i++)
                {
                    ntemp = strList.at(i).toInt();
                    if(ntemp > 3)
                        return ret;
                    temp1[i] = temp[ntemp];
                }
                //float
                if(dataType == MDDataType::Float)
                {
                    float ftdata;
                    memcpy(&ftdata,temp1,4);
                    dbvalue = double(ftdata);
                }
                else
                {
                    memcpy(&data,temp1,4);
                    dbvalue = double(data);
                }

                ret = true;
            }
          break;
      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:
      case MDDataType::Float:
      case MDDataType::Double:
      default:
          break;
    }//end switch
    if(ret)
        engineeringValue = dbvalue;
    return ret;
}

//!8位二进制转换无报警
bool CMDVariableDataConverter::binary8ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                             const QVariant &originalValue,
                             const MDDataType &dataType,
                             double &engineeringValue)
{
    bool ret = false;
    quint64 tempval = (quint64)originalValue.toULongLong();
    tempval &= 0xff;
    if(tempval <= 255)
    {
        ret = true;
        engineeringValue = double(tempval);
    }
    return ret;
}

//!12位二进制转换无报警
bool CMDVariableDataConverter::binary12ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                             const QVariant &originalValue,
                             const MDDataType &dataType,
                             double &engineeringValue)
{
    bool ret = false;

    quint64 tempval = (quint64)originalValue.toULongLong();
    if(tempval <= 0xFFF)
    {
        ret = true;
        engineeringValue = double(tempval);
    }
    return ret;
}

//!13位二进制转换无报警
bool CMDVariableDataConverter::binary13ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                             const QVariant &originalValue,
                             const MDDataType &dataType,
                             double &engineeringValue)
{
    bool ret = false;

    quint64 tempval = (quint64)originalValue.toULongLong();
    if(tempval <= 0x1FFF)
    {
        ret = true;
        engineeringValue = double(tempval);
    }
    return ret;
}

//!15位二进制转换无报警
bool CMDVariableDataConverter::binary15ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                             const QVariant &originalValue,
                             const MDDataType &dataType,
                             double &engineeringValue)
{
    bool ret = false;

    quint64 tempval = (quint64)originalValue.toULongLong();
    if(tempval <= 0xEFFF)
    {
        ret = true;
        engineeringValue = double(tempval);
    }
    return ret;
}

bool  CMDVariableDataConverter::ReverseByteOrderChange(const MDVariableDataConversionParameter& dataConversionParameter,
                                  const MDDataType &originalDataType,
                                  QVariant &originalValue,
                                  const MDDataType &dataType,
                                  double &engineeringValue)
{
    bool ret = false;
    QStringList strList = dataConversionParameter.m_byteOrder.split("-");
    if(strList.count() < 2)//at least 0,1,short
        return ret;
    //再类型转换
    switch (originalDataType)
    {
      case MDDataType::UInt16:
      case MDDataType::Int16:
            {
                if(strList.count() != 2 || (dataType != MDDataType::UInt16 && dataType != MDDataType::Int16))//at least 0,1,short
                    return ret;
                if(strList.at(0) == "1")
                {
                    quint16 data = (quint16)(engineeringValue);
                    quint8 temp[2] = {0};
                    quint8 temp1[2] = {0};
                    memcpy(temp,&data,2);
                    temp1[0] = temp[1];
                    temp1[1] = temp[0];
                    memcpy(&data,temp1,2);
                    originalValue = QVariant((quint16)data);
                }
                else
                {
                    originalValue = QVariant(quint16(engineeringValue));
                }
                ret = true;
            }
            break;
      case MDDataType::UInt32:
      case MDDataType::Int32:
            {
                if(dataType != MDDataType::UInt32 &&
                   dataType != MDDataType::Int32 &&
                   dataType != MDDataType::Float)
                    return ret;
                if(strList.count() != 4)
                    return ret;

                //内存拷贝
                quint8 temp[4] = {0};
                quint8 temp1[4] = {0};
                //float
                if(dataType == MDDataType::Float)
                {
                    float ftdata = float(engineeringValue);
                    memcpy(temp,&ftdata,4);
                }
                else
                {
                    quint32 uidata = quint32(engineeringValue);
                    memcpy(temp,&uidata,4);
                }
                //按顺序放数据
                int ntemp = 0;
                for(int i = 0; i<4; i++)
                {
                    ntemp = strList.at(i).toInt();
                    if(ntemp > 3)
                        return ret;
                    temp1[ntemp] = temp[i];
                }
                //赋值
                quint32 ordata = 0;
                memcpy(&ordata,temp1,4);
                originalValue = QVariant((quint32)ordata);
                ret = true;
            }
          break;
      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:
      case MDDataType::Float:
      case MDDataType::Double:
      default:
          break;
    }//end switch
    return ret;
}

//十进制转3位BCD
bool CMDVariableDataConverter::decimalToBCD3(const MDVariableDataConversionParameter& dataConversionParameter,
                                              const MDDataType &originalDataType,
                                              QVariant &originalValue,
                                              double &engineeringValue)
{
    if(quint16(engineeringValue) > 999)
        return false;

    bool ret = false;
    switch (originalDataType)
    {
      case MDDataType::Byte:
      case MDDataType::SByte:
          {
              quint8 engineeringValueTemp= (quint8)engineeringValue;
              if(engineeringValueTemp>99)
              {
                 return ret;
              }
              quint8 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 1; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::Int16:
      case MDDataType::UInt32:
      case MDDataType::Int32:
      case MDDataType::UInt64:
      case MDDataType::Int64:
          {
              quint16 engineeringValueTemp= (quint16)engineeringValue;
              if(engineeringValueTemp>9999)
              {
                 return ret;
              }
              quint16 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 3; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:
      case MDDataType::Float:
      case MDDataType::Double:
      default:
        break;
    }
    return ret;
}
//十进制转4位BCD
bool CMDVariableDataConverter::decimalToBCD4( const MDVariableDataConversionParameter& dataConversionParameter,
                                              const MDDataType &originalDataType,
                                              QVariant &originalValue,
                                              double &engineeringValue)
{
    if(quint16(engineeringValue) > 9999)
        return false;

    bool ret = false;
    switch (originalDataType)
    {
      case MDDataType::Byte:
      case MDDataType::SByte:
          {
              quint8 engineeringValueTemp= (quint8)engineeringValue;
              if(engineeringValueTemp>99)
              {
                 return ret;
              }
              quint8 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 1; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::Int16:
      case MDDataType::UInt32:
      case MDDataType::Int32:
      case MDDataType::UInt64:
      case MDDataType::Int64:
          {
              quint16 engineeringValueTemp= (quint16)engineeringValue;
              if(engineeringValueTemp>9999)
              {
                 return ret;
              }
              quint16 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 4; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:
      case MDDataType::Float:
      case MDDataType::Double:
      default:
        break;
    }
    return ret;
}
////十进制转8位BCD
bool CMDVariableDataConverter::decimalToBCD8(const MDVariableDataConversionParameter& dataConversionParameter,
                                              const MDDataType &originalDataType,
                                              QVariant &originalValue,
                                              double &engineeringValue)
{
    if(quint32(engineeringValue) > 99999999)
        return false;

    bool ret = false;
    switch (originalDataType)
    {
      case MDDataType::Byte:
      case MDDataType::SByte:
          {
              quint8 engineeringValueTemp= (quint8)engineeringValue;
              if(engineeringValueTemp>99)
              {
                 return ret;
              }
              quint8 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 1; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::Int16:
          {
              quint16 engineeringValueTemp= (quint16)engineeringValue;
              if(engineeringValueTemp>9999)
              {
                 return ret;
              }
              quint16 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 4; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::UInt32:
      case MDDataType::Int32:
      case MDDataType::UInt64:
      case MDDataType::Int64:
          {
              quint32 engineeringValueTemp= (quint32)engineeringValue;
              if(engineeringValueTemp>99999999)
              {
                 return ret;
              }
              quint16 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 8; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::Bool:
      case MDDataType::DateTime:
      case MDDataType::String:
      case MDDataType::Float:
      case MDDataType::Double:
      default:
        break;
    }
    return ret;
}
//十进制转OMRON 4BCD：有符号BCD码最高1位二进制位为符号位
bool CMDVariableDataConverter::decimalToOmronBCD4(const MDVariableDataConversionParameter& dataConversionParameter,
                                                  const MDDataType &originalDataType,
                                                  QVariant &originalValue,
                                                  double &engineeringValue)
{
    bool ret = false;
   if(quint16(engineeringValue) > 9999)
       return false;

   switch (originalDataType)
   {
     case MDDataType::Byte:
         {
             quint8 engineeringValueTemp= (quint8)engineeringValue;
             if(engineeringValueTemp>99)
             {
                return ret;
             }
             quint8 BCDV =0;
             quint8 digit =0;
             for (int i = 0; i < 1; i++)
             {
                 digit = engineeringValueTemp % 10;
                 engineeringValueTemp /= 10;
                 BCDV |= digit << (i*4);
             }
             originalValue= QVariant(quint16(BCDV));
             ret = true;
         }
         break;
     case MDDataType::UInt16:
     case MDDataType::UInt32:
     case MDDataType::UInt64:
         {
             quint16 engineeringValueTemp= (quint16)engineeringValue;
             if(engineeringValueTemp>9999)
             {
                return ret;
             }
             quint16 BCDV =0;
             quint8 digit =0;
             for (int i = 0; i < 4; i++)
             {
                 digit = engineeringValueTemp % 10;
                 engineeringValueTemp /= 10;
                 BCDV |= digit << (i*4);
             }
             originalValue= QVariant(quint16(BCDV));
             ret = true;
         }
         break;
     case MDDataType::SByte:
         {
             qint8 engineeringValueTemp= (qint8)engineeringValue;
             qint8 engineeringValueTempAbs=qAbs(engineeringValueTemp);
             if(engineeringValueTempAbs>79)
             {
                 return ret;
             }
             qint8 BCDV =0;
             qint8 digit =0;
             for (int i = 0; i < 2; i++)
             {
                 digit = engineeringValueTempAbs % 10;
                 engineeringValueTempAbs /= 10;
                 BCDV |= digit << (i*4);
             }

             if(engineeringValueTemp<0)
             {
                 BCDV |= 0x08 << 4;
             }
             originalValue= QVariant(qint16(BCDV));
             ret = true;
         }
         break;
     case MDDataType::Int16:
     case MDDataType::Int32:
     case MDDataType::Int64:
         {
           qint16 engineeringValueTemp= (qint16)engineeringValue;
           qint16 engineeringValueTempAbs=qAbs(engineeringValueTemp);
           if(engineeringValueTempAbs>7999)
           {
               return ret;
           }
           qint16 BCDV =0;
           qint8 digit =0;
           for (int i = 0; i < 4; i++)
           {
               digit = engineeringValueTempAbs % 10;
               engineeringValueTempAbs /= 10;
               BCDV |= digit << (i*4);
           }
           if(engineeringValueTemp<0)
           {
               BCDV |= 0x08 << 12;
           }
           originalValue= QVariant(qint16(BCDV));
           ret = true;
         }
         break;
   case MDDataType::Bool:
   case MDDataType::DateTime:
   case MDDataType::String:
   case MDDataType::Float:
   case MDDataType::Double:
   default:
       break;
   }
   return ret;
}
//十进制转Siemens 4BCD：有符号BCD码最高4位二进制位为符号位
bool CMDVariableDataConverter::decimalToSiemensBCD4(const MDVariableDataConversionParameter& dataConversionParameter,
                                                    const MDDataType &originalDataType,
                                                    QVariant &originalValue,
                                                    double &engineeringValue)
{
    if(quint32(engineeringValue) > 9999)
        return false;

    bool ret = false;

    switch (originalDataType)
    {
      case MDDataType::Byte:
          {
              quint8 engineeringValueTemp= (quint8)engineeringValue;
              if(engineeringValueTemp>99)
              {
                 return ret;
              }
              quint8 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 1; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::UInt16:
      case MDDataType::UInt32:
      case MDDataType::UInt64:
          {
              quint16 engineeringValueTemp= (quint16)engineeringValue;
              if(engineeringValueTemp>9999)
              {
                 return ret;
              }
              quint16 BCDV =0;
              quint8 digit =0;
              for (int i = 0; i < 4; i++)
              {
                  digit = engineeringValueTemp % 10;
                  engineeringValueTemp /= 10;
                  BCDV |= digit << (i*4);
              }
              originalValue= QVariant(quint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::SByte:
          {
              qint8 engineeringValueTemp= (qint8)engineeringValue;
              qint8 engineeringValueTempAbs=qAbs(engineeringValueTemp);
              if(engineeringValueTempAbs>9)
              {
                  return ret;
              }
              qint8 BCDV =0;
              qint8 digit =engineeringValueTempAbs;
              if(engineeringValueTemp<0)
              {
                  BCDV |= 0x0F << 4;
                  BCDV |= digit;
              }
              else
              {
                  BCDV = digit;
              }
              originalValue= QVariant(qint16(BCDV));
              ret = true;
          }
          break;
      case MDDataType::Int16:
      case MDDataType::Int32:
      case MDDataType::Int64:
          {
            qint16 engineeringValueTemp= (qint16)engineeringValue;
            qint16 engineeringValueTempAbs=qAbs(engineeringValueTemp);
            if(engineeringValueTempAbs>999)
            {
                return ret;
            }
            qint16 BCDV =0;
            qint8 digit =0;
            for (int i = 0; i < 3; i++)
            {
                digit = engineeringValueTempAbs % 10;
                engineeringValueTempAbs /= 10;
                BCDV |= digit << (i*4);
            }
            if(engineeringValueTemp<0)
            {
                BCDV |= 0x0F << 12;
            }
            originalValue= QVariant(qint16(BCDV));
            ret = true;
          }
          break;
    case MDDataType::Bool:
    case MDDataType::DateTime:
    case MDDataType::String:
    case MDDataType::Float:
    case MDDataType::Double:
    default:
        break;
    }
    return ret;
}

bool CMDVariableDataConverter::decimalToBinary8(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const MDDataType &originalDataType,
                                 QVariant &originalValue,
                                 double &engineeringValue)
{
    if(qAbs(engineeringValue) > 255)
        return false;
    originalValue = QVariant(engineeringValue);
    return true;
}

bool CMDVariableDataConverter::decimalToBinary12(const MDVariableDataConversionParameter& dataConversionParameter,
                                  const MDDataType &originalDataType,
                                  QVariant &originalValue,
                                  double &engineeringValue)
{
    if(qAbs(engineeringValue) > 0xFFF)
        return false;
    originalValue = QVariant(engineeringValue);
    return true;
}

bool CMDVariableDataConverter::decimalToBinary13(const MDVariableDataConversionParameter& dataConversionParameter,
                                  const MDDataType &originalDataType,
                                  QVariant &originalValue,
                                  double &engineeringValue)
{
    if(qAbs(engineeringValue) > 0x1FFF)
        return false;
    originalValue = QVariant(engineeringValue);
    return true;
}

bool CMDVariableDataConverter::decimalToBinary15(const MDVariableDataConversionParameter& dataConversionParameter,
                                  const MDDataType &originalDataType,
                                  QVariant &originalValue,
                                  double &engineeringValue)
{
    if(qAbs(engineeringValue) > 0xEFFF)
        return false;
    originalValue = QVariant(engineeringValue);
    return true;
}

bool CMDVariableDataConverter::convertDbToVariant(double dbValue, const MDDataType &variantDataType,QVariant &variantValue)
{
    bool ret = false;
    switch (variantDataType)
    {
		case MDDataType::Bool:
			{
				variantValue = QVariant((bool)dbValue);
				ret = true;
			}
			break;
        case MDDataType::Byte:
            {
                variantValue= QVariant((quint8)dbValue);
                ret = true;
            }
            break;
        case MDDataType::UInt16:
            {
                variantValue= QVariant((quint16)dbValue);
                ret = true;
            }
            break;
        case MDDataType::UInt32:
            {
                variantValue= QVariant((quint32)dbValue);
                ret = true;
            }
            break;
        case MDDataType::UInt64:
            {
                variantValue= QVariant((quint64)dbValue);
                ret = true;
            }
            break;
        case MDDataType::SByte:        
            {
                variantValue= QVariant((qint8)dbValue);
                ret = true;
            }
            break;
        case MDDataType::Int16:
            {
                variantValue= QVariant((qint16)dbValue);
                ret = true;
            }
            break;
        case MDDataType::Int32:
            {
                variantValue= QVariant((qint32)dbValue);
                ret = true;
            }
             break;
        case MDDataType::Int64:
            {
                variantValue= QVariant((qint64)dbValue);
                ret = true;
            }
             break;
        case MDDataType::Float:
            {
                variantValue= QVariant((float)dbValue);
                ret = true;
            }
             break;
        case MDDataType::Double:
            {
                variantValue= QVariant(dbValue);
                ret = true;
            }
             break;
        default:
            break;
    }
    return ret;
}
bool CMDVariableDataConverter::getY(const MDVariableDataConversionParameter& dataConversionParameter, double x, const MDDataType & dataType, QVariant &variantValue)
{
    bool ret= false;
    double dbValue;
    if(dataConversionParameter.m_enableLinearConversion)
    {
        if(dataConversionParameter.m_maxValue != dataConversionParameter.m_minValue &&
                                dataConversionParameter.m_originalMaxValue != dataConversionParameter.m_originalMinValue)
        {
            dbValue = (x-dataConversionParameter.m_originalMinValue)*
                    ((dataConversionParameter.m_maxValue - dataConversionParameter.m_minValue)/(dataConversionParameter.m_originalMaxValue - dataConversionParameter.m_originalMinValue)) + dataConversionParameter.m_minValue;
        }
        else
        {
            dbValue = x;
        }
    }
    else
    {
        dbValue = x;
    }
    ret = convertDbToVariant(dbValue,dataType,variantValue);
    return ret;
}
//如果有其它类型转换，getX获取的中间值，double类型可以
bool CMDVariableDataConverter::getX(const MDVariableDataConversionParameter& dataConversionParameter,double y, double& doubleValue)
{
    bool ret= true;

    if(dataConversionParameter.m_enableLinearConversion)
    {
        if(dataConversionParameter.m_maxValue != dataConversionParameter.m_minValue &&
                        dataConversionParameter.m_originalMaxValue != dataConversionParameter.m_originalMinValue)
        {
            doubleValue = (y-dataConversionParameter.m_minValue)*
                    ((dataConversionParameter.m_originalMaxValue - dataConversionParameter.m_originalMinValue)/(dataConversionParameter.m_maxValue - dataConversionParameter.m_minValue)) + dataConversionParameter.m_originalMinValue;
        }
        else
            doubleValue = y;
    }
    else
    {
        doubleValue = y;
    }
    //ret = convertDbToVariant(dbValue,dataType,variantValue);
    return ret;
}
