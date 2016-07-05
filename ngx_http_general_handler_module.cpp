/*
 * =====================================================================================
 *
 *       Filename:  ngx_http_general_handler_module.cpp
 *
 *    Description:  define the module interface
 *
 *        Version:  1.0
 *        Created:  2015年06月06日 15时11分30秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (all rights reserverd), liujun-xy@360.cn
 *        Company:  qihoo corp.inc
 *
 * =====================================================================================
 */


extern "C" {

#ifndef DDEBUG
#define DDEBUG 1
#endif

#include"ddebug.h"

#include<ngx_config.h>
#include<ngx_core.h>
#include<ngx_http.h>


}

#include<string>
#include<map>
#include<handler.h>

#include "module_loader.h"
#include "map_util.h"
#include "img_util.h"


#define NOT_SUPPORTED_METHOD "not supported method"

//定义定义结构体
typedef struct{
	std::string mod_name;
	std::string mod_path;
	std::string mod_args;
}module_info;

//一个location下只能使用一个
typedef struct{
	bool handler_set;
	bool handler_inited;
	module_info mod_info;
	SHARED_PTR<ModuleLoader> module_handler;
}ngx_http_general_handler_loc_conf_t;

static char *ngx_http_general_handler(ngx_conf_t *cf,ngx_command_t *cmd,void *conf);

static void *ngx_http_general_handler_create_loc_conf(ngx_conf_t *cf);

static ngx_int_t  ngx_http_handle_call(ngx_http_request_t *r,std::map<std::string,std::string> &params);

static ngx_int_t ngx_http_handle_unkown_request(ngx_http_request_t *r);

static ngx_int_t  ngx_http_handle_post_request(ngx_http_request_t *r);

static ngx_int_t  ngx_http_handle_get_request(ngx_http_request_t *r);

static void ngx_http_general_handler_post_handler(ngx_http_request_t *r);

static ngx_int_t ngx_http_post_data(ngx_http_request_t *r,ngx_str_t *post);

//用来添加处理器模块
//指令使用方式:AddHandler module  configure
static ngx_command_t ngx_http_general_handler_commands[]={
	{ngx_string("AddHandler"),
	 NGX_HTTP_LOC_CONF|NGX_CONF_TAKE12,
	 ngx_http_general_handler,
	 NGX_HTTP_LOC_CONF_OFFSET,
	 0,
	 NULL},
	ngx_null_command
};

static ngx_http_module_t ngx_http_general_handler_module_ctx={
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	ngx_http_general_handler_create_loc_conf,
	NULL
};


ngx_module_t ngx_http_general_handler_module={
	NGX_MODULE_V1,
	&ngx_http_general_handler_module_ctx,
	ngx_http_general_handler_commands,
	NGX_HTTP_MODULE,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NGX_MODULE_V1_PADDING
};

//对应location下的处理模块
static ngx_int_t ngx_http_general_handler_handler(ngx_http_request_t *r)
{
	//进行request_handler 处理
	ngx_log_error(NGX_LOG_DEBUG_HTTP,r->connection->log,0,"%s","handle the request");

	if(r->method == NGX_HTTP_GET)
		return ngx_http_handle_get_request(r);
	else if(r->method == NGX_HTTP_POST)
		return ngx_http_handle_post_request(r);
	else
		return ngx_http_handle_unkown_request(r);

	return NGX_DECLINED;
}

//用于处理post请求,对于post请求比较特殊
static ngx_int_t  ngx_http_handle_post_request(ngx_http_request_t *r)
{
	ngx_log_error(NGX_LOG_DEBUG_HTTP,r->connection->log,0,"%s","handle post request");

	dd("handle post request");

	ngx_int_t rc;
	rc=ngx_http_read_client_request_body(r,ngx_http_general_handler_post_handler);

	if(rc >= NGX_HTTP_SPECIAL_RESPONSE)
		return rc;

	return NGX_DONE;
}

