#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraStudent {
	int id;
	char* nume;
	char* facultate;
    float medie;
    int anStudiu;
};
typedef struct StructuraStudent Student;

struct Heap {
    int lungime;
    Student* vector;
    int nrStudenti;
};
typedef struct Heap Heap;

Student citireStudentDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    Student s;

    fgets(buffer, 100, file);

    // id
    aux = strtok(buffer, sep);
    s.id = atoi(aux);

    // nume
    aux = strtok(NULL, sep);
    s.nume = malloc(strlen(aux) + 1);
    strcpy_s(s.nume, strlen(aux) + 1, aux);

    // facultate
    aux = strtok(NULL, sep);
    s.facultate = malloc(strlen(aux) + 1);
    strcpy_s(s.facultate, strlen(aux) + 1, aux);

    // medie
    s.medie = atof(strtok(NULL, sep));

    // anStudiu
    s.anStudiu = atoi(strtok(NULL, sep));

    return s;
}


void afisareStudent(Student student) {
    printf("Id: %d\n", student.id);
    printf("Nume: %s\n", student.nume);
    printf("Facultate: %s\n", student.facultate);
    printf("Medie: %.2f\n", student.medie);
    printf("An studiu: %d\n", student.anStudiu);
}

Heap initializareHeap(int lungime) {
    Heap heap;
    heap.lungime = lungime;
    heap.nrStudenti = 0;
    heap.vector = (Student*)malloc(sizeof(Student) * lungime);
    return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int pozFiuSt = 2 * pozitieNod + 1;
    int pozFiuDr = 2 * pozitieNod + 2;
    int pozMax = pozitieNod;
    if (pozFiuSt < heap.nrStudenti && heap.vector[pozMax].medie < heap.vector[pozFiuSt].medie) {
        pozMax = pozFiuSt;
    }
    if (pozFiuDr < heap.nrStudenti && heap.vector[pozMax].medie < heap.vector[pozFiuDr].medie) {
        pozMax = pozFiuDr;
    }
    if (pozMax != pozitieNod) {
        Student aux = heap.vector[pozMax];
        heap.vector[pozMax] = heap.vector[pozitieNod];
        heap.vector[pozitieNod] = aux;
        if (pozMax <= (heap.nrStudenti - 2) / 2) {
            filtreazaHeap(heap, pozMax);
        }
    }
}

Heap citireHeapDeStudentiDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    Heap heap = initializareHeap(10);
    while (!feof(file)) {
        heap.vector[heap.nrStudenti++] = citireStudentDinFisier(file);
    }
    fclose(file);
    for (int i = (heap.nrStudenti - 2) / 2; i >= 0; i--) {
        filtreazaHeap(heap, i);
    }
    return heap;
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrStudenti; i++) {
        afisareStudent(heap.vector[i]);
    }
}

void afisareHeapAscuns(Heap heap) {
    for(int i =heap.nrStudenti; i< heap.lungime; i++)
        afisareStudent(heap.vector[i]);
    }

Student extrageStudent(Heap* heap) {
    if (heap->nrStudenti > 0) {
        Student aux = heap->vector[0];
        heap->vector[0] = heap->vector[heap->nrStudenti - 1];
        heap->vector[heap->nrStudenti - 1] = aux;
        heap->nrStudenti--;
        for (int i = (heap->nrStudenti - 2) / 2;i>=0;i--){
            filtreazaHeap(*heap, i);
        }
        return aux;
    }
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->lungime; i++) {
        free(heap->vector[i].nume);
        free(heap->vector[i].facultate);
    }
        free(heap->vector);
        heap->vector = NULL;
        heap->lungime = 0;
        heap->nrStudenti = 0;
    }

int main() {
    Heap heap = citireHeapDeStudentiDinFisier("studenti.txt");
    afisareHeap(heap);
    printf("Studnti extrasi: \n");
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));
    afisareStudent(extrageStudent(&heap));

    printf("Heap-ul ascuns: \n");
    afisareHeapAscuns(heap);


    dezalocareHeap(&heap);

    return 0;
}
