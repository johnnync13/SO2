#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compara (const void *p1, const void *p2) {
	return strcmp (*((char **) p2), *((char **) p1));
}
int main () {
	FILE *fitxer;
	int i, numEntrades;
	char **vector;
	char aux[30];
	fitxer = fopen ("/dades/aeroports.txt", "r");
	fgets (aux, 30, fitxer);
	numEntrades = atoi (aux);
	vector = (char **) malloc (numEntrades * sizeof (char*));
	for (i = 0; i < numEntrades; i++) {
		fgets (aux, 30, fitxer);
		vector[i] = (char *) malloc (strlen (aux) * sizeof (char));
		strcpy (vector[i], aux);
		vector[i][strlen(aux)-1] = '\0';
	}
	fclose (fitxer);
	qsort (vector, numEntrades, sizeof (char *), compara);
	fitxer = fopen ("stringOredenatContingut.txt", "w");
	for (i = 0; i < numEntrades; i++) fprintf (fitxer, "%s\n", vector[i]);
	fclose (fitxer);
	for (i = 0; i < numEntrades; i++) free (vector[i]);
	free (vector);
	return 0;
}