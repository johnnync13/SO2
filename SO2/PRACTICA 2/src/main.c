#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "red-black-tree.h"
#include "linked-list.h"

#define MAXCHAR  100


char** separar_per_comes (char* linea, int nMax);
void obtainInformation(rb_tree* tree, char* nom_fitxer);
node_data* nou_node_t (char* key);
void iniciar_aeroports (rb_tree* tree, char* nom_fitxer);
void print_retards (rb_tree* tree, char* aeroport);


int main (int argc, char** argv) {
	rb_tree* tree;
	
	if (argc != 4) perror ("Falten o sobren arguments\n");
	
	/* Allocate memory for tree */
    tree = (rb_tree *)malloc(sizeof(rb_tree));
    /* Initialize the tree */
    init_tree(tree);
	
	iniciar_aeroports (tree, argv[2]);
	obtainInformation(tree, argv[1]);
	print_retards (tree, argv[3]);
	
	delete_tree (tree);
	free (tree);
	return 0;
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
				l_data = malloc (sizeof (list_data));
				l_data->key = malloc (4*sizeof (char));
				strcpy (l_data->key, desti);
				l_data->num_times = 1;
				l_data->retard = retard;
				insert_list(n_data->l, l_data);
			}
		} else {
			n_data = malloc (sizeof (node_data));
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


void iniciar_aeroports (rb_tree* tree, char* nom_fitxer) {
	char aero[10];
	FILE* f = fopen (nom_fitxer, "r");
	node_data* n_data;
	fgets (aero, 10, f);
	aero[3] = '\0';
	while (fgets (aero, 10, f) != NULL){
		aero[3] = '\0';
		n_data = malloc (sizeof (node_data));
		n_data->key = malloc ((strlen(aero)+1)*sizeof (char));
		strcpy (n_data->key, aero);
		n_data->key[3] = '\0';
		n_data->l = (list*) malloc (sizeof (list));
		init_list (n_data->l);
		insert_node (tree, n_data);
	}
	fclose (f);
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
