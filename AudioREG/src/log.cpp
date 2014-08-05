#include "log.h"

void LogWrite(char *logText,int Type,int CMD_Type)
{
	SYSTEMTIME now;
	FILE* m_log;
	char  m_LogFileName[1000];
	GetLocalTime(&now);
	if (Type==NORMALTYPE)
	{
		sprintf(m_LogFileName,"log\\%4d-%02d-%02d_Wav2Text_NORMAL.log",now.wYear,now.wMonth,now.wDay);
	}
	else if(Type==ERRORTYPE)
	{
		sprintf(m_LogFileName,"log\\%4d-%02d-%02d_Wav2Text_ERROR.log",now.wYear,now.wMonth,now.wDay);
	}
	else if(Type==ENDSIGNTYPE)
	{
		sprintf(m_LogFileName,"log\\%4d-%02d-%02d_Wav2Text_END.log",now.wYear,now.wMonth,now.wDay);
	}
	else if(Type==TIMETYPE)
	{
		sprintf(m_LogFileName,"log\\%4d-%02d-%02d_Wav2Text_TIME.log",now.wYear,now.wMonth,now.wDay);
	}

	if (access("log",0))
	{
		mkdir("log");
	}
	
	m_log=fopen(m_LogFileName,"at");
	
	switch(CMD_Type)
	{
	case CMD_FILE :
		fprintf(m_log,"[%4d/%02d/%02d %02d:%02d:%02d]FILE: %s\r\n",now.wYear,now.wMonth,now.wDay,now.wHour,now.wMinute,now.wSecond,logText);
		break;
	case CMD_VAD :
		fprintf(m_log,"[%4d/%02d/%02d %02d:%02d:%02d]VAD: %s\r\n",now.wYear,now.wMonth,now.wDay,now.wHour,now.wMinute,now.wSecond,logText);
		break;
	case CMD_ASR:
		fprintf(m_log,"[%4d/%02d/%02d %02d:%02d:%02d]ASR: %s\r\n",now.wYear,now.wMonth,now.wDay,now.wHour,now.wMinute,now.wSecond,logText);
		break;
	case CMD_END:
		fprintf(m_log,"[%4d/%02d/%02d %02d:%02d:%02d]END: %s\r\n",now.wYear,now.wMonth,now.wDay,now.wHour,now.wMinute,now.wSecond,logText);
		break;
	default:
		break;
	}
	fclose(m_log);
}