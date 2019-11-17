/*
 * bildverarbeitung.c
 *
 *  Created on: 17.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "main.h"
#include "bildverarbeitung.h"
#include "farben.h"

static RECHTECK blauerRahmen;
static RECHTECK rechtecke[10] = {0};
static RECHTECK andereObjekte[10] = {0};

static int getKoordinatenBlauerRahmen();
static int findeRechteck(BYTE bilddaten[height][width], int welcheFarbe, int rechteckNummer);
static int istTeilEinesRechtecks(int x, int y);
static int istRechteck(BYTE bilddaten[height][width], RECHTECK rechteck, int welcheFarbe);
static int istRechteck2(BYTE bilddaten[height][width], RECHTECK rechteck, int welcheFarbe);

int initBildverarbeitung() {
	int e;
	e = initFarben();
	if (e != EOK) {
		return e;
	}

	blauerRahmen.x_anfang = 0;
	blauerRahmen.x_ende = 0;
	blauerRahmen.y_anfang = 0;
	blauerRahmen.y_ende = 0;
	blauerRahmen.x_laenge = 0;
	blauerRahmen.y_laenge = 0;
	blauerRahmen.groesse = 0;

	int lengthRechtecke = sizeof(rechtecke) / sizeof(rechtecke[0]);
	for (int i = 0; i < lengthRechtecke; i++) {
		rechtecke[i].x_anfang = 0;
		rechtecke[i].x_ende = 0;
		rechtecke[i].y_anfang = 0;
		rechtecke[i].y_ende = 0;
		rechtecke[i].x_laenge = 0;
		rechtecke[i].y_laenge = 0;
		rechtecke[i].groesse = 0;
		andereObjekte[i].x_anfang = 0;
		andereObjekte[i].x_ende = 0;
		andereObjekte[i].y_anfang = 0;
		andereObjekte[i].y_ende = 0;
		andereObjekte[i].x_laenge = 0;
		andereObjekte[i].y_laenge = 0;
		andereObjekte[i].groesse = 0;
	}

	return EOK;
}

int zeichneBlauenRahmen(BYTE bilddaten[height][width]) {
	BYTE blau;
	int e;
	e = getBlau(&blau);
	if (e == FARBE_NICHT_GEFUNDEN) {
		return e;
	}
	e = getKoordinatenBlauerRahmen();
	if (e == KEIN_RECHTECK_GEFUNDEN) {
		return e;
	}

	// Überprüfung aller Bilddaten auf Lage auf blauem Rahmen
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Linien in x-Richtung
			if (((i == blauerRahmen.x_anfang) && (j >= blauerRahmen.y_anfang) && (j <= blauerRahmen.y_ende))
					|| ((i == blauerRahmen.x_ende) && (j >= blauerRahmen.y_anfang) && (j <= blauerRahmen.y_ende))) {
				bilddaten[i][j] = blau;
			}
			// Linien in y-Richtung
			if (((j == blauerRahmen.y_anfang) && (i >= blauerRahmen.x_anfang) && (i <= blauerRahmen.x_ende))
					|| ((j == blauerRahmen.y_ende) && (i >= blauerRahmen.x_anfang) && (i <= blauerRahmen.x_ende))) {
				bilddaten[i][j] = blau;
			}
		}
	}

	return EOK;
}

static int getKoordinatenBlauerRahmen() {
	blauerRahmen.x_anfang = MAX_INT;
	blauerRahmen.x_ende = 0;
	blauerRahmen.y_anfang = MAX_INT;
	blauerRahmen.y_ende = 0;

	int lengthRechtecke = sizeof(rechtecke) / sizeof(rechtecke[0]);

	for (int i = 0; i < lengthRechtecke; i++) {
		if (rechtecke[i].groesse == 0) {
			continue;
		}
		if (rechtecke[i].x_anfang < blauerRahmen.x_anfang) {
			blauerRahmen.x_anfang = rechtecke[i].x_anfang;
		}

		if (rechtecke[i].x_ende > blauerRahmen.x_ende) {
			blauerRahmen.x_ende = rechtecke[i].x_ende;
		}
		if (rechtecke[i].y_anfang < blauerRahmen.y_anfang) {
			blauerRahmen.y_anfang = rechtecke[i].y_anfang;
		}
		if (rechtecke[i].y_ende > blauerRahmen.y_ende) {
			blauerRahmen.y_ende = rechtecke[i].y_ende;
		}
	}

	if ((blauerRahmen.x_anfang == 0) && (blauerRahmen.x_ende == 0) && (blauerRahmen.y_anfang == 0) && (blauerRahmen.y_ende == 0)) {
		return KEIN_RECHTECK_GEFUNDEN;
	}

	return EOK;
}

static int istTeilEinesRechtecks(int x, int y) {
	int lengthRechtecke = sizeof(rechtecke) / sizeof(rechtecke[0]);
	int lengthAndereObjekte = sizeof(andereObjekte) / sizeof(andereObjekte[0]);

	for (int i = 0; i < lengthRechtecke; i++) {
		if (rechtecke[i].groesse == 0) {
			continue;
		}
		if ((rechtecke[i].x_anfang <= x) && (x <= rechtecke[i].x_ende) && (rechtecke[i].y_anfang <= y) && (y <= rechtecke[i].y_ende)) {
			return 1;
		}
	}

	for (int i = 0; i < lengthAndereObjekte; i++) {
		if (andereObjekte[i].groesse == 0) {
			continue;
		}
//		printf("Anderes Objekt gefunden (%d):\nAnfang: %d, %d\nEnde: %d, %d\nAbmessungen: %d x %d Pixel\nGroesse: %d Pixel\n\n", i,
//				andereObjekte[i].y_anfang, andereObjekte[i].x_anfang, andereObjekte[i].y_ende,
//				andereObjekte[i].x_ende, andereObjekte[i].y_laenge, andereObjekte[i].x_laenge,
//				andereObjekte[i].groesse);
		if ((andereObjekte[i].x_anfang <= x) && (x <= andereObjekte[i].x_ende) && (andereObjekte[i].y_anfang <= y) && (y <= andereObjekte[i].y_ende)) {
			return 1;
		}
	}
	return 0;
}

static int istRechteck(BYTE bilddaten[height][width], RECHTECK rechteck, int welcheFarbe) {
	RGBQUAD farbe;

	for (int x = rechteck.x_anfang; x <= rechteck.x_ende; x++) {
		for (int y = rechteck.y_anfang; y <= rechteck.y_ende; y++) {
			farbe = farbtabelle[bilddaten[x][y]];
			if (istFarbe(farbe, welcheFarbe) == 0) {
				return KEIN_RECHTECK;
			}
		}
	}

	return EOK;
}

/**
 * Überprüft an allen vier Kanten des Rechtecks,
 * ob alle Pixel dieselbe Farbe haben. Es wird immer
 * ein Pixel mit dem vorhergehenden Pixel verglichen.
 */
