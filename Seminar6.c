#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct Nod nod;
struct Nod {
	Masina info;
	nod* next;
};

struct HashTable {
	int dim;
	nod** vector;
};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(nod** cap, Masina masinaNoua) {
	nod* temp = (nod*)malloc(sizeof(nod));
	temp->info = masinaNoua;
	temp->next = NULL;
	if ((*cap) == NULL) {
		(*cap) = temp;
	}
	else {
		nod* aux = (*cap);
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = temp;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (nod**)malloc(sizeof(nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* numeSofer, int dimensiune) {
	if (!dimensiune || dimensiune < 0) {
		return -1;
	}
	unsigned int suma = 0;
	for (int i = 0; i < strlen(numeSofer); i++) {
		suma += (int)numeSofer[i];
	}
	suma %= dimensiune;
	return suma;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dim) {
	FILE* f = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dim);
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(f);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Suntem la lantul %d\n", i + 1);
		afisareListaMasini(ht.vector[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		nod* p = ht->vector[i];
		while (p != NULL) {
			nod* desters = p;
			p = p->next;
			free(desters->info.model);
			free(desters->info.numeSofer);
			free(desters);
		}
		ht->vector[i] = NULL;
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL)
			(*nrClustere)++;
	}

	if (*nrClustere == 0)
		return NULL;

	float* rezultat = (float*)malloc(sizeof(float) * (*nrClustere));
	int idx = 0;

	for (int i = 0; i < ht.dim; i++) {
		if (ht.vector[i] != NULL) {
			float suma = 0;
			int cnt = 0;
			nod* p = ht.vector[i];
			while (p != NULL) {
				suma += p->info.pret;
				cnt++;
				p = p->next;
			}
			rezultat[idx++] = suma / cnt;
		}
	}

	return rezultat;
}

Masina getMasinaDupaCheie(HashTable ht, const char* numeSofer) {
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;
	int pozitie = calculeazaHash(numeSofer, ht.dim);
	if (pozitie < 0 || pozitie >= ht.dim || ht.vector[pozitie] == NULL) {
		return m;
	}
	nod* aux = ht.vector[pozitie];
	while (aux != NULL && strcmp(aux->info.numeSofer, numeSofer) != 0) {
		aux = aux->next;
	}
	if (aux != NULL) {
		m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(numeSofer) + 1, numeSofer);
		m.model = (char*)malloc(strlen(aux->info.model) + 1);
		strcpy_s(m.model, strlen(aux->info.model) + 1, aux->info.model);
		m.id = aux->info.id;
		m.nrUsi = aux->info.nrUsi;
		m.pret = aux->info.pret;
		m.serie = aux->info.serie;
	}
	return m;
}

int main() {
	HashTable hash;
	hash = citireMasiniDinFisier("masini.txt", 5);

	afisareTabelaDeMasini(hash);

	int nrClustere = 0;
	float* preturi = calculeazaPreturiMediiPerClustere(hash, &nrClustere);
	printf("Preturi medii per clustere (%d clustere cu masini):\n", nrClustere);
	for (int i = 0; i < nrClustere; i++) {
		printf("  Cluster %d: %.2f\n", i + 1, preturi[i]);
	}
	free(preturi);

	printf("\nMasina lui Ionescu este:\n");
	Masina IonescuMasina = getMasinaDupaCheie(hash, "Ionescu");
	if (IonescuMasina.id != -1) {
		afisareMasina(IonescuMasina);
		free(IonescuMasina.model);
		free(IonescuMasina.numeSofer);
	}
	else {
		printf("Nu am gasit masina\n");
	}

	dezalocareTabelaDeMasini(&hash);
	return 0;
}
