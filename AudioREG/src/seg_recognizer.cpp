#include "seg_recognizer.h"
#include "AudioRegcore.h"


//  UTF8  convert to GB2312
void UTF8ToGB2312( const char *  utf8, char * &gb2312 )
{
	int nLen = MultiByteToWideChar( CP_UTF8, 0, utf8, -1, NULL, 0 );
	if (nLen == 0)
	{
		gb2312 = NULL;
		return ;
	}

	WCHAR* pwszGB2312 = new WCHAR[ nLen + 1 ];
	RtlZeroMemory( pwszGB2312, nLen * 2 + 2 );

	MultiByteToWideChar( CP_UTF8, 0, utf8, -1, (LPWSTR)pwszGB2312, nLen );

	nLen = WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pwszGB2312, -1, NULL, 0, NULL, NULL );

	CHAR* pszGB2312 = new CHAR[ nLen + 1 ];
	RtlZeroMemory( pszGB2312, nLen + 1 );

	WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)pwszGB2312, -1, pszGB2312, nLen, NULL, NULL );

	gb2312 = pszGB2312;

	delete [] pwszGB2312;
}

void get_str_text(const char* jsonfile,string *resStr)
{
	char* xmlfile ;
	UTF8ToGB2312( jsonfile, xmlfile);

	char* mystr = new char[100];
	//string tempstr;

	for(int i = 1; i<strlen(xmlfile)-1; i++)
	{
		if(xmlfile[i] == 'w'&&xmlfile[i-1] == '\"'&&xmlfile[i+1] == '\"') 
		{
			int n = 0;
			for(int j = i+4; xmlfile[j]!= '\"'; j++)
			{
				mystr[n++] = xmlfile[j];
			}
			mystr[n]='\0';
			resStr->append(mystr);
		}		
	}
	delete [ ] mystr;
}

/*void print_result( const char* szXml ) 
{
	char* pgb2312 = 0;
	UTF8ToGB2312( szXml, pgb2312 );
	printf( "ret:%s", pgb2312 );
	delete []pgb2312;
}*/


