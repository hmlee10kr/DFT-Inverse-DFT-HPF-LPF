#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
typedef unsigned char uint8;
typedef unsigned int uint32;
#define PI 3.14159265


int main(void) {
	FILE*     fpInputImage = 0;
	FILE*     fpOutputImage = 0;
	uint8**   ppInputImageBuffer = 0;
	uint8**   ppOutputImageBuffer = 0;
	double**  transInputImageBuffer = 0;
	double**  realBuffer=0;
	double**  imaginaryBuffer=0;
	int i, j, s, t;
	double dc_value;
	double coeff;

	fpInputImage = fopen("lena.img", "rb");

	ppInputImageBuffer = (unsigned char**)malloc(sizeof(unsigned char*) * 256);
	for (i = 0; i<256; i++) {
		ppInputImageBuffer[i] = (unsigned char*)malloc(sizeof(unsigned char) * 256);
	}
	ppOutputImageBuffer = (unsigned char**)malloc(sizeof(unsigned char*) * 256);
	for (i = 0; i<256; i++) {
		ppOutputImageBuffer[i] = (unsigned char*)malloc(sizeof(unsigned char) * 256);
	}
	transInputImageBuffer = (double**)malloc(sizeof(double*) * 256);
	for (i = 0; i<256; i++) {
		transInputImageBuffer[i] = (double*)malloc(sizeof(double) * 256);
	}
	realBuffer = (double**)malloc(sizeof(double*) * 256);
	for (i = 0; i<256; i++) {
		realBuffer[i] = (double*)malloc(sizeof(double) * 256);
	}
	imaginaryBuffer = (double**)malloc(sizeof(double*) * 256);
	for (i = 0; i<256; i++) {
		imaginaryBuffer[i] = (double*)malloc(sizeof(double) * 256);
	}

	for (i = 0; i<256; i++) {
		fread(ppInputImageBuffer[i], sizeof(unsigned char), 256, fpInputImage);
	}

	for (i = 0; i < 256; i++) {
		for (j = 0; j < 256; j++) {
			realBuffer[i][j] = 0;
			imaginaryBuffer[i][j] = 0;
			for (s = 0; s < 256; s++) {
				for (t = 0; t < 256; t++) {
					realBuffer[i][j] += (-1.0 / (256.0 * 256.0) * pow(-1.0,(double)s+t) * ppInputImageBuffer[s][t] * (cos(2.0 * PI * (s*i + t*j) / 256.0)));
					imaginaryBuffer[i][j] += (-1.0 / (256.0 * 256.0) * pow(-1.0,(double)s+t) * ppInputImageBuffer[s][t] * (sin(2.0 * PI * (s*i + t*j) / 256.0)));
				}
			}
		}
	}
	printf("DONE\n");


	dc_value = sqrt(realBuffer[128][128]*realBuffer[128][128]+imaginaryBuffer[128][128]*imaginaryBuffer[128][128]);
	for (i = 0; i < 256; i++) {
		for (j = 0; j < 256; j++) {
			transInputImageBuffer[i][j] = 0;
			coeff = sqrt(realBuffer[i][j]*realBuffer[i][j]+imaginaryBuffer[i][j]*imaginaryBuffer[i][j]);
			transInputImageBuffer[i][j] = (255.0 * log10(coeff + 1.0)) / log10(dc_value + 1.0);

		}
	}
	printf("DONE @@@@@\n");
	for (i = 0; i< 256; i++) {
		for (j = 0; j< 256; j++) {
			ppOutputImageBuffer[i][j] = (unsigned char)transInputImageBuffer[i][j];
		}
	}



	fpOutputImage = fopen("DFT.img", "wb");

	for (i = 0; i<256; i++) {
		fwrite(ppOutputImageBuffer[i], sizeof(unsigned char), 256, fpOutputImage);
	}
	printf("º¯È¯");
	free(ppInputImageBuffer);
	free(transInputImageBuffer);
	fclose(fpInputImage);
	fclose(fpOutputImage);

	return 0;
}