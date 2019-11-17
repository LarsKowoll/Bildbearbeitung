/*
 * main.h
 *
 *  Created on: 07.11.2018
 *      Author: Lars Kowoll, Philip Zirfasshilip
 */

#include "definitions.h"

#ifndef MAIN_H_
#define MAIN_H_

int start();

RGBQUAD farbtabelle[256];
BYTE (*pointerBilddaten)[]; // Zeiger auf das Array, das die Bilddaten enthält
unsigned int height;
unsigned int width;
int paddingBytesInput;
int paddingBytesOutput;
int bildNummer;

#endif /* MAIN_H_ */
