#pragma once

#include "ecs/events/EventData.h"
#include "ecs/utils/Handle.h"

using EngageClickEventRef = std::shared_ptr<class EngageClickEvent>;
class EngageClickEvent : public EventData {

public:

	static EventType TYPE;

	static EngageClickEventRef create(int pdp_sub_state, const Handle& engage_product ) { return EngageClickEventRef( new EngageClickEvent( pdp_sub_state, engage_product) ); }

	virtual const char* getName() const { return "EngageClickEvent"; }
	virtual EventType getEventType() const { return TYPE; }

	int getSubState() const { return mSubState; }
	Handle getProduct() { return mEngageProduct; }

private:

	EngageClickEvent(int pdp_sub_state, const Handle& engage_product );

	int mSubState;
	Handle mEngageProduct;
};

using CollectionEventRef = std::shared_ptr<class CollectionEvent>;
class CollectionEvent : public EventData {

public:

	static EventType TYPE;

	static CollectionEventRef create() { return CollectionEventRef(new CollectionEvent); }

	virtual const char* getName() const { return "CollectionEvent"; }
	virtual EventType getEventType() const { return TYPE; }

private:

	CollectionEvent();

};