int seg_recognizer(const char* audiowave_file, float startTime, float endTime, string *segtxtStr)
{
	int ret = 0;
	int errorCode = 0;
	int resultStatus;
	char logStr[1000];
	const char *session_id = NULL;
	char *params = "lang=sms,acous=anhui,rate=16k,aue=speex-wb;7,nbest=3,auf=audio/L16;rate=16000";

	if(endTime<startTime)
	{
		printf("End time %f should be greater than startTime %f!\n",endTime,startTime);
		sprintf(logStr, "End time %f should be greater than startTime %f!\n",endTime-0.5,startTime+0.5);
		LogWrite(logStr,1,2);
		return -1;
	}

	const char *xml_result = NULL;

	session_id = QISRSessionBegin(NULL, params,&errorCode);

	
	if ( session_id == NULL )
	{
		//QISRFini();
		//FreeLibrary(qisr_mod);
		printf("QISRSessionBegin fail, %08X\n", errorCode);
		sprintf(logStr, "QISRSessionBegin fail, %08X\n", errorCode);
		LogWrite(logStr,1,2);
		return errorCode;		
	}

	

	/* This example shows how to read wave file */
	/* then put wave data to recognizer         */
	char    buffer[800];
	size_t  readno = 0;         // read data length
	FILE    *fp = NULL;         // wave file
	int     first_clip = 1;     // first clip of the file
	int     audio_status;
	int     ep_status;
	int     recog_status;

	if ((fp = fopen(audiowave_file, "rb")) == NULL)
	{
		printf("open audio wave file: %s failed.\n", audiowave_file);
		sprintf(logStr, "open audio wave file: %s failed.\n", audiowave_file);
		LogWrite(logStr,1,2);
		//QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
		return ret;
	}

	int sampleCount, readNum, restCount;
	const float sampleRate = 16000.0;
	const unsigned int bytespersample = 2;
	int startByte = (int)(startTime*sampleRate)*bytespersample+44;
	sampleCount = (int)((endTime-startTime)*sampleRate*2+0.5);
	fseek(fp,startByte,SEEK_SET);
	LogWrite("开始识别语音片段",0,2);

	//xiongchenhui修改最大写入大小，参照demo
	if(sampleCount >= 800)
	{
		readNum = floor((double)sampleCount/sizeof(buffer));
		restCount = sampleCount-readNum*sizeof(buffer);

		for(int i = 0; i<readNum; i++)
		{
			readno = fread(buffer, 1, sizeof(buffer), fp);
			if(first_clip)
		    {
				audio_status = MSP_AUDIO_SAMPLE_FIRST;
				first_clip = 0;
			}
			else
			{
				audio_status =  MSP_AUDIO_SAMPLE_CONTINUE;
			}

			/* there calls QISRAudioWrite, you should check the status */
		    /* to see if recognizer give out the result                */
		    ret = QISRAudioWrite(session_id, buffer, readno, audio_status, &ep_status, &recog_status);
		    if(ret)
		    {
				printf("QISRAudioWrite failed with code=%d.\n", ret);
				sprintf(logStr, "QISRAudioWrite failed with code=%d.\n", ret);
				LogWrite(logStr,1,2);
				QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
				fclose(fp);
				return ret;
			}

			/* check EP status */
			if(ep_status == MSP_EP_TIMEOUT || ep_status == MSP_EP_ERROR)
			{
				printf("QISRAudioWrite EP failed with status=%d.\n", ep_status);
				sprintf(logStr, "QISRAudioWrite EP failed with status=%d.\n", ep_status);
				LogWrite(logStr,1,2);
				QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
				fclose(fp);
				return ret;
			}
			/* check recog_status, if recog_status equals one of     */
			/* following values, it means recognizer gave the result */
			if(recog_status == MSP_REC_STATUS_SUCCESS)
			{
				/* now XML result can be fetched */
				xml_result = QISRGetResult(session_id, &resultStatus, 1000,&errorCode);
				if(xml_result != NULL)
				{
					get_str_text(xml_result,segtxtStr);
					//print_result(xml_result);
				}
				//print_result( xml_result );
				if( errorCode != 0 )
				{
					break;
				}

				if( resultStatus == MSP_REC_STATUS_NO_MATCH ||
					resultStatus == MSP_REC_STATUS_COMPLETE)
				{
					break;
				}
			}
			if(ep_status == MSP_EP_AFTER_SPEECH)
			{
				break;
			}
			else if (recog_status == MSP_REC_STATUS_NO_MATCH )
			{
				break;
			}

			//Sleep(100);
		}
		
		LogWrite("开始识别语音片段最后一段",0,2);
		if( restCount == 0)
		{
			char silBuffer[800] = {0};
			memcpy(buffer, silBuffer, sizeof(buffer));
			readno = sizeof(buffer);
		}
		else
		{
			readno = fread(buffer, 1, restCount, fp);
		}
		audio_status =  MSP_AUDIO_SAMPLE_LAST;
		/* there calls QISRAudioWrite, you should check the status */
		/* to see if recognizer give out the result                */
		ret = QISRAudioWrite(session_id, buffer, readno, audio_status, &ep_status, &recog_status);
		if(ret)
		{
			printf("QISRAudioWrite failed with code=%d.\n", ret);
			sprintf(logStr, "QISRAudioWrite failed with code=%d.\n", ret);
			LogWrite(logStr,1,2);
			QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
			fclose(fp);
			return ret;
		}

		/* check EP status */
		if(ep_status == MSP_EP_TIMEOUT || ep_status == MSP_EP_ERROR)
		{
			printf("QISRAudioWrite EP failed with status=%d.\n", ep_status);
			sprintf(logStr, "QISRAudioWrite EP failed with status=%d.\n", ep_status);
			LogWrite(logStr,1,2);
			QISRSessionEnd(session_id, "ISR_REC_STOP_OTHER");
			fclose(fp);
			return ret;
		}

		/* check recog_status, if recog_status equals one of     */
		/* following values, it means recognizer gave the result xiongchenhui修改*/
		
		
			sprintf(logStr, "开始循环获取最后片段识别结果，进行收尾工作\n");
			LogWrite(logStr,0,2);
			int endcount = 0;
			Sleep(50);
			while(1)
			{
				/* now XML result can be fetched */
				xml_result = QISRGetResult(session_id, &resultStatus, 1000,&errorCode);
				if(xml_result != NULL)
				{
					get_str_text(xml_result,segtxtStr);
					//print_result(xml_result);
				}
				//print_result( xml_result );
				if( errorCode != 0 )
				{
					sprintf(logStr, "识别出现错误%d\n", errorCode);
					LogWrite(logStr,1,2);
					break;
				}

				if( resultStatus == MSP_REC_STATUS_NO_MATCH ||
					resultStatus == MSP_REC_STATUS_COMPLETE )
				{
					sprintf(logStr, "检测到识别结束标志，最后片段已经接收完成\n");
					LogWrite(logStr,0,2);
					break;
				}

				endcount++;
				//Sleep(150);
			}
			sprintf(logStr, "最后片段识别结果已全部取出，共进行了%d次循环，准备结束识别会话\n",endcount);
			LogWrite(logStr,0,2);
		

	}

	
	

	LogWrite("识别语音片段成功",0,2);
	QISRSessionEnd(session_id, "MSP_REC_STOP_SUCCESS");
	Sleep(50);
	fclose(fp);
	sprintf(logStr, "该片段已经识别成功会话结束，资源已释放\n");
	LogWrite(logStr,0,2);
	return 0;
}