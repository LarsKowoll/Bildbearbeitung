/*
 * input.c
 *
 *  Created on: 07.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "main.h"
#include "input.h"
#include "errorModule.h"

static FILE *file;
static BITMAPFILEHEADER bitmapFileHeader;
static BITMAPINFOHEADER bitmapInfoHeader;

static int readFileHeader();
static int readInfoHeader();
static int readFarbtabelle();
static int readBilddatenUnkomprimiert();
static int readBilddatenKomprimiert();
static int writeBilddaten(BYTE bilddaten[height][width], BYTE value, int zeile, int spalte);
static int readByte(BYTE *byte);
static int createArray(BYTE bilddaten[height][width]);
static void initArray(BYTE array[height][width], BYTE bilddaten[height][width]);
static void printArray(BYTE array[height][width]);
static void printHeaders(BITMAPFILEHEADER bitmapFileHeader, BITMAPINFOHEADER bitmapInfoHeader);
static void printFarbtabelle(RGBQUAD farbtabelle[]);

void initInput() {
	height = 0;
	width = 0;
}

/**
 * Liest Bitmap Datei ein, erstellt BITMAPFILEHEADER, BITMAPINFOHEADER,
 * eine Farbtabelle und einen Pointer auf Array, das die Bilddaten enthält.
 */
int readFile(char* dateiname) {

	FILE* x = fopen("test.bmp", "wb");
		if(x == NULL){
			fclose(x);
			return WRONGFILETYPE;
		}


	int e = EOK;
	// "testBilder/22x14_8_bit_komprimiert_mit_padding_bytes_sehr_kleines_testbild.bmp"
	// Datei öffnen
	file = fopen(dateiname, "rb");
	if(file == NULL){
		fclose(file);
		return WRONGFILETYPE;
	}

	// FileHeader einlesen
	e = readFileHeader();
	if (e != EOK) {
		return e;
	}

	// InfoHeader einlesen
	e = readInfoHeader();
	if (e != EOK) {
		return e;
	}

	// Einlesen der Farbtabelle
	if ((bitmapInfoHeader.biBitCount == 1) || (bitmapInfoHeader.biBitCount == 4) || (bitmapInfoHeader.biBitCount == 8)) {
		e = readFarbtabelle();
		if (e != EOK) {
			return e;
		}
	}

	// Einlesen der Bilddaten
	switch (bitmapInfoHeader.biCompression) {
		case BI_RGB:		e = readBilddatenUnkomprimiert();
							if (e != EOK) {
								return e;
							}
							break;
		case BI_RLE8:		e = readBilddatenKomprimiert();
							if (e != EOK) {
								return e;
							}
							break;
		default:			return UNGUELTIGE_KOMPRIMIERUNG;
	}

	if (e != EOK) {
		// TODO
	}

	// Datei schliessen
	fclose(file);

	// Ausgabe der Bilddaten
	//printf("Bilddaten (height: %d, width: %d):\n", height, width);
	//printArray(pointerBilddaten);

	// Ausgabe zum Testen
	//printHeaders(bitmapFileHeader, bitmapInfoHeader);
	//printFarbtabelle(farbtabelle);

	return 0;
}

static int readFileHeader() {
	// FileHeader einlesen
	if (1 != fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file)){
		return READERROR;
	}

	// Ueberpruefen, ob Datei eine Bitmap-Datei ist
	if (bitmapFileHeader.bfType != 0x4D42) {
		fclose(file);
		return WRONGFILETYPE;
	}
	return EOK;
}

static int readInfoHeader() {
	// InfoHeader einlesen
	if (1 != fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, file)){
		return READERROR;
	}

	if (bitmapInfoHeader.biSize != sizeof(bitmapInfoHeader)) {
		return READERROR;
	}

	// Ermittlung Hoehe und Breite des Bilddaten-Arrays
	height = bitmapInfoHeader.biHeight;
	width = bitmapInfoHeader.biWidth;

	if (width > bitmapInfoHeader.biSizeImage) {
		return READERROR;
	}

	// Länge einer Zeile ist durch 4 teilbar, daher muss width eventuell angepasst werden
	while (width % 4 != 0) {
		width++;
	}
	paddingBytesInput = width - bitmapInfoHeader.biWidth;
	if (paddingBytesInput == 0) {
		paddingBytesOutput = paddingBytesInput;
	}
	else {
		paddingBytesOutput = 4 - paddingBytesInput;
	}
