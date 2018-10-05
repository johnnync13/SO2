/* Joan Travé Gordillo
 * Johnny Nunez Cano
 * Ordena un array de double
 */
#include <stdio.h>
#include <stdlib.h>

int compara (const void *p1, const void *p2) {
	double *num1, *num2;
	num1 = (double *) p1;
	num2 = (double *) p2;
	if (*num1 > *num2) return -1;
	else if (*num1 < *num2) return 1;
	else return 0;
}

int main () {
	FILE *fitxer;
	int i, numEntrades;
	double *vector;
	char aux[100];
	fitxer = fopen ("float.txt", "r");
	fgets (aux, 100, fitxer);
	numEntrades = atoi (aux);
	vector = (double*) malloc (numEntrades * sizeof (double));
	for (i = 0; i < numEntrades; i++) {
		fgets (aux, 100, fitxer);
		vector[i] = atof (aux);
	}
	fclose (fitxer);
	qsort (vector, numEntrades, sizeof (double), compara);
	fitxer = fopen ("doubleOrdenat.txt", "w");
	for (i = 0; i < numEntrades; i++) fprintf (fitxer, "%f\n", vector[i]);
	fclose (fitxer);
	free (vector);
	return 0;
}
    
