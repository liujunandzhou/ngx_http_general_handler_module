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

//since gcc 4.3 support shared_ptr class
#if (__i386 || __amd64) && __GNUC__
    #define GNUC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
    #if GNUC_VERSION >= 40300
        #define HAVE_SHARED_PTR
    #endif	
#endif


#ifdef HAVE_SHARED_PTR
#include <tr1/shared_ptr.h>
#define  SHARED_PTR std::tr1::shared_ptr;
#else
#include <boost/shared_ptr.hpp>
#define  SHARED_PTR boost::shared_ptr 
#endif

#endif
