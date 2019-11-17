/*
 * farben.c
 *
 *  Created on: 17.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#include "definitions.h"
#include "main.h"
#include "farben.h"



//static int getFarbnummer(RGBQUAD farbe, unsigned int *farbnummer);
static int getRot(BYTE *farbnummer);
static int getGruen(BYTE *farbnummer);

int initFarben() {
	int e;
	e = getRot(&rot);
	if (e != EOK) {
		return e;
	}
	e = getGruen(&gruen);
	if (e != EOK) {
		return e;
	}
	e = getBlau(&blau);
	if (e != EOK) {
		return e;
	}
	e = getWeiss(&weiss);
	return EOK;
}

int istFarbe(RGBQUAD farbe, int welcheFarbe) {
	switch (welcheFarbe) {
		case ROT: return istRot(farbe);
		case GRUEN: return istGruen(farbe);
		case HELLGRUEN: return istHellgruen(farbe);
		default: return UNGUELTIGE_FARBE;
	}
}

int istRot(RGBQUAD farbe) {
	if ((farbe.rgbtRed > 200) && (farbe.rgbtGreen < 100) && (farbe.rgbtBlue < 100)) {
		return 1;
	}
	else {
		return EOK;
	}
}

int istGruen(RGBQUAD farbe) {
	if ((farbe.rgbtRed < 100) && (farbe.rgbtGreen > 200) && (farbe.rgbtBlue < 100)) {
		return 1;
	}
	else {
		return EOK;
	}
}

int istHellgruen(RGBQUAD farbe) {
	if ((farbe.rgbtRed == 0xA0) && (farbe.rgbtGreen == 0xE0) && (farbe.rgbtBlue == 0x80)) {
		return 1;
	}
	else {
		return EOK;
	}
}

int istWeiss(RGBQUAD farbe) {
	if ((farbe.rgbtRed == 0xFF) && (farbe.rgbtGreen == 0xFF) && (farbe.rgbtBlue == 0xFF)) {
		return 1;
	}
	else {
		return EOK;
	}
}

//static int getFarbnummer(RGBQUAD farbe, unsigned int *farbnummer) {
//	RGBQUAD vergleichsFarbe;
//	int anzahlFarben = sizeof(farbtabelle) / sizeof(farbtabelle[0]);
//	for (int i = 0; i < anzahlFarben; i++) {
//		vergleichsFarbe = farbtabelle[i];
//		if (compareFarben(farbe, vergleichsFarbe) == 1) {
//			*farbnummer = i;
//			return EOK;
//		}
//	}
//	return FARBE_NICHT_GEFUNDEN;
//}

int compareFarben(RGBQUAD farbe1, RGBQUAD farbe2) {
	if (farbe1.rgbtRed != farbe2.rgbtRed) {
		return EOK; // TODO
	}
	if (farbe1.rgbtGreen != farbe2.rgbtGreen) {
		return EOK;
	}
	if (farbe1.rgbtBlue != farbe2.rgbtBlue) {
		return EOK;
	}
	return 1;
}

static int getRot(BYTE *farbnummer) {
	RGBQUAD farbe;
	int anzahlFarben = sizeof(farbtabelle) / sizeof(farbtabelle[0]);
	for (int i = 0; i < anzahlFarben; i++) {
		farbe = farbtabelle[i];
		if ((farbe.rgbtRed == 0xFF) && (farbe.rgbtGreen == 0) && (farbe.rgbtBlue == 0)) {
			*farbnummer = i;
			return EOK;
		}
	}
	return FARBE_NICHT_GEFUNDEN;
}

static int getGruen(BYTE *farbnummer) {
	RGBQUAD farbe;
	int anzahlFarben = sizeof(farbtabelle) / sizeof(farbtabelle[0]);
	for (int i = 0; i < anzahlFarben; i++) {
		farbe = farbtabelle[i];
		if ((farbe.rgbtRed == 0) && (farbe.rgbtGreen == 0xFF) && (farbe.rgbtBlue == 0)) {
			*farbnummer = i;
			return EOK;
		}
	}
	return FARBE_NICHT_GEFUNDEN;
}

int getWeiss(BYTE *farbnummer) {
	RGBQUAD farbe;
	int anzahlFarben = sizeof(farbtabelle) / sizeof(farbtabelle[0]);
	for (int i = 0; i < anzahlFarben; i++) {
		farbe = farbtabelle[i];
		if ((farbe.rgbtRed == 0xFF) && (farbe.rgbtGreen == 0xFF) && (farbe.rgbtBlue == 0xFF)) {
			*farbnummer = i;
			return EOK;
		}
	}
	return FARBE_NICHT_GEFUNDEN;
}

int getBlau(BYTE *farbnummer) {
	RGBQUAD farbe;
	int anzahlFarben = sizeof(farbtabelle) / sizeof(farbtabelle[0]);
	for (int i = 0; i < anzahlFarben; i++) {
		farbe = farbtabelle[i];
		if ((farbe.rgbtRed < 100) && (farbe.rgbtGreen < 100) && (farbe.rgbtBlue > 200)) {
			*farbnummer = i;
			return EOK;
		}
	}
	farbtabelle[255].rgbtRed = 0;
	farbtabelle[255].rgbtGreen = 0;
	farbtabelle[255].rgbtBlue = 0xFF;
	*farbnummer = 255;
	return EOK;
}
