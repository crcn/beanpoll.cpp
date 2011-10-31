        
                    
#include "request.h"
#include "router.h"  
#include "listener.h"     
#include "utils.h"
#include <iostream>                
#include <vector>   
#include <pthread.h>   
                  

namespace Beanpole
{
	void ConcreteDispatcher::dispatch(Data* data)
	{
		std::vector<RouteListener*>* listeners = this->_collection.getRouteListeners(data->channel);    

		ConcreteDispatcher::dispatch(data, listeners);
	}                                 


	void* dispatch_threaded_request(void* request)
	{                                                      
		((Request*)request)->next(); 
		delete (Request*)request;                        
		return NULL;
	}                     


	void ConcreteDispatcher::dispatch(Data* data, std::vector<RouteListener*>* listeners)
	{                         
                            
		for(int i = listeners->size(); i--;)
		{               
			void* val;

			//TODO: check if request is threaded.
			RouteListener* listener = (*listeners)[i];

			Request* request = this->request(data, listener);            
                                                                 

			//async call per listener. Typically one except push requests
			this->_threadPool.createTask((void*)request, &dispatch_threaded_request);
		}
	}        

	Request* ConcreteDispatcher::request(Data* data, RouteListener* listener)
	{                                     
		return new Request(data, listener, this);
	}

	void ConcreteDispatcher::addRouteListener(RouteListener* listener)
	{                    
		this->_collection.addRouteListener(listener);
	}          

	Data* Router::request(const char* channel)
	{          
		return new Data(Parser::parseChannel(channel), this);
	}


	void Router::on(std::string route, PullCallback* callback)
	{                                          
		this->on<PullCallback, PullRouteListener, PullDispatcher>(route, callback, this->_puller);
	}

	void Router::on(std::string route, PushCallback* callback)
	{                                                                   
		this->on<PushCallback, PushRouteListener, PushDispatcher>(route, callback, this->_pusher);                                 

	};            
	
	/**
	 */
	                  
    template<class T, class U, class V>
	void Router::on(std::string route, T* callback, V* dispatcher)
	{
		std::vector<RouteExpression*> expressions;

		Parser::parseRoute(route, expressions);


		for(int i = expressions.size(); i--;)
		{                                       
			dispatcher->addRouteListener(new U(expressions[i], callback));
		}
	}   

	void Router::push(Data* data)
	{
		this->_pusher->dispatch(data);  
	}   

	void Router::pull(Data* data)
	{
		this->_puller->dispatch(data);
	}
};

