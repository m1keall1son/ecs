#include "IWEvents.h"

#include "cinder/app/App.h"

static uint64_t sEventType = 0;

EventType getNextEventType()
{
	return (EventType)sEventType++;
}

//////////////////////////////////////////////////////////////////////////////////////////

EventType PresenceDetectionEvent::TYPE = getNextEventType();

PresenceDetectionEventRef PresenceDetectionEvent::create(bool is_person_present)
{
	return PresenceDetectionEventRef(new PresenceDetectionEvent(is_person_present));
}
PresenceDetectionEvent::PresenceDetectionEvent(bool is_person_present) : EventData(ci::app::getElapsedSeconds()), mIsPresent(is_person_present)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

EventType SubCategoryClickEvent::TYPE = getNextEventType();

SubCategoryClickEventRef SubCategoryClickEvent::create(const Handle & subcategory_handle)
{
	return SubCategoryClickEventRef(new SubCategoryClickEvent(subcategory_handle));
}

SubCategoryClickEvent::SubCategoryClickEvent(const Handle & subcategory_handle) : EventData( ci::app::getElapsedSeconds() ), mSubcategory(subcategory_handle) {}

//////////////////////////////////////////////////////////////////////////////////////////

EventType ProductClickEvent::TYPE = getNextEventType();

ProductClickEventRef ProductClickEvent::create(const Handle & product_handle)
{
	return ProductClickEventRef( new ProductClickEvent( product_handle ) );
}

ProductClickEvent::ProductClickEvent(const Handle & product_handle) : EventData(ci::app::getElapsedSeconds()), mProduct(product_handle) {}

//////////////////////////////////////////////////////////////////////////////////////////

EventType ClickOutEvent::TYPE = getNextEventType();

ClickOutEventRef ClickOutEvent::create()
{
	return ClickOutEventRef( new ClickOutEvent );
}

ClickOutEvent::ClickOutEvent() : EventData(ci::app::getElapsedSeconds()) {}

//////////////////////////////////////////////////////////////////////////////////////////

EventType ResetExperieceEvent::TYPE = getNextEventType();

ResetExperieceEventRef ResetExperieceEvent::create(Reason reason)
{
	return ResetExperieceEventRef(new ResetExperieceEvent(reason) );
}

ResetExperieceEvent::ResetExperieceEvent(Reason reason) : EventData(ci::app::getElapsedSeconds()), mReason(reason)
{
}

