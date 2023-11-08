#ifndef DRAWDEFINE
#define DRAWDEFINE
/*!
\section 绘图组件类型的枚举
*/
enum drawType
{dtCap, dtNeedle, dtRange, dtScaleText, dtScale, dtScaleBack, dtBack, dtHalfBack, dtLabel, dtLed, dtLayer,
dtRNeedle, dtRRange, dtRScaleText, dtRScale, dtRScaleBack, dtRMarker, dtTemperature};

/*!
\section 线的类型的枚举
*/
enum  lineStyle
{lsSolid, lsDash, lsDot, lsDashDot, lsDashDotDot, lsNull};

/*!
\section 渐变填充类型的枚举
*/
enum gradientStyle
{gsSolid, gs1, gs2, gs3, gs4, gs5, gs6, gs7, gs8, gs9, gsNull};
/*!
\section CPaintButton的绘图类型
*/
enum paintType
{ /*ptPaint,*/ ptImage, ptColor};
/*!
\section 绘图组件风格类型的枚举
*/
enum styleType
{ stCap, stNeedle, stRange, stScaleText, stScaleStyle, stScaleArrangement, stScaleBack, stBack, stHalfStyle, stHalfArrangement,
stLine, stGradient, stRMarkerStyle, stRMarkerArrangement, stRNeedle, stRRangeStyle, stRRangeArrangement, stRScaleText,
stRScaleStyle, stRScaleArrangement, stRScaleBack, stTemperature, stTemHelp};

#endif // DRAWDEFINE

