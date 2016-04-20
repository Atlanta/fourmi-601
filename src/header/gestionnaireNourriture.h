#ifndef __GESTIONNAIRE_NOURRITURE_H__
#define __GESTIONNAIRE_NOURRITURE_H__

typedef struct nourriture {
	unsigned short quantite;
} nourriture_t;

typedef struct gestionnaire_nourriture {
	unsigned short delay;
	unsigned char nbSources;
	unsigned short qteNourritureParSource;
} gestionnaire_nourriture_t;

void* routineGestionnaireNourriture(void* args);

#endif
