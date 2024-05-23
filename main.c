#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "libserialport.h"
#include "time.h"
#define BUFFER_SIZE 2048

struct sp_port *port;

typedef struct {
    char action[20];
    char param1[20];
    char param2[20];
}ActionData;

typedef struct node {
    ActionData data;
    struct node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;

/**
 * @file
 * @brief Ce fichier rassemble les fonctions utiles au traitement de texte
*/

Queue createQueue() {    //Initialise et renvoie une nouvelle file vide.
    Queue q;
    q.front = q.rear = NULL;
    q.size = 0;
    return q;
}

void enqueue(Queue* q, ActionData data) {    //Rajoute un élément à la file.
    Node* temp = (Node*) malloc(sizeof(Node));
    temp->data = data;
    temp->next = NULL;

    if (q->rear == NULL) {    //Si la file est vide, le nouvel élément devient à la fois le premier et le dernier élément.
        q->front = q->rear = temp;
        q->size = 1;
        return;
    }

    q->rear->next = temp;    //Sinon, il est rajouté à la fin et devient le nouvel élément rear.
    q->rear = temp;
    q->size++;    //Incrémente la taille de la file.
}

ActionData dequeue(Queue* q) {    //Dégage un élément à la file.
    if (q->front == NULL) {    //Si la file est deja vide on affichera une erreur.
        fprintf(stderr, "Erreur: La file est deja vide.\n");
        exit(EXIT_FAILURE);
    }

    Node *temp = q->front;    //Un pointeur temporaire temp est créé pour conserver le pointeur vers le noeud de tête actuel, qui sera retiré.
    ActionData data = temp->data;  //On copie tous les parametres de data.

    q->front = q->front->next;    //Le pointeur front de la file est déplacé vers le nœud suivant, car le nœud de tête actuel est sur le point d'être supprimé.
    if (q->front == NULL) {    //Si après cette opération, q->front devient NULL, cela signifie que la file est maintenant vide.
        q->rear = NULL;    //Par conséquent, q->rear est également mis à NULL.
    }
    q->size--;    //Et on met a jour la taille de la file.

    free(temp);    //On libere de la mémoire.
    return data;    //La focntion retourne finalement les parametres de data qui viennent d'étre enlevés.
}

int isQueueEmpty(Queue q) {    //Fonction qui nous indiquera simplement si la file est vide ou pas et renvoie 1 pour un front NULL.
    return (q.front == NULL);
}

/**
 * @defgroup fonctions_sec Fonctions partie texte secondaires
 * @brief Fonctions utilisés dans les fonctions principales du traitement de phrase
 * @{
 */
/**
 *
 * @brief Fonction pour transformer des str en double.
 */

int convertDouble(char *str) {
    char *temp;
    int val = strtod(str, &temp);
    return val;
}

/**
 * @brief Fonction pour passer n'importe quelle chaine en minuscule.
 * @param str Pointeur vers une chaine de caractères d'un mot quelconque
 */

void toLowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char) str[i]);
    }
}

/**
 * @brief Retournera un entier en fonction du str qui lui correspond en français.
 * @param nombre Chaine de caractéres désignant un chiffre écrit.
 * @return Chiffre désignant le chiffre écrit en lettres.
 */


int str_to_num_fr(char *nombre) {
    if (strcmp(nombre, "un") == 0) return 1;
    if (strcmp(nombre, "deux") == 0) return 2;
    if (strcmp(nombre, "trois") == 0) return 3;
    if (strcmp(nombre, "quatre") == 0) return 4;
    if (strcmp(nombre, "cinq") == 0) return 5;
    if (strcmp(nombre, "six") == 0) return 6;
    if (strcmp(nombre, "sept") == 0) return 7;
    if (strcmp(nombre, "huit") == 0) return 8;
    if (strcmp(nombre, "neuf") == 0) return 9;
    if (strcmp(nombre, "dix") == 0) return 10;
    if (strcmp(nombre, "onze") == 0) return 11;
    if (strcmp(nombre, "douze") == 0) return 12;
    if (strcmp(nombre, "treize") == 0) return 13;
    if (strcmp(nombre, "quatorze") == 0) return 14;
    if (strcmp(nombre, "quinze") == 0) return 15;
    if (strcmp(nombre, "seize") == 0) return 16;
    if (strcmp(nombre, "vingt") == 0) return 20;
    if (strcmp(nombre, "trente") == 0) return 30;
    if (strcmp(nombre, "quarante") == 0) return 40;
    if (strcmp(nombre, "cinquante") == 0) return 50;
    if (strcmp(nombre, "soixante") == 0) return 60;
    if (strcmp(nombre, "cent") == 0 || strcmp(nombre, "cents") == 0) return 100;
    if (strcmp(nombre, "mille") == 0 || strcmp(nombre, "milles") == 0) return 1000;
    else
        return 0;
}

