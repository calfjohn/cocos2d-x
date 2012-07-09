//
//  CCMessagequeue.h
//  cocos2dx
//
//  Created by Cocos2d on 12-7-6.
//  Copyright (c) 2012年 厦门雅基软件有限公司. All rights reserved.
//

#ifndef cocos2dx_CCMessagequeue_h
#define cocos2dx_CCMessagequeue_h

#include "cocos2d.h"
#include "curl/curl.h"

NS_CC_EXT_BEGIN //or NS_CC_NETWORK_BEGIN ?

typedef struct _RequestInfo
{
    unsigned int requestId;         //request id
    std::string  strUrl;            //request url
    std::string  strRequestData;    //request data
    std::string  strResponseData;   //response data
    CCObject*       pListener;      //callback listener
    SEL_CallFuncND  pfnSelector;    //callback selector
    CURLcode stateCode;             //response state
    cc_timeval sendTime;               //send time
    cc_timeval responseTime;           //response time
}RequestInfo;


class CCMessagequeue {
public:
	static CCMessagequeue* sharedMessagequeue(void)
	{
		if (!m_pInstance)  
		{
			m_pInstance = new CCMessagequeue();
			m_pInstance->init();
		}
		return m_pInstance; 
	}    
    
    //First Input First Output
    void pushRequest(std::string strUrl, std::string requestData, CCObject *rec, SEL_CallFuncND selector);
    
protected:
    CCMessagequeue();
    virtual ~CCMessagequeue();

	static CCMessagequeue *m_pInstance;
    
    class CCSingletonRelease //purpose of the class is only to delete instance of CCMessagequeue
	{  
	public:  
		~CCSingletonRelease()  
		{  
            CC_SAFE_DELETE(CCMessagequeue::m_pInstance); 
		}  
	};  
	static CCSingletonRelease Garbo; //destructor will be invoked before program exit  

    virtual bool init(void);
    
    RequestInfo *popUpRequest();
 
    void pushResponse(RequestInfo *requestInfo);
    
    RequestInfo *popUpResponse();
    
    static void* requestThread(void* data);

    static void* responseThread(void* data);
    
protected:
    std::vector<RequestInfo *> m_requestQueue;
    std::vector<RequestInfo *> m_responseQueue;
    
    static bool m_bQuit;
};

NS_CC_EXT_END


#endif
