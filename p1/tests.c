#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transforma.h"

bool test1(bool debug);
bool test2(bool debug);
bool test3(bool debug);
bool test(AFND *afnd, AFND *afd, int *n, char *cadena);
bool procesar_entrada(AFND *afnd, char *cadena);

int main(int argc, char **argv) {
	bool debug;

	if (argc > 1 && strcmp(argv[1], "--debug") == 0) debug = true;
	else debug = false;

	printf("Ejecutando los tests.\n\n");
	
	if (!test1(debug)) return 0;
	if (!test2(debug)) return 0;
	if (!test3(debug)) return 0;

	printf("Todos los tests se han ejecutado correctamente.\n");

	return 0;
}

bool test1(bool debug) {
	AFND *afnd;
	AFND *afd;
	int n;

	afnd = AFNDNuevo("test1", 6, 3);
	
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

	n = 1;
	printf("Test 1 (autómata del enunciado):\n");
	if (!test(afnd, afd, &n, "0.0")) return false;
	if (!test(afnd, afd, &n, ".0")) return false;
	if (!test(afnd, afd, &n, "0.")) return false;
	if (!test(afnd, afd, &n, ".")) return false;
	if (!test(afnd, afd, &n, "0")) return false;
	if (!test(afnd, afd, &n, "+0.0")) return false;
	if (!test(afnd, afd, &n, "+.0")) return false;
	if (!test(afnd, afd, &n, "+0.")) return false;
	if (!test(afnd, afd, &n, "+.")) return false;
	if (!test(afnd, afd, &n, "+0")) return false;
	if (!test(afnd, afd, &n, "+000000.00000000")) return false;
	printf("Test 1 ejecutado correctamente.\n");

	AFNDADot(afnd);
	AFNDADot(afd);
	printf("\n");

	AFNDElimina(afd);
	AFNDElimina(afnd);

	return true;
}

bool test2(bool debug) {
	AFND *afnd;
	AFND *afd;
	int n;

	afnd = AFNDNuevo("test2", 6, 4);
	
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

	n = 1;
	printf("Test 2 (autómata de las diapositivas):\n");
	if (!test(afnd, afd, &n, "0.0")) return false;
	if (!test(afnd, afd, &n, "0.")) return false;
	if (!test(afnd, afd, &n, "0")) return false;
	if (!test(afnd, afd, &n, ".")) return false;
	if (!test(afnd, afd, &n, "+0.0")) return false;
	if (!test(afnd, afd, &n, "+0.")) return false;
	if (!test(afnd, afd, &n, "+0")) return false;
	if (!test(afnd, afd, &n, "+.")) return false;
	if (!test(afnd, afd, &n, "-0.0")) return false;
	if (!test(afnd, afd, &n, "-0.")) return false;
	if (!test(afnd, afd, &n, "-0")) return false;
	if (!test(afnd, afd, &n, "-.")) return false;
	if (!test(afnd, afd, &n, "-000000.00000000000")) return false;
	printf("Test 2 ejecutado correctamente.\n");

	AFNDADot(afnd);
	AFNDADot(afd);
	printf("\n");

	AFNDElimina(afd);
	AFNDElimina(afnd);

	return true;
}

bool test3(bool debug) {
	AFND *afnd;
	AFND *afd;
	int n;

	afnd = AFNDNuevo("test3", 5, 2);
	
	AFNDInsertaSimbolo(afnd, "0");
	AFNDInsertaSimbolo(afnd, "1");
	
	AFNDInsertaEstado(afnd, "q0", INICIAL);
	AFNDInsertaEstado(afnd, "q1", NORMAL);
	AFNDInsertaEstado(afnd, "q2", NORMAL);
	AFNDInsertaEstado(afnd, "q3", NORMAL);
	AFNDInsertaEstado(afnd, "q4", FINAL);
	
	AFNDInsertaTransicion(afnd, "q0", "0", "q0");
	AFNDInsertaTransicion(afnd, "q0", "1", "q0");
	AFNDInsertaTransicion(afnd, "q0", "1", "q1");
	AFNDInsertaTransicion(afnd, "q1", "0", "q2");
	AFNDInsertaTransicion(afnd, "q1", "1", "q2");
	AFNDInsertaTransicion(afnd, "q2", "0", "q3");
	AFNDInsertaTransicion(afnd, "q2", "1", "q3");
	AFNDInsertaTransicion(afnd, "q3", "0", "q4");
	AFNDInsertaTransicion(afnd, "q3", "1", "q4");

	AFNDCierraLTransicion(afnd);

	afd = AFNDTransforma(afnd, debug);

	n = 1;
	printf("Test 3 (autómata con transformación de gran tamaño):\n");
	if (!test(afnd, afd, &n, "1000")) return false;
	if (!test(afnd, afd, &n, "1111")) return false;
	if (!test(afnd, afd, &n, "1010")) return false;
	if (!test(afnd, afd, &n, "1101")) return false;
	if (!test(afnd, afd, &n, "000")) return false;
	if (!test(afnd, afd, &n, "111")) return false;
	if (!test(afnd, afd, &n, "010")) return false;
	if (!test(afnd, afd, &n, "010")) return false;
	if (!test(afnd, afd, &n, "00000000")) return false;
	if (!test(afnd, afd, &n, "11111111")) return false;
	if (!test(afnd, afd, &n, "00001000")) return false;
	if (!test(afnd, afd, &n, "11110111")) return false;
	printf("Test 3 ejecutado correctamente.\n");

	AFNDADot(afnd);
	AFNDADot(afd);

	AFNDElimina(afd);
	AFNDElimina(afnd);
	printf("\n");

	return true;
}

bool test(AFND *afnd, AFND *afd, int *n, char *cadena) {
	bool acepta_afnd, acepta_afd;
	acepta_afnd = procesar_entrada(afnd, cadena);
	acepta_afd = procesar_entrada(afd, cadena);
	if (acepta_afnd) {
		if (acepta_afd) {
			printf(" -> Cadena %d (%s): ambos aceptan.\n", *n, cadena);
		} else {
			printf(" -> Cadena %d (%s): AFND acepta y AFD rechaza. ERROR.\n", *n, cadena);
		}
	} else {
		if (acepta_afd) {
			printf(" -> Cadena %d (%s): ambos aceptan.\n", *n, cadena);
		} else {
			printf(" -> Cadena %d (%s): ambos rechazan.\n", *n, cadena);
		}
	}
	*n = *n+1;
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
