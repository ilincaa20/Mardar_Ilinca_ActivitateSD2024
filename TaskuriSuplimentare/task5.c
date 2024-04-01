#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Farmacie Farmacie;
typedef struct Nod Nod;

 struct Farmacie {
    int cod;
    char* denumire;
    int nrProduse;
    float* preturi;
};

struct Nod{
    Farmacie info;
    Nod* next;
};

Farmacie initializare(int cod, const char* denumire, int nrProduse, float* preturi) {
    Farmacie f;
    f.cod = cod;
    f.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
    strcpy(f.denumire, denumire);
    f.nrProduse = nrProduse;
    f.preturi = (float*)malloc(sizeof(float) * nrProduse);
    for (int i = 0; i < nrProduse; i++) {
        f.preturi[i] = preturi[i];
    }
    return f;
}

Farmacie* citesteFarmaciiDinFisier(const char* numeFisier, int* nrFarmacii) {
    FILE* file = fopen(numeFisier, "r");
    if (!file) {
        perror("Nu se poate deschide fișierul");
        return NULL;
    }

    Farmacie* farmacii = NULL;
    char linie[256];
    int cod, nrProduse;
    char denumire[100];
    float preturi[100];

    *nrFarmacii = 0;
    while (fgets(linie, sizeof(linie), file)) {
        char* token = strtok(linie, ",");
        cod = atoi(token);

        token = strtok(NULL, ",");
        strcpy(denumire, token);

        token = strtok(NULL, ",");
        nrProduse = atoi(token);

        for (int i = 0; i < nrProduse; i++) {
            token = strtok(NULL, ",");
            preturi[i] = atof(token);
        }

        farmacii = realloc(farmacii, (*nrFarmacii + 1) * sizeof(Farmacie));
        farmacii[*nrFarmacii] = initializare(cod, denumire, nrProduse, preturi);
        (*nrFarmacii)++;
    }

    fclose(file);
    return farmacii;
}



void afisareFarmacie(Farmacie f) {
    printf("Farmacia cu codul %d se numeste %s, are %d produse. Preturile sunt: ", f.cod, f.denumire, f.nrProduse);
    for (int i = 0; i < f.nrProduse; i++) {
        printf("%5.2f ", f.preturi[i]);
    }
    printf("\n");
}

void afisareVectorFarmacii(Farmacie* farmacii, int nrFarmacii) {
    for (int i = 0; i < nrFarmacii; i++) {
        afisareFarmacie(farmacii[i]);
    }
}


void salveazaFarmacieInFisier(Farmacie f, const char* numeFisier) {
    FILE* file = fopen(numeFisier, "a"); 
    if (!file) {
        perror("Nu se poate deschide fișierul");
        return;
    }

    fprintf(file, "%d,%s,%d", f.cod, f.denumire, f.nrProduse);
    for (int i = 0; i < f.nrProduse; i++) {
        fprintf(file, ",%.2f", f.preturi[i]);
    }
    fprintf(file, "\n");

    fclose(file);
}

void salveazaVectorFarmaciiInFisier(Farmacie* farmacii, int nrFarmacii, const char* numeFisier) {
    FILE* file = fopen(numeFisier, "w"); 
    if (!file) {
        perror("Nu se poate deschide fișierul");
        return;
    }

    for (int i = 0; i < nrFarmacii; i++) {
        fprintf(file, "%d,%s,%d", farmacii[i].cod, farmacii[i].denumire, farmacii[i].nrProduse);
        for (int j = 0; j < farmacii[i].nrProduse; j++) {
            fprintf(file, ",%.2f", farmacii[i].preturi[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void insereazaSortat(Nod** cap, Farmacie nou) {
    Nod* nouNod = (Nod*)malloc(sizeof(Nod));
    nouNod->info = nou; 
    nouNod->next = NULL;

    if (*cap == NULL || (*cap)->info.cod >= nou.cod) {
        nouNod->next = *cap;
        *cap = nouNod;
    } else {
        Nod* temp = *cap;
        while (temp->next != NULL && temp->next->info.cod < nou.cod) {
            temp = temp->next;
        }
        nouNod->next = temp->next;
        temp->next = nouNod;
    }
}

Farmacie* salveazaInVectorDupaConditie(Nod* cap, int pragNrProduse, int* nrElemente) {
    Farmacie* vector = NULL; 
    *nrElemente = 0; 
    Nod* curent = cap; 

    while (curent != NULL) { 
        if (curent->info.nrProduse > pragNrProduse) { 
            vector = (Farmacie*)realloc(vector, (*nrElemente + 1) * sizeof(Farmacie)); 
            Farmacie nouaFarmacie; 
            
            nouaFarmacie.denumire = (char*)malloc(strlen(curent->info.denumire) + 1);
            strcpy(nouaFarmacie.denumire, curent->info.denumire);
            
            
            nouaFarmacie.cod = curent->info.cod;
            nouaFarmacie.nrProduse = curent->info.nrProduse;
            
          
            nouaFarmacie.preturi = (float*)malloc(nouaFarmacie.nrProduse * sizeof(float));
            for (int i = 0; i < nouaFarmacie.nrProduse; i++) {
                nouaFarmacie.preturi[i] = curent->info.preturi[i];
            }
            
            vector[*nrElemente] = nouaFarmacie; 
            (*nrElemente)++; 
        }
        curent = curent->next; 
    }

    return vector; 
}



void stergeNodPePozitie(Nod** cap, int pozitie) {
    if (*cap == NULL) return; 

    Nod* temp = *cap;
    if (pozitie == 0) { 
        *cap = temp->next;
        free(temp->info.denumire);
        free(temp->info.preturi);
        free(temp);
        return;
    }

    for (int i = 0; temp != NULL && i < pozitie - 1; i++) {
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL) return; 

    Nod* next = temp->next->next;
    free(temp->next->info.denumire);
    free(temp->next->info.preturi);
    free(temp->next); 
    temp->next = next; 
}




int main() {
    Nod* listaFarmacii = NULL; 
    int nrFarmaciiInVector;
    Farmacie farmacie;

    
    float preturi1[] = {10.5, 20.75, 30.0};
    Farmacie f1 = initializare(1, "Farmacia1", 3, preturi1);

    float preturi2[] = {15.5, 25.75, 35.0, 45.5};
    Farmacie f2 = initializare(2, "Farmacia2", 4, preturi2);

    float preturi3[] = {5.0, 10.0};
    Farmacie f3 = initializare(3, "Farmacia3", 2, preturi3);

    
    insereazaSortat(&listaFarmacii, f1);
    insereazaSortat(&listaFarmacii, f2);
    insereazaSortat(&listaFarmacii, f3);

    
    printf("Farmaciile din lista:\n");
    for (Nod* p = listaFarmacii; p != NULL; p = p->next) {
        afisareFarmacie(p->info);
    }
    
    Farmacie* vectorFarmacii = salveazaInVectorDupaConditie(listaFarmacii, 2, &nrFarmaciiInVector);

    
    printf("\nFarmaciile cu mai mult de 2 produse:\n");
    for (int i = 0; i < nrFarmaciiInVector; i++) {
        afisareFarmacie(vectorFarmacii[i]);
    }

   

    return 0;
}