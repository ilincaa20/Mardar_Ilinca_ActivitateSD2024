#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Farmacie {
    int cod;
    char* denumire;
    int nrProduse;
    float* preturi;
} Farmacie;

typedef struct ListaDubla {
    Farmacie info;
    struct ListaDubla *prev, *next;
} ListaDubla;

typedef struct NodSimplu {
    Farmacie info;
    struct NodSimplu* next;
} NodSimplu;

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


void stergeNodDinListaDubla(ListaDubla** cap, int pozitie) {
    if (*cap == NULL) return;

    ListaDubla* temp = *cap;
    if (pozitie == 0) {
        *cap = temp->next;
        if (*cap != NULL) {
            (*cap)->prev = NULL;
        }
        free(temp->info.denumire);
        free(temp->info.preturi);
        free(temp);
        return;
    }

    for (int i = 0; temp != NULL && i < pozitie; i++) {
        temp = temp->next;
    }

    if (temp == NULL) return;

    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }

    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    }

    free(temp->info.denumire);
    free(temp->info.preturi);
    free(temp);
}

void insereazaSortatInListaDubla(ListaDubla** cap, Farmacie farmacie) {
    ListaDubla* nouNod = (ListaDubla*)malloc(sizeof(ListaDubla));
    nouNod->info = farmacie;
    nouNod->next = nouNod->prev = NULL;

    if (*cap == NULL || (*cap)->info.cod >= farmacie.cod) {
        nouNod->next = *cap;
        if (*cap != NULL) {
            (*cap)->prev = nouNod;
        }
        *cap = nouNod;
    } else {
        ListaDubla* temp = *cap;
        while (temp->next != NULL && temp->next->info.cod < farmacie.cod) {
            temp = temp->next;
        }
        nouNod->next = temp->next;
        if (temp->next != NULL) {
            temp->next->prev = nouNod;
        }
        temp->next = nouNod;
        nouNod->prev = temp;
    }
}

void afiseazaCrescator(ListaDubla* cap) {
    for (ListaDubla* temp = cap; temp != NULL; temp = temp->next) {
        printf("Cod: %d, Denumire: %s\n", temp->info.cod, temp->info.denumire);
    }
}


void afiseazaDescrescator(ListaDubla* cap) {
    ListaDubla* temp = cap;
    if (temp == NULL) return;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    while (temp != NULL) {
        printf("Cod: %d, Denumire: %s\n", temp->info.cod, temp->info.denumire);
        temp = temp->prev;
    }
}

NodSimplu* salveazaInListaSimpla(ListaDubla* cap, int pragNrProduse) {
    NodSimplu* capSimplu = NULL;
    NodSimplu** ultim = &capSimplu;
    while (cap != NULL) {
        if (cap->info.nrProduse > pragNrProduse) {
            *ultim = (NodSimplu*)malloc(sizeof(NodSimplu));
            (*ultim)->info = cap->info; 
            (*ultim)->next = NULL;
            ultim = &(*ultim)->next;
        }
        cap = cap->next;
    }
    return capSimplu;
}

void citesteFarmaciiDinFisierSiInitializeazaLista(const char* numeFisier, ListaDubla** cap) {
    FILE* file = fopen(numeFisier, "r");
    if (!file) {
        perror("Eroare la deschiderea fișierului");
        return;
    }
    
    int cod, nrProduse;
    char denumire[100];
    float preturi[100]; 

    while (fscanf(file, "%d,%[^,],%d", &cod, denumire, &nrProduse) == 3) {
        for (int i = 0; i < nrProduse; i++) {
            fscanf(file, ",%f", &preturi[i]);
        }
        
        Farmacie farmacie = initializare(cod, denumire, nrProduse, preturi);
        insereazaSortatInListaDubla(cap, farmacie);
    }
    
    fclose(file);
}

void main(){
    ListaDubla* listaFarmacii = NULL;
    
    citesteFarmaciiDinFisierSiInitializeazaLista("farmacii.txt", &listaFarmacii);
    
    
    printf("Lista farmaciilor (crescător):\n");
    afiseazaCrescator(listaFarmacii);
    
    printf("\nLista farmaciilor (descrescător):\n");
    afiseazaDescrescator(listaFarmacii);
    
   
    stergeNodDinListaDubla(&listaFarmacii, 2); 
    printf("\nLista după ștergere:\n");
    afiseazaCrescator(listaFarmacii);

}