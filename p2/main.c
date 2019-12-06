#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "minimiza.h"

int main(int argc, char **argv) {
	AFND *afd;
	bool debug;

	if (argc > 1 && strcmp(argv[1], "--debug") == 0) debug = true;
	else debug = false;

	afd = AFNDNuevo("afd_diapositivas", 9, 2);
	
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
	AFNDInsertaEstado(afd, "I", NORMAL);
	
	AFNDInsertaTransicion(afd, "A", "0", "B");
	AFNDInsertaTransicion(afd, "A", "1", "F");
	AFNDInsertaTransicion(afd, "B", "1", "C");
	AFNDInsertaTransicion(afd, "B", "0", "G");
	AFNDInsertaTransicion(afd, "C", "0", "A");
	AFNDInsertaTransicion(afd, "C", "1", "C");
	AFNDInsertaTransicion(afd, "D", "0", "C");
	AFNDInsertaTransicion(afd, "D", "1", "G");
	AFNDInsertaTransicion(afd, "E", "1", "F");
	AFNDInsertaTransicion(afd, "E", "0", "H");
	AFNDInsertaTransicion(afd, "F", "0", "C");
	AFNDInsertaTransicion(afd, "F", "1", "G");
	AFNDInsertaTransicion(afd, "G", "0", "G");
	AFNDInsertaTransicion(afd, "G", "1", "E");
	AFNDInsertaTransicion(afd, "H", "0", "G");
	AFNDInsertaTransicion(afd, "H", "0", "I");

	AFNDMinimiza(afd, debug);

	AFNDElimina(afd);

	return 0;
}
