#ifndef __EVENT_HANDLER_HPP__
#define __EVENT_HANDLER_HPP__

#include "Singleton.hpp"
#include <typeinfo>
class message_base_t {};
enum message_code{
    message_1,
    message_2,
    message_3,
    message_count,
};

template<int code>
class message_t : public message_base_t
{
    enum 
    {
        message = code
    };
};

class message_1_t : public message_t<message_1> {};
class message_2_t : public message_t<message_2> {};
class message_3_t : public message_t<message_3> {};



class EventHandler : private Singleton<EventHandler>
{
public:
    typedef void (*base_handler_t)(const message_base_t&);
    // typedef std::map<message_code, base_handler_t> handler_table_t;
    typedef std::array<base_handler_t, message_count> handler_table_t;

    static handler_table_t _handler_table_t;

    static void empty_handler(const message_base_t &) {}
    static void handler_init_t (int max_count)
    {
        for(int i = 0 ; i < max_count ; ++i)
        {
            _handler_table_t[i] = empty_handler;
        }
    }

    static void handler_register_t(int index, base_handler_t handler)
    {
        _handler_table_t[index] = handler;
    }
    static void handler_message_struct2(const message_2_t & msg)
    {
        
    }
};


//어딘가에서



// 
// class Event 
// { 
// protected: 
// 	virtual ~Event() {}; 
// }; 

// class HandlerFunctionBase 
// { 
// public: 
// 	virtual ~HandlerFunctionBase() {}; 
// 	void exec(const Event* event) {call(event);} 
	
// private: 
// 	virtual void call(const Event*) = 0; 
// }; 

// template<class T, class EventT >
// class MemberFunctionHandler : public HandlerFunctionBase
// {
// public:
//     typedef void (T::*MemberFunc)(EventT* );
//     MemberFunctionHandler(T * instance, MemberFunc memFn)
//         : _instance(instance)
//         , _function(memFn)
//     {

//     }

//     void call(const Event * event)
//     {
//         (_instance->*_function)(static_cast <EventT *>(event));
//     }
// private:
//     T* _instance;
//     MemberFunc _function;
// };

// class EventHandler
// {
// public:
//     void handleEvent(const Event *);
//     template < class T, class EventT > 
//     void registerEventFunc(T *, void (T::*memFn)(EventT*));
// private:
//     typedef std::map<std::type_info, HandlerFunctionBase *> Handler;
//     Handler _handler;

//     template<class T, class EventT>
//     void registerEventFunc(T * obj, void (T::*memFn)(EventT*))
//     {
//         // _handler[typeid(EventT)] = new MemberFunctionHandler<T,EventT> (obj, memFn);
//     }
// };

// class EventHandler 
// { 
// public: 
// 	void handleEvent(const Event*); 
// 	template < class T, class EventT > void registerEventFunc(T*, void (T::*memFn)(EventT*)); 
	
// private: 
// 	typedef std::map< TypeInfo, HandlerFunctionBase* > Handlers; 
//     Handlers _handlers; 
// 	template < class T, class EventT > 
//     void EventHandler::registerEventFunc(T* obj, void (T::*memFn)(EventT*)) 
// 	{ 
// 		_handlers[TypeInfo(typeid(EventT))]= new MemberFunctionHandler< T, EventT >(obj, memFn); 
// 	} 
	
// 	void EventHandler::handleEvent(const Event* event) 
// 	{ 
// 		Handlers::iterator it = _handlers.find(TypeInfo(typeid(*event))); 
// 		if(it != _handlers.end()) { it->second->exec(event); } 
// 	} 
// };
#endif