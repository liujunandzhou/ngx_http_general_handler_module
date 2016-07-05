/*
 * =====================================================================================
 *
 *       Filename:  map_util.h
 *
 *    Description:  implement map util
 *
 *        Version:  1.0
 *        Created:  2015年05月14日 16时35分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef __GJBMERGER_MAP_UTIL__
#define __GJBMERGER_MAP_UTIL__

#include<map>

namespace map_util
{
	class map_wrapper
	{
		public:
			template<typename K,typename V>
				static V get_value(std::map<K,V> &map,const K &key,const V &default_value)
				{
					typename std::map<K,V>::const_iterator cit;
					cit=map.find(key);
					if(cit!=map.end())
						return cit->second;

					return default_value;
				}

			template<typename K,typename V>
				static void set_value(std::map<K,V> &map,const K &key,const V &value)
				{
					map[key]=value;
				}
	};
}

#endif
