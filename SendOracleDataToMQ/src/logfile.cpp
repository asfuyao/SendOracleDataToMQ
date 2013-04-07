#include <stdio.h>
#include <time.h>
#include <string.h>
#include "logfile.h"
char LOGFILE[100]="MessageToMQ";
void AppendTextToFile(char* c_filename,char* c_log)
{
    time_t t;
    time(&t);
    struct tm* tp= localtime(&t);
    char c_time[100];
    char c_day[100];
    char c_file[100];
    strcpy(c_file,c_filename);
    strftime(c_day,  100, "%Y%m%d", tp);
    strftime(c_time, 100, "%H:%M:%S", tp);
    strcat(c_file,c_day);
    strcat(c_file,".log");
    FILE *fo;
    fo = fopen(c_file, "a");
    if (fo == 0)
    {
        printf("Open log file error!");
        return;
    }
    fprintf(fo, "%s %s\r\n",c_time, c_log);
    fclose(fo);
}

void WriteLog( char *c_str )
{
    AppendTextToFile(LOGFILE,c_str);
}

void WriteMQErrorLog(char *c_str,long l_errorCode)
{
    char c_errorCode[20];
    char c_errorMessage[200];
    sprintf(c_errorCode,"%d",l_errorCode);
    strcat(c_errorMessage,c_str);
    strcat(c_errorMessage,c_errorCode);
    WriteLog(c_errorMessage);
}
