#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Farmacie Farmacie;

 struct Farmacie {
    int cod;
    char* denumire;
    int nrProduse;
    float* preturi;
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

Farmacie** copiazaInMatrice(Farmacie* vector, int nrFarmacii, int* nrLinii, int** nrElemPeLinie) {
    
    *nrLinii = 0;
    *nrElemPeLinie = calloc(nrFarmacii, sizeof(int)); 
    Farmacie** matrice = calloc(nrFarmacii, sizeof(Farmacie*)); 
    
    for (int i = 0; i < nrFarmacii; i++) {
        int gasit = 0;
        for (int j = 0; j < *nrLinii && !gasit; j++) {
            if (vector[i].nrProduse == vector[matrice[j][0].cod - 1].nrProduse) { 
                matrice[j] = realloc(matrice[j], ((*nrElemPeLinie)[j] + 1) * sizeof(Farmacie));
                matrice[j][(*nrElemPeLinie)[j]] = vector[i]; 
                (*nrElemPeLinie)[j]++;
                gasit = 1;
            }
        }
        if (!gasit) {
            matrice[*nrLinii] = malloc(sizeof(Farmacie));
            matrice[*nrLinii][0] = vector[i];
            (*nrElemPeLinie)[*nrLinii] = 1;
            (*nrLinii)++;
        }
    }
    
    return matrice;
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

void sorteazaMatriceDupaNrElem(Farmacie*** matrice, int* nrLinii, int* nrElemPeLinie) {
    
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        for (int i = 0; i < *nrLinii - 1; i++) {
            if (nrElemPeLinie[i] < nrElemPeLinie[i + 1]) {
                int temp = nrElemPeLinie[i];
                nrElemPeLinie[i] = nrElemPeLinie[i + 1];
                nrElemPeLinie[i + 1] = temp;
                
                Farmacie* tempPtr = (*matrice)[i];
                (*matrice)[i] = (*matrice)[i + 1];
                (*matrice)[i + 1] = tempPtr;
                
                sorted = 0;
            }
        }
    }
}

void afiseazaMatrice(Farmacie** matrice, int nrLinii, int* nrElemPeLinie) {
    for (int i = 0; i < nrLinii; i++) {
        printf("Linia %d (cu %d farmacii):\n", i, nrElemPeLinie[i]);
        for (int j = 0; j < nrElemPeLinie; j++){
    printf("\tCod: %d, Denumire: %s, Nr produse: %d, Preturi: ", matrice[i][j].cod, matrice[i][j].denumire, matrice[i][j].nrProduse);
        for (int k = 0; k < matrice[i][j].nrProduse; k++) {
    printf("%.2f ", matrice[i][j].preturi[k]);
        }
    printf("\n");
    }
    printf("\n");
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


int main() {
    int nrFarmacii;
    Farmacie* vectorFarmacii = citesteFarmaciiDinFisier("farmacii.txt", &nrFarmacii);
    if (!vectorFarmacii) {
        printf("Eroare la citirea farmaciilor din fisier.\n");
        return 1;
    }

    int nrLinii, *nrElemPeLinie;
    Farmacie** matriceFarmacii = copiazaInMatrice(vectorFarmacii, nrFarmacii, &nrLinii, &nrElemPeLinie);

    sorteazaMatriceDupaNrElem(&matriceFarmacii, &nrLinii, nrElemPeLinie);

    printf("Matricea de farmacii dupa sortare:\n");
    afiseazaMatrice(matriceFarmacii, nrLinii, nrElemPeLinie);

    for (int i = 0; i < nrFarmacii; i++) {
        free(vectorFarmacii[i].denumire);
        free(vectorFarmacii[i].preturi);
    }
    free(vectorFarmacii);

    for (int i = 0; i < nrLinii; i++) {
        for (int j = 0; j < nrElemPeLinie[i]; j++) {
            free(matriceFarmacii[i][j].denumire);
            free(matriceFarmacii[i][j].preturi);
        }
        free(matriceFarmacii[i]);
    }
    free(matriceFarmacii);
    free(nrElemPeLinie);

    return 0;
}