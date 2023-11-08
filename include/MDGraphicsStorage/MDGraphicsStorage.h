/*!
\author aGRw 2015.07.27
\section 图库构造类，导出类
*/
#ifndef MDGRAPHICSSTORAGE_H
#define MDGRAPHICSSTORAGE_H

#include <QtCore/qglobal.h>

#if defined(MDGRAPHICSSTORAGE_LIBRARY)
#  define MDGRAPHICSSTORAGESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDGRAPHICSSTORAGESHARED_EXPORT Q_DECL_IMPORT
#endif

#include "IGraphicsStorage.h"

class MDGRAPHICSSTORAGESHARED_EXPORT CMDGraphicsStorage
{
public:
    IGraphicsStorage* create(const QString& imagePath, QWidget *parent);
};

#endif // MDGRAPHICSSTORAGE_H
