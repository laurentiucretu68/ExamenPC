#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct produs{
    char nume[30];
    float pret;
    float distanta_maxima;
    int nr_locatii;
    struct locatie *L;
};

struct locatie{
    char *nume_sector;
    char gps_x;
    char gps_y;
};

struct nod{
    struct produs prod;
    struct nod* urm;
};

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

void afisareElementeLista(struct nod *cap){
    while (cap!=NULL){
        printf("%s %.2f %.2f ",cap->prod.nume,cap->prod.pret,cap->prod.distanta_maxima);
        for (int j=0; j<cap->prod.nr_locatii; j++)
            printf("%s %d %d ",cap->prod.L[j].nume_sector,cap->prod.L[j].gps_x,cap->prod.L[j].gps_y);
        printf("\n");
        cap = cap->urm;
    }
}

void citire(struct produs **p,int *nrProduse,FILE* fin){
    *nrProduse = 0;
    *p =(struct produs*)malloc(sizeof(struct produs));
    char line[300];
    while (fgets(line,300,fin)){
        *p = realloc(*p,((*nrProduse)+1)*sizeof(struct produs));
        char *sep = strtok(line," ,");
        strcpy((*p)[*nrProduse].nume,sep); sep = strtok(NULL," ,");
        (*p)[*nrProduse].pret = atof(sep); sep = strtok(NULL," ,");
        (*p)[*nrProduse].L = (struct locatie*)malloc(sizeof(struct locatie));
        int contor_locatii = 0;
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

void afisare(struct produs *p,int nrProduse){
    for (int i=0; i<nrProduse; i++){
        printf("%s %.2f %.2f ",p[i].nume,p[i].pret,p[i].distanta_maxima);
        for (int j=0; j<p[i].nr_locatii; j++)
            printf("%s %d %d ",p[i].L[j].nume_sector,p[i].L[j].gps_x,p[i].L[j].gps_y);
        printf("\n");
    }
}

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

int comp(const void *a,const void *b){
    struct locatie l1 = *((struct locatie*)a);
    struct locatie l2 = *((struct locatie*)b);

    if (strcmp(l1.nume_sector,l2.nume_sector)>0)
        return 1;
    if (strcmp(l1.nume_sector,l2.nume_sector)==0)
        return 0;
    return -1;
}

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

void sortare(struct produs **p,int nrProduse){
    qsort(*p,nrProduse,sizeof(struct produs),cmp);
}

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
    FILE* fin = fopen("C:\\Users\\asus\\Desktop\\Facultate\\Programarea Calculatoarelor\\Sesiune\\TestLaborator2_1\\produse.txt","r");
    citire(&p,&nrProduse,fin);
    distantaMaxima(&p,nrProduse);
    sortare(&p,nrProduse);
    afisare(p,nrProduse);
    for (int i=0; i<nrProduse; i++)
        adaugareElementeLista(&cap,p[i]);
    printf("\n\nLista:\n");
    adaugare(cap,"Mouse",p[0]);
    return 0;
}
