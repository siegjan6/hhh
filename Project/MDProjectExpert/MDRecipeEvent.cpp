#include "MDRecipeEvent.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

MDRecipeEvent::MDRecipeEvent(QUaServer *server)
        : MDEvent(server)
{

}

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS
