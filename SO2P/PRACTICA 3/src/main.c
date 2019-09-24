/**
 *
 * Practica 3 
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "red-black-tree.h"
#include "linked-list.h"

#define MAXLINE      200
#define MAGIC_NUMBER 0x0133C8F9

char** separar_per_comes (char* linea, int nMax);
void obtainInformation(rb_tree* tree, char* nom_fitxer);
node_data* nou_node_t (char* key);
long int iniciar_aeroports (rb_tree* tree, char* nom_fitxer);
void print_retards (rb_tree* tree, char* aeroport);
int menu();
void guardar_info_arbre (FILE* f, node* n);
void trobar_mes_destins (int* max, node* n, char* resultat);
rb_tree *loaded_tree(char *filename);


/**
 * 
 *  Main procedure
 *
 */

int main(int argc, char **argv)
{
    char str1[MAXLINE], str2[MAXLINE];
    int opcio, num_nodes, aux = 0;
    rb_tree* tree;
    int magic;
    FILE* f;
    
    /* Allocate memory for tree */
    tree = (rb_tree *)malloc(sizeof(rb_tree));
    /* Initialize the tree */
    init_tree(tree);

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case 1:
                printf("Introdueix fitxer que conte llistat d'aeroports: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                printf("Introdueix fitxer de dades: ");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;
                

                num_nodes = iniciar_aeroports (tree, str1);
                obtainInformation(tree, str2);

                break;

            case 2:
                printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;
                magic = MAGIC_NUMBER;
                f = fopen (str1, "w");
                fwrite (&magic, 4, 1, f);
                fwrite (&num_nodes, 4, 1, f);
                guardar_info_arbre (f, tree->root);
                
                fclose (f);
                printf ("S'ha escrit correctament!\n");

                break;

            case 3:
                delete_tree(tree);
                free(tree);
                printf("Introdueix nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;
                printf ("%s\n", str1);
                printf("This computer is loading tree for you ^.^ ...\n");
                tree = loaded_tree(str1);
                break;

            case 4:
                printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destins: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                if (str1[0] == 0){
                    trobar_mes_destins (&aux, tree->root, str2);
                } else {
                    strcpy(str2, str1);
                }
                
                print_retards (tree, str2);
                aux = 0;
                    
                break;

            case 5:

                delete_tree (tree);
                free (tree);
                printf ("Adeu siau!\n");

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);

    return 0;
}

rb_tree *loaded_tree(char *filename)
{
    FILE *fp;
    rb_tree *tree;
    node_data *n_data;
    list_data *l_data;

    int len, nombre_magic, num_times, retard;
    int nodes;
    char word[MAXLINE];
    int i, j;
    //Comprovació de les dades perque estigui tot be 
    //Seguim l'ordre de comprovació logica i segons la pràctica
    fp = fopen(filename, "r");
    if (!fp) {
        perror("ERROR: No puc obrir l'arxiu\n");
        return NULL;
    }

    // Comprovem que el nombre nombre_magic es correcte 
    fread(&nombre_magic, 4, 1, fp);
    if (nombre_magic != MAGIC_NUMBER) {
        perror("ERROR: nombre_magic number no es correcte\n");
        return NULL;
    }

    // Hem de saber el numero de nodes que tenim 
    fread(&nodes, 4, 1, fp);
    if (nodes <= 0) {
        perror("Numero de nodes pot ser negatiu o zero, revisa-ho!\n");
        return NULL;
    }

    tree = (rb_tree *) malloc(sizeof(rb_tree));

    /* Initialize the tree */
    init_tree(tree);
    
    for(i = 0; i < nodes; i++)
    {
        fread (word, sizeof (char), 3, fp);
        word[3] = 0;
        
        fread(&len, sizeof(int), 1, fp);
        
        n_data = (node_data*) malloc (sizeof (node_data));
        n_data->l = (list*) malloc (sizeof (list));
        init_list (n_data->l);
        n_data->key = (char*) malloc (sizeof (char) * 4);
        strcpy (n_data->key, word);
        n_data->num_times = len;
        insert_node (tree, n_data);

        for (j = 0; j < len; j++) {
            fread(word, sizeof(char), 3, fp);
            word[3] = 0;
            fread(&num_times, sizeof(int), 1, fp);
            if (num_times <= 0) {
                perror("ERROR: error en el arbre(num_times), potser se li han caigut les fulles\n");
                return tree;
            }
            fread (&retard, sizeof (int), 1, fp);
            
            l_data = malloc (sizeof (list_data));
			l_data->key = malloc (4*sizeof (char));
			strcpy (l_data->key, word);
			l_data->num_times = num_times;
			l_data->retard = retard;
			insert_list(n_data->l, l_data);
        }
    }

    fclose(fp);

    return tree;
}


char** separar_per_comes (char* linea, int nMax) {
	
	char actual = linea[0], subString[100];
	char** resultat = malloc (nMax * sizeof (char*));
	int nComes, posicio, len, i;
	posicio = nComes = len = i = 0;
	while (nComes < nMax) {
		
		if (actual == ',' || actual == '\n') {
			nComes++;
			resultat[i] = (char*) malloc ((len+1) * sizeof (char));
			strncpy (resultat[i], subString, len);
			resultat[i][len] = '\0';
			i++;
			len = 0;
		} else {
			subString[len] = actual;
			len++;
		}
        if (actual == '\n') break;
        actual = linea[++posicio];
	}
	
	
	return resultat;
}

void obtainInformation(rb_tree* tree, char* nom_fitxer) {
	//Declaracio de variables del programa.
    char str[200];
    char **cadena;
	char origen[4], desti[4];
    int retard;
    FILE *fp = fopen(nom_fitxer, "r");
	int i;
    node_data *n_data;
	list_data *l_data;

    printf("Anem a carregar un fichers de vols\n");
    if (fp == NULL)
    {
        perror("Error opening file");
        exit (-1);
    }

    fgets (str, 200, fp);
	fgets (str, 200, fp);
	while (fgets(str, 200, fp) != NULL)
	{
		cadena = separar_per_comes (str, 29);
		retard = atoi (cadena[14]);
		strcpy (origen, cadena[16]);
		strcpy (desti, cadena[17]);
		/* Search if the key is in the tree */
		n_data = find_node(tree, origen);
		if (n_data != NULL) {
			l_data = find_list (n_data->l, desti);
			if (l_data != NULL) {
				l_data->num_times++;
				l_data->retard += retard;
			} else {
                n_data->num_times++;
				l_data = malloc (sizeof (list_data));
				l_data->key = malloc (4*sizeof (char));
				strcpy (l_data->key, desti);
                l_data->key[3] = 0;
				l_data->num_times = 1;
				l_data->retard = retard;
				insert_list(n_data->l, l_data);
			}
		} else {
			n_data = malloc (sizeof (node_data));
            n_data->num_times = 1;
			n_data->key = malloc ((strlen(origen))*sizeof (char));
			strcpy (n_data->key, origen);
			n_data->key[3] = '\0';
			n_data->l = (list*) malloc (sizeof (list));
			init_list (n_data->l);
			insert_node (tree, n_data);
			l_data = malloc (sizeof (list_data));
			l_data->key = malloc (4*sizeof (char));
			strcpy (l_data->key, desti);
			l_data->num_times = 1;
			l_data->retard = retard;
			insert_list(n_data->l, l_data);
		}
		for (i = 0; i < 29; i++) free (cadena[i]);
		free (cadena);
    }
    printf("\nFitxer llegit correctament...\n");
	fclose (fp);
}

node_data* nou_node_t (char* key) {
	node_data* n_data = malloc (sizeof (node_data));
	n_data->key = malloc ((strlen(key)+3)*sizeof (char));
	strcpy (n_data->key, key);
	n_data->key[3] = '\0';
	printf ("%s\n", n_data->key);
	n_data->l = (list*) malloc (sizeof (list));
	init_list (n_data->l);
    return n_data;
}

long int iniciar_aeroports (rb_tree* tree, char* nom_fitxer) {
	char aero[10];
	FILE* f = fopen (nom_fitxer, "r");
    long int num = 0;
	node_data* n_data;
	fgets (aero, 10, f);
	aero[3] = '\0';
	while (fgets (aero, 10, f) != NULL){
        num++;
		aero[3] = '\0';
		n_data = malloc (sizeof (node_data));
		n_data->key = malloc ((strlen(aero)+1)*sizeof (char));
		strcpy (n_data->key, aero);
        n_data->num_times = 0;
		n_data->key[3] = '\0';
		n_data->l = (list*) malloc (sizeof (list));
		init_list (n_data->l);
		insert_node (tree, n_data);
	}
	fclose (f);
    return num;
}


void print_retards (rb_tree* tree, char* aeroport) {
	node_data* n_data = find_node (tree, aeroport);
	list_item* l_item = n_data->l->first;
	list_data* l_data;
	printf ("Retards de l'aeroport %s:\n", aeroport);
	while (l_item != NULL) {
		l_data = l_item->data;
		printf("%s.........%d minuts\n", l_data->key, l_data->retard/l_data->num_times);
		l_item = l_item->next;
	}
}



int menu() 
{
    char str[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(str, 5, stdin);
    opcio = atoi(str); 

    return opcio;
}

void guardar_info_arbre (FILE* f, node* n) {
    node_data* n_data = n->data;
    list_item* l_item = n_data->l->first;
	list_data* l_data;
    
    fwrite (n_data->key, sizeof (char), 3, f);
    fwrite (&(n_data->num_times), sizeof (int), 1, f);
    
	while (l_item != NULL) {
		l_data = l_item->data;
        
        fwrite (l_data->key, sizeof (char), 3, f);
        fwrite (&(l_data->num_times), sizeof (int), 1, f);
        fwrite (&(l_data->retard), sizeof (int), 1, f);
        
		l_item = l_item->next;
	}
	
	if (n->left != NIL) guardar_info_arbre (f, n->left);
    if (n->right != NIL) guardar_info_arbre (f, n->right);
    
}

void trobar_mes_destins (int* max, node* n, char* resultat) {
    int destins = n->data->num_times;
    
    if ((*max) < destins) {
        strcpy (resultat, n->data->key);
        (*max) = destins;
    }
    
    if (n->left != NIL) trobar_mes_destins (max, n->left, resultat);
    if (n->right != NIL) trobar_mes_destins (max, n->right, resultat);
    
}
