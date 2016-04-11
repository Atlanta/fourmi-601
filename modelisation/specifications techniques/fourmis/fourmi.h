/* Fourmis
 Structures et fonctions: */

#define SOLDATE 1
#define OUVRIERE 2


typedef struct fourmi{
    /* coord_t = struct contenant les positons X Y */
    coord_t* position_fourmiliere;
    /* Permet à la fourmi de savoir d'où elle vient */
    int id_zone;
    /* Permet à la fourmi de savoir de quelle fourmilière elle vient
       Si elle rencontre une fourmi d'une autre fourmilère, elle peut l'affronter */
    int id_fourmilière;
    int pv_max;
    int pv_actuel
    int capacite_stockage
    int pts_attaque;
    int type; /* SOLDATE || OUVRIERE */
}fourmi_t;
/* Crée une fourmis selon le fichier de configuration
 fd : descripteur de fichier */
fourmi_t* creerFourmi(int fd);

/* Stats ouvriere (exemple):
 pv_max = 30;
 capacite_stockage = 20;
 pts_attaque = 5;

 Stats soldate :
 pv_max = 50;
 capacite_stockage = 0;
 pts_attaque = 10; */


/* Fonction de déplacement d'une ouvriere
 fonction appelée au lancement d'un thread
 args = coordonnées de la fourmi
 L'algo est décrit dans le fichier "algo_ouvriere" */
void* deplacementOuvriere(void* args);

/* Fonction de déplacement d'une soldate
 fonction appelée au lancement d'un thread
 args = coordonnées de la fourmi
 L'algo est décrit dans le fichier "algo_soldate"  */
void* deplacementSoldate(void* args);


/* Fonction permettant à la fourmi de consommer de la nourriture pour regenerer des PV
 nourriture_t : structure nourriture */
void manger(nourriture_t* nourriture);

/* Fonction permettant à une fourmi de transporter de la nourriture
 La fourmi charge un maximum de nourriture (jusqu'à ce qu'il n'y ait plus de nourriture ou qu'elle ne puisse plus en transporter plus) */
void chargerNourriture(nourriture_t* nourriture);

/* Attaque une bibite
 Reviens à un simple jet de dés en fonction de l'attaque de la fourmi
 ennemi = fourmi étrangère ou bibitte
 On définit la nature de l'ennemi en fonction de la taille de structure passée en paramètre */
void attaquer(void* ennemi);

/* Transforme une fourmi en source de nourriture */
void mourrir(fourmi_t* fourmi);

/* Detruit la fourmi (ne génère pas de nourriture)
 Utilisé lors du passage d'une fourmi d'une zone A à une zone B
 La fourmi ne meurt pas, la zone arrête juste de la gérer */
void detruire(fourmi_t* fourmi);

/* La fourmi se met en attente d'un signal
   Lorsque ce signal est recu, la fourmi meurt
   (destruction de la fourmi + fin du thread)
   Le signal à attenre dépends la zone d'origine de la fourmi (numero du signal à définir) */
void gestionnaireMort(int sig);
