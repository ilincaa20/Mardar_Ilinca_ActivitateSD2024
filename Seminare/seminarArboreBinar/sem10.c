#include<stdio.h>
//#include<malloc.h>
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

//cautare dupa ID tema

void main(){
    Nod* radacina = citireFisier("elevi.txt");
    afisareArboreInOrdine(radacina);

    printf("Media este: %5.2f", calculMedieGenerala(radacina));
}