static int istRechteck2(BYTE bilddaten[height][width], RECHTECK rechteck, int welcheFarbe) {
	RGBQUAD farbe1, farbe2;

	// x-Richtung (y_anfang)
	farbe1 = farbtabelle[bilddaten[rechteck.x_anfang][rechteck.y_anfang]];
	for (int x = rechteck.x_anfang; x <= rechteck.x_ende; x++) {
		farbe2 = farbtabelle[bilddaten[x][rechteck.y_anfang]];
		if (compareFarben(farbe1, farbe2) == 0) {
			return KEIN_RECHTECK;
		}
		farbe1 = farbtabelle[bilddaten[x][rechteck.y_anfang]];
	}

	// x-Richtung (y_ende)
	farbe1 = farbtabelle[bilddaten[rechteck.x_anfang][rechteck.y_ende]];
	for (int x = rechteck.x_anfang; x <= rechteck.x_ende; x++) {
		farbe2 = farbtabelle[bilddaten[x][rechteck.y_ende]];
		if (compareFarben(farbe1, farbe2) == 0) {
			return KEIN_RECHTECK;
		}
		farbe1 = farbtabelle[bilddaten[x][rechteck.y_ende]];
	}

	// y-Richtung (x_anfang)
	farbe1 = farbtabelle[bilddaten[rechteck.x_anfang][rechteck.y_anfang]];
	for (int y = rechteck.y_anfang; y <= rechteck.y_ende; y++) {
		farbe2 = farbtabelle[bilddaten[rechteck.x_anfang][y]];
		if (compareFarben(farbe1, farbe2) == 0) {
			return KEIN_RECHTECK;
		}
		farbe1 = farbtabelle[bilddaten[rechteck.x_anfang][y]];
	}

	// y-Richtung (x_ende)
	farbe1 = farbtabelle[bilddaten[rechteck.x_ende][rechteck.y_anfang]];
	for (int y = rechteck.y_anfang; y <= rechteck.y_ende; y++) {
		farbe2 = farbtabelle[bilddaten[rechteck.x_ende][y]];
		if (compareFarben(farbe1, farbe2) == 0) {
			return KEIN_RECHTECK;
		}
		farbe1 = farbtabelle[bilddaten[rechteck.x_ende][y]];
	}

	return EOK;
}

