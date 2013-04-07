#ifndef __LOGFILE_H__
#define __LOGFILE_H__

void AppendTextToFile(char* c_filename,char* c_log);
void WriteLog(char *c_str);
void WriteMQErrorLog(char *c_str,long l_errorCode);
#endif
