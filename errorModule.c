/*
 * errorModule.c
 *
 *  Created on: 17.11.2018
 *      Author: larsk
 */


#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"
#include "errorModule.h"
#include "main.h"

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

void errorHandling(int errorNumber){
	free(pointerBilddaten);
	switch(errorNumber){
		case EOK:
			break;
		case WRONGFILETYPE:
			printf("Keine Bitmap-Datei\n");
			break;
		case READERROR:
			printf("Fehler beim Einlesen\n");
			break;
		case FARBE_NICHT_GEFUNDEN:
			printf("Farbe nicht gefunden\n");
			break;
		case UNGUELTIGE_KOMPRIMIERUNG:
			printf("Ungueltige Komprimierung\n");
			break;
		case NULLPOINTER:
			printf("NullPointerException\n");
			break;
		case ROTES_RECHTECK_NICHT_GEFUNDEN:
			printf("Kein rotes Rechteck gefunden\n");
			break;
		case GRUENES_RECHTECK_NICHT_GEFUNDEN:
			printf("Kein gruenes Rechteck gefunden\n");
			break;
		case KEIN_RECHTECK:
			printf("Kein Rechteck\n");
			break;
		case UNGUELTIGE_FARBE:
			printf("Ungueltige Farbe\n");
			break;
		case KEIN_RECHTECK_GEFUNDEN:
			printf("Kein Rechteck gefunden\n");
			break;
		default:
			printf("Unbekannter Fehler\n");
			break;
	}
	printf("\n");
}
