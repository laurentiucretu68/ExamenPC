#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

///Definirea structurii produs
struct produs{
    char nume[30];
    float pret;
    float distanta_maxima;
    int nr_locatii;
    struct locatie *L;
};

///Definirea structurii locatie
struct locatie{
    char *nume_sector;
    char gps_x;
    char gps_y;
};

///Definirea unui nod al listei
struct nod{
    struct produs prod;
    struct nod* urm;
};

///Adaugarea elementelor vectorului de produse in lista simplu inlantuita
///Adaugarea elementelor se realizeaza la finalul listei
void adaugareElementeLista(struct nod **cap,struct produs p){
    if ((*cap)==NULL){
        struct nod *nou = (struct nod*)malloc(sizeof(struct nod));
        nou->prod = p;
        nou->urm = (*cap);
        (*cap) = nou;
        return;
    }
    struct nod *curent = (*cap);
    while (curent->urm!=NULL)
        curent = curent->urm;
    struct nod *nou = (struct nod*)malloc(sizeof(struct nod));
    nou->prod = p;
    nou->urm = NULL;
    curent->urm = nou;
}

///Functie care afiseaza elementele din lista
void afisareElementeLista(struct nod *cap){
    while (cap!=NULL){
        printf("%s %.2f %.2f ",cap->prod.nume,cap->prod.pret,cap->prod.distanta_maxima);
        for (int j=0; j<cap->prod.nr_locatii; j++)
            printf("%s %d %d ",cap->prod.L[j].nume_sector,cap->prod.L[j].gps_x,cap->prod.L[j].gps_y);
        printf("\n");
        cap = cap->urm;
    }
}

///Functie de citire a produselor din fisier
void citire(struct produs **p,int *nrProduse,FILE* fin){
    *nrProduse = 0;
    *p =(struct produs*)malloc(sizeof(struct produs));
    char line[300];
    while (fgets(line,300,fin)){ ///Citirea se realizeaza la nivel de linie de fisier
        *p = realloc(*p,((*nrProduse)+1)*sizeof(struct produs));
        char *sep = strtok(line," ,"); ///Se sparge linia in campurile unui produs cu functia strtok
        strcpy((*p)[*nrProduse].nume,sep); sep = strtok(NULL," ,"); ///Memorarea numelui
        (*p)[*nrProduse].pret = atof(sep); sep = strtok(NULL," ,"); ///Memorarea pretului
        (*p)[*nrProduse].L = (struct locatie*)malloc(sizeof(struct locatie));
        int contor_locatii = 0; ///Retine numarul de locatii pentru fiecare produs
        while (sep){
            (*p)[*nrProduse].L = realloc((*p)[*nrProduse].L,(contor_locatii+1)*sizeof(struct locatie));
            (*p)[*nrProduse].L[contor_locatii].nume_sector = (char*)malloc(30*sizeof(char));
            strcpy((*p)[*nrProduse].L[contor_locatii].nume_sector,sep); sep = strtok(NULL," ,");
            (*p)[*nrProduse].L[contor_locatii].gps_x = (char)atoi(sep); sep = strtok(NULL," ,");
            (*p)[*nrProduse].L[contor_locatii].gps_y = (char)atoi(sep); sep = strtok(NULL," ,");
            contor_locatii++;
        }
        (*p)[*nrProduse].nr_locatii = contor_locatii;
        (*nrProduse) = (*nrProduse) + 1;
    }
}

///Functie ce afiseaza produsele din vector
void afisare(struct produs *p,int nrProduse){
    for (int i=0; i<nrProduse; i++){
        printf("%s %.2f %.2f ",p[i].nume,p[i].pret,p[i].distanta_maxima);
        for (int j=0; j<p[i].nr_locatii; j++)
            printf("%s %d %d ",p[i].L[j].nume_sector,p[i].L[j].gps_x,p[i].L[j].gps_y);
        printf("\n");
    }
}

///Functie ce determina distanta maxima intre doua localitati ale unui produs
void distantaMaxima(struct produs **p,int nrProduse){
    for (int i=0; i<nrProduse; i++){
        float distanta_maxima = 0;
        for (int j=0; j<(*p)[i].nr_locatii; j++)
            for (int k=j+1; k<(*p)[i].nr_locatii; k++){
                float distanta_curenta = sqrt(pow((*p)[i].L[j].gps_x-(*p)[i].L[k].gps_x,2)+pow((*p)[i].L[j].gps_y-(*p)[i].L[k].gps_y,2));
                if (distanta_curenta>distanta_maxima)
                    distanta_maxima = distanta_curenta;
            }
        (*p)[i].distanta_maxima = distanta_maxima;
    }
}

///Returneaza numarul de locatii duplicat pentru un produs
int numarDuplicate(struct produs prod){
    int duplicate=0;
    for (int i=0; i<prod.nr_locatii-1; i++)
        for (int j=i+1; j<prod.nr_locatii; j++){
            if (strcmp(prod.L[i].nume_sector,prod.L[j].nume_sector)==0){
                duplicate++;
            }
        }
    return duplicate;
}

///Functie comparatoare pentru functia qsort
int cmp(const void *a,const void *b){
    struct produs p1 = *((struct produs*)a);
    struct produs p2 = *((struct produs*)b);

    int duplicate_p1 = numarDuplicate(p1);
    int duplicate_p2 = numarDuplicate(p2);

    if (duplicate_p1>duplicate_p2)
        return 1;
    if (duplicate_p1==duplicate_p2)
        return 0;
    return -1;
}

///Functia de sortare
void sortare(struct produs **p,int nrProduse){
    qsort(*p,nrProduse,sizeof(struct produs),cmp);
}

///Functie de adaugare noduri in lista - subpunctul 4
void adaugare(struct nod *lista, char* nume_produs, struct produs p){
    struct nod *curent = lista;
    while (curent!=NULL){
        if (strcmp(curent->prod.nume,nume_produs)==0){
            struct nod *nou = (struct nod*)malloc(sizeof(struct nod));
            nou->prod = p;
            nou->urm = curent->urm;
            curent->urm = nou;
        }
        curent = curent->urm;
    }
    afisareElementeLista(lista);
}

int main() {
    struct produs *p;
    int nrProduse;
    struct nod *cap = NULL;
    FILE* fin = fopen("produse.txt","r");
    citire(&p,&nrProduse,fin);
    distantaMaxima(&p,nrProduse);
    sortare(&p,nrProduse);
    afisare(p,nrProduse);

    ///Adauga elementele in lista
    for (int i=0; i<nrProduse; i++)
        adaugareElementeLista(&cap,p[i]);
    printf("\n\nLista:\n");
    adaugare(cap,"Mouse",p[0]);
    return 0;
}
