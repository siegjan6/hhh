#include "RecipeGridStyle.h"
#include <QStyleOption>
#include <QPainter>

CRecipeGridStyle::CRecipeGridStyle(QStyle *style) :
    QProxyStyle(style)
{
}

void CRecipeGridStyle::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (element)
    {
        /*
        在unbuntu环境下,高亮显示单元格无效.因为其默认style是QGtkStyle类,而此类并没有对tableview做过多处理.
        故此处使用其父类的处理接口.
        */
        case (PE_PanelItemViewItem):
        {
            QCommonStyle::drawPrimitive(element,option,painter,widget);
            break;
        }
        default:
            QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}
