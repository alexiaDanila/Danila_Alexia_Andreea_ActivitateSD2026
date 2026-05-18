#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraPlatforma {
	int id;
	float pret;
	char* nume;
	char* categorie;
	int* nrUtilizatori;
};
typedef struct StructuraPlatforma Platforma;

struct Heap {
	int lungime;
	Platforma* vector;
	int nrPlatforme;
};
typedef struct Heap Heap;

Platforma citirePlatformaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Platforma p;
	aux = strtok(buffer, sep);
	p.id = atoi(aux);
	aux = strtok(NULL, sep);
	p.pret = atof(aux);
	aux = strtok(NULL, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy_s(p.nume, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	p.categorie = malloc(strlen(aux) + 1);
	strcpy_s(p.categorie, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	p.nrUtilizatori = (int*)malloc(sizeof(int));
	*p.nrUtilizatori = atoi(aux);
	return p;
}

void afisarePlatforma(Platforma platforma) {
	printf("Id: %d\n", platforma.id);
	printf("Pret: %.2f\n", platforma.pret);
	printf("Nume: %s\n", platforma.nume);
	printf("Categorie: %s\n", platforma.categorie);
	printf("Nr. utilizatori: %d\n", *platforma.nrUtilizatori);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrPlatforme = 0;
	heap.vector = (Platforma*)malloc(sizeof(Platforma) * lungime);
	return heap;
}


void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMin = pozitieNod;
	if (pozFiuSt < heap.nrPlatforme && heap.vector[pozMin].pret > heap.vector[pozFiuSt].pret) {
		pozMin = pozFiuSt;
	}
	if (pozFiuDr < heap.nrPlatforme && heap.vector[pozMin].pret > heap.vector[pozFiuDr].pret) {
		pozMin = pozFiuDr;
	}
	if (pozMin != pozitieNod) {
		Platforma aux = heap.vector[pozMin];
		heap.vector[pozMin] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMin <= (heap.nrPlatforme - 2) / 2) {
			filtreazaHeap(heap, pozMin);
		}
	}
}

Heap citireHeapDePlatformeDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(file)) {
		heap.vector[heap.nrPlatforme++] = citirePlatformaDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrPlatforme - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrPlatforme; i++) {
		afisarePlatforma(heap.vector[i]);
	}
}

void afisareHeapAscuns(Heap heap) {
	for (int i = heap.nrPlatforme; i < heap.lungime;i++) {
		afisarePlatforma(heap.vector[i]);
	}
}

void modificaPretMinim(Heap heap, float pretNou) {
	heap.vector[0].pret = pretNou;
	filtreazaHeap(heap, 0);
}
void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].nume);
		free(heap->vector[i].categorie);
		free(heap->vector[i].nrUtilizatori);  // e int*, alocat cu malloc
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrPlatforme = 0;
}


int main() {
	Heap heap = citireHeapDePlatformeDinFisier("platforme.txt");

	printf("=== MIN-HEAP initial ===\n");
	afisareHeap(heap);

	printf("=== Platforma cu pretul minim (pozitia 0) ===\n");
	afisarePlatforma(heap.vector[0]);

	printf("=== Modificam pretul minimului la 999.99 ===\n");
	modificaPretMinim(heap, 999.99);

	printf("=== Heap dupa modificare ===\n");
	afisareHeap(heap);

	printf("=== Noul minim (pozitia 0) ===\n");
	afisarePlatforma(heap.vector[0]);


	dezalocareHeap(&heap);

	return 0;
}
