/*
 * =====================================================================================
 *
 *       Filename:  util.h
 *
 *    Description:  util function
 *
 *        Version:  1.0
 *        Created:  2015年01月13日 12时33分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef __IMG_SPIDER_UTIL_
#define __IMG_SPIDER_UTIL_

#include<string>
#include<vector>
#include<map>
#include<sys/time.h>
#include<sys/types.h>
#include<assert.h>

using namespace std;

namespace img_util
{

//count表示最多切分成几块
void split_string(const string &text,vector<string> &parts,const string &delimiter=" ",int count=-1);

void from_vec2map(const vector<string> &vec,map<string,string> &parts,const string &delimiter=":");

double get_cur_sec();

}

#endif
