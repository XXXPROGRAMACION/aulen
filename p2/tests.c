#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "minimiza.h"

bool test1(bool debug);
bool test2(bool debug);
bool test3(bool debug);
bool test(AFND *afd, AFND *afd_minimizado, int *n, char *cadena);
bool procesar_entrada(AFND *afd, char *cadena);

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
	AFND *afd;
	AFND *afd_minimizado;
	int n;

	afd = AFNDNuevo("afd_diapositivas", 8, 2);
	
	AFNDInsertaSimbolo(afd, "0");
	AFNDInsertaSimbolo(afd, "1");
	
	AFNDInsertaEstado(afd, "A", INICIAL);
	AFNDInsertaEstado(afd, "B", NORMAL);
	AFNDInsertaEstado(afd, "C", FINAL);
	AFNDInsertaEstado(afd, "D", NORMAL);
	AFNDInsertaEstado(afd, "E", NORMAL);
	AFNDInsertaEstado(afd, "F", NORMAL);
	AFNDInsertaEstado(afd, "G", NORMAL);
	AFNDInsertaEstado(afd, "H", NORMAL);
	
	AFNDInsertaTransicion(afd, "A", "0", "B");
	AFNDInsertaTransicion(afd, "A", "1", "F");
	AFNDInsertaTransicion(afd, "B", "0", "G");
	AFNDInsertaTransicion(afd, "B", "1", "C");
	AFNDInsertaTransicion(afd, "C", "0", "A");
	AFNDInsertaTransicion(afd, "C", "1", "C");
	AFNDInsertaTransicion(afd, "D", "0", "C");
	AFNDInsertaTransicion(afd, "D", "1", "G");
	AFNDInsertaTransicion(afd, "E", "0", "H");
	AFNDInsertaTransicion(afd, "E", "1", "F");
	AFNDInsertaTransicion(afd, "F", "0", "C");
	AFNDInsertaTransicion(afd, "F", "1", "G");
	AFNDInsertaTransicion(afd, "G", "0", "G");
	AFNDInsertaTransicion(afd, "G", "1", "E");
	AFNDInsertaTransicion(afd, "H", "0", "G");
	AFNDInsertaTransicion(afd, "H", "1", "C");

	afd_minimizado = AFNDMinimiza(afd, debug);

	n = 1;
	printf("Test 1 (autómata del enunciado):\n");
	printf("Reduce de %d a %d estados\n", AFNDNumEstados(afd), AFNDNumEstados(afd_minimizado));
	if (!test(afd, afd_minimizado, &n, "0")) return false;
	if (!test(afd, afd_minimizado, &n, "11")) return false;
	if (!test(afd, afd_minimizado, &n, "000")) return false;
	if (!test(afd, afd_minimizado, &n, "0011")) return false;
	if (!test(afd, afd_minimizado, &n, "01")) return false;
	if (!test(afd, afd_minimizado, &n, "011")) return false;
	if (!test(afd, afd_minimizado, &n, "01001")) return false;
	if (!test(afd, afd_minimizado, &n, "00101")) return false;
	printf("Test 1 ejecutado correctamente.\n");

	AFNDADot(afd);
	AFNDADot(afd_minimizado);
	printf("\n");

	AFNDElimina(afd_minimizado);
	AFNDElimina(afd);

	return true;
}

bool test2(bool debug) {
	AFND *afd;
	AFND *afd_minimizado;
	int n;
	
	afd = AFNDNuevo("afd_(0)*11_con_inaccesibles", 5, 2);
	
	AFNDInsertaSimbolo(afd, "0");
	AFNDInsertaSimbolo(afd, "1");
	
	AFNDInsertaEstado(afd, "A", INICIAL);
	AFNDInsertaEstado(afd, "B", NORMAL);
	AFNDInsertaEstado(afd, "C", FINAL);
	AFNDInsertaEstado(afd, "D", NORMAL);
	AFNDInsertaEstado(afd, "E", NORMAL);
	
	AFNDInsertaTransicion(afd, "A", "0", "A");
	AFNDInsertaTransicion(afd, "A", "1", "B");
	AFNDInsertaTransicion(afd, "B", "1", "C");
	AFNDInsertaTransicion(afd, "D", "1", "D");
	AFNDInsertaTransicion(afd, "D", "0", "E");
	AFNDInsertaTransicion(afd, "E", "0", "C");

	afd_minimizado = AFNDMinimiza(afd, debug);

	n = 1;
	printf("Test 2 ((0)*11 con inaccesibles):\n");
	printf("Reduce de %d a %d estados\n", AFNDNumEstados(afd), AFNDNumEstados(afd_minimizado));
	if (!test(afd, afd_minimizado, &n, "0")) return false;
	if (!test(afd, afd_minimizado, &n, "1")) return false;
	if (!test(afd, afd_minimizado, &n, "00")) return false;
	if (!test(afd, afd_minimizado, &n, "01")) return false;
	if (!test(afd, afd_minimizado, &n, "10")) return false;
	if (!test(afd, afd_minimizado, &n, "11")) return false;
	if (!test(afd, afd_minimizado, &n, "011")) return false;
	if (!test(afd, afd_minimizado, &n, "111")) return false;
	if (!test(afd, afd_minimizado, &n, "000")) return false;
	if (!test(afd, afd_minimizado, &n, "100")) return false;
	printf("Test 2 ejecutado correctamente.\n");

	AFNDADot(afd);
	AFNDADot(afd_minimizado);
	printf("\n");

	AFNDElimina(afd_minimizado);
	AFNDElimina(afd);

	return true;
}

