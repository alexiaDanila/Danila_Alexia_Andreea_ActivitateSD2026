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

struct Nod
{
	Masina info;
	struct Nod* dr;
	struct Nod* st;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(Nod** nod, Masina masinaNoua) {
	if ((*nod) != NULL)
	{
		if (masinaNoua.id > (*nod)->info.id)
		{
			adaugaMasinaInArbore(&(*nod)->dr, masinaNoua);
		}
		else if (masinaNoua.id < (*nod)->info.id)
		{
			adaugaMasinaInArbore(&(*nod)->st, masinaNoua);
		}
	}
	else
	{
		(*nod) = (Nod*)malloc(sizeof(Nod));
		(*nod)->info = masinaNoua;
		(*nod)->st = NULL;
		(*nod)->dr = NULL;
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* nod = NULL;
	for (int i = 0; i < 10; i++) {
		adaugaMasinaInArbore(&nod, citireMasinaDinFisier(f));
	}
	fclose(f);
	return nod;
}

void afisareMasiniDinArbore(/*arbore de masini*/) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
}

void afisarePreOrdineRSD(Nod* radacina) {
	if (radacina != NULL)
	{
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->st);
		afisarePreOrdineRSD(radacina->dr);
	}
}


void afisareInOrdineSRD(Nod* radacina) {
	if (radacina != NULL)
	{
		afisareInOrdineSRD(radacina->st);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dr);
	}
}


void dezalocareArboreDeMasini(Nod** arbore) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);
		(*arbore) = NULL;
	}
}

Masina getMasinaByID(Nod *radacina, int id) {
	Masina m;
	m.id = -1;
	if (radacina != NULL) {
		if (id < radacina->info.id) {
			return getMasinaByID(radacina->st, id);
		}
		else {
			if (id > radacina->info.id) {
				return getMasinaByID(radacina->dr, id);
			}
			else {
				return radacina->info;
			}
		}

	}
	return m;
}

int determinaNumarNoduri(Nod * radacina) {
	int nrNoduri = 0;
	if (radacina != NULL) {
		nrNoduri += determinaNumarNoduri(radacina->st);
		nrNoduri += determinaNumarNoduri(radacina->dr);
		nrNoduri += 1;
	}
	return nrNoduri;
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore == NULL)
		return 0;
	int inaltimeStanga = calculeazaInaltimeArbore(arbore->st);
	int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dr);
	if (inaltimeStanga > inaltimeDreapta)
		return 1 + inaltimeStanga;
	else
		return 1 + inaltimeDreapta;
}

float calculeazaPretTotal(Nod*arbore) {
	//calculeaza pretul tuturor masinilor din arbore.
	if (arbore == NULL) {
		return 0;
	}
	float totalStanga= calculeazaPretTotal(arbore->st);
	float totalDreapta=calculeazaPretTotal(arbore->dr);
	return arbore->info.pret + totalStanga + totalDreapta;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore == NULL)
		return 0;
	float stanga = calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer);
	float dreapta = calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
	if (strcmp(arbore->info.numeSofer, numeSofer) == 0)
		return arbore->info.pret + stanga + dreapta;
	return stanga + dreapta;
}

int main() {
	Nod* rad = citireArboreDeMasiniDinFisier("masini_arbore.txt");

	afisarePreOrdineRSD(rad);
	afisareInOrdineSRD(rad);

	printf("\nMasina gasita: ");
	afisareMasina(getMasinaByID(rad, 6));

	printf("\nNr noduri: %d", determinaNumarNoduri(rad));
	printf("\nInaltime: %d", calculeazaInaltimeArbore(rad));
	printf("\nPret total: %.2f", calculeazaPretTotal(rad));
	printf("\nPret Ionescu: %.2f", calculeazaPretulMasinilorUnuiSofer(rad, "Ionescu"));

	dezalocareArboreDeMasini(&rad);
	return 0;
}
