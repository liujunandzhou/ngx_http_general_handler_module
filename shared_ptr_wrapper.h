/*
 * =====================================================================================
 *
 *       Filename:  shared_ptr.h
 *
 *    Description:  warpper the shared_ptr interface
 *
 *        Version:  1.0
 *        Created:  2016年07月05日 12时03分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */


#ifndef __SHARED_PTR_WRAPPER_H__
#define __SHARED_PTR_WRAPPER_H__

extern "C" {

#include<ngx_config.h>
#include<ngx_core.h>
#include<ngx_http.h>

}

#ifdef HAVE_SHARED_PTR
#include <tr1/memory>
#include <tr1/shared_ptr.h>

using std::tr1::shared_ptr;

#define  SHARED_PTR std::tr1::shared_ptr

#else
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

#define  SHARED_PTR boost::shared_ptr 

#endif

#endif