//	printf("width - bitmapInfoHeader.biWidth = %d - %d\n\n", width, bitmapInfoHeader.biWidth);
//	printf("paddingBytesInput: %d\npaddingBytesOutput: %d\n\n", paddingBytesInput, paddingBytesOutput);

	return EOK;
}

static int readFarbtabelle() {
	// Einlesen der Farbtabelle
	if (1 != fread(&farbtabelle, sizeof(farbtabelle), 1, file)){
		return READERROR;
	}
	return EOK;
}

static int readBilddatenUnkomprimiert() {
	if ((height == 0) || (width == 0)) {
		return READERROR;
	}
	// Einlesen der Bilddaten
	BYTE bilddaten[height][width];
	if (1 != fread(&bilddaten, sizeof(bilddaten), 1, file)){
		return READERROR;
	}
	if (EOK != createArray(bilddaten)){
		return NULLPOINTER;
	}
	return EOK;
}

static int readBilddatenKomprimiert() {
	// TODO
	if ((height == 0) || (width == 0)) {
		return READERROR;
	}

	//width = bitmapInfoHeader.biWidth;

	// Einlesen der Bilddaten und Dekomprimierung
	BYTE bilddaten[height][width]; // könnte Probleme bereiten, wenn width ursprünglich nicht durch 4 teilbar war
	BYTE datensatz[2];
	BYTE byte;
	int zeile = 0;
	int spalte = 0;
	int bitmapVorbei = 0;
	while (!bitmapVorbei) {
	//for (int a = 0; a < bitmapInfoHeader.biSizeImage / 2; a++) {
		if (1 != fread(&datensatz, sizeof(datensatz), 1, file)) {
			return READERROR;
		}
		if (datensatz[0] != 0) {
			for (int i = 0; i < datensatz[0]; i++) {
				if (spalte > width) {
					return READERROR;
				}
				bilddaten[zeile][spalte] = datensatz[1];
				spalte++;
			}
		}
		else if (datensatz[0] == 0) {
			if (datensatz[1] > 0xFF) {
				return READERROR;
			}
			switch (datensatz[1]) {
				case 0:		zeile++;			// Ende der Bildzeile
							spalte = 0;
							break;
				case 1:		bitmapVorbei = 1;	// Ende der Bitmap
							break;
				case 2:		break;				// Verschiebung der aktuellen Pixelposition. Die beiden nächsten Bytes geben die Verschiebung nach rechts und nach unten an.
				default:	// 3 - 255		Die folgenden n (datensatz[1]) Bytes werden direkt übernommen; der nächste Datensatz findet sich am darauffolgenden geraden Offset (vom Start der Bilddaten aus gezählt).
							for (int i = 0; i < datensatz[1]; i++) {
								readByte(&byte); // TODO Fehlerbehandlung
//								printf("%X ", byte);
								if (spalte > width) {
									return READERROR;
								}
								bilddaten[zeile][spalte] = byte;
								spalte++;
							}
							if (datensatz[1] % 2 == 1) { // n ungerade
								readByte(&byte);
							}
			}
		}
	}

	createArray(bilddaten);
	return EOK;
}

//static int writeBilddaten(BYTE bilddaten[height][width], BYTE value, int zeile, int spalte) {
//	if ((zeile < 0) || (zeile >= height) || (spalte < 0) || (spalte >= width)) {
//		return READERROR;
//	}
//	bilddaten[zeile][spalte] = value;
//	return EOK;
//}

static int readByte(BYTE *byte) {
	BYTE einByte;
	if (1 != fread(&einByte, sizeof(BYTE), 1, file)) {
		return READERROR;
	}
	*byte = einByte;
	return EOK;
}

