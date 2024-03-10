#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char* nume;
    int nrTerminale;
} Aeroport;

Aeroport* adaugaAeroport(Aeroport* vector, Aeroport aeroport, int* dimensiune) {
    Aeroport* copie = (Aeroport*)malloc(sizeof(Aeroport) * ((*dimensiune) + 1));
    if (!copie) {
        printf("Eroare la alocare memorie.\n");
        return vector; 
    }
    
    copie[(*dimensiune)] = aeroport;
    for (int i = 0; i < (*dimensiune); i++) {
        copie[i] = vector[i];
    }
    
    if (vector != NULL) {
        free(vector);
    }
    
    (*dimensiune)++;
    return copie;
}

Aeroport* citireFisier(const char* numeFisier, int* nrAeroporturi) {
    Aeroport* vectorAeroporturi = NULL;
    *nrAeroporturi = 0;
    
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Fisierul nu a putut fi deschis.\n");
        return NULL;
    }
    
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        Aeroport aeroport;
        char delimitator[] = ",\n";
        char* token = strtok(buffer, delimitator);
        aeroport.id = atoi(token);
        token = strtok(NULL, delimitator);
        aeroport.nume = malloc(strlen(token) + 1);
        if (!aeroport.nume) {
            printf("Eroare la alocare memorie pentru nume.\n"); 
        }
        strcpy(aeroport.nume, token);
        
        token = strtok(NULL, delimitator);
        aeroport.nrTerminale = atoi(token);
        vectorAeroporturi = adaugaAeroport(vectorAeroporturi, aeroport, nrAeroporturi);
    }
    
    fclose(f);
    return vectorAeroporturi;
}

void afisareAeroport(const Aeroport* aeroport) {
    printf("Aeroportul cu id-ul %d, numele %s si cu nr de terminale %d\n", aeroport->id, aeroport->nume, aeroport->nrTerminale);
}

void afisareAeroporturi(const Aeroport* aeroporturi, int dim) {
    printf("Afisare aeroporturi:\n");
    for (int i = 0; i < dim; i++) {
        afisareAeroport(&aeroporturi[i]);
    }
}

void dezalocareVector(Aeroport** aeroporturi, int* dim) {
    for (int i = 0; i < *dim; i++) {
        free((*aeroporturi)[i].nume);
    }
    free(*aeroporturi);
    *aeroporturi = NULL;
    *dim = 0;
}

int main() {
    int dimensiune = 0;
    Aeroport* aeroporturi = citireFisier("aeroporturi.txt", &dimensiune);
    if (aeroporturi != NULL) {
        afisareAeroporturi(aeroporturi, dimensiune);
        dezalocareVector(&aeroporturi, &dimensiune);
    } else {
        printf("Nu s-au putut citi aeroporturile.\n");
    }
    return 0;
}
