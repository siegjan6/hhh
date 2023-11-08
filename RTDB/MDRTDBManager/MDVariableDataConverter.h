/*!
author enli 2015.01.08
\section 变量工程值转换
*/
#ifndef MDVARIABLEDATACONVERTER_H
#define MDVARIABLEDATACONVERTER_H

#include "IMDRTDBVariable.h"
class CMDVariableDataConverter
{
public:
    CMDVariableDataConverter();
public:
    //! 由原始值获取工程值，分别调用下面4种类型转换
    static bool getEngineeringValue(const MDVariableDataConversionParameter& dataConversionParameter,
                                    const MDDataType &originalDataType,const MDDataType & dataType,const int & precision,
                                    const QVariant &originalValue,QVariant &engineeringValue );

    //! 由工程值获取原始值
    static bool getOriginalValue(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const MDDataType &originalDataType,const MDDataType & dataType,const int & precision,
                                 const QVariant &engineeringValue,QVariant &originalValue );



    //! 线性转换
    static bool forwardLinearConversion(const MDVariableDataConversionParameter& dataConversionParameter,
                                    const MDDataType &originalDataType,const MDDataType & dataType,const int & precision,
                                    const QVariant &originalValue,QVariant &engineeringValue );

    //! 3位BCD码转换
    static bool  BCD3ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                               const QVariant &originalValue,
                               const MDDataType &dataType,
                               double&engineeringValue);
    //! 4位BCD码转换
    static bool  BCD4ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                               const QVariant &originalValue,
                               const MDDataType &dataType,
                               double&engineeringValue);
    //! 8位BCD码转换
    static bool  BCD8ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                               const QVariant &originalValue,
                               const MDDataType &dataType,
                               double&engineeringValue);
    //! 欧姆龙4位BCD码转换（有符号BCD码最高1位二进制位为符号位）
    static bool  OmronBCD4ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                                    const QVariant &originalValue,
                                    const MDDataType &dataType,
                                    double &engineeringValue);
    //! 西门子4位BCD码转换（有符号BCD码最高4位二进制位为符号位）
    static bool  SiemensBCD4ToDecimal(  const MDVariableDataConversionParameter& dataConversionParameter,
                                        const QVariant &originalValue,
                                        const MDDataType &dataType,
                                        double &engineeringValue);
    //!8位二进制转换无报警
    static bool binary8ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const QVariant &originalValue,
                                 const MDDataType &dataType,
                                 double &engineeringValue);
    //!12位二进制转换无报警
    static bool binary12ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const QVariant &originalValue,
                                 const MDDataType &dataType,
                                 double &engineeringValue);

    //!13位二进制转换无报警
    static bool binary13ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const QVariant &originalValue,
                                 const MDDataType &dataType,
                                 double &engineeringValue);

    //!15位二进制转换无报警
    static bool binary15ToDecimal(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const QVariant &originalValue,
                                 const MDDataType &dataType,
                                 double &engineeringValue);
    //!字节顺序调换
    static bool  byteOrderChange(  const MDVariableDataConversionParameter& dataConversionParameter,
                                   const MDDataType &originalDataType,
                                   const QVariant &originalValue,
                                   const MDDataType &dataType,
                                   double &engineeringValue);


    //! 反向转换
    static bool reverseLinearConversion(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const MDDataType &originalDataType,const MDDataType & dataType,const int & precision,
                                 const QVariant &engineeringValue,QVariant &originalValue );    

    static bool  decimalToBCD3(const MDVariableDataConversionParameter& dataConversionParameter,
                               const MDDataType &originalDataType,
                               QVariant &originalValue,
                               double &engineeringValue);
    static bool  decimalToBCD4(const MDVariableDataConversionParameter& dataConversionParameter,
                               const MDDataType &originalDataType,
                               QVariant &originalValue,
                               double &engineeringValue);
    static bool  decimalToBCD8(const MDVariableDataConversionParameter& dataConversionParameter,
                               const MDDataType &originalDataType,
                               QVariant &originalValue,
                               double &engineeringValue);

    static bool  decimalToOmronBCD4(const MDVariableDataConversionParameter& dataConversionParameter,
                                    const MDDataType &originalDataType,
                                    QVariant &originalValue,
                                    double &engineeringValue);
    static bool  decimalToSiemensBCD4(const MDVariableDataConversionParameter& dataConversionParameter,
                                      const MDDataType &originalDataType,
                                      QVariant &originalValue,
                                      double &engineeringValue);

    static bool  ReverseByteOrderChange(const MDVariableDataConversionParameter& dataConversionParameter,
                                      const MDDataType &originalDataType,
                                      QVariant &originalValue,
                                      const MDDataType &dataType,
                                      double &engineeringValue);

    static bool decimalToBinary8(const MDVariableDataConversionParameter& dataConversionParameter,
                                 const MDDataType &originalDataType,
                                 QVariant &originalValue,
                                 double &engineeringValue);

    static bool decimalToBinary12(const MDVariableDataConversionParameter& dataConversionParameter,
                                  const MDDataType &originalDataType,
                                  QVariant &originalValue,
                                  double &engineeringValue);

    static bool decimalToBinary13(const MDVariableDataConversionParameter& dataConversionParameter,
                                  const MDDataType &originalDataType,
                                  QVariant &originalValue,
                                  double &engineeringValue);

    static bool decimalToBinary15(const MDVariableDataConversionParameter& dataConversionParameter,
                                  const MDDataType &originalDataType,
                                  QVariant &originalValue,
                                  double &engineeringValue);
    static bool   convertDbToVariant(double dbValue, const MDDataType &variantDataType,QVariant &variantValue);
    static bool   getY(const MDVariableDataConversionParameter& dataConversionParameter,double x, const MDDataType & dataType, QVariant &variantValue);
    static bool   getX(const MDVariableDataConversionParameter& dataConversionParameter,double y, double& doubleValue);
};

#endif // MDVARIABLEDATACONVERTER_H
