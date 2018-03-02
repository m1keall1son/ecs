#pragma once

#include "ecs/events/EventData.h"
#include "ecs/utils/Handle.h"

EventType getNextEventType();

//////////////////////////////////////////////////////////////////////////////////////////

using PresenceDetectionEventRef = std::shared_ptr<class PresenceDetectionEvent>;
class PresenceDetectionEvent : public EventData {

public:

	static EventType TYPE;

	static PresenceDetectionEventRef create( bool is_person_present );

	virtual const char* getName() const override { return "PresenceDetectionEvent"; }
	virtual EventType getEventType() const override { return TYPE; }

	bool isPersonPresent() const { return mIsPresent; }

private:

	PresenceDetectionEvent(bool present);

	bool mIsPresent;

};

//////////////////////////////////////////////////////////////////////////////////////////

using SubCategoryClickEventRef = std::shared_ptr<class SubCategoryClickEvent>;
class SubCategoryClickEvent : public EventData {

public:

	static EventType TYPE;

	static SubCategoryClickEventRef create( const Handle& subcategory_handle );

	virtual const char* getName() const override { return "SubCategoryClickEvent"; }
	virtual EventType getEventType() const override { return TYPE; }

	Handle getSubCategory() const { return mSubcategory; }

private:

	SubCategoryClickEvent(const Handle& subcategory_handle);

	Handle mSubcategory;

};

//////////////////////////////////////////////////////////////////////////////////////////

using ProductClickEventRef = std::shared_ptr<class ProductClickEvent>;
class ProductClickEvent : public EventData {

public:

	static EventType TYPE;

	static ProductClickEventRef create( const Handle& product_handle );

	virtual const char* getName() const { return "ProductClickEvent"; }
	virtual EventType getEventType() const { return TYPE; }

	Handle getProductHandle() const { return mProduct; }

private:

	ProductClickEvent(const Handle& product_handle);

	Handle mProduct;

};

//////////////////////////////////////////////////////////////////////////////////////////

using ClickOutEventRef = std::shared_ptr<class ClickOutEvent>;
class ClickOutEvent : public EventData {

public:

	static EventType TYPE;

	static ClickOutEventRef create();

	virtual const char* getName() const { return "ClickOutEvent"; }
	virtual EventType getEventType() const { return TYPE; }

private:

	ClickOutEvent();

};

//////////////////////////////////////////////////////////////////////////////////////////

using ResetExperieceEventRef = std::shared_ptr<class ResetExperieceEvent>;
class ResetExperieceEvent : public EventData {

public:

	enum class Reason { USER_TIMED_OUT, LOST_NETWORK_CONNECTION };

	static EventType TYPE;

	static ResetExperieceEventRef create( Reason reason );

	virtual const char* getName() const { return "ResetExperieceEvent"; }
	virtual EventType getEventType() const { return TYPE; }

	const Reason getReason() const { return mReason; }

private:

	ResetExperieceEvent(Reason reason);

	Reason mReason;

};