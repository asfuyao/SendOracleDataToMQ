#ifndef __DATATABLE_H__
#define __DATATABLE_H__

#include "otlv4.h"

struct TableStruct
{
    char c_columnName[30];
    char c_dataType[106];
    int  i_dataLength;
    int  i_dataPrecision;
    int  i_dataScale;
    struct TableStruct *pNext;
};

struct TableData
{
    char c_itemData[1024];
    struct TableData *pNext;
};

bool GetTableStruct(otl_connect *otl_oracleDB,const char *c_tabName,struct TableStruct *st_tabStru);
bool CreateSqlString(otl_connect *otl_oracleDB,const char *c_tabName,char *c_sqlStr);
bool GetTableData(otl_connect *otl_oracleDB,const char *c_tabName,struct TableData *ts_tabData);
int GetTableColumns(otl_connect *otl_oracleDB,const char *c_tabName);
int GetTableRows(otl_connect *otl_oracleDB,const char *c_tabName);

#endif
