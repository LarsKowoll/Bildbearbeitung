/*
 * output.c
 *
 *  Created on: 17.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "main.h"
#include "output.h"

static FILE *file;
static FILE *txt;
static BITMAPFILEHEADER bitmapFileHeader;
static BITMAPINFOHEADER bitmapInfoHeader;

static int setFileHeader();
static int setInfoHeader();
static int writeFileHeader();
static int writeInfoHeader();
static int writeBilddaten(BYTE bilddaten[height][width]);
static void changeFileHeader();
static void changeInfoHeader();

int writeFile() {
	// Datei öffnen
    char* str1 = ".\\neueBilder\\" ;
    char str2[10];
    sprintf(str2, "%d", bildNummer);

    char* str3 = ("-Bild.bmp");

    char buf[256];
    snprintf(buf, sizeof buf, "%s%s%s", str1, str2, str3);



	file = fopen(buf, "w+");
	//txt = fopen("neueBilder/bilddaten.txt", "w+");
	if(file == NULL){
		fclose(file);
		// TODO: return Fehler;
	}

	// FileHeader in Datei schreiben
	setFileHeader();
//	changeFileHeader();
	writeFileHeader();

	// InfoHeader in Datei schreiben
	setInfoHeader();
//	changeInfoHeader();
	writeInfoHeader();

	// Bilddaten in Datei schreiben
	writeBilddaten(pointerBilddaten);

	//writeBilddatenToTXT(pointerBilddaten);

	fclose(file);
	fclose(txt);

	return EOK;
}

static int setFileHeader() {
	bitmapFileHeader.bfType = 0x4D42;
	bitmapFileHeader.bfSize = 54 + height * width * 3;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits = 54;
	return EOK;
}

static int setInfoHeader() {
	bitmapInfoHeader.biSize = 40; // Größe der BITMAPINFOHEADER-Struktur in Byte
	bitmapInfoHeader.biWidth = width - paddingBytesOutput; // Breite der Bitmap in Pixel
	bitmapInfoHeader.biHeight = height; // Höhe der Bitmap in Pixel
	bitmapInfoHeader.biPlanes = 1; // festgelegter Wert
	bitmapInfoHeader.biBitCount = 24; // Farbtiefe der Bitmap in bpp
	bitmapInfoHeader.biCompression = 0; // Komprimierungsart
	bitmapInfoHeader.biSizeImage = height * (width - paddingBytesOutput) * 3 + height * paddingBytesOutput; // Größe der Bilddaten in Byte
	bitmapInfoHeader.biXPelsPerMeter = 23622; // TODO Horizontale Auflösung des Zielausgabegerätes in Pixel pro Meter
	bitmapInfoHeader.biYPelsPerMeter = 23622; // TODO Vertikale Auflösung des Zielausgabegerätes in Pixel pro Meter
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	return EOK;
}

static int writeFileHeader() {
	fwrite(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), file); // TODO: Fehlerbehandlung fwrite
	return EOK;
}

static int writeInfoHeader() {
	fwrite(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), file); // TODO: Fehlerbehandlung fwrite
	return EOK;
}

int writeBilddatenToTXT(BYTE bilddaten[height][width]) {
	BYTE byte;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			byte = bilddaten[i][j];
			fwrite(&byte, 1, sizeof(BYTE), txt); // TODO: Fehlerbehandlung fwrite
		}
	}

	return EOK;
}

static int writeBilddaten(BYTE bilddaten[height][width]) {
	RGBTRIPLE farbeTriple;
	RGBQUAD farbeQuad;
	BYTE padding = 0; // padding byte
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j >= width - paddingBytesOutput) {
				fwrite(&padding, 1, sizeof(BYTE), file); // TODO: Fehlerbehandlung fwrite
			}
			else {
				farbeQuad = farbtabelle[bilddaten[i][j]];
				farbeTriple.rgbtRed = farbeQuad.rgbtRed;
				farbeTriple.rgbtGreen = farbeQuad.rgbtGreen;
				farbeTriple.rgbtBlue = farbeQuad.rgbtBlue;
				fwrite(&farbeTriple, 1, sizeof(RGBTRIPLE), file); // TODO: Fehlerbehandlung fwrite
			}
		}
	}
	return EOK;
}

static void changeFileHeader() {
	// TODO: bfSize aendern
	bitmapFileHeader.bfOffBits = bitmapFileHeader.bfOffBits - 1024;
}

static void changeInfoHeader() {
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biHeight * bitmapInfoHeader.biWidth * 3;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
}
