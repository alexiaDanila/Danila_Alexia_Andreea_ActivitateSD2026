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

void afisareMasina(Masina masina) {
    printf("ID: %d, nrUsi: %d, pret: %.2f, model: %s, sofer: %s, serie: %c\n",
        masina.id, masina.nrUsi, masina.pret, masina.model, masina.numeSofer, masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
    if (masini != NULL) {
        for (int i = 0; i < nrMasini; i++) {
            afisareMasina(masini[i]);
        }
    }
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
    Masina* temp = malloc(sizeof(Masina) * ((*nrMasini) + 1));
    for (int i = 0; i < *nrMasini; i++) {
        temp[i] = (*masini)[i];
    }
    temp[*nrMasini] = masinaNoua;
    (*nrMasini)++;
    free(*masini);
    *masini = temp;
}

// Modificat pentru a primi linia gata citita, e mai sigur
Masina citireMasinaDinLinie(char* line) {
    char sep[] = ",\n";
    Masina temp;
    char* token = strtok(line, sep);
    temp.id = (token) ? atoi(token) : 0;

    token = strtok(NULL, sep);
    temp.nrUsi = (token) ? atoi(token) : 0;

    token = strtok(NULL, sep);
    temp.pret = (token) ? (float)atof(token) : 0.0f;

    char* buffer = strtok(NULL, sep);
    if (buffer) {
        temp.model = _strdup(buffer); // _strdup aloca si copiaza automat
    }
    else {
        temp.model = _strdup("N/A");
    }

    buffer = strtok(NULL, sep);
    if (buffer) {
        temp.numeSofer = _strdup(buffer);
    }
    else {
        temp.numeSofer = _strdup("N/A");
    }

    buffer = strtok(NULL, sep);
    temp.serie = (buffer) ? buffer[0] : ' ';

    return temp;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
    FILE* file = fopen(numeFisier, "r");
    if (!file) return NULL;

    Masina* vec = NULL;
    char line[200]; // Buffer mai mare pentru siguranta
    while (fgets(line, 200, file)) { // Citeste cat timp exista linii
        if (strlen(line) > 5) { // Evita liniile goale
            Masina m = citireMasinaDinLinie(line);
            adaugaMasinaInVector(&vec, nrMasiniCitite, m);
        }
    }
    fclose(file);
    return vec;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
    if (*vector != NULL) {
        for (int i = 0; i < (*nrMasini); i++) {
            free((*vector)[i].model);
            free((*vector)[i].numeSofer);
        }
        free(*vector);
        *vector = NULL;
        *nrMasini = 0;
    }
}

int main() {
    int nr = 0;
    // CRITICAL: Salveaza return-ul functiei in variabila!
    Masina* vectorMasini = citireVectorMasiniFisier("masini.txt", &nr);

    printf("Masini incarcate:\n");
    afisareVectorMasini(vectorMasini, nr);

    dezalocareVectorMasini(&vectorMasini, &nr);
    return 0;
}