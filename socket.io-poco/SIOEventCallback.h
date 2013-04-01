#pragma once

#include "SIOEventRegistry.h"
#include "SIOClient.h"

#include "Poco/JSON/Parser.h"

using Poco::JSON::Object;

/**
 * This class provides an interface to call event functions
 */
class SIOEventCallbackBase
{
public:
    virtual ~SIOEventCallbackBase()
    {}

    virtual void fire( SIOClient *client, Object::Ptr data ) = 0;
};

/**
 * This template class adapts to the destination object and provides
 * a method to call the registered function
 */
template <class T> class SIOEventCallback : public SIOEventCallbackBase
{
public:

    /**
     * Register a callback function to be called from collide()
     * @param ftp pointer to the method to call
     */
    SIOEventCallback(void(T::*fpt)( Object::Ptr data ))
    {
        mFpt=fpt;
    };

    /**
     * Calls the registered collision function on receiver,
     * using otherObject and c as parameter
     * @param receiver object to call the callback on
     * @param otherObject parameter to pass to the callback function
     * @param c collision parameter to pass to the callback object
     */
    virtual void fire(SIOClient *client, Object::Ptr data)
    {
        T* r = dynamic_cast<T*>(client);
        (*r.*mFpt)(data);
    };

private:
   void(T::*mFpt)(Object::Ptr  data);
};

/**
 * Macro to register the A:onB()
 * @param A class to call the method on
 * @param B class which A collides with
 */
#define ON_EVENT(A,B) static bool sioOn_##A##_##B##_registered = SIOEventRegistry::sharedInstance()->registerEvent(#A"::on"#B,new SIOEventCallback<A>(&A::on##B));
