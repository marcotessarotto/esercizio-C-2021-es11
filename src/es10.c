#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 *
****esercizio 10****

è dato un array di int chiamato int_array di dimensione int_array_len.

scrivere la funzione che inserisce il valore val alla posizione insert_position
(cioè le celle dalla posizione insert_position in poi vengono spostate verso destra di una posizione
per fare spazio al nuovo valore)
int * insert_into_array(int * array, int array_len, int val, int insert_position)

se insert_position < 0 || insert_position > int_array_len la funzione restituisce NULL
se insert_position == int_array_len, il valore da aggiungere viene messo in coda all'array
la funzione restituisce l'indirizzo dell'array modificato

es:
int int_array[] = {0,1,2,3,4,5};
int int_array_len = 6;

int * result = insert_into_array(int_array, int_array_len, -1, 3);
// insert_into_array restituisce un array di dimensione 6+1 fatto così:
// {0,1,2, -1, 3,4,5}

// insert_into_array(int_array, int_array_len, -1, 0);
// {-1, 0,1,2,3,4,5}

// insert_into_array(int_array, int_array_len, -1, 6);
// {0,1,2,3,4,5,-1}

 */

int * insert_into_sorted_array(int * array, int array_len, int val, int insert_position) {
	if (array == NULL) {
		return NULL;
	}

	// se insert_position < 0 || insert_position > int_array_len la funzione restituisce NULL
	if (insert_position < 0 || insert_position > array_len) {
		return NULL;
	}

	int * result = malloc((array_len+1) * sizeof(int));
	if (result == NULL) {
		perror("malloc");
		return NULL;
	}

	// se insert_position == int_array_len, il valore da aggiungere viene messo in coda all'array
	if (insert_position == array_len) {
		// copiamo tutti i valori ed in coda aggiungiamo val
		memcpy(result, array, array_len * sizeof(int));

		result[array_len] = val;

		return result;
	}

	memcpy(result, array, insert_position * sizeof(int));

	result[insert_position] = val;

	memcpy(&result[insert_position+1], &array[insert_position], (array_len-insert_position)*sizeof(int));


	//la funzione restituisce l'indirizzo dell'array modificato
	return result;
}

