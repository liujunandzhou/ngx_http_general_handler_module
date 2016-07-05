/*
 * =====================================================================================
 *
 *       Filename:  module_loader.cpp
 *
 *    Description:  implement the module_loader
 *
 *        Version:  1.0
 *        Created:  2015年06月30日 12时10分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */
#include <sys/types.h>
#include <string.h>
#include <dlfcn.h>
#include <iostream>

#include"handler.h"
#include"module_loader.h"

const char *kCreateModuleFnName="CreateModuleInstance";
const char *kDestroyModuleFnName="DestroyModuleInstance";


ModuleLoader::~ModuleLoader()
{
	handler.reset();
	if(dh_!=NULL){
		dlclose(dh_);
		create_module_=NULL;
		destroy_module_=NULL;
	}
}

bool ModuleLoader::InitModule()
{
	handler=Load(module_path);	
	if(handler){
		if(handler->InitHandler(module_args)==false)
			return false;

		return true;
	}
	
	return false;
}

SHARED_PTR<Handler>& ModuleLoader::GetHandler()
{
	return handler;
}

SHARED_PTR<Handler> ModuleLoader::Load(const std::string& path)
{
	Handler* module = NULL;
	void * dlhandle = NULL;
	CreateModuleFn create_module = NULL;
	DestroyModuleFn destroy_module = NULL;

	dlhandle = dlopen(path.c_str(), RTLD_LAZY);
	if (!dlhandle) {
		std::cerr<<"dlopen failed"<<std::endl;
		goto failed;
	}   

	create_module = (CreateModuleFn)dlsym(dlhandle, kCreateModuleFnName);
	destroy_module = (DestroyModuleFn)dlsym(dlhandle, kDestroyModuleFnName);
	if (!create_module || !destroy_module) {
		
		std::cerr<<"dlsym failed"<<std::endl;
		goto failed;
	}   

	module = create_module();
	if (!module) {
		std::cerr<< "Initialize module failed!"<<std::endl;
		goto failed;
	}   

	dh_ = dlhandle;
	create_module_ = create_module;
	destroy_module_ = destroy_module;
	return SHARED_PTR<Handler>(module, destroy_module);

failed:
	if (dlhandle) {
		dlclose(dlhandle);
		dlhandle=NULL;
	}
	return SHARED_PTR<Handler>();
}
