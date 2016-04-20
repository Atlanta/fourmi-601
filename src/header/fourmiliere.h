#ifndef __FOURMILIERE_H__
#define __FOURMILIERE_H__

#include "fourmi.h"

typedef struct fourmiliere {
	int id;
	fourmi_t** fourmis;
	int nbTypes;
	type_fourmi_t* types;		/* Les types de fourmi à créer */
	float* proba;				/* Les probabilités de créer chaque type (dans le même ordre que les types auxquelles elles correspondent) */
} fourmiliere_t;

void push_back(fourmi_t* fourmi);
void genererFourmi();

#endif
