//
//  CCNetwork.h
//  cocos2dx
//
//  Created by Calf on 12-6-29.
//  Copyright (c) 2012年 厦门雅基软件有限公司. All rights reserved.
//


#ifndef COCOS2DX_CCNETWORK_H
#define COCOS2DX_CCNETWORK_H

#include "cocos2d.h"
#include "curl/curl.h"

/**
 @brief    The cocos2d Network Module.
           implement asyn/sync request base on curl
           thanks to huyiyang2010
           LINK: http://blog.csdn.net/huyiyang2010/article/details/7664201
 */

NS_CC_EXT_BEGIN //or NS_CC_NETWORK_BEGIN ?

class CCNetwork {
public:
    /** returns a shared instance of the network */
	static CCNetwork* sharedNetwork(void)
	{
		if (!m_pInstance)  
		{
			m_pInstance = new CCNetwork();
			m_pInstance->init();
		}
		return m_pInstance; 
	}

	/** HTTP POST REQUEST*/
	CURLcode Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);
    
	/** HTTP GET REQUEST*/
	CURLcode Get(const std::string & strUrl, std::string & strResponse);
    
	/** HTTPS POSTS QEQUEST*/
//	CURLcode Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);
    
	/** HTTPS GETS,无证书版本*/
//	CURLcode Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);
    
    CC_PROPERTY(bool, m_bDebug, Debug);
        
protected:    
    virtual bool init(void);

    static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *);
    
    static size_t WriteCallback( void *buff, size_t size, size_t nmemb, void *lpVoid);   
    
protected:
    CCNetwork(void);
    virtual ~CCNetwork(void);
	static CCNetwork *m_pInstance;
    
    class CCSingletonRelease //purpose of the class is only to delete instance of CCNetwork
	{  
	public:  
		~CCSingletonRelease()  
		{  
            CC_SAFE_DELETE(CCNetwork::m_pInstance); 
		}  
	};  
	static CCSingletonRelease Garbo; //destructor will be invoked before program exit  
    
    CURL * m_curl;
};

NS_CC_EXT_END
#endif