bool test3(bool debug) {	
	AFND *afd;
	AFND *afd_minimizado;
	int n;
	
	afd = AFNDNuevo("afd_(0)*11_con_inutiles", 5, 2);
	
	AFNDInsertaSimbolo(afd, "0");
	AFNDInsertaSimbolo(afd, "1");
	
	AFNDInsertaEstado(afd, "A", INICIAL);
	AFNDInsertaEstado(afd, "B", NORMAL);
	AFNDInsertaEstado(afd, "C", FINAL);
	AFNDInsertaEstado(afd, "D", NORMAL);
	AFNDInsertaEstado(afd, "E", NORMAL);
	
	AFNDInsertaTransicion(afd, "A", "0", "A");
	AFNDInsertaTransicion(afd, "A", "1", "B");
	AFNDInsertaTransicion(afd, "B", "1", "C");
	AFNDInsertaTransicion(afd, "B", "1", "D");
	AFNDInsertaTransicion(afd, "D", "1", "D");
	AFNDInsertaTransicion(afd, "D", "0", "E");

	afd_minimizado = AFNDMinimiza(afd, debug);

	n = 1;
	printf("Test 3 ((0)*11 con inútiles):\n");
	printf("Reduce de %d a %d estados\n", AFNDNumEstados(afd), AFNDNumEstados(afd_minimizado));
	if (!test(afd, afd_minimizado, &n, "0")) return false;
	if (!test(afd, afd_minimizado, &n, "1")) return false;
	if (!test(afd, afd_minimizado, &n, "00")) return false;
	if (!test(afd, afd_minimizado, &n, "01")) return false;
	if (!test(afd, afd_minimizado, &n, "10")) return false;
	if (!test(afd, afd_minimizado, &n, "111")) return false;
	if (!test(afd, afd_minimizado, &n, "000")) return false;
	if (!test(afd, afd_minimizado, &n, "100")) return false;
	if (!test(afd, afd_minimizado, &n, "11")) return false;
	if (!test(afd, afd_minimizado, &n, "011")) return false;
	if (!test(afd, afd_minimizado, &n, "0011")) return false;
	if (!test(afd, afd_minimizado, &n, "00011")) return false;
	if (!test(afd, afd_minimizado, &n, "000011")) return false;
	printf("Test 3 ejecutado correctamente.\n");

	AFNDADot(afd);
	AFNDADot(afd_minimizado);
	printf("\n");

	AFNDElimina(afd_minimizado);
	AFNDElimina(afd);

	return true;
}

bool test(AFND *afd, AFND *afd_minimizado, int *n, char *cadena) {
	bool acepta_afd, acepta_afd_minimizado;
	acepta_afd = procesar_entrada(afd, cadena);
	acepta_afd_minimizado = procesar_entrada(afd_minimizado, cadena);
	if (acepta_afd) {
		if (acepta_afd_minimizado) {
			printf(" -> Cadena %d (%s): ambos aceptan.\n", *n, cadena);
		} else {
			printf(" -> Cadena %d (%s): AFND acepta y AFD reducido rechaza. ERROR.\n", *n, cadena);
		}
	} else {
		if (acepta_afd_minimizado) {
			printf(" -> Cadena %d (%s): AFND rechaza y AFD reducido acepta. ERROR.\n", *n, cadena);
		} else {
			printf(" -> Cadena %d (%s): ambos rechazan.\n", *n, cadena);
		}
	}
	*n = *n+1;
	if (acepta_afd != acepta_afd_minimizado) printf("Saliendo por fallo\n");
	return acepta_afd == acepta_afd_minimizado;
}

bool procesar_entrada(AFND *afd, char* cadena) {
	FILE *file;
	char letra;
	char aux[2];
	bool es_final;
	int len, i;

	AFNDInicializaEstado(afd);
	AFNDInicializaCadenaActual(afd);

	len = strlen(cadena);
	aux[1] = '\0';
	for (i = 0; i < len; i++) {
		aux[0] = cadena[i];
		AFNDInsertaLetra(afd, aux);
	}
	
	for (i = 0; i < len; i++) {
		AFNDTransita(afd);
	}

	file = fopen("estados_aux.txt", "w");
	AFNDImprimeConjuntoEstadosActual(file, afd);
	fclose(file);

	file = fopen("estados_aux.txt", "r");
	AFNDImprimeConjuntoEstadosActual(file, afd);

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
