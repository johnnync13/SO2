#pragma once

#include <pthread.h>

#include "linked-list.h"
#include "red-black-tree.h"

#define MAXCHAR 500

#define N_LINES 10000

#define ATRASO_LLEGADA_AEROPUERTO 15
#define AEROPUERTO_ORIGEN 17
#define AEROPUERTO_DESTINO 18

typedef struct flight_information {
    char origin[4];
    char destination[4];
    int delay;
} flight_information;

rb_tree *tree;

/* Clave para la lectura del fichero */
pthread_mutex_t file_mutex;

rb_tree *create_tree(char *str_airports, char *str_dades);
void read_airports(rb_tree *tree, FILE *fp);
void *read_airports_data(void *fp);