/**
 * @brief Utilisera les chiffres écrits obtenus séparement pour les traiter après et obtenir le chiffre total.
 * @param str Chaine de caractéres désignant un chiffre écrit en français.
 * @return Le chiffre désignant le résultat total des chiffres nommées puis traités
 */


int num_to_chiffre_tot_fr(char *str) {
    char *mot;
    char ponctuation[] = " ',.-";
    int total = 0, temp = 0;

    mot = strtok(str, ponctuation);    //Sépare les mots de la phrase (str) pour chaque ponctuation qu'on a déjà defini.
    while (mot != NULL) {
        int num = str_to_num_fr(mot);    //Traitement des chiffres (somme ou multiplication, dépendant de leurs utilisation dans une phrase.
        if (num == 1000 || num == 100 || num == 20) {
            temp = (temp == 0 ? 1 : temp) * num;
            total += temp;
            temp = 0;
        } else {
            temp += num;
        }
        mot = strtok(NULL, ponctuation);    //Appel au mot defini.
    }
    total += temp;    //On rajoute les valeurs réstants.
    return total;
}

/**
 * @brief Fonction booléenne ayant le role de détecter si le str introduit corréspond à un chiffre écrit en français.
 * @param word Le mot qu'on compare avec les chaines de caractères de la fonction
 * @return true ou false
 */

bool detect_chiffre_fr(const char *word) {
    const char *nombre_mot[] = {
            "un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf",
            "dix", "onze", "douze", "treize", "quatorze", "quinze", "seize",
            "vingt", "trente", "quarante", "cinquante", "soixante", "quatre vingts",
            "cent","cents", "mille","milles", NULL
    };

    for (int i = 0; nombre_mot[i] != NULL; i++) {
        if (strcmp(word, nombre_mot[i]) == 0) {
            return true;
        }
    }
    return false;
}


/**
 * @}
 */
/**
 * @defgroup Fonctions_prin Fonctions partie texte principales
 * @brief Fonctions qui traitent la phrase
 * @{
 */
/**
 * @brief Une des fonctions principales du code,"afficher_Action_fr" traitera une phrase en français
 * Elle s'aidera de celles défà définies pour traiter une phrase et la tokeniser, pour la pouvoir
 * ainsi diviser en plusieurs sous-phrases et les traiter individuellement.
 * @param phrase Chaine de caractères désignant la requette texte.
 * @param q File qu'on utilisera pour enregistrer les paramettres qu'on utilisera dans le code qui
 * gére les commandes envoyés au robot
 * @return Paramettres décrivant l'action, la diréction, les unités d'action (mètres ou degrés),
 * les obstacles à prendre en compte et si la phrase est une négation ou une affirmation
 */


void preprocess_phrase(char *phrase) {
    char temp[1000] = "";
    int i, j = 0;

    for (i = 0; phrase[i] != '\0'; i++) {
        if (phrase[i] == ',') {
            // Saltar las comas
            continue;
        }else {
            temp[j++] = phrase[i];
        }
    }
    temp[j] = '\0';
    strcpy(phrase, temp);
}

void trim_leading_spaces(char *str) {
    int index = 0;

    // Encuentra el primer carácter no espacial
    while (str[index] == ' ') {
        index++;
    }

    // Mueve la cadena hacia la izquierda para eliminar los espacios iniciales
    int i = 0;
    while (str[index]) {
        str[i++] = str[index++];
    }
    str[i] = '\0';
}

