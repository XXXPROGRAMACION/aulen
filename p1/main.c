#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"
#include "transforma.h"

int main(int argc, char **argv) {
	AFND *afnd;
	AFND *afd;
	bool debug;

	if (argc > 1 && strcmp(argv[1], "--debug") == 0) debug = true;
	else debug = false;
	
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

	AFNDADot(afnd);
	AFNDADot(afd);

	AFNDElimina(afd);
	AFNDElimina(afnd);

	return 0;
}
