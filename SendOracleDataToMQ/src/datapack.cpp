#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "datatable.h"
#include "datapack.h"

int ROWCHARSIZE=4;

using namespace std;

bool TableDataPack(char *c_packedData,char *c_messageID,char *c_tabName,otl_connect *otl_oracleDB)
{
    strcpy(c_packedData,c_messageID);
    int i_rows = GetTableRows(otl_oracleDB,c_tabName);
    char c_rows[10];
    for (int i=0; i<ROWCHARSIZE; i++)
    {
        c_rows[i]='0';
    }
    char c_temp[10];
    sprintf(c_temp,"%d",i_rows);
    for (int i=1; i<=strlen(c_temp); i++)
    {
        c_rows[ROWCHARSIZE-i] = c_temp[strlen(c_temp)-i];
    }
    strcat(c_packedData,"\x7f\x26");
    strcat(c_packedData,c_rows);

    struct TableData *ts_tabData = (TableData *)malloc(sizeof(TableData));
    if (!GetTableData(otl_oracleDB,c_tabName,ts_tabData))
    {
        printf("Get Table Data error!!!\n");
        return false;
    }

    struct TableData *ts_temp = ts_tabData;

    while(ts_temp!=NULL)
    {
        strcat(c_packedData,"\x7f\x26");
        strcat(c_packedData,ts_temp->c_itemData);
        ts_temp = ts_temp->pNext;
    }
    if (strlen(c_packedData)==0)
    {
        printf("Table data is zero!!!\n");
        return false;
    }
    strcat(c_packedData,"\x7f\x26");
    return true;
}
