#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transforma.h"

bool test1(bool debug);
bool test2(bool debug);
bool test(AFND *afnd, AFND *afd, int n, char *cadena);
bool procesar_entrada(AFND *afnd, char *cadena);

int main(int argc, char **argv) {
	bool debug;

	if (argc > 1 && strcmp(argv[1], "--debug") == 0) debug = true;
	else debug = false;
	
	if (!test1(debug)) return 0;
	if (!test2(debug)) return 0;

	return 0;
}

bool test1(bool debug) {
	AFND *afnd;
	AFND *afd;

	afnd = AFNDNuevo("af11", 6, 3);
	
	AFNDInsertaSimbolo(afnd, "+");
	AFNDInsertaSimbolo(afnd, "0");
	AFNDInsertaSimbolo(afnd, ".");
	
	AFNDInsertaEstado(afnd, "q0", INICIAL);
	AFNDInsertaEstado(afnd, "q1", NORMAL);
	AFNDInsertaEstado(afnd, "q2", NORMAL);
	AFNDInsertaEstado(afnd, "q3", NORMAL);
	AFNDInsertaEstado(afnd, "q4", NORMAL);
	AFNDInsertaEstado(afnd, "q5", FINAL);
	
	AFNDInsertaTransicion(afnd, "q0", "+", "q1");
	AFNDInsertaTransicion(afnd, "q1", "0", "q1");
	AFNDInsertaTransicion(afnd, "q1", "0", "q4");
	AFNDInsertaTransicion(afnd, "q1", ".", "q2");
	AFNDInsertaTransicion(afnd, "q2", "0", "q3");
	AFNDInsertaTransicion(afnd, "q3", "0", "q3");
	AFNDInsertaTransicion(afnd, "q4", ".", "q3");
	
	AFNDInsertaLTransicion(afnd, "q0", "q1");
	AFNDInsertaLTransicion(afnd, "q3", "q5");
	AFNDCierraLTransicion(afnd);

	afd = AFNDTransforma(afnd, debug);

	printf("Test 1 (autómata del enunciado):\n");
	if (!test(afnd, afd, 1, "0.0")) return false;
	if (!test(afnd, afd, 2, ".0")) return false;
	if (!test(afnd, afd, 3, "0.")) return false;
	if (!test(afnd, afd, 4, ".")) return false;
	if (!test(afnd, afd, 5, "0")) return false;
	printf("Test 1 ejecutado correctamente.\n");

	AFNDElimina(afd);
	AFNDElimina(afnd);

	return true;
}

bool test2(bool debug) {
	AFND *afnd;
	AFND *afd;

	afnd = AFNDNuevo("af11", 6, 4);
	
	AFNDInsertaSimbolo(afnd, "+");
	AFNDInsertaSimbolo(afnd, "-");
	AFNDInsertaSimbolo(afnd, "0");
	AFNDInsertaSimbolo(afnd, ".");
	
	AFNDInsertaEstado(afnd, "q0", INICIAL);
	AFNDInsertaEstado(afnd, "q1", NORMAL);
	AFNDInsertaEstado(afnd, "q2", NORMAL);
	AFNDInsertaEstado(afnd, "q3", NORMAL);
	AFNDInsertaEstado(afnd, "q4", NORMAL);
	AFNDInsertaEstado(afnd, "q5", FINAL);
	
	AFNDInsertaTransicion(afnd, "q0", "+", "q1");
	AFNDInsertaTransicion(afnd, "q0", "-", "q1");
	AFNDInsertaTransicion(afnd, "q1", "0", "q1");
	AFNDInsertaTransicion(afnd, "q1", ".", "q2");
	AFNDInsertaTransicion(afnd, "q1", "0", "q4");
	AFNDInsertaTransicion(afnd, "q2", "0", "q3");
	AFNDInsertaTransicion(afnd, "q3", "0", "q3");
	
	AFNDInsertaLTransicion(afnd, "q0", "q1");
	AFNDInsertaLTransicion(afnd, "q3", "q5");
	AFNDCierraLTransicion(afnd);

	afd = AFNDTransforma(afnd, debug);

	printf("Test 2 (autómata de las diapositivas):\n");
	if (!test(afnd, afd, 1, "0.0")) return false;
	if (!test(afnd, afd, 2, "0.")) return false;
	if (!test(afnd, afd, 3, "0")) return false;
	if (!test(afnd, afd, 4, ".")) return false;
	if (!test(afnd, afd, 5, "+0.0")) return false;
	if (!test(afnd, afd, 6, "+0.")) return false;
	if (!test(afnd, afd, 7, "+0")) return false;
	if (!test(afnd, afd, 8, "+.")) return false;
	if (!test(afnd, afd, 9, "-0.0")) return false;
	if (!test(afnd, afd, 10, "-0.")) return false;
	if (!test(afnd, afd, 11, "-0")) return false;
	if (!test(afnd, afd, 12, "-.")) return false;
	printf("Test 2 ejecutado correctamente.\n");

	AFNDElimina(afd);
	AFNDElimina(afnd);

	return true;
}

bool test(AFND *afnd, AFND *afd, int n, char *cadena) {
	bool acepta_afnd, acepta_afd;
	acepta_afnd = procesar_entrada(afnd, cadena);
	acepta_afd = procesar_entrada(afd, cadena);
	if (acepta_afnd) {
		if (acepta_afd) {
			printf(" -> Cadena %d (%s): ambos aceptan.\n", n, cadena);
		} else {
			printf(" -> Cadena %d (%s): AFND acepta y AFD rechaza. ERROR.\n", n, cadena);
		}
	} else {
		if (acepta_afd) {
			printf(" -> Cadena %d (%s): ambos aceptan.\n", n, cadena);
		} else {
			printf(" -> Cadena %d (%s): ambos rechazan.\n", n, cadena);
		}
	}
	if (acepta_afnd != acepta_afd) printf("Saliendo por fallo\n");
	return acepta_afnd == acepta_afd;
}

bool procesar_entrada(AFND *afnd, char* cadena) {
	FILE *file;
	char letra;
	char aux[2];
	bool es_final;
	int len, i;

	AFNDInicializaEstado(afnd);
	AFNDInicializaCadenaActual(afnd);

	len = strlen(cadena);
	aux[1] = '\0';
	for (i = 0; i < len; i++) {
		aux[0] = cadena[i];
		AFNDInsertaLetra(afnd, aux);
	}
	
	for (i = 0; i < len; i++) {
		AFNDTransita(afnd);
	}

	file = fopen("estados_aux.txt", "w");
	AFNDImprimeConjuntoEstadosActual(file, afnd);
	fclose(file);

	file = fopen("estados_aux.txt", "r");
	AFNDImprimeConjuntoEstadosActual(file, afnd);

	es_final = false;
	while ((letra = fgetc(file)) != EOF) {
		if (letra == '*') {
			es_final = true;
			break;
		}
    }
	
	fclose(file);
	return es_final;
}