static int createArray(BYTE bilddaten[height][width]) {
   // pointer ist ein Zeiger, der auf ein Feld von Typ int zeigt
   pointerBilddaten = (BYTE(*)[]) malloc(height*width*sizeof(BYTE));

   // Fehlerbehandlung
   if (NULL == pointerBilddaten) {
	   return NULLPOINTER;
   }

   initArray(pointerBilddaten, bilddaten);

   return EOK;

   // Gebe den Speicher auf dem Heap wieder frei
   //free(pointerBilddaten);
}

static void initArray(BYTE array[height][width], BYTE bilddaten[height][width]) {
	// Initialisierung des Arrays
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			array[i][j] = bilddaten[i][j];
		}
	}

}

static void printArray(BYTE array[height][width]) {
   // drucke das Array
   for (int i = 0; i < height; ++i) {
       for (int j = 0; j < width; ++j) {
           printf("%d ", array[i][j]);
       }
       printf("\n");
   }
}

static void printHeaders(BITMAPFILEHEADER bitmapFileHeader, BITMAPINFOHEADER bitmapInfoHeader) {
	printf("\n");
	printf("bitmapFileHeader:\n");
	printf("bfType: 0x%X (%d)\n", bitmapFileHeader.bfType, bitmapFileHeader.bfType);
	printf("bfSize: 0x%X (%d)\n", bitmapFileHeader.bfSize, bitmapFileHeader.bfSize);
	printf("bfReserved1: 0x%X (%d)\n", bitmapFileHeader.bfReserved1, bitmapFileHeader.bfReserved1);
	printf("bfReserved2: 0x%X (%d)\n", bitmapFileHeader.bfReserved2, bitmapFileHeader.bfReserved2);
	printf("bfOffBits: 0x%X (%d)\n", bitmapFileHeader.bfOffBits, bitmapFileHeader.bfOffBits);
	printf("\n");
	printf("bitmapInfoHeader:\n");
	printf("biSize: 0x%X (%d)\n", bitmapInfoHeader.biSize, bitmapInfoHeader.biSize);
	printf("biWidth: 0x%X (%d)\n", bitmapInfoHeader.biWidth, bitmapInfoHeader.biWidth);
	printf("biHeight: 0x%X (%d)\n", bitmapInfoHeader.biHeight, bitmapInfoHeader.biHeight);
	printf("biPlanes: 0x%X (%d)\n", bitmapInfoHeader.biPlanes, bitmapInfoHeader.biPlanes);
	printf("biBitCount: 0x%X (%d)\n", bitmapInfoHeader.biBitCount, bitmapInfoHeader.biBitCount);
	printf("biCompression: 0x%X (%d)\n", bitmapInfoHeader.biCompression, bitmapInfoHeader.biCompression);
	printf("biSizeImage: 0x%X (%d)\n", bitmapInfoHeader.biSizeImage, bitmapInfoHeader.biSizeImage);
	printf("biXPelsPerMeter: 0x%X (%d)\n", bitmapInfoHeader.biXPelsPerMeter, bitmapInfoHeader.biXPelsPerMeter);
	printf("biYPelsPerMeter: 0x%X (%d)\n", bitmapInfoHeader.biYPelsPerMeter, bitmapInfoHeader.biYPelsPerMeter);
	printf("biClrUsed: 0x%X (%d)\n", bitmapInfoHeader.biClrUsed, bitmapInfoHeader.biClrUsed);
	printf("biClrImportant: 0x%X (%d)\n", bitmapInfoHeader.biClrImportant, bitmapInfoHeader.biClrImportant);
	printf("\n");
}

static void printFarbtabelle(RGBQUAD farbtabelle[]) {
	for (int i = 0; i < 256; i++) {
		printf("%X: %X %X %X\n", i, farbtabelle[i].rgbtRed, farbtabelle[i].rgbtGreen, farbtabelle[i].rgbtBlue);
	}
}


