/*
 * definitions.h
 *
 *  Created on: 16.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#include <stdint.h>
#include <stdlib.h>


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

typedef int8_t		CHAR;
typedef int16_t		SHORT;
typedef int32_t		LONG;
typedef uint32_t	DWORD;
typedef int32_t		BOOL;
typedef uint8_t		BYTE;
typedef uint16_t	WORD;

// Bitmap Headers
#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER {
	WORD bfType;  //specifies the file type, must be BM (0x4D42)
	DWORD bfSize;  //specifies the size in bytes of the bitmap file
	WORD bfReserved1;  //reserved; must be 0
	WORD bfReserved2;  //reserved; must be 0
	DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //species height in pixels
    WORD biPlanes; //specifies the number of color planes, must be 1
    WORD biBitCount; //specifies the number of bit per pixel
    DWORD biCompression;//specifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by the bitmap
    DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)

// Farben
#pragma pack(push,1)
typedef struct tagRGBTRIPLE {
	unsigned char rgbtBlue;
	unsigned char rgbtGreen;
	unsigned char rgbtRed;
} RGBTRIPLE;
#pragma pack(push,1)

#pragma pack(push,1)
typedef struct tagRGBQUAD {
	unsigned char rgbtBlue;
	unsigned char rgbtGreen;
	unsigned char rgbtRed;
	unsigned char rgbtReserved;
} RGBQUAD;
#pragma pack(push,1)

// Rechtecke
typedef struct tagRECHTECK {
	int x_anfang;
	int x_ende;
	int x_laenge;
	int y_anfang;
	int y_ende;
	int y_laenge;
	int groesse;
} RECHTECK;

#define MAX_INT		2147483647

// Farben
#define ROT			0
#define GRUEN		1
#define HELLGRUEN	2

// Kompressionsarten
#define BI_RGB			0
#define BI_RLE8			1


//___________________Fehlerfaelle
#define EOK								0
#define WRONGFILETYPE					-1
#define READERROR 						-2
#define FARBE_NICHT_GEFUNDEN			-3
#define UNGUELTIGE_KOMPRIMIERUNG		-4
#define NULLPOINTER 					-5
#define ROTES_RECHTECK_NICHT_GEFUNDEN	-6
#define GRUENES_RECHTECK_NICHT_GEFUNDEN	-7
#define KEIN_RECHTECK					-8
#define UNGUELTIGE_FARBE				-9
#define KEIN_RECHTECK_GEFUNDEN			-10


#endif /* DEFINITIONS_H_ */
