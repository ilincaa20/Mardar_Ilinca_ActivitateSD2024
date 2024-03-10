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
Farmacie citireTastatura(){
    Farmacie f;
    char buffer[100]; 
    printf("Introduceti id-ul: ");
    scanf("%d", &f.cod);
    printf("\nIntroduceti denumirea farmaciei: ");
    scanf("%s", buffer); 
    f.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(f.denumire, buffer); 
    printf("Introduceti nr produse: ");
    scanf("%d", &f.nrProduse); 
    f.preturi = (float*)malloc(sizeof(float) * f.nrProduse); 
    for (int i = 0; i < f.nrProduse; i++) {
        printf("Introduceti pretul produsului %d: ", i + 1);
        scanf("%f", &f.preturi[i]); 
    }

    return f;
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


int main() {
    int nrFarmacii = 5;
    Farmacie* farmacii = (Farmacie*)malloc(nrFarmacii * sizeof(Farmacie));
    if (!farmacii) {
        printf("Alocarea memoriei a eșuat.\n");
        return 1;
    }

    
    float preturi[] = {10.0, 15.5, 20.0};
    for (int i = 0; i < nrFarmacii; i++) {
        farmacii[i] = initializare(i, "Farmacie", 3, preturi);
    }
   
    printf("Farmacii inițiale:\n");
    afisareVectorFarmacii(farmacii, nrFarmacii);

    int nrFarmaciiFiltrate;
    Farmacie* farmaciiFiltrate = filtreazaFarmacii(farmacii, nrFarmacii, &nrFarmaciiFiltrate, 3);
 
    printf("\nFarmacii filtrate:\n");
    afisareVectorFarmacii(farmaciiFiltrate, nrFarmaciiFiltrate);

    for (int i = 0; i < nrFarmacii; i++) {
        free(farmacii[i].denumire);
        free(farmacii[i].preturi);
    }
    free(farmacii);

    for (int i = 0; i < nrFarmaciiFiltrate; i++) {
        free(farmaciiFiltrate[i].denumire);
        free(farmaciiFiltrate[i].preturi);
    }
    free(farmaciiFiltrate);

    return 0;
}