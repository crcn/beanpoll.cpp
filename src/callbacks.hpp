#ifndef CALLBACKS_H_
#define CALLBACKS_H_     

// #include "request.hpp"                
namespace Beanpoll
{
	class PushRequest;
	class PullRequest;  
	class RequestStream;
};


typedef void (PushCallback)(Beanpoll::PushRequest*);  
typedef void (PullCallback)(Beanpoll::PullRequest*);
typedef void (StreamCallback)(Beanpoll::RequestStream*);

#endif