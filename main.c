/*
 * main.c
 *
 *  Created on: 07.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "main.h"
#include "input.h"
#include "output.h"
#include "bildverarbeitung.h"
#include "errorModule.h"


int main(int argc, char* argv[]) {

	for (int i = 1; i < 18; i++) {
		bildNummer = i;

		// Init
		height = 0;
		width = 0;
		paddingBytesInput = 0;
		paddingBytesOutput = 0;
		initInput();
		pointerBilddaten = (BYTE(*)[]) malloc(height*width*sizeof(BYTE));
		int e = EOK;

		printf("Dateipfad: %s\n",argv[i]);
		e = readFile(argv[i]);
		if (e != EOK) {
			errorHandling(e);
			continue;
		}
		e = initBildverarbeitung();
		if (e != EOK) {
			errorHandling(e);
			continue;
		}

		findeRechtecke();
		e = zeichneBlauenRahmen(pointerBilddaten);
		if (e != EOK) {
			errorHandling(e);
			continue;
		}
		e = writeFile();
		if (e != EOK) {
			errorHandling(e);
			continue;
		}

		free(pointerBilddaten);

	}

	return EXIT_SUCCESS;
}


