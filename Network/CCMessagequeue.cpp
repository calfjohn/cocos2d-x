//
//  CCMessagequeue.cpp
//  cocos2dx
//
//  Created by Cocos2d on 12-7-6.
//  Copyright (c) 2012年 厦门雅基软件有限公司. All rights reserved.
//

#include "CCMessagequeue.h"
#include "CCNetwork.h"

NS_CC_EXT_BEGIN


static pthread_mutex_t      s_asyncRequestMutex;
static pthread_mutex_t      s_asyncResponseMutex;
static pthread_t s_requestThread;
static pthread_t s_responseThread;
static unsigned int s_requestCount = 1;

bool CCMessagequeue::m_bQuit = false;
CCMessagequeue* CCMessagequeue::m_pInstance = NULL;
CCMessagequeue::CCSingletonRelease CCMessagequeue::Garbo;

CCMessagequeue::CCMessagequeue()
{
    m_requestQueue.clear();
    m_responseQueue.clear();
}

CCMessagequeue::~CCMessagequeue()
{
    m_bQuit = true;

    pthread_mutex_destroy(&s_asyncRequestMutex);
    pthread_mutex_destroy(&s_asyncResponseMutex);
    pthread_kill(s_requestThread, 0);
    pthread_kill(s_responseThread, 0);


    std::vector<RequestInfo *>::iterator iterTemp;
    for (iterTemp = m_requestQueue.begin(); iterTemp != m_requestQueue.end(); iterTemp++) 
    {
        CC_SAFE_DELETE(*iterTemp);
    }
   
    for (iterTemp = m_responseQueue.begin(); iterTemp != m_responseQueue.end(); iterTemp++) 
    {
        CC_SAFE_DELETE(*iterTemp);
    }        
}

bool CCMessagequeue::init(void)
{
    pthread_mutex_init(&s_asyncRequestMutex, NULL);
    pthread_mutex_init(&s_asyncResponseMutex, NULL);
    pthread_create(&s_requestThread, NULL, requestThread, NULL);
    pthread_create(&s_responseThread, NULL, responseThread, NULL);
    
    return true;
}

void CCMessagequeue::pushRequest(std::string strUrl, std::string requestData, CCObject *rec, SEL_CallFuncND selector)
{
    // push request on request queue
    RequestInfo *tempInfo = new RequestInfo();
    tempInfo->requestId = s_requestCount++;
    tempInfo->strUrl = strUrl;
    tempInfo->strRequestData = requestData;
    tempInfo->strResponseData.clear();
    tempInfo->pListener = rec;
    tempInfo->pfnSelector = selector;
    tempInfo->stateCode = CURL_LAST;
    CCTime::gettimeofdayCocos2d(&tempInfo->sendTime, NULL);
    CCLog("request %d send", tempInfo->requestId);//, tempInfo->sendTime.tv_usec*1000+tempInfo->sendTime.tv_usec/1000);
    
    pthread_mutex_lock(&s_asyncRequestMutex);
    m_requestQueue.insert(m_requestQueue.begin(), tempInfo);
//    m_requestQueue.push_back(tempInfo);
    pthread_mutex_unlock(&s_asyncRequestMutex);
}

RequestInfo* CCMessagequeue::popUpRequest()
{
    // pop up request from request queue
    RequestInfo *tempInfo = NULL;
    
    pthread_mutex_lock(&s_asyncRequestMutex);
    if (!m_requestQueue.empty())
    {
        tempInfo = m_requestQueue.back();
        m_requestQueue.pop_back();
    }
    pthread_mutex_unlock(&s_asyncRequestMutex);
    
    // push request on response queue    
    if (tempInfo) 
    {
        pushResponse(tempInfo);
    }
    
    return tempInfo;
}

void CCMessagequeue::pushResponse(RequestInfo *requestInfo)
{    
    pthread_mutex_lock(&s_asyncResponseMutex);
    m_responseQueue.insert(m_responseQueue.begin(), requestInfo);
//    m_responseQueue.push_back(requestInfo);
    pthread_mutex_unlock(&s_asyncResponseMutex);
}

RequestInfo* CCMessagequeue::popUpResponse()
{
    RequestInfo *tempInfo = NULL;
    
    pthread_mutex_lock(&s_asyncResponseMutex);
    if (!m_responseQueue.empty())
    {
        tempInfo = m_responseQueue.back();
        if (tempInfo->stateCode == CURLE_OK) 
        {
            m_responseQueue.pop_back();
        }
        else 
        {
            tempInfo = NULL;
        }
    }
    pthread_mutex_unlock(&s_asyncResponseMutex);
    
    return tempInfo;
}

void* CCMessagequeue::requestThread(void* data)
{
    // create autorelease pool for iOS
    CCThread thread;
    thread.createAutoreleasePool();
    
    while (!m_bQuit)
    {
        RequestInfo *pRequestInfo = CCMessagequeue::sharedMessagequeue()->popUpRequest();
        if (!pRequestInfo)
        {
            sleep(0.1);
            continue;
        }
        
        pRequestInfo->stateCode = CCNetwork::sharedNetwork()->Get(pRequestInfo->strUrl, pRequestInfo->strResponseData);
    }
    
    return 0;
}

void* CCMessagequeue::responseThread(void* data)
{
    // create autorelease pool for iOS
    CCThread thread;
    thread.createAutoreleasePool();
    
    while (!m_bQuit)
    {
        RequestInfo *pRequestInfo = CCMessagequeue::sharedMessagequeue()->popUpResponse();
        if (!pRequestInfo)
        {
            sleep(0.1);
            continue;
        }

        CCTime::gettimeofdayCocos2d(&pRequestInfo->responseTime, NULL);
		((pRequestInfo->pListener)->*(pRequestInfo->pfnSelector))(NULL, pRequestInfo);
        CCLog("request %d back at %.2f(send:%d response:%d)", pRequestInfo->requestId, CCTime::timersubCocos2d(&pRequestInfo->sendTime, &pRequestInfo->responseTime), pRequestInfo->sendTime.tv_sec*1000+pRequestInfo->sendTime.tv_usec/1000, pRequestInfo->responseTime.tv_sec*1000+pRequestInfo->responseTime.tv_usec/1000);        
        CC_SAFE_DELETE(pRequestInfo);
    }

    return 0;
}

NS_CC_EXT_END