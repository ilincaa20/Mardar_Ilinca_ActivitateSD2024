#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Nod Nod;
typedef struct Biblioteca Biblioteca;

struct Biblioteca {
    char* nume;
    int nrCarti;
    int nrCititori;
};


struct Nod {
    Biblioteca info;
    Nod* next;
};

Biblioteca initializare(const char* nume, int nrCarti, int nrCititori){
    Biblioteca b;
    b.nume = (char*)malloc(sizeof(char)*(strlen(nume)+1));
    strcpy(b.nume,nume);
    b.nrCarti = nrCarti;
    b.nrCititori = nrCititori;
    return b;
}

Nod* inserareInceput(Nod* cap, Biblioteca b){
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info= initializare(b.nume,b.nrCarti,b.nrCititori);
    nou->next=cap;
    return nou;
}

char* getBibliotecaNrCartiPerCititor(Nod* cap){
    float max = 0;
    char* numeAux = NULL;
    while(cap) {
        if(cap->info.nrCarti / cap->info.nrCititori > max){
            max = cap->info.nrCarti / cap->info.nrCititori ;
            numeAux = cap->info.nume;
        }
        cap = cap->next;
    }
    char* nume = (char*)malloc(sizeof(char)*(strlen(numeAux)+1));
    strcpy(nume,numeAux);
    return nume;
}


void inserareSfarsit(Nod** cap, Biblioteca b){
    Nod* sfarsit=(Nod*)malloc(sizeof(Nod));
    sfarsit->info = initializare(b.nume,b.nrCarti,b.nrCititori);
    sfarsit->next = NULL;
    if((*cap)!=NULL){
        Nod* aux = (*cap);
        while(aux->next !=NULL){
            aux = aux->next;
        }
        aux->next = sfarsit;
    }
    else{
        (*cap) = sfarsit; 
    }
}


void stergereLista(Nod** cap){
    while((*cap) !=NULL){
        free((*cap)->info.nume);
        Nod* copie= *cap;
         *cap = (*cap)->next;
         free(copie);
    }
}

void afisareLista(Nod* cap){
    while(cap !=NULL){
        printf("Biblioteca %s are %d carti si %d cititori.\n",cap->info.nume,cap->info.nrCarti,cap->info.nrCititori);
        cap = cap->next; //procesare
    }
}



void main(){
    Nod* cap = NULL;
    Biblioteca b1 = initializare("Mihai Eminescu",2,5);
    cap = inserareInceput(cap, b1);
    Biblioteca b2 = initializare("Ioan Slavici",3,6);
    cap = inserareInceput(cap, b2);
    Biblioteca b3 = initializare("Ion Creanga",4,8);
    cap = inserareInceput(cap, b3);

    afisareLista(cap);
    printf("Biblioteca cu media maxima: %s",getBibliotecaNrCartiPerCititor(cap));

    Biblioteca b4 = initializare("B4",200,50);
    inserareSfarsit(&cap,b4);
    afisareLista(cap);

    stergereLista(&cap);
    afisareLista(cap);



}