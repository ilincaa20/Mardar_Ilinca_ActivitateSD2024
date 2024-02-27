#include<stdio.h>
#include<stdlib.h>
#include <string.h>
typedef struct Calculator Calculator;

struct Calculator {
    char* serie;
    float pret;
    int nrPorturi;
};

Calculator initializare(const char* serie, float pret, int nrPorturi){
    Calculator c;
    c.serie = (char*)malloc(sizeof(char)*(strlen(serie+1)));
    strcpy(c.serie, serie);
    c.pret = pret;
    c.nrPorturi = nrPorturi;
    return c;
}

void afisareCalculator(Calculator c){
    printf("Calculatorul cu seria %s, costa %5.2f si are %d porturi\n",c.serie,c.pret,c.nrPorturi);
}

void afisareVectorCalculatoare(Calculator* vector,int nrCalculatoare){
    if(nrCalculatoare>0 && vector!= NULL){
    for(int i=0;i<nrCalculatoare;i++){
        afisareCalculator(vector[i]);
    }
    }
}

Calculator* copiazaNCalculatoare(Calculator* calculatoare, int nrCalculatoare, int nrCalculatoareCopiate){
    Calculator* copiate = (Calculator*)malloc(sizeof(Calculator)* nrCalculatoareCopiate);
    for(int i=0;i<nrCalculatoareCopiate;i++){
        copiate[i]=calculatoare[i];
        copiate[i].serie=(char*)malloc(sizeof(char)*(1+strlen(calculatoare[i].serie)));
        strcpy(copiate[i].serie,calculatoare[i].serie);
    }
    return copiate;
}


Calculator* copiazaCalculatoareCuMultePorturi(Calculator* calculatoare, int nrCalculatoare, int nrPorturiPrag, int* nrCalculatoarePorturi){
    *nrCalculatoarePorturi=0;
    for(int i=0;i<nrCalculatoare;i++){
        if(calculatoare[i].nrPorturi>nrPorturiPrag){
            (*nrCalculatoarePorturi)++;

        }
    }
    Calculator* v=(Calculator*)malloc(sizeof(Calculator)*(*nrCalculatoarePorturi));
    int k=0;
    for(int i=0;i<nrCalculatoare;i++){
        if(calculatoare[i].nrPorturi>nrPorturiPrag){
            v[k++]=initializare(calculatoare[i].serie,calculatoare[i].pret,calculatoare[i].nrPorturi);
        }
    }
    return v;
}

void dezalocare(Calculator** vector, int* dim){
    for(int i=0;i<(*dim);i++){
        free((*vector)[i].serie);

    }
    free(*vector);
    *vector=NULL;
    *dim=0;
}

void main(){

    int nrCalculatoare=5;

    Calculator* calculatoare=(Calculator*)malloc(sizeof(Calculator)*nrCalculatoare);
    for(int i=0;i<nrCalculatoare;i++){
        calculatoare[i]=initializare("1234567890",i*20+10,i+1);
    }
    afisareVectorCalculatoare(calculatoare,nrCalculatoare);
    int nrCalculatoareCopiate=3;
    Calculator* calculatoareCopiate= copiazaNCalculatoare(calculatoare,nrCalculatoare,nrCalculatoareCopiate);
    printf("\n\n");
    afisareVectorCalculatoare(calculatoareCopiate,nrCalculatoareCopiate);

    printf("\n\n");
    int nrCalculatoarePorturi=0;
    Calculator* calculatoarePorturi = copiazaCalculatoareCuMultePorturi(calculatoare,nrCalculatoare,3,&nrCalculatoarePorturi);
    
    dezalocare(&calculatoare, &nrCalculatoare);
    dezalocare(&calculatoareCopiate,&nrCalculatoareCopiate);
    dezalocare(&calculatoarePorturi, &nrCalculatoarePorturi);
}