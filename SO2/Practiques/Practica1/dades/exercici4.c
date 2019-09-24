#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int compara(const void *p1, const void *p2)
{
  char *str1, *str2;
  str1 = *((char **)p1);
  str2 = *((char **)p2);
  int string1, string2;
  string1 = strlen(str1);
  string2 = strlen(str2);
  if (string1 > string2)
    return -1;
  if (string1 < string2)
    return 1;
  return 0;
}
int main()
{
  int i = 0;
  char **vector = (char **) malloc(sizeof(char) * 10001);
  FILE *fp;
  /* opening file for reading */
  fp = fopen("strings.txt", "r");
  char line[100];
  if (fp == NULL)
  {
    perror("Error opening file");
    return (-1);
  }
  while (fgets(line, 10001, fp) != NULL)
  {
    /* writing content to stdout */
    int palabra = strlen(line);
    printf(vector[i]);
    vector[i] = malloc(sizeof(char) * palabra);

    if (vector[i] == NULL)
    {
      printf("No he pogut reservar la memòria\n");
      exit(1);
    }

    for(int j = 0; j < palabra; j++){
    vector[i][j] = palabra;
    }

    
    for(int j=0; j<palabra-1;j++){
        vector[i][j]= line[j];
    }
  }
  fclose(fp);
  qsort(vector, 10001, sizeof(char *), compara);
  printf("El vector ordenat és ");
  for (i = 0; i < 10001; i++)
    printf("%d ", vector[i]);
  printf("\n");
  free(vector);
  return (0);
}