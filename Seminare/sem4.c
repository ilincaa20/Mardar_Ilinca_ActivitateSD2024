#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Aeroport Aeroport;
struct Aeroport {
	int id;
	char* nume;
	int nrTerminale;
};


struct Aeroport* adaugaAeroport(struct Aeroport* vector, struct Aeroport aeroport, int* dimensiune) {
	struct Aeroport* copie = (struct Aeroport*)malloc(sizeof(struct Aeroport) * ((*dimensiune) + 1));

	copie[(*dimensiune)] = aeroport;

	for (int i = 0; i < (*dimensiune); i++) {
		copie[i] = vector[i];
	}
	
	if(vector)
		free(vector);

	(*dimensiune)++;
	return copie;
}

struct Aeroport* citireFisier(const char* numeFisier, int* nrAeroporturi) {
	struct Aeroport* vectorAeroporturi;
	vectorAeroporturi = NULL;
	FILE* f = fopen(numeFisier, "r");


	if (f != NULL) {
		(*nrAeroporturi) = 0;
		char buffer[100];
		while (fgets(buffer, 99, f)!=NULL) {
			struct Aeroport aeroport;
			char delimitator[] = ",\n";
			char* token = strtok(buffer, delimitator);
			aeroport.id = atoi(token);
			token = strtok(NULL, delimitator);
			aeroport.nume = malloc(sizeof(char) * (strlen(token)+1));
			strcpy(aeroport.nume, token);
			token = strtok(NULL, delimitator);
			aeroport.nrTerminale = atoi(token);

			vectorAeroporturi = adaugaAeroport(vectorAeroporturi, aeroport, nrAeroporturi);
		}

		fclose(f);
	}
	return vectorAeroporturi;

}


void citireMatriceFisier(const char* numeFisier, int* nrAeroporturiPeLinie, int nrClustere, Aeroport** aeroporturi) {
	FILE* f = fopen(numeFisier, "r");

	if (f != NULL) {
		char buffer[100];
		while (fgets(buffer, 99, f)!=NULL) {
			struct Aeroport aeroport;
			char delimitator[] = ",\n";
			char* token = strtok(buffer, delimitator);
			aeroport.id = atoi(token);
			token = strtok(NULL, delimitator);
			aeroport.nume = malloc(sizeof(char) * (strlen(token)+1));
			strcpy(aeroport.nume, token);
			token = strtok(NULL, delimitator);
			aeroport.nrTerminale = atoi(token);
            int poz= aeroport.nrTerminale %2;
            
			aeroporturi[poz] = adaugaAeroport(aeroporturi[poz], aeroport, &(nrAeroporturiPeLinie[poz]));
		}

		fclose(f);
	}
	
}




void afisareAeroport(struct Aeroport* aeroport)
{
    if(aeroport != NULL){
        printf("%d. %s are %d terminale.\t", aeroport->id,aeroport->nume,aeroport->nrTerminale);
    }


}

void afisareMatrice(Aeroport** matrice, int nrClustere, int*nrColoane){
    for(int i=0;i<nrClustere;i++){
        for(int j=0;j< nrColoane[i];j++)
        {
            afisareAeroport(&matrice[i][j]);
        }
        printf("\n");
    }
}


struct Aeroport idCautat(struct Aeroport* aeroporturi, int dim, int idCautat)
{

	for (int i = 0; i < dim; i++)
	{
		if (aeroporturi[i].id == idCautat)
		{
			return aeroporturi[i];

		}

	}
	struct Aeroport aeroport;
	aeroport.id = -1;
	aeroport.nrTerminale = -1;
	aeroport.nume = malloc(sizeof(char) * (1 + strlen("N/A")));
	strcpy(aeroport.nume, "N/A");
	return aeroport;

}

float mediaTerminale(Aeroport** matrice, int* nrAeroporturiPeLinie, int paritate) {
    paritate = paritate % 2;
    float suma = 0;
    for (int i = 0; i < nrAeroporturiPeLinie[paritate]; i++) {
        suma += matrice[paritate][i].nrTerminale;
    }
    if (nrAeroporturiPeLinie[paritate] > 0) {
        return suma / nrAeroporturiPeLinie[paritate];
    }
    return 0;
}


Aeroport* cautareDupaId(Aeroport** matrice, int* nrColoane, int nrLinii, int id){
    for(int i=0;i<nrLinii;i++){
        for(int j=0;j<nrColoane[i];i++){
            if(matrice[i][j].id == id){
                return &(matrice[i][j]);
            }
        }
    }
    return NULL;
}


void dezalocareVector(struct Aeroport** aeroporturi, int* dim)
{
	for (int i = 0; i < (*dim); i++)
	{
		free((*aeroporturi)[i].nume);
	}
	free(*aeroporturi);
	(*aeroporturi)= NULL;
	(*dim) = 0;
}

void dezalocareMatrice(Aeroport** matrice, int** nrColoane, int* nrClustere){
    for(int i=0;i<(*nrClustere);i++){
        dezalocareVector(&((*matrice)[i]),&((*nrColoane)[i]));
    }
    free(*matrice);
    (*matrice) = NULL;
    free(*nrClustere);
    (*nrColoane)= NULL;
    (*nrClustere)= 0;
}

void main() {

Aeroport** aeroporturi;
int nrLinii=2; 
int* nrColoane = (int*)malloc(sizeof(int)*nrLinii);

aeroporturi =(Aeroport**)malloc(sizeof(Aeroport*)*nrLinii);
for(int i=0;i<nrLinii;i++){
    nrColoane[i]=0;
   aeroporturi[i]= NULL;
}

citireMatriceFisier("aeroporturi.txt",nrColoane,nrLinii,aeroporturi);
afisareMatrice(aeroporturi,nrLinii, nrColoane);

float medie= mediaTerminale(aeroporturi,nrColoane,12);
printf("Media paritate -> para: %.2f",medie);

printf("---------\n");
afisareAeroport(cautareDupaId(aeroporturi,nrColoane,nrLinii,2));



dezalocareMatrice(&aeroporturi,&nrColoane,&nrLinii);
	
}