#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define tailleLigne 2 //Définition de la taille du nombre de ligne du plateau 
#define tailleColonne 6 //Définition de la taille du nombre de colonne du plateau 

//Structure du joueur à utiliser durant la partie 
struct joueur
{
    char prenom[10];
    int nb_graine;
    int choix_ligne; // 0 si il prend la première ligne ! 1 sinon
    int choix_colonne; // choisit la case de départ pour la répartition des graines 
};
typedef struct joueur joueur; 


/* Inititiation de la partie et des valeurs de joueur 1 */ 
joueur initJoueur1(joueur j1) {
    j1.nb_graine = 0;
    printf("Bienvenu ! Vous allez jouer au Jeu-Awale ! \n");
    printf("Joueur 1 -> Veuillez saisir votre prenom : ");
    scanf("%s", j1.prenom);
    printf("\n%s -> Veuillez saisir votre côté en saisissant 0 ou 1 : ", j1.prenom);
    printf("\n%s -> Attention si 0 est votre choix vous commencerez : ", j1.prenom);
    scanf("%d", &j1.choix_ligne);
    while (j1.choix_ligne != 0 && j1.choix_ligne != 1) //Choix entre la ligne 0 et la ligne 1
    {
        printf("\n%s -> Veuillez ressaisir votre côté en saisissant 0 ou 1 : ", j1.prenom);
        scanf("%d", &j1.choix_ligne);
    }
    return j1;
}

/* Inititiation des valeurs de joueur 2 */
joueur initJoueur2(joueur j2, joueur j1) {
    j2.nb_graine = 0;
    printf("\nJoueur 2 -> Veuillez saisir votre prenom : ");
    scanf("%s", j2.prenom);
    if (j1.choix_ligne == 1) //Attribution par défaut de la ligne de joueur 2 en fonction du choix de joueur 1
    { 
        j2.choix_ligne = 0;
    }else
    {
        j2.choix_ligne = 1;
    }
    printf("\n%s -> Vous avez le côté : %d \n\n", j2.prenom,j2.choix_ligne);
    return j2;
}

/* Vérifie si la case choisie par le joueur est bien compris dans le plateau (entre 0 et 5) */
bool verifSaisieCase(joueur j){
    if (j.choix_colonne >= 0 && j.choix_colonne < tailleColonne)
    {
        return true;
    }
    return false;
}

/* Vérifie SaisieCase() dans un 1er temps puis vérifie si la case est vide ou non */
bool verifCase(int **plateau, joueur j){
    if(!verifSaisieCase(j)){ // Vérifier si la saisie est valide
        printf ("Saisie case invalide (doit être compris entre 0 et 5). Ressaisissez une case valide : \n");
        return false;
    }
    if (plateau[j.choix_ligne][j.choix_colonne] == 0) { // Vérifier si la case est vide 
        printf ("La case séléctionnée est vide. Ressaisissez une case valide : \n");
        return false;
    }
    else{ //Returne True si la case n'est pas vide 
        return true;
    }
}


//Fonction qui permet d'avoir un bon affichage du plateau 
void affichePlateau(int **plateau){
    printf("-------------------------\n");
    for(int i = 0; i < tailleLigne; i++){
        for (int j = 0; j < tailleColonne; j++)
        {
            printf("| %d ", plateau[i][j]);
        }
        printf("|\n-------------------------\n");
    }
}

/* Fonction qui permet de sauter la case de départ en cas de tour complet (exemple : répartition de 15 graines) */ 
bool estCaseDepart(joueur j, int l, int c){
    if (l == j.choix_ligne && c == j.choix_colonne)
    {
        return true;
    }
    return false;
}

/* Calcule la somme de graines dans la ligne du joueur (permet la fin de la partie) */
int sommeLigne(int **plateau,joueur j) {
    int somme = 0 ;
    for (int i = 0; i < tailleColonne; i++) {
        somme += plateau[j.choix_ligne][i];
    }
    return somme;
}

/* Affichage du joueur gagnant en comparant leurs nombre de graines respectifs */
void victoireJoueur(joueur j1,joueur j2) {
    if (j1.nb_graine>j2.nb_graine) {
       printf("%s à gagner la partie avec un total de %d points contre %d points pour %s! FELICITATION ",j1.prenom,j1.nb_graine,j2.nb_graine,j2.prenom);
   }
   else {
       printf("%s à gagner la partie avec un total de %d points contre %d points pour %s! FELICITATION ",j2.prenom,j2.nb_graine,j1.nb_graine,j1.prenom);
   }
}

