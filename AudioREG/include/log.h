#ifndef _log_h_
#define _log_h_

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <afx.h>


#define CMD_FILE       0
#define CMD_VAD        1
#define CMD_ASR        2
#define CMD_END        4

#define NORMALTYPE     0
#define ERRORTYPE      1
#define ENDSIGNTYPE    2
#define TIMETYPE       3

#ifdef __cplusplus
extern "C" void LogWrite(char* logText,int Type,int CMD_Type);
#else
void LogWrite(char* logText,int Type,int CMD_Type);
#endif

#endif