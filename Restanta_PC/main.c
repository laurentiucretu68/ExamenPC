#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lista_vechituri{
    int numar_obiecte;
    char *denumire_obiect;
};

struct destinatar{
    int numar_campuri;
    char *nume_prenume;
};

void citire(struct lista_vechituri ***matrice,struct destinatar **studenti,int *n,FILE* fin){
    fscanf(fin,"%d ",n);
    *studenti = (struct destinatar*)malloc(*n*sizeof(struct destinatar));
    *matrice  = (struct lista_vechituri**)malloc(*n*sizeof(struct lista_vechituri*));
    for (int i=0; i<*n; i++){
        char *line;
        line = (char*)malloc(300*sizeof(char));
        fgets(line,300,fin);
        char *cuv = strtok(line," ,./!*");
        (*matrice)[i] = (struct lista_vechituri*)malloc(sizeof(struct lista_vechituri));
        int contor_coloane = 0;
        while (cuv){
            (*matrice)[i] = realloc((*matrice)[i],(contor_coloane+1)*sizeof(struct lista_vechituri));
            (*matrice)[i][contor_coloane].numar_obiecte = atoi(cuv); cuv = strtok(NULL," ,./!*");
            (*matrice)[i][contor_coloane].denumire_obiect = (char*)malloc(30*sizeof(char));
            strcpy((*matrice)[i][contor_coloane].denumire_obiect,cuv); cuv = strtok(NULL," ,./!*");
            contor_coloane++;
            if (cuv[0]>'A' && cuv[0]<'Z')
                break;
        }
        (*studenti)[i].numar_campuri = contor_coloane;
        (*studenti)[i].nume_prenume = (char*)malloc(30*sizeof(char));
        strcpy((*studenti)[i].nume_prenume,cuv);
        free(line);
    }
}

void afisare(struct lista_vechituri **matrice,struct destinatar *studenti,int n){
    for (int i=0; i<n; i++){
        for (int j=0; j<studenti[i].numar_campuri; j++)
            printf("%d %s ",matrice[i][j].numar_obiecte,matrice[i][j].denumire_obiect);
        printf("%s\n",studenti[i].nume_prenume);
    }
}

void dezalocare_memorie(struct lista_vechituri ***matrice,struct destinatar **studenti,int n){
    for (int i=0; i<n; i++){
        for (int j=0; j<(*studenti)[i].numar_campuri; j++)
            free((*matrice)[i][j].denumire_obiect);
    }
    for (int i=0; i<n; i++){
        free((*matrice)[i]);
    }
    free(*matrice);
    for (int i=0; i<n; i++)
        free((*studenti)[i].nume_prenume);
    free(*studenti);
}

void adaugareElementNou(struct lista_vechituri ***matrice,struct destinatar **studenti,int *n,char *telegrama){
    (*n) = (*n)+1;
    *studenti = realloc(*studenti,*n*sizeof(struct destinatar));
    *matrice = realloc(*matrice,*n*sizeof(struct lista_vechituri*));
    (*matrice)[(*n)-1] = (struct lista_vechituri*)malloc(sizeof(struct lista_vechituri));
    char *cuv = strtok(telegrama," ,./!*");
    int contor_coloane = 0;
    while (cuv){
        (*matrice)[(*n)-1] = realloc((*matrice)[(*n)-1],(contor_coloane+1)*sizeof(struct lista_vechituri));
        (*matrice)[(*n)-1][contor_coloane].numar_obiecte = atoi(cuv); cuv = strtok(NULL," ,./!*");
        (*matrice)[(*n)-1][contor_coloane].denumire_obiect = (char*)malloc(30*sizeof(char));
        strcpy((*matrice)[(*n)-1][contor_coloane].denumire_obiect,cuv); cuv = strtok(NULL," ,./!*");
        contor_coloane++;
        if (cuv[0]>'A' && cuv[0]<'Z')
            break;
    }
    (*studenti)[(*n)-1].numar_campuri = contor_coloane;
    (*studenti)[(*n)-1].nume_prenume = (char*)malloc(30*sizeof(char));
    strcpy((*studenti)[(*n)-1].nume_prenume,cuv);
}

void scriere_fisier(struct lista_vechituri **matrice,struct destinatar *studenti,int n,FILE* fout){
    for (int i=0; i<n; i++){
        int lungime_nume = strlen(studenti[i].nume_prenume);
        for (int j=0; j<lungime_nume; j++){
            if (studenti[i].nume_prenume[j]>='A' && studenti[i].nume_prenume[j]<='Z')
                fprintf(fout,"%c",studenti[i].nume_prenume[j]);
        }
        for (int j=0; j<studenti[i].numar_campuri; j++)
            fprintf(fout,"%d %s ",matrice[i][j].numar_obiecte,matrice[i][j].denumire_obiect);
        fprintf(fout,"\n");
    }
}

int main(){
    int n;
    struct lista_vechituri **matrice;
    struct destinatar *studenti;
    FILE* fin = fopen("studenti.txt","r");
    FILE* fout = fopen("studenti.out","w");
    citire(&matrice,&studenti,&n,fin);
    char *telegrama;
    telegrama = (char*)malloc(100*sizeof(char));
    strcpy(telegrama,"3 tastaturi, 2 carti, 6 tricouri, Popescu_Ionel");
    adaugareElementNou(&matrice,&studenti,&n,telegrama);
    free(telegrama);
    scriere_fisier(matrice,studenti,n,fout);
    dezalocare_memorie(&matrice,&studenti,n);
    fclose(fin); fclose(fout);
    return 0;
}
