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

float calculeazaPretMediu(Farmacie f) {
    if (f.nrProduse == 0) 
    return 0; 
    float suma = 0;
    for (int i = 0; i < f.nrProduse; i++) {
        suma += f.preturi[i];
    }
    return suma / f.nrProduse;
}

void modificareDenumire(Farmacie *f, const char *nouaDenumire) {
    free(f->denumire); 
    f->denumire = (char*)malloc(sizeof(char) * (strlen(nouaDenumire) + 1));
    strcpy(f->denumire, nouaDenumire);
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


Farmacie* filtreazaFarmacii(Farmacie* farmacii, int nrFarmacii, int* nrRezultate, int pragProduse) {
    *nrRezultate = 0;
    for (int i = 0; i < nrFarmacii; i++) {
        if (farmacii[i].nrProduse >= pragProduse) {
            (*nrRezultate)++;
        }
    }

    
    Farmacie* rezultate = (Farmacie*)malloc((*nrRezultate) * sizeof(Farmacie));
    if (!rezultate) return NULL; 

    int j = 0;
    for (int i = 0; i < nrFarmacii; i++) {
        if (farmacii[i].nrProduse >= pragProduse) {
            rezultate[j++] = farmacii[i]; 
        }
    }

    return rezultate;
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
    int nrFarmacii = 0;
    Farmacie* farmacii = citesteFarmaciiDinFisier("farmacii.txt", &nrFarmacii);

    if (farmacii == NULL) {
        printf("Nu s-au putut citi farmaciile din fisier.\n");
        return 1;
    }

    
    if (nrFarmacii > 0) {
        salveazaFarmacieInFisier(farmacii[0], "farmacie_noua.txt");
    }

    
    salveazaVectorFarmaciiInFisier(farmacii, nrFarmacii, "farmacii_salvate.txt");

    
    for (int i = 0; i < nrFarmacii; i++) {
        free(farmacii[i].denumire);
        free(farmacii[i].preturi);
    }
    free(farmacii);

    return 0;
}
