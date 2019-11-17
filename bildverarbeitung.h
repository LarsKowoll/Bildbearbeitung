/*
 * bildverarbeitung.h
 *
 *  Created on: 17.11.2018
 *      Author: Lars Kowoll, Philip Zirfass
 */

#ifndef BILDVERARBEITUNG_H_
#define BILDVERARBEITUNG_H_

int initBildverarbeitung();
int zeichneBlauenRahmen(BYTE bilddaten[height][width]);
int findeRechtecke();

#endif /* BILDVERARBEITUNG_H_ */
