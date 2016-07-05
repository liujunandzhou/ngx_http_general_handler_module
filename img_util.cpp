/*
 * =====================================================================================
 *
 *       Filename:  util.cc
 *
 *    Description:  implement the func in the util.h
 *
 *        Version:  1.0
 *        Created:  2015年01月13日 12时37分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */
#include"img_util.h"
#include<limits>
#include<iostream>

namespace img_util
{

void split_string(const string &text,vector<string> &parts,const string &delimiter,int count)
{
	assert(count==-1 || count>1);

	parts.clear();
	size_t delimiterPos=text.find(delimiter);
	size_t lastPos=0;
	if(delimiterPos == string::npos){
		parts.push_back(text);
		return;
	}

	if(count==-1)
		count=std::numeric_limits<int>::max();

	int split_cnt=0;

	while(delimiterPos != string::npos && split_cnt++ <count-1 ){

		parts.push_back(text.substr(lastPos, delimiterPos - lastPos));
		lastPos = delimiterPos + delimiter.size();
		delimiterPos = text.find(delimiter, lastPos);
	}
	parts.push_back(text.substr(lastPos));
}

void from_vec2map(const vector<string> &vec,map<string,string> &parts,const string &delimiter)
{
	int nsize=vec.size();
	for(int i=0;i<nsize;i++){

		vector<string> temp;

		split_string(vec[i],temp,delimiter,2);

		if(temp.size()<2)
			continue;

		parts[temp[0]]=temp[1];
	}
}

double get_cur_sec()
{
	struct timeval tv; 
	gettimeofday( &tv, NULL );
	return tv.tv_sec + (double)tv.tv_usec/1000000.0f;
}

}
