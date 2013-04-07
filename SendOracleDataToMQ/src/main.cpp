#include <cstdlib>
#include <iostream>
#include "datapack.h"
#include "datatable.h"
#include "mq.h"
#include "logfile.h"

using namespace std;

//char queueManager[] = "qm_a";
//char queue[] = "Q_A";
int DATAMAX = 100000;

int main(int argc, char *argv[])
{
    if (argc<5)
    {
        cout<<"Missing parameter!"<<endl;
        cout<<"Example: sendERPMessage QueueName QueueManagerName MessageID TableName"<<endl;
        return -1;
    }

    try
    {
        otl_connect::otl_initialize();
        otl_connect *otl_oracleDB = new otl_connect;
        otl_oracleDB->rlogon("TYXC_DBA/rdis5840808@rdis_zh");
        if (GetTableRows(otl_oracleDB,argv[4]) == 0)
        {

            WriteLog(argv[4]);
            WriteLog("row is 0");
            printf("%s is 0 row!",argv[4]);
        }
        else
        {
            char c_packedData[DATAMAX];
            if (TableDataPack(c_packedData,argv[3],argv[4],otl_oracleDB))
            {
                //cout<<c_packedData<<endl;
                if(PutMessage(argv[1],argv[2],c_packedData))
                {
                    WriteLog(c_packedData);
                    printf("Put Message to MQ success!\n");
                }
                else
                {
                    printf("Put Message to MQ error, find errorcode in log file.\n");
                }
            }
            else
            {
                printf("Table data pack error\n");
            }
        }
        otl_oracleDB->logoff();
    }
    catch(otl_exception& p)
    {
        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.var_info<<endl; // print out the variable that caused
        return -1;
    }

    return 0;
}
