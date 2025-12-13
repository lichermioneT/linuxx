#pragma once
#include <iostream>
#include <string>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
using namespace std;

#define LOG_ERR    "log.error"
#define LOG_NORMAL "log.txt"

#define DEBUG   0
#define NORMAL  1
#define WARNING 2
#define ERROR   3
#define FATAL   4

const char* to_levelstr(int level)
{
    switch(level)
    {
        case DEBUG:  return "DEBUG";
        case NORMAL: return "NORMAL";
        case WARNING:return "WARNING";
        case ERROR:  return "ERROR";
        case FATAL:  return "FATAL"; 
        default: return nullptr;
    }
}

void logMessage(int level, const char* format, ...)
{
    //[日志等级][时间戳/时间] [pid] [message]
    // std::cout<< message <<std::endl;

    // va_list
    // va_start()
    // va_arg()
    // va_end()
#define NUM 1024
    char logprefix[NUM];
    snprintf(logprefix, sizeof(logprefix), "[%s][%ld][%d]", to_levelstr(level), (long int)time(nullptr), getpid());


    char logcontent[NUM];
    va_list arg;
    va_start(arg, format);

    vsnprintf(logcontent, sizeof(logcontent), format, arg);
    // cout<< logprefix << " : " <<logcontent <<endl;

    FILE* log = fopen(LOG_NORMAL, "a");
    FILE* err = fopen(LOG_ERR,    "a");

    if(log != nullptr && err != nullptr)
    {
        FILE* cur = nullptr;
        if(level == DEBUG || level == NORMAL || level == WARNING)  cur = log;
        if(level == ERROR || level == ERROR) cur = err;
        fprintf(cur, "%s%s\n", logprefix, logcontent);
        fclose(log);
        fclose(err);
    }
}