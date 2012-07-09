//
//  CCNetwork.cpp
//  cocos2dx
//
//  Created by Calf on 12-6-29.
//  Copyright (c) 2012年 厦门雅基软件有限公司. All rights reserved.
//

#include "CCNetwork.h"

NS_CC_EXT_BEGIN //or NS_CC_NETWORK_BEGIN ?

#define CC_NETWORK_TIMEOUT 10

CCNetwork* CCNetwork::m_pInstance = NULL;
CCNetwork::CCSingletonRelease CCNetwork::Garbo;

CCNetwork::CCNetwork(void) : 
m_bDebug(false)
{
    curl_global_init(CURL_GLOBAL_NOTHING);
    
    /* init easy handle */
    m_curl =curl_easy_init();
}

CCNetwork::~CCNetwork(void)
{
	/* cleanup easy handle */
	curl_easy_cleanup(m_curl);
    
    curl_global_cleanup();
}

bool CCNetwork::init()
{
    if (m_curl == NULL) 
    {
        return false;
    }
    /* reset all curl option */
    curl_easy_setopt(m_curl, CURLOPT_HTTPGET, 1L); 

	/* init curl option */    
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, CC_NETWORK_TIMEOUT);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, CC_NETWORK_TIMEOUT);
    curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(m_curl, CURLOPT_DEBUGFUNCTION, &CCNetwork::OnDebug);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &CCNetwork::WriteCallback);   
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, m_bDebug ? 1: 0);

    return true;
}

int CCNetwork::OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if(itype == CURLINFO_TEXT)
	{
		//printf("[TEXT]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_IN)
	{
		printf("[HEADER_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_OUT)
	{
		printf("[HEADER_OUT]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_IN)
	{
		printf("[DATA_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_OUT)
	{
		printf("[DATA_OUT]%s\n", pData);
	}
	return 0;
}

void CCNetwork::setDebug(bool bDebug)
{
	m_bDebug = bDebug;
}

bool CCNetwork::getDebug()
{
	return m_bDebug;
}

size_t CCNetwork::WriteCallback(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		return -1;
	}
    
    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
	return nmemb;
}

CURLcode CCNetwork::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
    init();
    
	CURLcode res;

	curl_easy_setopt(m_curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_POST, 1);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    res = curl_easy_perform(m_curl);

	return res;
}

CURLcode CCNetwork::Get(const std::string & strUrl, std::string & strResponse)
{
    init();
    
	CURLcode res;

    curl_easy_setopt(m_curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_POST, 0);    
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	res = curl_easy_perform(m_curl);

	return res;
}
/*
CURLcode CCNetwork::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath)
{
    init();
    
	CURLcode res;

    curl_easy_setopt(m_curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_POST, 1);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		//缺省情况就是PEM，所以无需设置，另外支持DER
		//curl_easy_setopt(m_curl,CURLOPT_SSLCERTTYPE,"PEM");
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(m_curl, CURLOPT_CAINFO, pCaPath);
	}

	res = curl_easy_perform(m_curl);

	return res;
}

CURLcode CCNetwork::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)
{
    init();
    
	CURLcode res;

	curl_easy_setopt(m_curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(m_curl, CURLOPT_CAINFO, pCaPath);
	}
	res = curl_easy_perform(m_curl);
    
	return res;
}
*/

NS_CC_EXT_END
