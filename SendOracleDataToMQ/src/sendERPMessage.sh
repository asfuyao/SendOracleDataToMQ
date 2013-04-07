#!/bin/sh

export ORACLE_BASE=/DBSoftware/app/oracle
export ORACLE_HOME=/DBSoftware/app/oracle/product/10.2.0/db_1
export NLS_LANG=AMERICAN_AMERICA.ZHS16GBK

/DBSoftware/app/oracle/product/10.2.0/db_1/bin/SendOracleDataToMQ/sendERPMessage LINUX_YCDL LINUX $1 $2
