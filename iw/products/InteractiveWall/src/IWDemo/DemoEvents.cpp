#include "DemoEvents.h"
#include "cinder/app/App.h"

EventType EngageClickEvent::TYPE = 7000;

EngageClickEvent::EngageClickEvent(int pdp_sub_state, const Handle& engage_product) : EventData( ci::app::getElapsedSeconds() ), mSubState(pdp_sub_state), mEngageProduct(engage_product)
{
}

EventType CollectionEvent::TYPE = 7001;

CollectionEvent::CollectionEvent()
{
}
