#ifndef _seg_recognizer_h
#define _seg_recognizer_h

#include <string.h>
#include <stdio.h>
//#include "windows.h"

#include <string>
#include "log.h"

#include "qisr.h"
#include "msp_cmn.h"
#include "msp_errors.h"



using namespace std;

void UTF8ToGB2312( const char *  utf8, char * &gb2312 );

void print_result( const char* szXml );

void write_text(const char* jsonfile,FILE* outfp);

int seg_recognizer(const char* audiowave_file, float startTime, float endTime, string *segtxtStr);

#endif 