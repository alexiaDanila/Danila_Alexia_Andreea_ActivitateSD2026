#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct Masina {
	int id;
	int nrKm;
	char* model;
	float pret;
	char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor) {
	struct Masina m;
	m.id = id;
	m.nrKm = nrKm;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy_s(m.model, strlen(model)+1, model);
	m.pret = pret;
	m.tipMotor = tipMotor;
	return m;
}


void afisare(struct Masina s) {
	printf("Id: %d, nr km: %d, model: %s, pret: %f, tip motor: %c", s.id, s.nrKm, s.model, s.pret, s.tipMotor);
}

void modifica_pret(struct Masina* s, float noul_pret) {
	//modificarea unui atribut
	if (noul_pret > 0) {
		s->pret = noul_pret;
	}
}

void dezalocare(struct Masina* s) {
	//dezalocare campuri alocate dinamic
	free(s->model);
	s->model = NULL;
}

int main() {
	struct Masina masina;
	masina = initializare(10, 110000, "logan", 10000.00, 'B');
	afisare(masina);

	modifica_pret(&masina, 10.00);
	dezalocare(&masina);

	return 0;
}