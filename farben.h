/*
 * farben.h
 *
 *  Created on: 17.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#ifndef FARBEN_H_
#define FARBEN_H_

BYTE rot;
BYTE gruen;
BYTE blau;
BYTE weiss;

int initFarben();
int istFarbe(RGBQUAD farbe, int welcheFarbe);
int istRot(RGBQUAD farbe);
int istGruen(RGBQUAD farbe);
int istWeiss(RGBQUAD farbe);
int getBlau(BYTE *farbnummer);
int getWeiss(BYTE *farbnummer);
int compareFarben(RGBQUAD farbe1, RGBQUAD farbe2);

#endif /* FARBEN_H_ */
