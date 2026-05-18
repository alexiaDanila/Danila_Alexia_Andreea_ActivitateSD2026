#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraProdus {
	int id;
	char* nume;
	char* categorie;
	float pret;
	int cantitate;
};
typedef struct StructuraProdus Produs;

struct Heap {
	int lungime;
	Produs* vector;
	int nrProduse;
};
typedef struct Heap Heap;

Produs citireProdusDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	Produs p;
	fgets(buffer, 100, file);
	aux = strtok(buffer, sep);
	p.id = atoi(aux);
	aux = strtok(NULL, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy_s(p.nume, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	p.categorie = malloc(strlen(aux) + 1);
	strcpy_s(p.categorie, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	p.pret = atof(aux);
	aux = strtok(NULL, sep);
	p.cantitate = atoi(aux);
	return p;
}

void afisareProdus(Produs produs) {
	printf("Id: %d\n", produs.id);
	printf("Nume: %s\n", produs.nume);
	printf("Categorie: %s\n", produs.categorie);
	printf("Pret: %.2f\n", produs.pret);
	printf("Cantitate: %d\n", produs.cantitate);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrProduse = 0;
	heap.vector = (Produs*)malloc(sizeof(Produs) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (pozFiuSt < heap.nrProduse && heap.vector[pozMax].pret < heap.vector[pozFiuSt].pret) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrProduse && heap.vector[pozMax].pret < heap.vector[pozFiuDr].pret) {
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod) {
		Produs aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax <= (heap.nrProduse - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeProduseDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(file)) {
		heap.vector[heap.nrProduse++] = citireProdusDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrProduse - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrProduse; i++) {
		afisareProdus(heap.vector[i]);
	}
}

void afisareHeapAscuns(Heap heap) {
	for (int i = heap.nrProduse; i < heap.lungime; i++) {
		afisareProdus(heap.vector[i]);
	}
}

Produs extrageProdus(Heap* heap) {
	if (heap->nrProduse > 0) {
		Produs aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrProduse - 1];
		heap->vector[heap->nrProduse - 1] = aux;
		heap->nrProduse--;
		for (int i = (heap->nrProduse - 2) / 2; i >= 0; i--){
			filtreazaHeap(*heap, i);
		}
		return aux;
	}		
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].nume);
		free(heap->vector[i].categorie);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrProduse = 0;
}

int main() {
	Heap heap = citireHeapDeProduseDinFisier("produse.txt");
	afisareHeap(heap);
	printf("Produse extrase: \n");
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));
	afisareProdus(extrageProdus(&heap));

	printf("Heap-ul ascuns: \n");
	afisareHeapAscuns(heap);

	dezalocareHeap(&heap);

}
