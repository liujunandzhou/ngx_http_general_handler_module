#一个用来编写handler模块的模块
##介绍:
针对nginx开发的一个handler模块,用来简化nginx的handler模块使用

##使用方法:
1. nginx源代码,然后解压，执行configure --add-module=[your_dir]/ngx_http_general_handler_module
2. 进入[your_dir]/ngx_http_general_handler_module/handler/general_handler目录,执行make命令,生成了一个general_handler.so文件
3. 参照general_handler.conf文件的配置,在某一个location下面,使用AddHandler指令,就可以完成了

##依赖关系
在使用过程中使用到了**shared_ptr**这个类,因此依赖于gcc4.3+或者是安装了boost库,如果你的boost安装路径不是标准路径,则需要通过BOOST_INSTALL环境变量来指定

##说明
**/usr/include** **/usr/local/include** **/usr/local/boost/include**不需要指定
