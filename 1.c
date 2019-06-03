#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define filter 11.0
#define PI 3.141592

int main()
{
	FILE* fpInputImage = 0;
	FILE* fpOutputImage = 0;
	FILE* fpOutLPF = 0;
	FILE* fpOutHPF = 0;
	FILE* fpOutDFT=0;

	unsigned char** ppInputImageBuffer = 0;
	unsigned char** LPFImage = 0;
	unsigned char** HPFImage = 0;
	unsigned char** Original = 0;
	unsigned char** DFTImage=0;

	int **LPF = 0;
	int **HPF = 0;
	float **DFT=0;

	double lpf[3][3] = { { 1 / 9, 1 / 9, 1 / 9 }, { 1 / 9, 1 / 9, 1 / 9 }, { 1 / 9, 1 / 9, 1 / 9 } };
	int hpf[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
	int i, j, k,l;

	// input file open
	fpInputImage = fopen("LENA.img", "rb");

	// memory allocaiton
	ppInputImageBuffer = (unsigned char**)malloc(sizeof(unsigned char*)* 256);
	Original = (unsigned char**)malloc(sizeof(unsigned char*)* 256);
	LPFImage = (unsigned char**)malloc(sizeof(unsigned char*)* 256);
	HPFImage = (unsigned char**)malloc(sizeof(unsigned char*)* 256);
	DFTImage = (unsigned char**)malloc(sizeof(unsigned char*)* 256);

	LPF = (int**)malloc(sizeof(int*)* 256);
	HPF = (int**)malloc(sizeof(int*)* 256);
	DFT = (float**)malloc(sizeof(float*)* 256);

	for (i = 0; i < 256; i++)
	{
		ppInputImageBuffer[i] = (unsigned char*)malloc(sizeof(unsigned char*)* 256);
		Original[i] = (unsigned char*)malloc(sizeof(unsigned char*)* 256);
		LPFImage[i] = (unsigned char*)malloc(sizeof(unsigned char*)* 256);
		HPFImage[i] = (unsigned char*)malloc(sizeof(unsigned char*)* 256);
		DFTImage[i]=(unsigned char*)malloc(sizeof(unsigned char*)* 256);

		LPF[i] = (int*)malloc(sizeof(int*)* 256);
		HPF[i] = (int*)malloc(sizeof(int*)* 256);
		DFT[i]=(float*)malloc(sizeof(float*)*256);
	}

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			Original[i][j] = 0;
			LPFImage[i][j] = 0;
			HPFImage[i][j] = 0;
			DFTImage[i][j]=0;

			LPF[i][j] = 0;
			HPF[i][j] = 0;
			DFT[i][j]=0;
		}
	}
	// input file read to memory from the file
	for (i = 0; i < 256; i++)
	{
		fread(ppInputImageBuffer[i], sizeof(unsigned char), 256, fpInputImage);
	}
	fclose(fpInputImage);

	fpInputImage = fopen("LENA.img", "rb");
	for (i = 0; i < 256; i++)
	{
		fread(Original[i], sizeof(unsigned char), 256, fpInputImage);
	}

	
	
	//LPF
	for (i = 1; i < 255; i++)
	{
		for (j = 1; j < 255; j++)
		{
			for (k = i-1; k <= i+1; k++){
				for (l = j-1; l <= j+1; l++){
					LPF[i][j] += (double)Original[k][l]*(1/9.0);
				}
			}
		}
	}

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			if (LPF[i][j] < 0)
				LPF[i][j] = 0;
			if (LPF[i][j] > 255)
				LPF[i][j] = 255;
		}
	}

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			LPFImage[i][j] = (unsigned char)LPF[i][j];
		}
	}


	//HPF
	for (i = 1; i < 255; i++)
	{
		for (j = 1; j < 255; j++)
		{
			for (k = -1; k <= 1; k++){
				for (l = -1; l <= 1; l++){
					HPF[i][j] += Original[i+k][j+l]*hpf[k+1][l+1];
				}
			}
		}
	}

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			if (HPF[i][j] < 0)
				HPF[i][j] = 0;
			if (HPF[i][j] > 255)
				HPF[i][j] = 255;
		}
	}

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			HPFImage[i][j] = (unsigned char)HPF[i][j];
		}
	}
	//DFT
	for (i = 0; i < 512; i++)
	{
		for (j = 0; j < 512; j++)
		{
			for (k = 0; k <= 512; k++){
				for (l = 0; l <= 512; l++){
					DFT[i][j] += (1/(256*256)*Original[i+k][j+l]*cos(2*PI*(i*k+j*l)/256));
				}
			}
		}
	}

	for (i = 0; i < 255; i++)
	{
		for (j = 0; j < 255; j++)
		{
			if (DFT[i][j] < 0)
				DFT[i][j] = 0;
			if (DFT[i][j] > 255)
				DFT[i][j] = 255;
		}
	}

	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			DFTImage[i][j] = (unsigned char)DFT[i][j];
		}
	}



	// output fileopen
	fpOutLPF = fopen("LPF.img", "wb");
	fpOutHPF = fopen("HPF.img", "wb");
	fpOutDFT = fopen("DFT.img", "wb");

	//write the file
	for (i = 0; i < 256; i++){
		fwrite(LPFImage[i], sizeof(unsigned char), 256, fpOutLPF);
		fwrite(HPFImage[i], sizeof(unsigned char), 256, fpOutHPF);
		fwrite(DFTImage[i], sizeof(unsigned char), 256, fpOutDFT);
	}

	for (i = 0; i < 256; i++){
		free(ppInputImageBuffer[i]);
	}

	free(ppInputImageBuffer);
	fclose(fpInputImage);
	fclose(fpOutputImage);
	return 0;
}