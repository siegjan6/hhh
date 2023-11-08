#include "MDGraphicsStorage.h"
#include "GraphicsDialog.h"
//////////////////////////////////////////////////////////////////////////
IGraphicsStorage *CMDGraphicsStorage::create(const QString &imagePath, QWidget* parent)
{
    return new CGraphicsDialog(imagePath, parent);
}