//对于未知请求处理
static ngx_int_t ngx_http_handle_unkown_request(ngx_http_request_t *r)
{
	ngx_buf_t *b;

	ngx_chain_t out;

	ngx_int_t rc;

	b=(ngx_buf_t*)ngx_create_temp_buf(r->pool,sizeof(NOT_SUPPORTED_METHOD));

	ngx_memcpy(b->pos,NOT_SUPPORTED_METHOD,sizeof(NOT_SUPPORTED_METHOD));

	b->last=b->pos+sizeof(NOT_SUPPORTED_METHOD);

	b->last_buf=1;

	out.buf=b;

	out.next=NULL;

	r->headers_out.status=NGX_HTTP_OK;
	r->headers_out.content_length_n=sizeof(NOT_SUPPORTED_METHOD);

	rc=ngx_http_send_header(r);

	if(rc==NGX_ERROR || rc>NGX_OK || r->header_only)
		return rc;

	//可以使用handler
	return ngx_http_output_filter(r,&out);
}

//用于处于get请求
static ngx_int_t  ngx_http_handle_get_request(ngx_http_request_t *r)
{
	ngx_log_error(NGX_LOG_DEBUG_HTTP,r->connection->log,0,"%s","handle get request");

	std::string str_args=std::string((char*)r->args.data,r->args.len);

	std::vector<std::string> parts;

	img_util::split_string(str_args,parts,"&");

	std::map<std::string,std::string> args_map;

	img_util::from_vec2map(parts,args_map,"=");

	return ngx_http_handle_call(r,args_map);
}

//根据解析出来的参数,调用具体的模块
static ngx_int_t  ngx_http_handle_call(ngx_http_request_t *r,std::map<std::string,std::string> &params)
{
	ngx_int_t rc;

	ngx_buf_t *b;

	ngx_chain_t *out;

	out=ngx_alloc_chain_link(r->pool);

	if(out==NULL)
		return NGX_HTTP_INTERNAL_SERVER_ERROR;

	ngx_http_general_handler_loc_conf_t *hlcf;

	hlcf=(ngx_http_general_handler_loc_conf_t*)ngx_http_get_module_loc_conf(r,ngx_http_general_handler_module);

	InvokeResult ir=hlcf->module_handler->GetHandler()->HandleRequest(params);

	//通过args构造出请求参数
	ngx_str_t tmp;

	r->headers_out.content_type.len=ir.res_type.length();

	tmp.len=ir.res_type.length();

	tmp.data=(u_char*)ir.res_type.c_str();

	r->headers_out.content_type.data=ngx_pstrdup(r->pool,&tmp);

	b=(ngx_buf_t*)ngx_pcalloc(r->pool,sizeof(ngx_buf_t));

	out->buf=b;
	out->next=NULL;

	tmp.len=ir.response.length();
	tmp.data=(u_char*)ir.response.c_str();

	b->pos=ngx_pstrdup(r->pool,&tmp);
	b->last=b->pos+tmp.len;
	b->memory=1;
	b->last_buf=1;

	r->headers_out.status=NGX_HTTP_OK;
	r->headers_out.content_length_n=tmp.len;

	rc=ngx_http_send_header(r);

	if(rc==NGX_ERROR || rc>NGX_OK || r->header_only)
		return rc;

	//可以使用handler
	return ngx_http_output_filter(r,out);
}

//定义用来删除内存的结构体
static void ngx_pool_delete_memory(void *data)
{   
		ngx_http_general_handler_loc_conf_t *conf=(ngx_http_general_handler_loc_conf_t *)data;
		if(conf){
			delete conf;
		}
}

static void* ngx_http_general_handler_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_general_handler_loc_conf_t *conf;
	//conf=(ngx_http_general_handler_loc_conf_t*)ngx_palloc(cf->pool,sizeof(ngx_http_general_handler_loc_conf_t));
	conf=new ngx_http_general_handler_loc_conf_t();
	if(conf==NULL)
		return NGX_CONF_ERROR;


	ngx_pool_cleanup_t * cleanup_t=NULL;
	cleanup_t=(ngx_pool_cleanup_t*)ngx_pool_cleanup_add(cf->pool,sizeof(void*));
	if(cleanup_t==NULL)
		return NULL;

	cleanup_t->handler=ngx_pool_delete_memory;
	cleanup_t->data=conf;

	conf->handler_set=false;
	conf->handler_inited=false;
	return conf;
}

