#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#define HAUTEUR 25
#define LARGEUR 45
#define BRIGHT_PURPLE  "\033[1;35m"
#define BRIHT_YELLOW "\033[93m"
#define BRIGHT_RED "\033[91m"
#define ORANGE "\033[38;5;208m"
#define ROSE "\033[38;5;205m"
#define YELLOW      "\033[0;33m"
#define BLEU_FONCE "\033[34m"
#define LIGHTGRAY   "\033[0;37m"
#define CYAN        "\033[0;36m"
#define VERT        "\033[0;32m"
#define VIOLET "\033[95m"
#define BLANC "\033[37m"
#define NOIR "\033[30m"
#define APRICOT "\033[38;2;251;206;177m"

#define RESET       "\033[0m"


char grille[44][24];



void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void text_color(const char* color) {
    printf("%s", color);
    fflush(stdout);
}
// cache et affichage du curseur
void hide_cursor(){
    printf("\033[?25l");
    fflush(stdout);
}
void show_cursor(){
    printf("\033[?25h");
    fflush(stdout);
}
//effacage du terminal et remise du curseur
void clrscr() {
    printf("\033[2J");
    printf("\033[H");
    fflush(stdout);
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Sauvegarde des paramètres du terminal
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // mode non canonique et pas d'écho
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Passage en non-bloquant
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // Restaure les paramètres
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin); // remet le caractère dans le flux
        return 1;          // une touche a été pressée
    }

    return 0;              // aucune touche pressée
}

int getch(void) {
    struct termios oldt, newt;
    int ch;

    // Sauvegarde des paramètres actuels du terminal
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // mode non canonique + pas d'écho
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar(); // lit un caractère

    // Restaure les paramètres du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}


///////////////////////////PROGRAMME DE JEU////////////////////////////

typedef struct {
    int niveau;
    int vie;
    int nb_coups;
    int objets_A;
    int objets_B;
    int objets_C;
    int objets_P;
    int objets_F;
    char contrat;
} Avancement;

void sauvegarderAvancement(const Avancement *avancement, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "wb"); // Ouvre en écriture binaire
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier pour la sauvegarde");
        return;
    }
    fwrite(avancement, sizeof(Avancement), 1, fichier); // Écrit la structure dans le fichier
    fclose(fichier);
}


void chargerAvancement(Avancement *avancement, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rb"); // Ouvre en lecture binaire
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier pour le chargement");
        return;
    }
    fread(avancement, sizeof(Avancement), 1, fichier); // Lit la structure depuis le fichier
    fclose(fichier);
}

void reprendrePartie() {
    Avancement avancement;
    chargerAvancement(&avancement, "sauvegarde.dat");
    // Utilise les données chargées pour initialiser ton jeu
    jeu(avancement.niveau, avancement.vie, avancement.nb_coups, avancement.objets_A, avancement.objets_B, avancement.objets_C, avancement.objets_P, avancement.objets_F);
}

char fonctionNomJoueur(){
    char saisieJoueur[100];
    fgets(saisieJoueur, 100, stdin);
    return saisieJoueur[100];
}

void ReglesJeu(){
}

void coups(int nb_coups){
    gotoxy(65, 9);
    printf("Vous avez encore : %d coup(s)", nb_coups);
}

void fonctionBonusMalus(){
    char bonus = 'B', malus = 'M';

        int xB = rand()%43+2;
        int yB = rand()%23+2;
        int xM = rand()%43+2;
        int yM = rand()%23+2;
        gotoxy(xB, yB);
        text_color(CYAN);
        printf("%c", bonus);
        gotoxy(xM, yM);
        text_color(BLEU_FONCE);
        printf("%c", malus);
}

void detection_lettre(){


}

void stockageTableau(int x, int y, char lettreActuelle){
		grille[y][x] = lettreActuelle;

}


void contrat(){
    gotoxy(65, 11);
    printf("Votre contrat est le suivant : Eliminez 12 Bananes, 16 Fraises, 4 Pommes, 10 clémentines et 4 Abricots");
}


void afficherCadre() {
    gotoxy(0,0);
    text_color(LIGHTGRAY);
    for (int y = 0; y <= HAUTEUR+1; y++) {
        for (int x = 0; x <= LARGEUR+1; x++) {
            if (y == 0 || y == HAUTEUR+1) printf("#");
            else if (x == 0 || x == LARGEUR+1) printf("#");
            else printf(" ");
        }
        printf("\n");
    }
}