int findeRechtecke() {
	int farbe;
	int e;

	int lengthRechtecke = sizeof(rechtecke) / sizeof(rechtecke[0]);

	farbe = ROT;
	for (int i = 0; i < lengthRechtecke / 2; i++) {
		e = findeRechteck(pointerBilddaten, farbe, i);
	}
	farbe = GRUEN;
	for (int i = lengthRechtecke / 2; i < lengthRechtecke; i++) {
		e = findeRechteck(pointerBilddaten, farbe, i);
	}

	return EOK;
}

static int findeRechteck(BYTE bilddaten[height][width], int welcheFarbe, int rechteckNummer) {
	RECHTECK rechteck;
	RGBQUAD farbe;
	int anfangGefunden = 0;

	// Anfang des Rechtecks
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width - paddingBytesInput; j++) {
			farbe = farbtabelle[bilddaten[i][j]];
//			if ((i < 2) && (j < 2)) {
//				printf("%d, %d: istFarbe = %d\nistTeilEinesRechtecks = %d\n\n", i, j, istFarbe(farbe, welcheFarbe), istTeilEinesRechtecks(i, j));
//			}
			if (istFarbe(farbe, welcheFarbe) == 1) {
				if ((anfangGefunden == 0) && (istTeilEinesRechtecks(i, j) == 0)) {
					rechteck.x_anfang = i;
					rechteck.y_anfang = j;
					anfangGefunden = 1;
				}
			}
		}
	}

	if (anfangGefunden == 0) {
		return KEIN_RECHTECK_GEFUNDEN;
	}

	// Ende des Rechtecks (x-Richtung)
	for (int x = rechteck.x_anfang; x < height; x++) {
		farbe = farbtabelle[bilddaten[x][rechteck.y_anfang]];
		if (istFarbe(farbe, welcheFarbe) == 1) {
			rechteck.x_ende = x;
		}
		else { // andere Farbe
			break;
		}
	}
	// Ende des Rechtecks (y-Richtung)
	for (int y = rechteck.y_anfang; y < width - paddingBytesInput; y++) {
		farbe = farbtabelle[bilddaten[rechteck.x_anfang][y]];
		if (istFarbe(farbe, welcheFarbe) == 1) {
			rechteck.y_ende = y;
		}
		else { // andere Farbe
			break;
		}
	}

	rechteck.x_laenge = rechteck.x_ende - rechteck.x_anfang + 1;
	rechteck.y_laenge = rechteck.y_ende - rechteck.y_anfang + 1;
	rechteck.groesse = rechteck.x_laenge * rechteck.y_laenge;

	// Überprüfung, ob es sich wirklich um ein Rechteck handelt
	int e;
	e = istRechteck(pointerBilddaten, rechteck, welcheFarbe);
	if (e == KEIN_RECHTECK) {
		andereObjekte[rechteckNummer] = rechteck;
//		printf("Anderes Objekt gefunden: (Farbe: %d)\nAnfang: %d, %d\nEnde: %d, %d\nAbmessungen: %d x %d Pixel\nGroesse: %d Pixel\n\n", welcheFarbe,
//				andereObjekte[rechteckNummer].y_anfang, andereObjekte[rechteckNummer].x_anfang, andereObjekte[rechteckNummer].y_ende,
//				andereObjekte[rechteckNummer].x_ende, andereObjekte[rechteckNummer].y_laenge, andereObjekte[rechteckNummer].x_laenge,
//				andereObjekte[rechteckNummer].groesse);
		return KEIN_RECHTECK_GEFUNDEN;
	}



	// Umrechnung von x_anfang und x_ende, da das Bild horizontal gespiegelt eingelesen wird
//	rotesRechteck.x_ende = height - rotesRechteck.x_anfang;
//	rotesRechteck.x_anfang = height - rotesRechteck.x_anfang - rotesRechteck.x_laenge;

	if (rechteck.groesse < 100) {
		return KEIN_RECHTECK_GEFUNDEN;
	}

	rechtecke[rechteckNummer] = rechteck;
//	printf("Rechteck gefunden: (Farbe: %d)\nAnfang: %d, %d\nEnde: %d, %d\nAbmessungen: %d x %d Pixel\nGroesse: %d Pixel\n\n", welcheFarbe,
//			rechtecke[rechteckNummer].y_anfang, rechtecke[rechteckNummer].x_anfang, rechtecke[rechteckNummer].y_ende,
//			rechtecke[rechteckNummer].x_ende, rechtecke[rechteckNummer].y_laenge, rechtecke[rechteckNummer].x_laenge,
//			rechtecke[rechteckNummer].groesse);

	return EOK;
}
