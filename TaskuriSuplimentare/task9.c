#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>
typedef struct Elev Elev;
typedef struct Nod Nod;

struct Elev{
    int id;
    int nrMedii;
    float* medii;
    
};

struct Nod{
    Elev info;
    Nod* stanga;
    Nod* dreapta;
};

Elev initializare(int id, int nrMedii, float* medii){
    Elev e;
    e.id=id;
    e.nrMedii=nrMedii;
    e.medii=(float*)malloc(sizeof(float)*nrMedii);
    for(int i=0;i<nrMedii;i++){
        e.medii[i]=medii[i];
    }
    return e;
}


void inserareArbore(Nod** radacina, Elev e){
    if((*radacina)!=NULL){
    if((*radacina)->info.id>e.id){
        inserareArbore(&((*radacina)->stanga),e);
       }else{
        inserareArbore(&((*radacina)->dreapta),e);
       }
    }else{
        Nod* nou=(Nod*)malloc(sizeof(Nod));
        nou->info=e;
        nou->dreapta=NULL;
        nou->stanga=NULL;

        *radacina=nou;
    }
}

Elev citireElev(FILE* f){
    Elev e;
    fscanf(f,"%d",&e.id);
    fscanf(f,"%d",&e.nrMedii);
    e.medii=(float*)malloc(sizeof(float)*e.nrMedii);
    for(int i=0;i<e.nrMedii;i++){
        fscanf(f,"%f",&(e.medii[i]));
    }
    return e;
}

Nod* citireFisier(const char* fisier){
    Nod* arbore=NULL;

    if(!fisier){
        return NULL;
    }else{
       FILE* f=fopen(fisier,"r"); 
       int nrElevi;
       fscanf(fisier,"%d",&nrElevi);
       for(int i=0;i<nrElevi;i++){
        Elev e= citireElev(fisier);
        inserareArbore(&arbore,e);
       }
       fclose(fisier);
    }
    return arbore;
    
}

void afisareElev(Elev e){
    printf("%d %d",e.id,e.nrMedii);
    for(int i=0;i<e.nrMedii;i++){
        printf("%5.2f",e.medii[i]);
    }
    printf("\n");
}

//SRD, RSD, SDR
void afisareArboreInOrdine(Nod* radacina){
    if(radacina){
        afisareArboreInOrdine(radacina->stanga);
        afisareElev(radacina->info);
        afisareArboreInOrdine(radacina->dreapta);
    }
}

float calculeazaMedieElevi(Elev e){
    if(e.nrMedii !=0){
    float suma =0;
    for(int i=0;i<e.nrMedii;i++){
        suma += e.medii[i];
    }
    return suma / e.nrMedii;
    }
    else{
        printf("Elevul nu are medii");
    }
}

float calculSumaNote(Nod* radacina, int* nrNote){
    if(radacina!=NULL){
        float suma = 0;
        //r
        for(int i=0;i<radacina->info.nrMedii;i++){
        suma += radacina->info.medii[i];
    }
    (*nrNote) += radacina->info.nrMedii;
    //s
    float sumaST = calculSumaNote(radacina->stanga,nrNote);
    //d
    float sumaDR = calculSumaNote(radacina->dreapta,nrNote);
    
    return suma + sumaST + sumaDR;
    } else{
        return 0;
    }
}

float calculMedieGenerala(Nod* radacina) {
    int nrNote=0;
    float suma = calculSumaNote(radacina,&nrNote);
    if(nrNote>0){
    return suma/ nrNote;
    }
    else{ return 0;}
}
Nod* stergeNod(Nod* radacina, int id) {
    if (radacina == NULL) return radacina;

    
    if (id < radacina->info.id) {
        radacina->stanga = stergeNod(radacina->stanga, id);
    } else if (id > radacina->info.id) {
        radacina->dreapta = stergeNod(radacina->dreapta, id);
    } else {
       
        if (radacina->stanga == NULL) {
            Nod* temp = radacina->dreapta;
            free(radacina->info.medii);
            free(radacina);
            return temp;
        } else if (radacina->dreapta == NULL) {
            Nod* temp = radacina->stanga;
            free(radacina->info.medii);
            free(radacina);
            return temp;
        }

        Nod* temp = radacina->dreapta;
        while (temp && temp->stanga != NULL) temp = temp->stanga;

       
        radacina->info = temp->info;
      
        radacina->dreapta = stergeNod(radacina->dreapta, temp->info.id);
    }
    return radacina;
}

int inaltime(Nod* radacina) {
    if (radacina == NULL) return 0;
    int inaltimeStanga = inaltime(radacina->stanga);
    int inaltimeDreapta = inaltime(radacina->dreapta);
    return (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
}

Nod* subarboreCuInaltimeMaiMare(Nod* radacina) {
    if (radacina == NULL) return NULL;
    int inaltimeStanga = inaltime(radacina->stanga);
    int inaltimeDreapta = inaltime(radacina->dreapta);
    return inaltimeStanga > inaltimeDreapta ? radacina->stanga : radacina->dreapta;
}

int numarNoduri(Nod* radacina) {
    if (radacina == NULL) return 0;
    return 1 + numarNoduri(radacina->stanga) + numarNoduri(radacina->dreapta);
}

Nod* subarboreCuMaiMulteNoduri(Nod* radacina) {
    if (radacina == NULL) return NULL;
    int nrNoduriStanga = numarNoduri(radacina->stanga);
    int nrNoduriDreapta = numarNoduri(radacina->dreapta);
    return nrNoduriStanga > nrNoduriDreapta ? radacina->stanga : radacina->dreapta;
}


int main() {
    Nod* radacina = citireFisier("elevi.txt");
    
    printf("Arborele initial in ordine:\n");
    afisareArboreInOrdine(radacina);
    printf("\n");

  
    int idDeSters = 3; 
    radacina = stergeNod(radacina, idDeSters);
    printf("Arborele dupa stergerea nodului cu ID %d:\n", idDeSters);
    afisareArboreInOrdine(radacina);
    printf("\n");

    Nod* subarboreInalt = subarboreCuInaltimeMaiMare(radacina);
    if (subarboreInalt != NULL) {
        printf("Radacina subarborelui cu inaltimea mai mare are ID-ul %d:\n", subarboreInalt->info.id);
    } else {
        printf("Radacina este NULL sau nu are subarbori.\n");
    }
  
    int numarTotalNoduri = numarNoduri(radacina);
    printf("Numarul total de noduri in arbore este: %d\n", numarTotalNoduri);

    Nod* subarboreCuMaiMulte = subarboreCuMaiMulteNoduri(radacina);
    if (subarboreCuMaiMulte != NULL) {
        printf("Radacina subarborelui cu mai multe noduri are ID-ul %d:\n", subarboreCuMaiMulte->info.id);
    } else {
        printf("Radacina este NULL sau nu are subarbori.\n");
    }

    printf("Media este: %5.2f\n", calculMedieGenerala(radacina));

    return 0;
}
