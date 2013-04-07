    数据发送软件设计说明

1、软件功能
      取Oracle数据发送到本地MQ队列，程序运行时输入不同的参数即可获得任意表数据放到不同的本地MQ队列中，增加或修改Oracle表结构、修改本地MQ队列名称都不需要对程序进行修改，只需要修改程序运行参数即可

1、开发工具和编译器
      Windows平台使用Dev-C++编写和其内置的gcc编译，建立工程文件后用开发工具即可调用gcc进行编译
      Linux下使用VI编写和linux系统自带的gcc编译，手工编写makefile文件然后在命令行下使用make命令编译

3、程序设计思路
      a. 使用开源的OTL模板库调用Oracle的OCI接口连接Oracle
      b. 通过Oracle的系统表user_tab_cols来获得表的结构（列名、字段类型、字段长度）
      c. 根据获得的表结构信息来动态生成一条查询语句，在查询时将日期类型和数字类型的字段都通过to_char函数转换成字符串
      d. 将用动态sql语句查询获得的数据根据固定的规则拼接成一个字符串后发送到本地MQ队列

4、源代码说明
     datapack.cpp                  数据打包
     函数：TableDataPack     将获得的Oracle表数据拼接成一个字符串

     datatable.cpp                 数据操作，获得表列数、行数，获取表结构，获取表数据
     函数：GetTableStruct    获取表结构，结果存入链表TableStruct里
                CreateSqlString   根据表结构生成动态sql查询语句
                GetTableData       获得表数据，结果存入链表TableData里
                GetTableColumns 获得表列数
                GetTableRows      获得表行数
     常量：MAXROWS          单表最大行数

     logfile.cpp                      写数据发送日志，数据发送到本地MQ队列时成功或失败都写入日志文件作为记录
     函数：AppendTextToFile写文本文件
                WriteLog              写日志
                WriteMQErrorLog  写MQ错误日志，可以写入错误代码

     main.cpp                        主函数
     常量：DATAMAX           打包后的字符串最大长度
     参数：argv[1]               队列名
                argv[2]               队列管理器名
                argv[3]               消息ID
                argv[4]               表名

     mq.cpp                          MQ相关的函数
     函数：PutMessage        放字符串到本地MQ队列

     otlv4.h                           OTL模板库

     Makefile                         linux下的make编译文件
  
     sendERPMessage.sh        linux下的任务调度程序执行脚本例子



5、linux下编译和使用说明
编译用make命令即可，include路径和lib路径根据实际请修改Makefile文件

执行程序前查看环境变量NLS_LANG如果没有设置或设置不正确，先设置环境变量
export NLS_LANG=AMERICAN_AMERICA.ZHS16GBK

编写sendERPMessage.sh文件，内容是命令加mq相关参数

#!/bin/sh
export ORACLE_BASE=/DBSoftware/app/oracle
export ORACLE_HOME=/DBSoftware/app/oracle/product/10.2.0/db_1
export NLS_LANG=AMERICAN_AMERICA.ZHS16GBK
/DBSoftware/app/oracle/product/10.2.0/db_1/bin/SendOracleDataToMQ/sendERPMessage ATY.ERP.D.1.A ADM_VIR.QM $1 $2

设置计划任务
crontab -u oracle -e  编辑oracle用户的计划任务

21 10 * * * sh /DBSoftware/app/oracle/product/10.2.0/db_1/bin/SendOracleDataToMQ/sendERPMessage.sh DT0001 TBL_MQ_JOB

日志文件默认是与程序放到同一目录，但如果用计划任务调用的话日志会写到任务所属用户的主目录下


