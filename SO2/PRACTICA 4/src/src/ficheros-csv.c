
/* Codigo escrito por Lluis Garrido */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "ficheros-csv.h"

#define N 10000
#define THREADS 8

/**
 *
 * Esta funcion crea el arbol a partir de los datos de los aeropuertos y de los ficheros de retardo
 *
 */
rb_tree *create_tree(char *str_airports, char *str_dades)
{
    FILE *fp;
    int err;
    pthread_t ntid[THREADS];
    int num_thread;
    char head[MAXCHAR];
    
    bool final_fichero = false;
    void *check_hilos[THREADS];
    /*
     * final_fichero: true si hemos leido todo el fichero
     * check_hilos: true la posicion correspondiente al hilo que ha encontrado
     *              el final del fichero
     */

    /* Reservamos memoria */ 
    tree = (rb_tree *) malloc(sizeof(rb_tree));

    /* Abrimos el fichero con la lista de aeropuertos */
    fp = fopen(str_airports, "r");
    if (!fp) {
        printf("Could not open file '%s'\n", str_airports);
        exit(EXIT_FAILURE);
    }

    /* Leemos los datos de ficheros de aeropuertos */ 
    init_tree(tree);
    read_airports(tree, fp); 
    fclose(fp);
    
    /* Abrimos el fichero con los datos de los vuelos */
    fp = fopen(str_dades, "r");
    if (!fp) {
        printf("Could not open file '%s'\n", str_dades);
        exit(EXIT_FAILURE);
    }
    
    /* Inicializamos la clave del fichero de datos con los vuelos */
    pthread_mutex_init (&file_mutex, NULL);
    
    /* Leemos la cabecera del fichero */
    fgets(head, MAXCHAR, fp);
    
    struct timeval tv1, tv2;

    /* Tiempo cronologico */
    gettimeofday(&tv1, NULL);
    
    /* Hasta que no lleguemos al final del fichero ejecutamos */
    while (!final_fichero)
    {
        
        /* Creamos tantos hilos como hayamos definido */
        for (num_thread = 0; num_thread < THREADS; num_thread++)
        {
            
            err = pthread_create (&ntid[num_thread], NULL, read_airports_data, (void *) fp);
            if (err != 0) {
                printf ("ERROR con hilos\n");
                exit (-1);
            }
            
        }
        
        /* Esperamos a que finalizen todos los hilos */
        for (num_thread = 0; num_thread < THREADS; num_thread++)
        {
            pthread_join (ntid[num_thread], &check_hilos[num_thread]);
        }
        
        /* Para cada hilo, comprobamos si alguno ha resultado */
        for (num_thread = 0; num_thread < THREADS; num_thread++)
            final_fichero |= (bool) check_hilos[num_thread];
        
        
    }
    
    /* Tiempo cronologico */
    gettimeofday(&tv2, NULL);

    /* Tiempo para la creacion del arbol */
    printf("Tiempo para crear el arbol: %f segundos\n",
            (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec));
    
    fclose(fp);

    return tree;
}


/**
 *
 * Esta funcion lee la lista de los aeropuertos y crea el arbol 
 *
 */

void read_airports(rb_tree *tree, FILE *fp) 
{
    int i, num_airports;
    char line[MAXCHAR];

    /*
     * eow es el caracter de fin de palabra
     */
    char eow = '\0';

    node_data *n_data;

    fgets(line, 100, fp);
    num_airports = atoi(line);

    i = 0;
    while (i < num_airports)
    {
        fgets(line, 100, fp);
        line[3] = eow; 

        /* Reservamos memoria para el nodo */
        n_data = malloc(sizeof(node_data));

        /* Copiamos los datos al nodo */
        n_data->key = malloc(sizeof(char) * 4);
        strcpy(n_data->key, line);

        /* Inicializamos la lista */
        n_data->l = malloc(sizeof(list));
        init_list(n_data->l); 
        
        /* Asignamos la clave de bloqueo */
        pthread_mutex_init (&(n_data->my_mutex), NULL);

        /* Suponemos que los nodos son unicos, no hace falta
         * comprobar si ya existen previamente.
         */
        insert_node(tree, n_data);

        i++;
    }
}

/**
 * Función que permite leer todos los campos de la línea de vuelo: origen,
 * destino, retardo.
 * 		
 */

