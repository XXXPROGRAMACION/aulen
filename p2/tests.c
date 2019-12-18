#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "minimiza.h"

bool test1(bool debug);
bool test2(bool debug);
bool test3(bool debug);
bool test4(bool debug);
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
    if (!test4(debug)) return 0;

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
	printf("Test 1 (autómata de las diapositivas):\n");
	printf("Reduce de %d a %d estados.\n", AFNDNumEstados(afd), AFNDNumEstados(afd_minimizado));
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
	
	afd = AFNDNuevo("afd_0x11_con_inaccesibles", 5, 2);
	
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
	printf("Test 2 (0*11 con inaccesibles):\n");
	printf("Reduce de %d a %d estados.\n", AFNDNumEstados(afd), AFNDNumEstados(afd_minimizado));
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
	
	afd = AFNDNuevo("afd_0x11_con_inutiles", 5, 2);
	
	AFNDInsertaSimbolo(afd, "0");
	AFNDInsertaSimbolo(afd, "1");
	
	AFNDInsertaEstado(afd, "A", INICIAL);
	AFNDInsertaEstado(afd, "B", NORMAL);
	AFNDInsertaEstado(afd, "C", FINAL);
	AFNDInsertaEstado(afd, "D", NORMAL);
	AFNDInsertaEstado(afd, "E", NORMAL);
	
	AFNDInsertaTransicion(afd, "A", "0", "A");
	AFNDInsertaTransicion(afd, "A", "1", "B");
	AFNDInsertaTransicion(afd, "B", "0", "D");
	AFNDInsertaTransicion(afd, "B", "1", "C");
	AFNDInsertaTransicion(afd, "D", "0", "E");
	AFNDInsertaTransicion(afd, "D", "1", "D");

	afd_minimizado = AFNDMinimiza(afd, debug);

	n = 1;
	printf("Test 3 (0*11 con inútiles):\n");
	printf("Reduce de %d a %d estados.\n", AFNDNumEstados(afd), AFNDNumEstados(afd_minimizado));
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
	printf("Test 3 ejecutado correctamente.\n");

	AFNDADot(afd);
	AFNDADot(afd_minimizado);
	printf("\n");

	AFNDElimina(afd_minimizado);
	AFNDElimina(afd);

	return true;
}

bool test4(bool debug) {	
	AFND *afd;
	AFND *afd_minimizado;
	int n;
	
	afd = AFNDNuevo("afd_((0+1)(0+1)(0+1)(0+1))x_4_iteraciones", 12, 2);
	
	AFNDInsertaSimbolo(afd, "0");
	AFNDInsertaSimbolo(afd, "1");
	
	AFNDInsertaEstado(afd, "q0", INICIAL_Y_FINAL);
	AFNDInsertaEstado(afd, "q1", NORMAL);
	AFNDInsertaEstado(afd, "q2", NORMAL);
	AFNDInsertaEstado(afd, "q3", NORMAL);
	AFNDInsertaEstado(afd, "q4", FINAL);
	AFNDInsertaEstado(afd, "q5", NORMAL);
	AFNDInsertaEstado(afd, "q6", NORMAL);
	AFNDInsertaEstado(afd, "q7", NORMAL);
	AFNDInsertaEstado(afd, "q8", FINAL);
	AFNDInsertaEstado(afd, "q9", NORMAL);
	AFNDInsertaEstado(afd, "q10", NORMAL);
	AFNDInsertaEstado(afd, "q11", NORMAL);
	
	AFNDInsertaTransicion(afd, "q0", "0", "q1");
	AFNDInsertaTransicion(afd, "q0", "1", "q1");
	AFNDInsertaTransicion(afd, "q1", "0", "q2");
	AFNDInsertaTransicion(afd, "q1", "1", "q2");
	AFNDInsertaTransicion(afd, "q2", "0", "q3");
	AFNDInsertaTransicion(afd, "q2", "1", "q3");
	AFNDInsertaTransicion(afd, "q3", "0", "q4");
	AFNDInsertaTransicion(afd, "q3", "1", "q4");
	AFNDInsertaTransicion(afd, "q4", "0", "q5");
	AFNDInsertaTransicion(afd, "q4", "1", "q5");
	AFNDInsertaTransicion(afd, "q5", "0", "q6");
	AFNDInsertaTransicion(afd, "q5", "1", "q6");
	AFNDInsertaTransicion(afd, "q6", "0", "q7");
	AFNDInsertaTransicion(afd, "q6", "1", "q7");
	AFNDInsertaTransicion(afd, "q7", "0", "q8");
	AFNDInsertaTransicion(afd, "q7", "1", "q8");
	AFNDInsertaTransicion(afd, "q8", "0", "q9");
	AFNDInsertaTransicion(afd, "q8", "1", "q9");
	AFNDInsertaTransicion(afd, "q9", "0", "q10");
	AFNDInsertaTransicion(afd, "q9", "1", "q10");
	AFNDInsertaTransicion(afd, "q10", "0", "q11");
	AFNDInsertaTransicion(afd, "q10", "1", "q11");
	AFNDInsertaTransicion(afd, "q11", "0", "q0");
	AFNDInsertaTransicion(afd, "q11", "1", "q0");

	afd_minimizado = AFNDMinimiza(afd, debug);

	n = 1;
	printf("Test 4 (enunciado - ((0+1)(0+1)(0+1)(0+1))* con 4 iteraciones):\n");
	printf("Reduce de %d a %d estados.\n", AFNDNumEstados(afd), AFNDNumEstados(afd_minimizado));
	if (!test(afd, afd_minimizado, &n, "0")) return false;
	if (!test(afd, afd_minimizado, &n, "1")) return false;
	if (!test(afd, afd_minimizado, &n, "00")) return false;
	if (!test(afd, afd_minimizado, &n, "11")) return false;
	if (!test(afd, afd_minimizado, &n, "000")) return false;
	if (!test(afd, afd_minimizado, &n, "111")) return false;
	if (!test(afd, afd_minimizado, &n, "0000")) return false;
	if (!test(afd, afd_minimizado, &n, "1111")) return false;
	if (!test(afd, afd_minimizado, &n, "0101")) return false;
	if (!test(afd, afd_minimizado, &n, "1010")) return false;
	if (!test(afd, afd_minimizado, &n, "00000")) return false;
	if (!test(afd, afd_minimizado, &n, "11111")) return false;
	if (!test(afd, afd_minimizado, &n, "01010101")) return false;
	printf("Test 4 ejecutado correctamente.\n");

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
