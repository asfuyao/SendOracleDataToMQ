#include <cstdlib>
#include <iostream>
#include "datatable.h"
#include "logfile.h"

using namespace std;

const int MAXROWS = 1024;

/*----------------------------------------------------------------------------*/
bool GetTableStruct(otl_connect *otl_oracleDB,const char *c_tabName,struct TableStruct *st_tabStru)
{
    struct TableStruct *st_new = NULL;
    struct TableStruct *st_current = st_tabStru;

    otl_stream otl_dataset(100,"select column_name,data_type,data_length,data_precision,data_scale from user_tab_cols where table_name=:v_tablename<char[30]> order by column_id",*otl_oracleDB);
    otl_dataset<<c_tabName;

    if (!otl_dataset.eof())
    {
        otl_dataset>>st_tabStru->c_columnName>>st_tabStru->c_dataType>>st_tabStru->i_dataLength>>st_tabStru->i_dataPrecision>>st_tabStru->i_dataScale;
        st_tabStru->pNext = NULL;
    }
    else return false;

    while(!otl_dataset.eof())
    {
        while(st_current->pNext!=NULL)
        {
            st_current = st_current->pNext;
        }
        st_new = (TableStruct *)malloc(sizeof(TableStruct));
        otl_dataset>>st_new->c_columnName>>st_new->c_dataType>>st_new->i_dataLength>>st_new->i_dataPrecision>>st_new->i_dataScale;
        st_new->pNext = NULL;
        st_current->pNext = st_new;
    }

    return true;
}
/*----------------------------------------------------------------------------*/
bool CreateSqlString(otl_connect *otl_oracleDB,const char *c_tabName,char *c_sqlStr)
{
    struct TableStruct *st_tabStru = (TableStruct *)malloc(sizeof(TableStruct));
    if (!GetTableStruct(otl_oracleDB,c_tabName,st_tabStru))
    {
        cout<<"error"<<endl;
        return false;
    }

    struct TableStruct *st_current;
    st_current = st_tabStru;

    char c_temp[2048] = "select ";

    while (st_current != NULL)
    {
        if (st_current!=st_tabStru) strcat(c_temp,",");
        if (!strcmp(st_current->c_columnName,"RQ") && !strcmp(st_current->c_dataType,"DATE"))
        {
            strcat(c_temp,"to_char(");
            strcat(c_temp,st_current->c_columnName);
            strcat(c_temp,",'yyyy-mm-dd') as ");
            strcat(c_temp,st_current->c_columnName);
        }
        else if (!strcmp(st_current->c_dataType,"DATE"))
        {
            strcat(c_temp,"to_char(");
            strcat(c_temp,st_current->c_columnName);
            strcat(c_temp,",'yyyy-mm-dd hh24:mi:ss') as ");
            strcat(c_temp,st_current->c_columnName);
        }
        else if (!strcmp(st_current->c_dataType,"NUMBER") && st_current->i_dataScale==0)
        {
            strcat(c_temp,"to_char(");
            strcat(c_temp,st_current->c_columnName);
            strcat(c_temp,",'FM999999990') as ");
            strcat(c_temp,st_current->c_columnName);
        }
        else if (!strcmp(st_current->c_dataType,"NUMBER") && st_current->i_dataScale>0)
        {
            strcat(c_temp,"to_char(");
            strcat(c_temp,st_current->c_columnName);
            strcat(c_temp,",'FM999999990.0099') as ");
            strcat(c_temp,st_current->c_columnName);
        }
        else
        {
            strcat(c_temp,st_current->c_columnName);
        }
        st_current = st_current->pNext;
    }
    strcat(c_temp," from ");
    strcat(c_temp,c_tabName);
    strcpy(c_sqlStr,c_temp);
    return true;
}
/*----------------------------------------------------------------------------*/
bool GetTableData(otl_connect *otl_oracleDB,const char *c_tabName,struct TableData *ts_tabData)
{
    char c_sqlStr[2048];
    if (!CreateSqlString(otl_oracleDB,c_tabName,c_sqlStr)) return false;

    struct TableData *st_new = NULL;
    struct TableData *st_current = ts_tabData;

    otl_stream otl_dataset(MAXROWS,c_sqlStr,*otl_oracleDB);
    if (!otl_dataset.eof())
    {
        otl_dataset>>ts_tabData->c_itemData;
        ts_tabData->pNext = NULL;
    }
    else return false;
    while(!otl_dataset.eof())
    {
        while(st_current->pNext!=NULL)
        {
            st_current = st_current->pNext;
        }
        st_new = (TableData *)malloc(sizeof(TableData));
        otl_dataset>>st_new->c_itemData;
        st_new->pNext = NULL;
        st_current->pNext = st_new;
    }

    return true;
}
/*----------------------------------------------------------------------------*/
int GetTableColumns(otl_connect *otl_oracleDB,const char *c_tabName)
{
    int i_rows = 0;
    otl_stream otl_dataset(1,"select count(*) from user_tab_cols where table_name=:v_tablename<char[30]>",*otl_oracleDB);
    otl_dataset<<c_tabName;
    if(!otl_dataset.eof())
    {
        otl_dataset>>i_rows;
    }
    return i_rows;
}
/*----------------------------------------------------------------------------*/
int GetTableRows(otl_connect *otl_oracleDB,const char *c_tabName)
{
    int i_rows = 0;
    char c_sqlStr[1024] = "select count(*) from ";
    strcat(c_sqlStr,c_tabName);
    otl_stream otl_dataset(1,c_sqlStr,*otl_oracleDB);
    if(!otl_dataset.eof())
    {
        otl_dataset>>i_rows;
    }
    return i_rows;
}