void afficher_Action_fr(char *phrase, Queue* q){
    char subphrases[5][1000];
    char *mots[100];
    int compt = 0;
    char Avant_chiffre[100] = "", Chiffre_mots[100] = "", Apres_chiffre[100] = "";
    int chiffre;
    char ponctuation_chiffre[] = " ";
    char *mot;
    char ponctuation[] = " ',.!?";
    bool nombre_trouve = false;
    char *action;
    char ponctuation_sub[] = " ',.!?";
    char *mot_sub;
    int i, j;
    int result;
    int entre_flag = 0;
    int ou_flag=0;
    char *mots_sub[100];
    int compt_sub = 0;
    int subphr_compt = 0;
    ActionData resultData;

    preprocess_phrase(phrase);
    trim_leading_spaces(phrase);

    for (i = 0; i < 5; i++) {    //Initialise toutes les sous-phrases à vide.
        subphrases[i][0] = '\0';
    }

    toLowercase(phrase);

    char *saveptr1;
    mot = strtok_r(phrase, ponctuation, &saveptr1);
    while (mot != NULL && compt < 100) {
        mots[compt++] = mot;
        mot = strtok_r(NULL, ponctuation, &saveptr1);
    }

    // Division en plusieurs sous-phrases
    for (int i = 0; i < compt; i++) {
        if (strcmp(mots[i], "puis") == 0 && subphr_compt < 4) {  //Éviter débordement de chaine de caractéres
            subphr_compt++;
        } else {
            strcat(subphrases[subphr_compt], mots[i]);
            strcat(subphrases[subphr_compt], " ");
        }
    }

    for (i = 0; i <= subphr_compt; i++) {
        //printf("\nSubphrase %d: %s\n", i + 1, subphrases[i]);

        //On réinitialise les variables suivantes à chaque itération pour ne pas avoir des problèmes en mémoire.
        nombre_trouve = false;
        strcpy(Avant_chiffre, "");
        strcpy(Chiffre_mots, "");
        strcpy(Apres_chiffre, "");

        mot = strtok(subphrases[i], ponctuation_chiffre);    //On diférentie les parties de la sous-phrase qui definissent les chiffres ou pas.
        while (mot != NULL) {
            if (detect_chiffre_fr(mot)) {    //Si on détecte un mot corréspondant à un chiffre on les garde comme "Chiffre_mots".
                nombre_trouve = true;
                strcat(Chiffre_mots, mot);
                strcat(Chiffre_mots, " ");
            } else {
                if (!nombre_trouve) {    //Si les mots se trouvent avant la détection des mots corréspondant aux chiffres on les garde comme "Avant_chiffre".
                    strcat(Avant_chiffre, mot);
                    strcat(Avant_chiffre, " ");
                } else {    //Si les mots se trouvent après la détection des mots corréspondant aux chiffres on les garde comme "Apres_chiffre".
                    strcat(Apres_chiffre, mot);
                    strcat(Apres_chiffre, " ");
                }
            }
            mot = strtok(NULL, ponctuation_chiffre);
        }

        //On transforme "Chiffre_mots" en chiffre puis on réconstruit la sous-phrase initiale en charactéres et chiffres
        chiffre = num_to_chiffre_tot_fr(Chiffre_mots);
        sprintf(subphrases[i], "%s%d %s", Avant_chiffre, chiffre, Apres_chiffre);
        //printf("Subphrase traité: %s\n", subphrases[i]);
    }

    //printf("Nb ss-phrases : %d\n",subphr_compt+1);

    for (i = 0; i <= subphr_compt; i++) {

        //On réinitialise les variables suivantes à chaque itération pour ne pas avoir des problèmes en mémoire.
        compt_sub = 0;
        entre_flag = 0;

        char param1[20] = "";
        char param2[20] = "";
        char type[20] = "";

        mot_sub = strtok(subphrases[i], ponctuation_sub);
        while (mot_sub != NULL && compt_sub < 20) {
            mots_sub[compt_sub++] = mot_sub;
            mot_sub = strtok(NULL, ponctuation_sub);
        }

        //for (j = 0; j < compt_sub; j++) {
        //    printf("mot[%d]=%s\n", j, mots_sub[j]);
        //}

        for (j = 0; j < compt_sub; j++) {
            //Si le mot de la sous-phrase corréspond a un de ceux-là alors le parametre action sur ActionData prendra cette valeur
            if (strcmp(mots_sub[j], "avancer") == 0 || strcmp(mots_sub[j], "avance") == 0 ||
                strcmp(mots_sub[j], "reculer") == 0 || strcmp(mots_sub[j], "recule") == 0 ||
                strcmp(mots_sub[j], "tourner") == 0 || strcmp(mots_sub[j], "tourne") == 0 ||
                strcmp(mots_sub[j], "localiser") == 0 || strcmp(mots_sub[j], "localise") == 0 ||
                strcmp(mots_sub[j], "trouver") == 0 || strcmp(mots_sub[j], "trouve") == 0 ||
                strcmp(mots_sub[j], "contourner") == 0 || strcmp(mots_sub[j], "contourne") == 0 ||
                strcmp(mots_sub[j], "passer") == 0 || strcmp(mots_sub[j], "passe") == 0 ||
                strcmp(mots_sub[j], "compter") == 0 || strcmp(mots_sub[j], "compte") == 0 ||
                strcmp(mots_sub[j], "scanner") == 0) {
                action = mots_sub[j];
            }


            //PHRASE DU TYPE "CONTOURNER PAR LA [DIRECTION] L'[OBJET]"
            //PHRASE DU TYPE "CONTOURNER L'[OBJET] PAR LA [DIRECTION]"
            if(strcmp(action, "contourner") == 0 || strcmp(action, "contourne") == 0) {

                if(strcmp(mots_sub[j], "droite") == 0 || strcmp(mots_sub[j], "gauche") == 0)
                    sprintf(param1, "%s", mots_sub[j]);

                if (strcmp(mots_sub[j], "cube") == 0 || strcmp(mots_sub[j], "carré") == 0 ||
                    strcmp(mots_sub[j], "balle") == 0 || strcmp(mots_sub[j], "boule") == 0 ||
                    strcmp(mots_sub[j], "objet") == 0 || strcmp(mots_sub[j], "obstacle") == 0) {
                    if (j + 1 < compt_sub && (strcmp(mots_sub[j + 1], "rouge") == 0 || strcmp(mots_sub[j + 1], "bleu") == 0 || strcmp(mots_sub[j + 1], "bleue") == 0 ||
                                              strcmp(mots_sub[j + 1], "jaune") == 0 || strcmp(mots_sub[j + 1], "orange") == 0)) {
                        sprintf(param2, "%s %s", mots_sub[j], mots_sub[j + 1]);
                    } else {
                        sprintf(param2, "%s", mots_sub[j]);
                    }
                }
            }


                //PHRASE DU TYPE "PASSER ENTRE [OBJET] ET [OBJET]"
            else if (strcmp(action, "passer") == 0 || strcmp(action, "passe") == 0) {

                if(strcmp(mots_sub[j], "entre") == 0) {
                    entre_flag = 1;                                         // Mot "entre" trouvé, donc flag_entre (qui indique ça) est mit a 1
                    continue;
                }

                // Procesar objetos y colores después de "entre"
                if (entre_flag == 1 && strcmp(mots_sub[j], "et") == 0){
                    entre_flag = 0;                                         // On remet lentre_flag à 0
                    continue;
                }

                if (entre_flag == 1) {
                    if (strcmp(mots_sub[j], "cube") == 0 || strcmp(mots_sub[j], "carré") == 0 ||
                        strcmp(mots_sub[j], "balle") == 0 || strcmp(mots_sub[j], "boule") == 0 ||
                        strcmp(mots_sub[j], "objet") == 0 || strcmp(mots_sub[j], "obstacle") == 0) {
                        if (j + 1 < compt_sub && (strcmp(mots_sub[j + 1], "rouge") == 0 || strcmp(mots_sub[j + 1], "bleu") == 0 || strcmp(mots_sub[j + 1], "bleue") == 0 ||
                                                  strcmp(mots_sub[j + 1], "jaune") == 0 || strcmp(mots_sub[j + 1], "orange") == 0)) {
                            sprintf(param1, "%s %s", mots_sub[j], mots_sub[j + 1]);
                        } else {
                            sprintf(param1, "%s", mots_sub[j]);
                        }
                    }
                } else {
                    if (strcmp(mots_sub[j], "cube") == 0 || strcmp(mots_sub[j], "carré") == 0 ||
                        strcmp(mots_sub[j], "balle") == 0 || strcmp(mots_sub[j], "boule") == 0 ||
                        strcmp(mots_sub[j], "objet") == 0 || strcmp(mots_sub[j], "obstacle") == 0) {
                        if (j + 1 < compt_sub && (strcmp(mots_sub[j + 1], "rouge") == 0 || strcmp(mots_sub[j + 1], "bleu") == 0 || strcmp(mots_sub[j + 1], "bleue") == 0 ||
                                                  strcmp(mots_sub[j + 1], "jaune") == 0 || strcmp(mots_sub[j + 1], "orange") == 0)) {
                            sprintf(param2, "%s %s", mots_sub[j], mots_sub[j + 1]);
                        } else {
                            sprintf(param2, "%s", mots_sub[j]);
                        }
                    }
                }
            }

                //PHRASE DU TYPE "LOCALISE/TROUVE L'[OBJET]"
                //PHRASE DU TYPE "LOCALISE/TROUVE L'[OBJET] OU L'[OBJET]"
            else if(strcmp(action, "localiser") == 0 || strcmp(action, "localise") == 0 ||
                    strcmp(action, "trouver") == 0 || strcmp(action, "trouve") == 0) {

                if (ou_flag == 0 && strcmp(mots_sub[j], "ou") == 0){
                    ou_flag = 1;                                         // On remet le flag de "ou" a 1
                    continue;
                }

                if (ou_flag == 0) {
                    if (strcmp(mots_sub[j], "cube") == 0 || strcmp(mots_sub[j], "carré") == 0 ||
                        strcmp(mots_sub[j], "balle") == 0 || strcmp(mots_sub[j], "boule") == 0 ||
                        strcmp(mots_sub[j], "objet") == 0 || strcmp(mots_sub[j], "obstacle") == 0) {
                        if (j + 1 < compt_sub && (strcmp(mots_sub[j + 1], "rouge") == 0 || strcmp(mots_sub[j + 1], "bleu") == 0 || strcmp(mots_sub[j + 1], "bleue") == 0 ||
                                                  strcmp(mots_sub[j + 1], "jaune") == 0 || strcmp(mots_sub[j + 1], "orange") == 0)) {
                            sprintf(param1, "%s %s", mots_sub[j], mots_sub[j + 1]);
                        } else {
                            sprintf(param1, "%s", mots_sub[j]);
                        }
                    }
                } else {
                    if (strcmp(mots_sub[j], "cube") == 0 || strcmp(mots_sub[j], "carré") == 0 ||
                        strcmp(mots_sub[j], "balle") == 0 || strcmp(mots_sub[j], "boule") == 0 ||
                        strcmp(mots_sub[j], "objet") == 0 || strcmp(mots_sub[j], "obstacle") == 0) {
                        if (j + 1 < compt_sub && (strcmp(mots_sub[j + 1], "rouge") == 0 || strcmp(mots_sub[j + 1], "bleu") == 0 || strcmp(mots_sub[j + 1], "bleue") == 0 ||
                                                  strcmp(mots_sub[j + 1], "jaune") == 0 || strcmp(mots_sub[j + 1], "orange") == 0)) {
                            sprintf(param2, "%s %s", mots_sub[j], mots_sub[j + 1]);
                        } else {
                            sprintf(param2, "%s", mots_sub[j]);
                        }
                    }
                }
            }

            else if(strcmp(action, "compter") == 0 || strcmp(action, "compte") == 0) {
                if (strcmp(mots_sub[j], "cubes") == 0 || strcmp(mots_sub[j], "carrés") == 0 ||
                    strcmp(mots_sub[j], "balles") == 0 || strcmp(mots_sub[j], "boules") == 0 ||
                    strcmp(mots_sub[j], "objets") == 0 || strcmp(mots_sub[j], "obstacles") == 0) {
                    if (j + 1 < compt_sub && (strcmp(mots_sub[j + 1], "rouges") == 0 || strcmp(mots_sub[j + 1], "bleus") == 0 || strcmp(mots_sub[j + 1], "bleues") == 0 ||
                                              strcmp(mots_sub[j + 1], "jaunes") == 0 || strcmp(mots_sub[j + 1], "oranges") == 0)) {
                        sprintf(param2, "%s %s", mots_sub[j], mots_sub[j + 1]);
                    } else {
                        sprintf(param2, "%s", mots_sub[j]);
                    }
                }
            }

            else{

                //PHRASE DU TYPE "AVANCER DE [DISTANCE]"
                if (strcmp(mots_sub[j], "m") == 0 || strcmp(mots_sub[j], "mètres") == 0 || strcmp(mots_sub[j], "mètre") == 0 || strcmp(mots_sub[j], "centimètres") == 0 || strcmp(mots_sub[j], "cm") == 0 || strcmp(mots_sub[j], "millimètres") == 0|| strcmp(mots_sub[j], "cm") == 0) {
                    if(strcmp(mots_sub[j-1], "et") == 0) {
                        result = convertDouble(mots_sub[j - 2]);
                    }else{
                        result = convertDouble(mots_sub[j - 1]);
                    }
                    if(strcmp(mots_sub[j], "m") == 0 || strcmp(mots_sub[j], "mètres") == 0 || strcmp(mots_sub[j], "mètre") == 0){
                        result *= 100;
                    }
                    else if (strcmp(mots_sub[j], "millimètres") == 0 || strcmp(mots_sub[j], "mm") == 0) {
                        result *= 0.1;
                        result=(int)result;
                    }
                    sprintf(param2, "%d", result);
                }

                //PHRASE DU TYPE "AVANCER JUSQU'À L'[OBJET]"
                //PHRASE DU TYPE "TOURNER JUSQU'À LOCALISER L'[OBJET]"
                //PHRASE DU TYPE "TOURNER À [DIRECTION] JUSQU'À LOCALISER L'[OBJET]"
                if (strcmp(mots_sub[j], "boule") == 0 || strcmp(mots_sub[j], "balle") == 0 ||
                    strcmp(mots_sub[j], "carré") == 0 || strcmp(mots_sub[j], "cube") == 0) {
                    // Vérifier s'il y a un mot suivant pour la couleur.
                    if (j + 1 < compt_sub && (strcmp(mots_sub[j + 1], "rouge") == 0 || strcmp(mots_sub[j + 1], "bleu") == 0 || strcmp(mots_sub[j + 1], "bleue") == 0 ||
                                              strcmp(mots_sub[j + 1], "jaune") == 0 || strcmp(mots_sub[j + 1], "orange") == 0)) {
                        sprintf(param2, "%s %s ", mots_sub[j], mots_sub[j + 1]);
                    } else {
                        sprintf(param2, "%s ", mots_sub[j]);
                    }
                }

                //PHRASE DU TYPE "TOURNER DE [VAL] DEGRÉS"
                //PHRASE DU TYPE "TOURNER À [DIRECTION] DE [VAL] DEGRÉS"
                if(strcmp(mots_sub[j], "droite") == 0 || strcmp(mots_sub[j], "gauche") == 0)
                    sprintf(param1, "%s", mots_sub[j]);

                if(strcmp(mots_sub[j], "de") == 0) {
                    if(strcmp(mots_sub[j-1], "et") == 0) {
                        result = convertDouble(mots_sub[j + 2]);
                    }else{
                        result = convertDouble(mots_sub[j + 1]);
                    }
                    sprintf(param2, "%d", result);
                }
            }
        }

        //On copie le contenu de action, param1, param2 et type dans les paramétres de resultData, qui est une variable du type ActionData
        strcpy(resultData.action, action);
        strcpy(resultData.param1, param1);
        strcpy(resultData.param2, param2);

        //On enfile le contenu de resultData dans le file du FIFO
        enqueue(q, resultData);
    }

}

