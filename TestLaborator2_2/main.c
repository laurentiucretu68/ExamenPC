#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

///Definirea structurii produs
struct produs{
    char nume[30];
    float pret;
    int nr_magazine;
    struct magazin *m;
};

///Definirea structurii magazin
struct magazin{
    char nume[30];
    int gps_x;
    int gps_y;
};

///Definirea unui nod al listei
struct nod{
    struct produs prod;
    struct nod* urm;
};

///Adaugarea elementelor vectorului de produse in lista simplu inlantuita
///Adaugarea elementelor se realizeaza la finalul listei
void adaugareElementeLista(struct nod **cap,struct produs p){
    struct nod *curent = (*cap);
    if ((*cap)==NULL){
        struct nod *nou = (struct nod*)malloc(sizeof(struct nod));
        nou->prod = p;
        nou->urm = (*cap);
        (*cap) = nou;
        return;
    }
    while (curent->urm!=NULL)
        curent = curent->urm;
    struct nod *nou = (struct nod*)malloc(sizeof(struct nod));
    nou->prod = p;
    nou->urm = NULL;
    curent->urm = nou;
}

///Functie care afiseaza elementele din lista
void afisareLista(struct nod *cap){
    while (cap!=NULL){
        printf("%s %.2f %d ",cap->prod.nume,cap->prod.pret,cap->prod.nr_magazine);
        for (int j=0; j<cap->prod.nr_magazine; j++)
            printf("%s %d %d ",cap->prod.m[j].nume,cap->prod.m[j].gps_x,cap->prod.m[j].gps_y);
        printf("\n");
        cap = cap->urm;
    }
}

///Functie de citire a produselor din fisier
void citire(struct produs **p,int *nrProduse,FILE* fin){
    *nrProduse = 0;
    *p =(struct produs*)malloc(sizeof(struct produs));
    char line[500];
    while (fgets(line,500,fin)){ ///Citirea se realizeaza la nivel de linie de fisier
        *p = realloc(*p,((*nrProduse)+1)*sizeof(struct produs));
        char *campuri = strtok(line," ,"); ///Se sparge linia in campurile unui produs cu functia strtok
        strcpy((*p)[*nrProduse].nume,campuri); campuri = strtok(NULL," ,"); ///Memorarea numelui
        (*p)[*nrProduse].pret = atof(campuri); campuri = strtok(NULL," ,"); ///Memorarea pretului
        int contor_magazine = 0; ///Retine numarul de magazine pentru fiecare produs
        (*p)[*nrProduse].m = (struct magazin*)malloc(sizeof(struct magazin));
        ///Memorarea campurilor magazinelor unui produs
        while (campuri){
            (*p)[*nrProduse].m = realloc((*p)[*nrProduse].m,(contor_magazine+1)*sizeof(struct magazin));
            strcpy((*p)[*nrProduse].m[contor_magazine].nume,campuri); campuri = strtok(NULL," ,");
            (*p)[*nrProduse].m[contor_magazine].gps_x = atoi(campuri); campuri = strtok(NULL," ,");
            (*p)[*nrProduse].m[contor_magazine].gps_y = atoi(campuri);
            campuri = strtok(NULL," ,");
            contor_magazine++;
        }
        (*p)[*nrProduse].nr_magazine = contor_magazine;
        (*nrProduse) = (*nrProduse)+1;
    }
}

///Functie ce afiseaza produsele din vector
void afisare(struct produs *p,int nrProduse){
    for (int i=0; i<nrProduse; i++){
        printf("%s %.2f %d ",p[i].nume,p[i].pret,p[i].nr_magazine);
        for (int j=0; j<p[i].nr_magazine; j++)
            printf("%s %d %d ",p[i].m[j].nume,p[i].m[j].gps_x,p[i].m[j].gps_y);
        printf("\n");
    }
}

///Functie ce returneaza numele magazinului aflat la distanta cea
/// mai mica fata de coordonatele date ca si argumente
char *locatie(char *nume_produs,int coordonata_x,int coordonata_y,struct produs *p,int nrProduse){
    for (int i=0; i<nrProduse; i++){
        if (strcmp(nume_produs,p[i].nume)==0){
            float distanta_minima = sqrt(pow(coordonata_x-p[i].m[0].gps_x,2)+pow(coordonata_y-p[i].m[0].gps_y,2));
            char *nume_magazin;
            nume_magazin = (char*)malloc(30*sizeof(char));
            strcpy(nume_magazin,p[i].m[0].nume);
            for (int j=1; j<nrProduse; j++){
                float distanta_curenta = sqrt(pow(coordonata_x-p[i].m[j].gps_x,2)+pow(coordonata_y-p[i].m[j].gps_y,2));
                if (distanta_curenta<distanta_minima){
                    distanta_minima = distanta_curenta;
                    strcpy(nume_magazin,p[i].m[j].nume);
                }
            }
            return nume_magazin;
        }
    }
}

///Functie comparatoare pentru functia qsort
int cmp(const void* a,const void* b){
    struct produs p1 = *((struct produs*)a);
    struct produs p2 = *((struct produs*)b);
    if (p1.pret>p2.pret)
        return 1;
    if (p1.pret==p2.pret){
        if (p1.nr_magazine>p2.nr_magazine)
            return 1;
        if (p1.nr_magazine==p2.nr_magazine)
            return 0;
    }
    return -1;
}

///Functia de sortare
void sortare(struct produs **p,int nrProduse){
    qsort(*p,nrProduse,sizeof(struct produs),cmp);
}

///Functie ce elimina nodul din lista cu numele *nume*
void eliminare(struct nod **L, char *nume){
    if (strcmp((*L)->prod.nume,nume)==0){
        struct nod *tinta = (*L);
        (*L) = (*L)->urm;
        free(tinta);
        return;
    }
    struct nod *curent = (*L);
    while (strcmp(curent->urm->prod.nume,nume)!=0 && curent->urm!=NULL)
        curent = curent->urm;
    struct nod *tinta = curent->urm;
    curent->urm = curent->urm->urm;
    free(tinta);
}

int main() {
    int nrProduse;
    struct produs *p;
    struct nod *cap = NULL;
    FILE* fin = fopen("produse.txt","r");
    citire(&p,&nrProduse,fin);
    printf("\n%s ",locatie("Mouse",43,65,p,nrProduse));
    sortare(&p,nrProduse);
    afisare(p,nrProduse);

    ///Adauga elementele in lista
    for (int i=0; i<nrProduse; i++)
        adaugareElementeLista(&cap,p[i]);
    printf("\n\nLista:\n");
    eliminare(&cap,"Mouse");
    afisareLista(cap);
    fclose(fin); free(p);
    return 0;
}