static ngx_int_t ngx_http_post_data(ngx_http_request_t *r,ngx_str_t *post)
{
	ngx_http_request_body_t *rb;
	
	ngx_buf_t *buf;
	ngx_chain_t *bufs;

	ngx_int_t len,content_len;

	rb=r->request_body;

	bufs=rb->bufs;

	content_len=r->headers_in.content_length_n;

	if(!bufs || content_len <=0){
		post->data=(u_char*)"";
		post->len=0;

		return NGX_OK;
	}


	post->data=(u_char*)ngx_palloc(r->pool,content_len);
	if(post->data==NULL)
		return NGX_ERROR;

	if(bufs->buf->last - bufs->buf->pos >=content_len){
		ngx_memcpy(post->data,bufs->buf->pos,content_len);
		post->len=content_len;

	}
	else{

		len=0;

		while(bufs){

			buf=bufs->buf;
			bufs=bufs->next;

			if(len+buf->last-buf->pos>=content_len){
				ngx_memcpy(post->data+len,buf->pos,content_len-len);
				len=content_len;
				break;
			}

			ngx_memcpy(post->data+len,buf->pos,buf->last-buf->pos);
			len+=buf->last-buf->pos;
		}

		post->len=len;
	}

	return NGX_OK;
}


static void ngx_http_general_handler_post_handler(ngx_http_request_t *r)
{
	ngx_buf_t *b;
	ngx_str_t post;

	b=ngx_create_temp_buf(r->pool,sizeof(ngx_buf_t));
	if(!b){
		ngx_http_finalize_request(r,NGX_HTTP_INTERNAL_SERVER_ERROR);
		return;
	}

	if(NGX_OK!=ngx_http_post_data(r,&post)){
		ngx_http_finalize_request(r,NGX_HTTP_INTERNAL_SERVER_ERROR);
		return;
	}

	std::string str_args=std::string((char*)post.data,post.len);

	std::map<std::string,std::string> args_map;

	/* 

	std::vector<std::string> parts;

	img_util::split_string(str_args,parts,"&");


	img_util::from_vec2map(parts,args_map,"=");

	*/

	args_map["postdata"]=str_args;

	return ngx_http_finalize_request(r,ngx_http_handle_call(r,args_map));
}

static char * ngx_http_general_handler(ngx_conf_t *cf,ngx_command_t *cmd,void *conf)
{
	ngx_http_core_loc_conf_t *clcf;
	clcf=(ngx_http_core_loc_conf_t *)ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
	clcf->handler=ngx_http_general_handler_handler;

	ngx_http_general_handler_loc_conf_t *cfg=(ngx_http_general_handler_loc_conf_t*)conf;

	if(cfg->handler_set==true){
		return (char*)"handler has been set";
	}

	ngx_str_t *args=(ngx_str_t*)cf->args->elts;

	cfg->mod_info.mod_path=std::string((char*)args[1].data,args[1].len);
	if(cfg->mod_info.mod_path=="")
		return (char*)NGX_CONF_ERROR;

	if(cf->args->nelts>2)
		cfg->mod_info.mod_args=std::string((char*)args[2].data,args[2].len);
	else
		cfg->mod_info.mod_args="";

	int npos=cfg->mod_info.mod_path.find_last_of('/');
	if(npos==-1)
		cfg->mod_info.mod_name=cfg->mod_info.mod_path;
	else
		cfg->mod_info.mod_name=cfg->mod_info.mod_path.substr(npos+1);

	cfg->handler_set=true;

	cfg->module_handler.reset(new ModuleLoader(cfg->mod_info.mod_name,
				cfg->mod_info.mod_path,cfg->mod_info.mod_args));

	if(cfg->module_handler){
		cfg->handler_inited=cfg->module_handler->InitModule();
	}
	else
		cfg->handler_inited=false;

	if(cfg->handler_inited==false){
		ngx_log_error(NGX_LOG_ERR,cf->log,0,"%s:%s:%s init failed",cfg->mod_info.mod_name.c_str(),
				cfg->mod_info.mod_path.c_str(),cfg->mod_info.mod_args.c_str());

		return (char*)NGX_CONF_ERROR;
	}

	//对handler已经初始化完成

	return NGX_CONF_OK;
}
