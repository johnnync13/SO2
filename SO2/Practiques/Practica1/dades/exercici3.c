#include <stdio.h>
#include <stdlib.h>
int compara(const void *p1, const void *p2)
{
  double *num1, *num2;
  num1 = (double *)p1;
  num2 = (double *)p2;
  if (*num1 < *num2)
    return -1;
  if (*num1 > *num2)
    return 1;
  return 0;
}
int main()
{
  int i = 0;
  double *vector = malloc(sizeof(double) * 10001);
  FILE *fp;
  /* opening file for reading */
  fp = fopen("float.txt", "r");
  char line[100];
  if (fp == NULL)
  {
    perror("Error opening file");
    return (-1);
  }
  while (fgets(line, 13, fp) != NULL)
  {
    /* writing content to stdout */
    double number = atof(line);
    vector[i] = number;
    puts(line);
    i++;
  }
  fclose(fp);
  
  qsort(vector, 10001, sizeof(double), compara);
  printf("El vector ordenat és ");
  for (i = 0; i < 10001; i++)
    printf("%lf ", vector[i]);
  printf("\n");
  free(vector);
  return (0);
}