static int extract_fields_airport(char *line, flight_information *fi) {

    /*Recorre la linea por caracteres*/
    char caracter;
    /* i sirve para recorrer la linea
     * iterator es para copiar el substring de la linea a char
     * coma_count es el contador de comas
     */
    int i, iterator, coma_count;
    /* start indica donde empieza el substring a copiar
     * end indica donde termina el substring a copiar
     * len indica la longitud del substring
     */
    int start, end, len;
    /* invalid nos permite saber si todos los campos son correctos
     * 1 hay error, 0 no hay error pero no hemos terminado
     */
    int invalid = 0;
    /*
     * eow es el caracter de fin de palabra
     */
    char eow = '\0';
    /*
     * contenedor del substring a copiar
     */
    char *word;
    /*
     * Inicializamos los valores de las variables
     */
    start = 0;
    end = -1;
    i = 0;
    coma_count = 0;
    /*
     * Empezamos a contar comas
     */
    do {
        caracter = line[i++];
        if (caracter == ',') {
            coma_count ++;
            /*
             * Cogemos el valor de end
             */
            end = i;
            /*
             * Si es uno de los campos que queremos procedemos a copiar el substring
             */
            if(coma_count == ATRASO_LLEGADA_AEROPUERTO || 
                    coma_count == AEROPUERTO_ORIGEN || 
                    coma_count == AEROPUERTO_DESTINO){
                /*
                 * Calculamos la longitud, si es mayor que 1 es que tenemos 
                 * algo que copiar
                 */
                len = end - start;
                if (len > 1) {
                    /*
                     * Alojamos memoria y copiamos el substring
                     */
                    word =(char*)malloc(sizeof(char)*(len));
                    for(iterator = start; iterator < end-1; iterator ++){
                        word[iterator-start] = line[iterator];
                    }
                    /*
                     * Introducimos el caracter de fin de palabra
                     */
                    word[iterator-start] = eow;
                    /*
                     * Comprobamos que el campo no sea NA (Not Available) 
                     */
                    if (strcmp("NA", word) == 0)
                        invalid = 1;
                    else {
                        switch (coma_count) {
                            case ATRASO_LLEGADA_AEROPUERTO:
                                fi->delay = atoi(word);
                                break;
                            case AEROPUERTO_ORIGEN:
                                strcpy(fi->origin, word);
                                break;
                            case AEROPUERTO_DESTINO:
                                strcpy(fi->destination, word);
                                break;
                            default:
                                printf("ERROR in coma_count\n");
                                exit(1);
                        }
                    }

                    free(word);

                } else {
                    /*
                     * Si el campo esta vacio invalidamos la linea entera 
                     */

                    invalid = 1;
                }
            }
            start = end;
        }
    } while (caracter && invalid==0);

    return invalid;
}

/**
 *
 * Esta funcion lee los datos de los vuelos y los inserta en el 
 * arbol (previamente creado)
 *
 */

void *read_airports_data (void *fp) {
    int invalid, i;
    int num_lineas_leidas = 0;
    char **lineas_informacion;
    
    /* Reservamos memoria para las lineas que leeremos */
    lineas_informacion = (char **) malloc (N * sizeof (char *));
    if (lineas_informacion == NULL)
    {
        printf ("Error pointer\n");
        exit (-1);
    }
    for (i = 0; i < N; i++)
    {
        lineas_informacion[i] = (char *) malloc (MAXCHAR * sizeof (char));
        if (lineas_informacion[i] == NULL)
        {
            printf ("Error pointer\n");
            exit (-1);
        }
    }
    
    fp = (FILE *) fp;

    flight_information fi;

    node_data *n_data;
    list_data *l_data;
    
    /* Bloqueamos el fichero para que ningun otro hilo pueda acceder a la misma
     * informacion que nosotros a la vez
     */
    pthread_mutex_lock (&file_mutex);
    //printf ("Locking file...\n");
    
    /* Leemos todas las lineas asociadas a nuestro hilo */
    for (num_lineas_leidas = 0; num_lineas_leidas < N; num_lineas_leidas++)
    {
        if (fgets(lineas_informacion[num_lineas_leidas], MAXCHAR, fp) == NULL)
            break;
    }
    
    //printf ("Lineas leidas: %d\n", num_lineas_leidas);
    
    /* Desbloqueamos el fichero de datos para que otros hilos puedan acceder
     * a la informacion
     */
    pthread_mutex_unlock (&file_mutex);
        
    /* Procesamos los datos leidos */
    for (i = 0; i < num_lineas_leidas; i++)
    {
        
        // printf ("%s\n", lineas_informacion[i]);
        
        invalid = extract_fields_airport(lineas_informacion[i], &fi);
        free (lineas_informacion[i]);

        if (!invalid) {
            n_data = find_node(tree, fi.origin);
            
            /* Bloqueamos el nodo para que ningun otro hilo pueda
             * sobreescribir datos
             */
            pthread_mutex_lock (&(n_data->my_mutex));

            if (n_data) {
                l_data = find_list(n_data->l, fi.destination);

                if (l_data) {
                    l_data->numero_vuelos += 1;
                    l_data->retardo_total += fi.delay;
                } else {
                    l_data = malloc(sizeof(list_data));

                    l_data->key = malloc(sizeof(char) * 4);
                    strcpy(l_data->key, fi.destination);

                    l_data->numero_vuelos = 1;
                    l_data->retardo_total = fi.delay; 

                    insert_list(n_data->l, l_data);
                }

            } else {
                printf("ERROR: aeropuerto %s no encontrado en el arbol.\n", fi.origin);
                exit(1);
            }
            
            /* Liberamos el nodo para que otros hilos puedan acceder a él */
            pthread_mutex_unlock (&(n_data->my_mutex));
            
        }
    }
    
    /* Liberamos el vector de informacion leida */
    free (lineas_informacion);
    
    bool check_final = !(num_lineas_leidas == N);
    
    //printf ("Final? %d\n", check_final);
    
    return (void *) check_final;
}