/* Fonciton mouvement simple d'un joueur (prise des graines, répartition, calcul des points) */ 
int priseSimple(int **plateau, joueur j){
    int ligne = j.choix_ligne;
    int colonne = j.choix_colonne;
    int subColonne = 0; //variable de substitution de colonne car colonne avance d'une case en trop 
    int points = 0;  
    int nbGraines = plateau[ligne][colonne]; //Nombre de graines à répartir 
    int tailleTab = nbGraines+1; //Taille du tableau "tab" 
    int *tab = (int*) malloc (tailleTab * sizeof(int)); //Tableau contenant les valeurs des cases parcourue par la répartition (aide au calcul des points)
    for(int r = 0; r < nbGraines; r++){
        tab[r] = 0;
    }
    plateau[ligne][colonne] = 0; //Graines prélevés de la case de départ 
    if (ligne == 1)
    {
        colonne++;
    }else
    {
        colonne--;
    }
    
    while (nbGraines != 0) //Tant qu'il reste des graines à répartir 
    {        
        if (ligne == 1 && colonne < tailleColonne){  
            while(colonne < tailleColonne && nbGraines != 0) //Parcours de la ligne 1 tant qu'on arrive pas au bout de la ligne 
            {
                if (estCaseDepart(j,ligne,colonne)) //Saut de la case s'il s'agit de la case de départ
                {
                    colonne++;
                }else
                {
                    plateau[ligne][colonne]++; //ajout de la graine sur la case
                    subColonne = colonne; //subColonne aura une valeur de moins que colonne
                    tab[nbGraines] = plateau[ligne][subColonne]; //on remplit le tableau pour la valeur de la case 
                    nbGraines--; 
                    colonne++; //avancée d'une case 
                }
                
            }
        }
        if(ligne == 1 && colonne >= tailleColonne){ //Cas où on arrive au bout de la ligne 1 (colonne = 6)
            ligne = 0; 
            colonne--; 
        }
        if(ligne == 0 && colonne < tailleColonne){ //meme cas que précédemment mais pour le parcours de ligne 0 
            while (colonne >= 0 && nbGraines != 0) 
            {   
                if (estCaseDepart(j,ligne,colonne))
                {
                    colonne--;
                }else{
                    plateau[ligne][colonne]++;
                    subColonne = colonne;
                    tab[nbGraines] = plateau[ligne][subColonne];
                    colonne--; 
                    nbGraines--;
                }
            }   
        }
        if(ligne == 0 && colonne < 0){  //Cas où on arrive au bout de la ligne 0 (colonne = -1)
            ligne = 1;
            colonne++;  
        }        
    }   

    //CALCUL DES POINTS
    nbGraines+=1;
    if ((plateau[ligne][subColonne] == 1 || plateau[ligne][subColonne] == 2) && j.choix_ligne != ligne) /*case finale = 1 ou 2 ET joueur ne peut pas 
                                                                                                            gagner de points dans une de ses cases */
    {
        while(nbGraines < tailleTab) //Parcours le tableau de valeurs des cases parcourues et vide la case lorsqu'elle est à 1 ou 2
        {
            if (tab[nbGraines] == 1 || tab[nbGraines] == 2)
            {
                plateau[ligne][subColonne] = 0; //On parcours le plateau dans le sens inverse et on vide les cases consecutives contenant 1 ou 2 
                if (ligne == 0) {
                    if(subColonne >= tailleColonne){ //Cas où on arrive au bout de la ligne (6)
                        ligne = 1;
                        subColonne--; 
                    }
                    else {
                        points += tab[nbGraines]; //ajout des points de la valeur de la case (1 ou 2)
                        subColonne++;
                        nbGraines++;
                    }
                }
                else if(ligne == 1) {
                    if(subColonne < 0){ //Cas où on arrive au bout de la ligne (6)
                        ligne = 0;
                        subColonne++;  
                    }
                    else {
                        points += tab[nbGraines]; 
                        subColonne--;
                        nbGraines++;
                    }
                }
            }else
            {
                break; //Stop la boucle dès qu'une case n'est plus de valeur 1 ou 2
            } 
        } 
    }
    return points; 
}




    
 

int main(void){
    /* Créer les 2 Joueurs */
    joueur j1;
    joueur j2;
    int nb = 0; //points à ajouter dans joueur.nb_graine
    j1 = initJoueur1(j1);
    j2 = initJoueur2(j2,j1); 


    /* Creer le plateau */
    int **plateau = (int**) malloc(tailleLigne * sizeof(int*));
    for(int i = 0; i < tailleLigne; i++){
        plateau[i] = (int*) malloc(tailleColonne * sizeof(int));
        for (int j = 0; j < tailleColonne; j++)
        {
            plateau[i][j] = 4;
        }
    }

    //Le joueur choisissant la ligne 0 commence 
    int j;
    if (j1.choix_ligne == 1) { j = 2;}
    else { j = 1; }
           
    affichePlateau(plateau);
    while (sommeLigne(plateau,j1)!=0  ||  sommeLigne(plateau,j2)!=0) //Fin de la partie : somme de la ligne d'un des 2 joueurs = 0 
    {
        if (j == 1)
        {
            printf("\n%s -> Veuillez choisir la case que vous voulez déplcer en saisissant le numColonne : ", j1.prenom);
            scanf("%d", &j1.choix_colonne);
            printf("\n");
            while (!verifCase(plateau,j1)) { 
                scanf("%d", &j1.choix_colonne);
            }
            nb = priseSimple(plateau,j1); //Retourne le nombre de points prit dans le coup et les ajoutes dans joueur 
            j1.nb_graine += nb;
            j++;
            affichePlateau(plateau);
            printf("\n\n%s -> Graines Totales : %d \n\n---------------------------------------------- \n",j1.prenom,j1.nb_graine);
        }else
        {
            printf("\n%s -> Veuillez choisir la case que vous voulez déplcer en saisissant le numColonne : ", j2.prenom);
            scanf("%d", &j2.choix_colonne);
            printf("\n");
            while (!verifCase(plateau,j2)) {
                scanf("%d", &j2.choix_colonne);
            }
            nb = priseSimple(plateau,j2);
            j2.nb_graine += nb;
            j--;
            affichePlateau(plateau);
            printf("\n\n%s -> Graines Totales : %d \n\n---------------------------------------------- \n",j2.prenom,j2.nb_graine);
        }
   }
   victoireJoueur(j1,j2); //Affiche le gagnant de la partie

}
