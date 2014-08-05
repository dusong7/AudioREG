#ifndef _AUDIOREG_H
#define _AUDIOREG_H

#include "stdlib.h"
#include "stdio.h"
//#include <windows.h>
#include <conio.h>

#include <errno.h>
#include <afxpriv.h>
#include <afx.h>


#include "wavheader.h"
#include "log.h"

#include "seg_recognizer.h"



extern "C"
{
#include "ssad.h"
};

extern "C"
{
	extern asseg_t *silence_detection(sigstream_t *s);

	extern sigstream_t *sig_stream_open(const char *, int, float, size_t, int);
}

#ifdef _WIN64
#pragma comment(lib,"lib/msc_x64.lib")//x64
#else
#pragma comment(lib,"lib/msc.lib")//x86
#endif


//int AudioReg_login();
//int audio2text(char *filename,char *outfile);
//int ComputeSegNum(asseg_t *seg);



#endif