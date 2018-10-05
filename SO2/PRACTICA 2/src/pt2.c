#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "red-black-tree.h"
#include "linked-list.h"

#define MAXCHAR  100

void obtainInformation(){
	    //Declaracio de variables del programa.
    char str[100];
    char *cadena;
    int maxLine;
    FILE *fp = fopen("dades/aeroports.csv", "r");

    rb_tree *tree;
    node_data *n_data;

    printf("Anem a carregar un fichers de vols");
    if (fp == NULL)
    {
        perror("Error opening file");
        return (-1);
    }

    /* Allocate memory for tree */
    tree = (rb_tree *)malloc(sizeof(rb_tree));

    /* Initialize the tree */
    init_tree(tree);

    if (fgets(str, 100, fp) != NULL)
    {
        maxLine = atoi(str);
        //cadena = malloc(sizeof(char*) * maxLine);
        while (fgets(str, 100, fp) != NULL)
        {
            int i = 0;
            int len = strlen(str);
            str[len - 1] = '\0';
            cadena = malloc(sizeof(char) * len);
            strcpy(cadena, str);
            printf("\n%s", cadena);
            /* Search if the key is in the tree */
            n_data = find_node(tree, cadena);
            if (n_data != NULL)
            {
                /* If the key is in the tree increment 'num' */
                n_data->num_vegades++;
            }
            else
            {
                /* If the key is not in the tree, allocate memory for the data
                * and insert in the tree */

                n_data = malloc(sizeof(node_data));

                /* This is the key by which the node is indexed in the tree */
                n_data->key = cadena;

                /* This is additional information that is stored in the tree */
                n_data->num_vegades = 1;

                /* We insert the node in the tree */
                insert_node(tree, n_data);
            }
            i++;
        }
    }
    printf("\nFitxer llegit correctament...\n");

}
int main () {
	obtainInformation();
	return 0;
}