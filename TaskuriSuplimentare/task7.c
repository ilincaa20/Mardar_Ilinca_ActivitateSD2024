#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Cladire Cladire;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

 struct Cladire {
    int id;
    int an_constructie;
    char* nume;
};

 struct Nod {
    Cladire info;
    struct nod* next;
};

 struct HashTable{
    Nod** vector;
    int dim;
};

HashTable initializareHashTable(int dimensiune) {
    HashTable hash;
    hash.dim = dimensiune;
    hash.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
    for (int i = 0; i < dimensiune; i++) {
        hash.vector[i] = NULL;
    }
    return hash;
}

Cladire initCladire(int id, int an_constructie, const char* nume) {
    Cladire c;
    c.id = id;
    c.an_constructie = an_constructie;
    c.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(c.nume, nume);
    return c;
}

int calculHash(int an_constructie, int dimensiune) {
    if (dimensiune > 0) {
        return an_constructie % dimensiune;
    }
    return -1;
}


void inserareLaFinal(Nod** cap, Cladire cladire) {
    Nod* temp = (Nod*)malloc(sizeof(Nod));
    temp->info = cladire;
    temp->next = NULL;
    if (*cap) {
        Nod* aux = *cap;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = temp;
    }
    else {
        *cap = temp;
    }
}

void inserareCladireInTabela(HashTable hash, Cladire cladire) {
    if (hash.dim > 0) {
        int pozitie = calculHash(cladire.an_constructie, hash.dim);
        inserareLaFinal(&(hash.vector[pozitie]), cladire);
    }
}

void afisareCladire(Cladire c) {
    printf("ID: %d, An constructie: %d, Nume: %s\n", c.id, c.an_constructie, c.nume);
}

void afisareListaCladiri(Nod* cap) {
    while (cap) {
        afisareCladire(cap->info);
        cap = cap->next;
    }
}

void afisareTabela(HashTable tabela) {
    for (int i = 0; i < tabela.dim; i++) {
        if (tabela.vector[i] != NULL) {
            printf("Clusterul pentru anul %d:\n", i);
            afisareListaCladiri(tabela.vector[i]);
        }
    }
}
void stergeCladireDupaID(HashTable *tabela, int id) {
    for (int i = 0; i < tabela->dim; i++) {
        Nod *curr = tabela->vector[i], *prev = NULL;
        while (curr) {
            if (curr->info.id == id) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    tabela->vector[i] = curr->next;
                }
                free(curr->info.nume);
                free(curr);
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
}

void modificaAnConstructie(HashTable *tabela, int id, int nouAnConstructie) {
    for (int i = 0; i < tabela->dim; i++) {
        Nod *curr = tabela->vector[i];
        while (curr) {
            if (curr->info.id == id) {
                int nouIndex = calculHash(nouAnConstructie, tabela->dim);
                if (i != nouIndex) {
                    
                    stergeCladireDupaID(tabela, id);
                    curr->info.an_constructie = nouAnConstructie;
                    inserareCladireInTabela(*tabela, curr->info);
                } else {
                    curr->info.an_constructie = nouAnConstructie;
                }
                return;
            }
            curr = curr->next;
        }
    }
}

Nod* copiazaListaCladiri(Nod* cap) {
    if (!cap) return NULL;
    Nod* nouCap = NULL, *nouNod, *ultim = NULL;
    while (cap) {
        nouNod = (Nod*)malloc(sizeof(Nod));
        nouNod->info = initCladire(cap->info.id, cap->info.an_constructie, cap->info.nume);
        nouNod->next = NULL;
        if (ultim) {
            ultim->next = nouNod;
        } else {
            nouCap = nouNod;
        }
        ultim = nouNod;
        cap = cap->next;
    }
    return nouCap;
}

void afisareCopie(Nod* cap) {
    printf("Copie a listei de cladiri:\n");
    afisareListaCladiri(cap);
}

int main() {
    HashTable tabela = initializareHashTable(10);

    inserareCladireInTabela(tabela, initCladire(1, 1990, "Cladirea A"));
    inserareCladireInTabela(tabela, initCladire(2, 1992, "Cladirea B"));
    inserareCladireInTabela(tabela, initCladire(3, 1990, "Cladirea C"));

    printf("Tabela initiala:\n");
    afisareTabela(tabela);

    stergeCladireDupaID(&tabela, 2);
    printf("\nDupa stergerea cladirii cu ID 2:\n");
    afisareTabela(tabela);

    modificaAnConstructie(&tabela, 3, 1991);
    printf("\nDupa modificarea anului de constructie pentru cladirea cu ID 3:\n");
    afisareTabela(tabela);

    Nod* copie = copiazaListaCladiri(tabela.vector[calculHash(1991, tabela.dim)]);
    afisareCopie(copie);

    return 0;
}
