/* Johnny Nuñez Cano
 * Joan Travé Gordillo
 * Ordena un array de strings en ordre alfabetic invers
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compara (const void *p1, const void *p2) {
	int len1, len2;
	len1 = strlen (*((char **) p1));
	len2 = strlen (*((char **) p2));
	if (len1 > len2) return -1;
	else if (len1 < len2) return 1;
	else return 0;
}

int main () {
	FILE *fitxer;
	int i, numEntrades;
	char **vector;
	char aux[30];
	fitxer = fopen ("strings.txt", "r");
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
	fitxer = fopen ("stringOredenatLlarg.txt", "w");
	for (i = 0; i < numEntrades; i++) fprintf (fitxer, "%s\n", vector[i]);
	fclose (fitxer);
	for (i = 0; i < numEntrades; i++) free (vector[i]);
	free (vector);
	return 0;
}
	