void execute_python_script(char *output) {
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "source /Users/ismaelmongeroucdi/Desktop/SolalP-C/venv/bin/activate && python3 /Users/ismaelmongeroucdi/Desktop/SolalP-C/main.py");
    FILE *fp = popen(command, "r");

    if (fp == NULL) {
        fprintf(stderr, "Failed to run Python script\n");
        exit(1);
    }

    char result[BUFFER_SIZE];
    while (fgets(result, sizeof(result), fp) != NULL) {
        strcat(output, result);
    }

    pclose(fp);
}


int main() {
    char output[BUFFER_SIZE] = {0};
    char transcribed_phrase[BUFFER_SIZE] = "";
    Queue myQueue = createQueue();
    char *mat[5][3];
    int mat_compt = 0;

    execute_python_script(output);
    strcpy(transcribed_phrase, output);
    trim_leading_spaces(transcribed_phrase);  // Asegurar que no haya espacios iniciales
    transcribed_phrase[strcspn(transcribed_phrase, "\n")] = 0;
    printf("Python script output:\n%s\n", transcribed_phrase);

    afficher_Action_fr(transcribed_phrase, &myQueue);

    printf("\n");
    while (!isQueueEmpty(myQueue) && mat_compt < 5) {
        ActionData action = dequeue(&myQueue);
        printf("\nAction: %s\nParametre 1: %s\nParametre 2: %s\n",
               action.action, action.param1, action.param2);
        mat[mat_compt][0] = strdup(action.action);
        mat[mat_compt][1] = strdup(action.param1);
        mat[mat_compt][2] = strdup(action.param2);
        mat_compt++;
    }
    printf("\n");

    enum sp_return result = sp_get_port_by_name("/dev/cu.usbserial-A401040I", &port);
    if (result != SP_OK) {
        fprintf(stderr, "Error finding port: %s\n", sp_last_error_message());
        return 1;
    }

    result = sp_open(port, SP_MODE_READ_WRITE);
    if (result != SP_OK) {
        fprintf(stderr, "Error opening port: %s\n", sp_last_error_message());
        sp_free_port(port);
        return 1;
    }

    sp_set_baudrate(port, 9600);
    if (result != SP_OK) {
        fprintf(stderr, "Error setting baudrate: %s\n", sp_last_error_message());
        sp_close(port);
        sp_free_port(port);
        return 1;
    }

    char buffer[2048];
    int offset = 0;
    // Primera fila con mat_compt
    offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%d,,|", mat_compt);

    // Rellenar con datos de la matriz mat
    for (int i = 0; i < mat_compt; i++) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%s,%s,%s|",
                           mat[i][0], mat[i][1], mat[i][2]);
        if (offset >= sizeof(buffer)) {
            fprintf(stderr, "Buffer overflow!\n");
            break; // Prevenir overflow del buffer
        }
    }

    printf("Sending message:\n%s\n", buffer);
    sp_blocking_write(port, buffer, strlen(buffer), 1000);

    printf("Waiting for response...\n");

    char response[2048];
    int total_bytes_read = 0;
    int bytes_read;
    time_t startTime = time(NULL);
    time_t currentTime = startTime;
    int timeout = 5;

    while (sp_input_waiting(port) > 0) {
        char dummy[256];
        sp_blocking_read(port, dummy, sizeof(dummy), 100);
    }

    while (difftime(currentTime, startTime) < timeout) {
        bytes_read = sp_blocking_read(port, response + total_bytes_read, sizeof(response) - total_bytes_read - 1, 100);
        if (bytes_read > 0) {
            total_bytes_read += bytes_read;
            startTime = time(NULL);
        }
        currentTime = time(NULL);
    }

    if (total_bytes_read > 0) {
        response[total_bytes_read] = '\0';
        for (int i = 0; i < total_bytes_read; i++) {
            if (response[i] == '\r') {
                response[i] = '\0';
                break;
            }
        }
        printf("\nReceived response:\n%s\n", response);
    } else {
        printf("No response received within the timeout period.\n");
    }

    sp_close(port);
    sp_free_port(port);

    return 0;
}
