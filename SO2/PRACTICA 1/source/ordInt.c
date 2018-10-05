/* Joan Trave Gordillo
 * Johnny Nunez Cano
 * Ordena de major a menor un array de enters
 */
#include <stdio.h>
#include <stdlib.h>

int compara (const void *p1, const void *p2) {
	int *num1, *num2;
	num1 = (int *) p1;
	num2 = (int *) p2;
	if (*num1 > *num2) return -1;
	else if (*num1 < *num2) return 1;
	else return 0;
}

int main () {
	FILE *fitxer;
	int i, numEntrades, *vector;
	char aux[100];
	fitxer = fopen ("integers.txt", "r");
	fgets (aux, 100, fitxer);
	numEntrades = atoi (aux);
	vector = (int*) malloc (numEntrades * sizeof (int));
	for (i = 0; i < numEntrades; i++) {
		fgets (aux, 100, fitxer);
		vector[i] = atoi (aux);
	}
	fclose (fitxer);
	qsort (vector, numEntrades, sizeof (int), compara);
	fitxer = fopen ("intOrdenat.txt", "w");
	for (i = 0; i < numEntrades; i++) fprintf (fitxer, "%d\n", vector[i]);
	fclose (fitxer);
	free (vector);
	return 0;
}
    




