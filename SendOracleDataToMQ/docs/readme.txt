    ���ݷ���������˵��

1���������
      ȡOracle���ݷ��͵�����MQ���У���������ʱ���벻ͬ�Ĳ������ɻ����������ݷŵ���ͬ�ı���MQ�����У����ӻ��޸�Oracle��ṹ���޸ı���MQ�������ƶ�����Ҫ�Գ�������޸ģ�ֻ��Ҫ�޸ĳ������в�������

1���������ߺͱ�����
      Windowsƽ̨ʹ��Dev-C++��д�������õ�gcc���룬���������ļ����ÿ������߼��ɵ���gcc���б���
      Linux��ʹ��VI��д��linuxϵͳ�Դ���gcc���룬�ֹ���дmakefile�ļ�Ȼ������������ʹ��make�������

3���������˼·
      a. ʹ�ÿ�Դ��OTLģ������Oracle��OCI�ӿ�����Oracle
      b. ͨ��Oracle��ϵͳ��user_tab_cols����ñ�Ľṹ���������ֶ����͡��ֶγ��ȣ�
      c. ���ݻ�õı�ṹ��Ϣ����̬����һ����ѯ��䣬�ڲ�ѯʱ���������ͺ��������͵��ֶζ�ͨ��to_char����ת�����ַ���
      d. ���ö�̬sql����ѯ��õ����ݸ��ݹ̶��Ĺ���ƴ�ӳ�һ���ַ������͵�����MQ����

4��Դ����˵��
     datapack.cpp                  ���ݴ��
     ������TableDataPack     ����õ�Oracle������ƴ�ӳ�һ���ַ���

     datatable.cpp                 ���ݲ�������ñ���������������ȡ��ṹ����ȡ������
     ������GetTableStruct    ��ȡ��ṹ�������������TableStruct��
                CreateSqlString   ���ݱ�ṹ���ɶ�̬sql��ѯ���
                GetTableData       ��ñ����ݣ������������TableData��
                GetTableColumns ��ñ�����
                GetTableRows      ��ñ�����
     ������MAXROWS          �����������

     logfile.cpp                      д���ݷ�����־�����ݷ��͵�����MQ����ʱ�ɹ���ʧ�ܶ�д����־�ļ���Ϊ��¼
     ������AppendTextToFileд�ı��ļ�
                WriteLog              д��־
                WriteMQErrorLog  дMQ������־������д��������

     main.cpp                        ������
     ������DATAMAX           �������ַ�����󳤶�
     ������argv[1]               ������
                argv[2]               ���й�������
                argv[3]               ��ϢID
                argv[4]               ����

     mq.cpp                          MQ��صĺ���
     ������PutMessage        ���ַ���������MQ����

     otlv4.h                           OTLģ���

     Makefile                         linux�µ�make�����ļ�
  
     sendERPMessage.sh        linux�µ�������ȳ���ִ�нű�����



5��linux�±����ʹ��˵��
������make����ɣ�include·����lib·������ʵ�����޸�Makefile�ļ�

ִ�г���ǰ�鿴��������NLS_LANG���û�����û����ò���ȷ�������û�������
export NLS_LANG=AMERICAN_AMERICA.ZHS16GBK

��дsendERPMessage.sh�ļ��������������mq��ز���

#!/bin/sh
export ORACLE_BASE=/DBSoftware/app/oracle
export ORACLE_HOME=/DBSoftware/app/oracle/product/10.2.0/db_1
export NLS_LANG=AMERICAN_AMERICA.ZHS16GBK
/DBSoftware/app/oracle/product/10.2.0/db_1/bin/SendOracleDataToMQ/sendERPMessage ATY.ERP.D.1.A ADM_VIR.QM $1 $2

���üƻ�����
crontab -u oracle -e  �༭oracle�û��ļƻ�����

21 10 * * * sh /DBSoftware/app/oracle/product/10.2.0/db_1/bin/SendOracleDataToMQ/sendERPMessage.sh DT0001 TBL_MQ_JOB

��־�ļ�Ĭ���������ŵ�ͬһĿ¼��������üƻ�������õĻ���־��д�����������û�����Ŀ¼��


