#include<stdio.h>
#include<stdlib.h>
#include <string.h>
typedef struct Farmacie Farmacie;

struct Farmacie {

int cod;
char* denumire;
int nrProduse;
float* preturi;

};

Farmacie initializare(int cod, const char* denumire, int nrProduse, float* preturi){
    Farmacie f;
    f.cod = cod;
    f.denumire=(char*)malloc(sizeof(char)*(strlen(denumire)+1));
    strcpy(f.denumire,denumire);
    f.nrProduse = nrProduse;
     f.preturi=(float*)malloc(sizeof(float)*nrProduse);
    for(int i=0;i<nrProduse;i++){
        f.preturi[i]=preturi[i];
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

void afisareFarmacie(Farmacie f){
    printf("Farmacia cu codul %d se numeste %s, are %d produse, cu preturile %5.2f \n",f.cod,f.denumire,f.nrProduse,f.preturi);
}



void main(){
    float preturi[]={20,56,90};
    Farmacie f;
    Farmacie f1 = initializare(1,"Catena",3,preturi);
    Farmacie f3 = initializare(4,"DrMax",3,preturi);
    afisareFarmacie(f1);
    Farmacie f2 = citireTastatura();

    printf("\n\n");
     float pretMediu = calculeazaPretMediu(f1);
    printf("Pretul mediu al produselor din farmacia %s este: %.2f\n", f1.denumire, pretMediu);

    modificareDenumire(&f1,"HelpNet");
    printf("Noua denumire: %s\n", f1.denumire);
    afisareFarmacie(f2);

    free(f1.denumire);
    free(f1.preturi);
}