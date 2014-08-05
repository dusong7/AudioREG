
#include "wavheader.h"


head_pama wav_header_read(const char* wavfile)
{
    char ChunkID[4],Format[4],Subchunk1ID[4],Subchunk2ID[4];
    int  ChunkSize,Subchunk1Size,SampleRate,Subchunk2Size;
    short AudioFormat,NumChannels,BitsPerSample;
    head_pama pt;

	FILE* fp;
	fp=fopen(wavfile,"rb+");
	if(fp == NULL)
	{
		printf("Can't open wave file!\n");
	}

    fseek(fp, 0, SEEK_SET);
    fread(ChunkID, 1, 4, fp);
    if(strncmp("RIFF", ChunkID, 4) != 0)
    {
        printf("The input file is not wav format!\n");
        exit(2);
    }

    fread(&ChunkSize, 4, 1, fp);

    fread(Format, 1, 4, fp);
    if(strncmp("WAVE", Format, 4) != 0)
    {
        printf("The input file is not wav format!\n");
        exit(2);
    }

    fread(Subchunk1ID, 1, 4, fp);
    if(strncmp("fmt ", Subchunk1ID, 4) != 0)
    {
        printf("The input file is not wav format!\n");
        exit(2);
    }
    
    fread(&Subchunk1Size, 4, 1, fp);
	
    fread(&AudioFormat, 2, 1, fp);

    fread(&NumChannels, 2, 1, fp);
 
    fread(&SampleRate , 4, 1, fp);

    fseek(fp,34,SEEK_SET);
    fread(&BitsPerSample, 2, 1, fp);

    fread(Subchunk2ID, 1, 4, fp);
    if(strncmp("data", Subchunk2ID, 4) != 0)
    {
        printf("This file is not wave!\n");
        exit(2);
    }

	fseek(fp,40,SEEK_SET);
	fread(&Subchunk2Size, 4, 1, fp);

    pt.bits=BitsPerSample;
    pt.channels=NumChannels;
    pt.rate=SampleRate;
	pt.datasize=Subchunk2Size;
    fclose(fp);
    return pt;
}