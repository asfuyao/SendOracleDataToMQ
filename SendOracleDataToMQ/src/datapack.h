#ifndef __DATAPACK_H__
#define __DATAPACK_H__

#include "otlv4.h"

bool TableRowDataPack(char *c_packedData,char *c_messageID,char *c_tabName,int i_rowNumber,otl_connect *otl_oracleDB);
bool TableDataPack(char *c_packedData,char *c_messageID,char *c_tabName,otl_connect *otl_oracleDB);

#endif