void affichageItems(int A, int B, int F, int C, int P){
    gotoxy(65, 15);
    text_color(VERT);
    printf("Il Vous reste à éliminer :\n");
    gotoxy(65, 16);
    printf("B : %d\n", B);
    gotoxy(65, 17);
    printf("F : %d\n", F);
    gotoxy(65, 18);
    printf("C : %d\n", C);
    gotoxy(65, 19);
    printf("P : %d\n", P);
    gotoxy(65, 20);
    printf("A : %d\n", A);
    gotoxy(65, 21);
}


char lettreAleatoire(){
    char lettre;
    char liste_lettres[5] = {'B', 'P', 'C', 'F', 'A'};
    int nombre = rand()%5;
    return lettre = liste_lettres[nombre];
}

void afficherNiveau(int niveau){
    gotoxy(65, 5);
    text_color(VERT);
    printf("Vous êtes au niveau : %d\t", niveau);
}

void afficherVie(int vie){
    gotoxy(65, 7);
    text_color(VERT);
    printf("Vous avez encore : %d vie(s)\n", vie);
}

int DeplacerJoueur(char touche, int *x,int *y){
    if (touche == 'z' && *y > 1){
        (*y)--;
    }
    if (touche == 'q' && *x > 1){
        (*x)--;
    }
    if (touche == 's' && *y < HAUTEUR){
        (*y)++;
    }
    if (touche == 'd' && *x < LARGEUR){
        (*x)++;
    }

}



void afficherCadreBonbon(){
    int i = 0, j = 2;
    char lettre_recup;
    do{
    gotoxy(2,j);
    for (i=0; i<LARGEUR; i++){
        lettre_recup = lettreAleatoire();
	stockageTableau(i, j, lettre_recup);
        switch (lettre_recup)
        {
        case 'B':text_color(BRIHT_YELLOW);break;
        case 'P':text_color(BRIGHT_RED);break;
        case 'C':text_color(ORANGE);break;
        case 'F': text_color(ROSE);break;
        case 'A':text_color(APRICOT);break;
        default:
            break;
        }
        printf("%c", lettre_recup);
    }
    j++;
    printf("\n");
    }while(j <= HAUTEUR+1);

}

void afficherObjet(char c, int x, int y, const char* couleur) {
    gotoxy(x, y);
    printf("%s%c%s", couleur, c, RESET);
    fflush(stdout);
}


int menu(){
    int reponse;
    printf("1 : Règles du jeu\n");
    printf("2 : Jouer\n");
    printf("3 : Reprendre une partie\n");
    printf("4 : QUITTER\n");
    printf("Que voulez-vous faire : ");
    scanf("%d", &reponse);
    switch (reponse){
    case 1: printf("utilisez les touches z q s d pour vous orientez, compris ?:(O/N)(n : quitter) :"); break;
    case 2: return 1; break;
    case 3: return 2; break;
    case 4: return 3; break;
}
}




void jeu(){
    int compteurX = 0, compteurY = 65;
    int B=0, F=0, C=0, P=0, A=0;
    int niveau = 0;
    int vie = 3;
    int nb_coups = 0;
    char touche;
    int xJoueur = LARGEUR / 2;
    int yJoueur = HAUTEUR / 2;
    int xObjet, yObjet;
    char lettre = grille[yJoueur][xJoueur];

    clrscr();
    afficherCadre();
    afficherCadreBonbon();
    afficherNiveau(niveau);
    afficherVie(vie);
    contrat();
    coups(nb_coups);
    fonctionBonusMalus();
    clock_t temps_debut;
    double temps_ecoule;
    temps_ecoule=0;
    temps_debut = clock();
    clock_t temps_restant;
    affichageItems(A, B, C, P, F);
    do{
    //variables pour la gestion du temps
    clock_t temps_restant;
    temps_ecoule = (double)(clock() - temps_debut) / CLOCKS_PER_SEC;
    temps_restant = 60.0 - temps_ecoule;
    gotoxy(65, 13);
    text_color(VERT);
    printf("Il vous reste : %1ld seconde(s)", temps_restant);
    text_color(RESET);
    usleep(30000);
    afficherObjet(lettre, xJoueur, yJoueur, VIOLET);
    if (temps_restant == 0){
    break;
    }
        if(kbhit() != 0){
            touche = getch();
            if (touche == 'z' || touche == 'q' || touche == 's' || touche == 'd'){
                DeplacerJoueur(touche, &xJoueur, &yJoueur);
                afficherObjet('_', xJoueur, yJoueur, VIOLET);
                lettre = grille[yJoueur][xJoueur];
            }

        }

    afficherObjet('_', xJoueur, yJoueur, VIOLET);

    }while(touche != 'n');
}

void main(){
    srand(time(NULL));
    jeu();
}
