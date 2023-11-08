#ifndef QUAFOLDEROBJECT_H
#define QUAFOLDEROBJECT_H

#include "../../include/MDUaServer/MDUaServer_Global.h"
#include <QUaBaseObject>

class MDUASERVER_EXPORT QUaFolderObject : public QUaBaseObject
{
    Q_OBJECT

friend class QUaServer;

public:
	Q_INVOKABLE explicit QUaFolderObject(QUaServer *server);

	

};

#endif // QUAFOLDEROBJECT_H
