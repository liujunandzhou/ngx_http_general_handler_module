/*
 * =====================================================================================
 *
 *       Filename:  handler.h
 *
 *    Description:  define the handler interface
 *
 *        Version:  1.0
 *        Created:  2015年06月30日 10时45分29秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */
#ifndef __QBUS_TRANSFER_HANDLER_H__
#define __QBUS_TRANSFER_HANDLER_H__

#include<string>
#include<map>

#define EXPORT_MODULE(m) \
	extern "C" void *CreateModuleInstance() { \
		    return new m(); \
	} \
	extern "C" void DestroyModuleInstance(void *ptr) { \
	    m *p = (m*)ptr; \
	    delete p; \
} // EXPORT_MODULE()

typedef struct{
	std::string res_type; //response_type
	std::string response; //response
}InvokeResult;


class Handler
{

public:
	virtual ~Handler(){}

	virtual bool InitHandler(std::string &args)=0;
	virtual InvokeResult  HandleRequest(std::map<std::string,std::string> &params)=0;
};

#endif
