#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


#define NUMERO_DI_CICLI_DEFAULT 10
#define INTERVALLO_DEFAULT 5

#define DEBUG

// tratto da:
// https://github.com/marcotessarotto/exOpSys/blob/91cd55d7f17402f15bbf6b0649956270ea5e39f6/004.4pass-array-to-function/pass-array-to-function.c#L55
double calculate_average2_double(double * data, int data_len) {
	// utilizza incremental average:
	// m(n) = m(n-1) + (a(n) - m(n-1)) / n

	double d = 0.0;

	for (int i = 0; i < data_len; i++) {
		d = d + (data[i] - d) / (i+1);
	}

	return d;
}


int main(int argc, char * argv[]) {

	int numero_di_cicli = NUMERO_DI_CICLI_DEFAULT;
	int intervallo = INTERVALLO_DEFAULT;
	int res;
	int log_fd;
	char * log_filename = "log.txt";
	double * valori_load; // punterà ad array dei valori raccolti nel ciclo

	double loadavg[3];
	char buffer[32];

	/*

	il programma prevede come parametri a linea di comando due valori (int):
	numero di cicli (default: 10)
	intervallo (secondi) (default: 5)

	se i parametri non sono forniti, usare i valori di default indicati sopra.


	 */


	if (argc >= 2) {

		res = sscanf(argv[1], "%u", &numero_di_cicli);

		if (res == 0) {
			printf("errore nell'interpretazione del parametro '%s'\n", argv[1]);
			exit(1);
		}

		if (numero_di_cicli <= 0)
			numero_di_cicli = NUMERO_DI_CICLI_DEFAULT;
	}

	if (argc >= 3) {

		res = sscanf(argv[2], "%u", &intervallo);

		if (res == 0) {
			printf("errore nell'interpretazione del parametro '%s'\n", argv[2]);
			exit(1);
		}

		if (intervallo < 0)
			intervallo = INTERVALLO_DEFAULT;

	}

/*

crea il file log.txt per scriverci di dati ottenuti sotto; il file viene sovrascritto, se esiste già.
 *
 */

	log_fd = open(log_filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (log_fd == -1) {
		perror("open");
		exit(1);
	}

	valori_load = malloc(sizeof(double) * numero_di_cicli);
	if (valori_load == NULL) {
		perror("malloc()");
		exit(1);
	}

	for (int i = 0; i < numero_di_cicli; i++) {

		if (getloadavg(loadavg, 3) == -1) {
			perror("getloadavg()");
			exit(1);
		}

		valori_load[i] = loadavg[0];

		// ad ogni interazione scrive il valore in formato testo all'interno del file 'log.txt'.
		sprintf(buffer, "%.3f\n", loadavg[0]);
		res = write(log_fd, buffer, strlen(buffer));
		if (res < 0) {
			perror("write");
			exit(1);
		}

		printf("interazione #%d: %s", i, buffer);

#ifndef DEBUG
		sleep(intervallo);
#endif

	}

/*

alla fine del ciclo:
- il processo padre crea un processo figlio che calcola e scrive su stdout il valor medio dei valori di
	'load average over 1 minute' ottenuti nel ciclo sopra
- il processo figlio scrive il valor medio calcolato in un file 'result.txt' ; il numero è scritto in formato testo
- il processo padre aspetta la terminazione del processo figlio; poi legge il valore calcolato
	dal processo figlio dal file 'result.txt'

 */

	int pid;
	double valor_medio;
	int result_fd;
	char * result_filename = "result.txt";

	switch (pid = fork()) {
	case -1:
		perror("fork()");
		exit(1);
	case 0: // child process
		printf("[child]inizio\n");

		valor_medio = calculate_average2_double(valori_load, numero_di_cicli);

		result_fd = open(result_filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		if (result_fd == -1) {
			perror("open");
			exit(1);
		}

		sprintf(buffer, "%.3f\n", valor_medio);
		res = write(result_fd, buffer, strlen(buffer));
		if (res < 0) {
			perror("write");
			exit(1);
		}

		printf("[child]valor_medio=%lf\n", valor_medio);

		printf("[child]fine\n");

		exit(0); // fine processo figlio
	default:
		;

	}

	res = wait(NULL); // processo padre aspetta la terminazione del processo figlio
	if (res == -1) {
		perror("wait()");
		exit(1);
	}

	printf("[parent]dopo wait()\n");

	result_fd = open(result_filename, O_RDWR);
	if (result_fd == -1) {
		perror("open");
		exit(1);
	}

	res = read(result_fd, buffer, sizeof(buffer));
	if (res < 0) {
		perror("read");
		exit(1);
	} else if (res == 0) { // EOF ???
		printf("problema: non c'è il risultato...\n");
		exit(1);
	}

	// dobbiamo terminare con \0 i dati che abbiamo ricevuto con read():
	buffer[res] = 0;

	res = sscanf(buffer, "%lf", &valor_medio);
	if (res == 0) {
		printf("sscanf non è riuscito ad interpretare il contenuto del file\n");
		exit(1);
	}

	printf("[parent]valore ricevuto da processo figlio: %lf\n", valor_medio);

/*
seconda parte dell'esercizio:

il processo padre fa il sort crescente dei valori rilevati nel ciclo principale;
poi inserisce il va
 */

    return 0;
}

