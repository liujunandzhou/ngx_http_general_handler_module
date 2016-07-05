/*
 * =====================================================================================
 *
 *       Filename:  general_handler.cc
 *
 *    Description:  implement the general_handler
 *
 *        Version:  1.0
 *        Created:  2015年06月30日 11时16分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#include"general_handler.h"
#include<iostream>

bool GeneralHandler::InitHandler(std::string &args)
{
	return true;
}

InvokeResult  GeneralHandler::HandleRequest(std::map<std::string,std::string> &params)
{
	InvokeResult ir;
	ir.res_type="text/html";
	ir.response="GeneralHandler:";

	std::map<std::string,std::string>::const_iterator cit;
	std::string resp;
	for(cit=params.begin();cit!=params.end();++cit){
		resp=resp+cit->first+":"+cit->second;
	}	

	ir.response+=resp;

	return ir;
}

EXPORT_MODULE(GeneralHandler)
