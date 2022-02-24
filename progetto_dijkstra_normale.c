#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define INF INT_MAX

typedef struct{
    int dato;
    int indice;
}elemento;


int atoi_custom_1(char *stringa);
int atoi_custom_2(char stringa[]);
int Dijkstra(int matrice[][1000],int n,int sorgente);
void max_heapify(elemento array[],int dim,int indice);
void build_max_heap(elemento array[],int dim);
void aggiungi_valore(elemento array[],int k,int valore_cammini,int *indice);
void aggiungi_grafo(elemento array[],int n,int k,int *indice);



int main(){


int d=0,k=0; // d è il numero di nodi,k la lunghezza della classifica
int flag=0;
char *puntatore;
int cnt=0; // conta il numero di grafi da inserire!
int *temp=&cnt; //temp è il puntatore al contatore dei nodi dell'albero
char *token;
char comando[100];
char delimitatore[2]=" ";

puntatore=fgets(comando,100,stdin);
if(puntatore!=NULL){
    token=strtok(comando,delimitatore);
    d=atoi_custom_1(token);
    token=strtok(NULL,delimitatore);
    k=atoi_custom_2(token);
}

elemento array[k];

while(flag==0){
    puntatore=fgets(comando,100,stdin);
    if(puntatore==NULL){
        flag=1;
        break;
    }
    else{
        if(comando[0]=='A'){
            aggiungi_grafo(array,d,k,temp);
            *(temp)=*(temp)+1;
        }
        if(comando[0]=='T'){
            if(*(temp)!=0){
                //stampa classifica
                if(*(temp)<k){
                    for(int i=0;i<*(temp);i++){
                        printf("%d",array[i].indice);
                        if(i<*(temp)-1){
                            printf(" ");
                        }
                    }
                }
                else{
                    for(int i=0;i<k;i++){
                        printf("%d",array[i].indice);
                        if(i<k-1){
                            printf(" ");
                        }
                    }
                }
            }
            printf("\n");
        }
    }
}

    return 0;
}

void aggiungi_grafo(elemento array[],int n,int k,int *indice){ // n è il numero di nodi del grafo
    int matrice_grafo[1000][1000];
    int i=0,j=0,t=0,z=0;
    char riga[1000000];
    char sottoriga[1000000];
    char *puntatore;
    int flag=0,lunghezza=0;
    int valore_cammini_minimi;
    int cnt=0;
    while(flag==0){
        if(cnt<n){
            puntatore=fgets(riga,1000000,stdin);
            cnt++;
        }
        if(puntatore==NULL || riga[0]=='A' || riga[0]=='T' || (t==n && z==n)){
            flag=1;
            break;
        }
        z=0;
        lunghezza=strlen(riga);
        for(i=0;i<lunghezza;i++){
            if(riga[i]==','){
                matrice_grafo[t][z]=atoi_custom_1(sottoriga);
                z++;
                memset(sottoriga,0,j);
                j=0;
            }
            else{
                sottoriga[j]=riga[i];
                j++;
            }
        }
        matrice_grafo[t][z]=atoi_custom_2(sottoriga);
        memset(sottoriga,0,j);
        j=0;
        t++;
        z++;
    }
    //a questo punto ho letto la matrice e sono pronto ad applicare dijkstra
    valore_cammini_minimi=Dijkstra(matrice_grafo,n,0); // mi salvo in una variabile il valore dei cammini minimi del grafo appena inserito
    //inserimento valore in classifica
    aggiungi_valore(array,k,valore_cammini_minimi,indice);
}
int Dijkstra(int matrice[][1000],int n,int sorgente){
    int i,j,cnt=0;
    int coda_distanze[n],visitati[n];
    int next_node=0,min_distanza;
    int somma_cammini_minimi=0;
    //inizializzo la coda delle distanze e visitati
    for(i=0;i<n;i++){
        if(i!=sorgente){
            coda_distanze[i]=INF;
        }
        visitati[i]=0; // inizializzo visitati a 0,poiche non ho ancora visitato nessun nodo
    }
    //tratto il nodo sorgente
    coda_distanze[sorgente]=0; // la distanza dal nodo sorgente è nulla 
    cnt=0; // variabile che mi conta quanti nodi ho visitato
    while(cnt<n-1){
        //la prima cosa da fare è estrarre il nodo con costo minimo dalla coda
        min_distanza=INF;
        for(i=0;i<n;i++){
            if(coda_distanze[i]<min_distanza && visitati[i]==0){
                min_distanza=coda_distanze[i];
                next_node=i;
            }
        }
        visitati[next_node]=1;
        for(j=0;j<n;j++){
            if(matrice[next_node][j]!=0 && visitati[j]==0 && coda_distanze[next_node]!=INF && (coda_distanze[next_node] + matrice[next_node][j])<coda_distanze[j]){
                coda_distanze[j]=coda_distanze[next_node] + matrice[next_node][j];
            }
        }
        cnt++;
    }
    for(i=0;i<n;i++){
        //grazie a questo if se il grafo è fatto solo di nodi irraggiungibili la somma è 0!
        if(coda_distanze[i]!=INF){
            somma_cammini_minimi=somma_cammini_minimi+coda_distanze[i];
        }
    }
    return somma_cammini_minimi;
}
int atoi_custom_2(char stringa[]){ //per trattare il secondo numero della prima riga del file
    int lunghezza=strlen(stringa)-1;
    int array[lunghezza];
    int i;
    int numero_codificato=0;
    for(i=0;i<lunghezza;i++){
        array[i]=(int)stringa[i]-48; // mi metto nell'array tutte le cifre del numero
        numero_codificato=(numero_codificato*10) + array[i];
    }
    
    return numero_codificato;
}
int atoi_custom_1(char *stringa){
    int numero_codificato = 0;
    char *new;
    while(*stringa!='\0') {
        new=stringa++;
        numero_codificato = numero_codificato*10 + (*new-'0');
    }
    return numero_codificato;
}
void max_heapify(elemento array[],int dim,int indice){
    int left=(2*indice)+1;
    int right=(2*indice)+2;
    int max;
    int temp_value,temp_index;
    if(left<dim && right<dim && left>=0 && right>=0){
        if(array[left].dato==array[indice].dato && array[right].dato==array[indice].dato){
            if(left<dim && array[left].indice>array[indice].indice){
                max=left;
            }   
            else{
                max=indice;
            }
            if(right<dim && array[right].indice>array[max].indice){
                max=right;
            }
        }
        else if(array[left].dato==array[indice].dato && array[right].dato<array[indice].dato){
            if(array[left].indice>array[indice].indice){
                max=left;
            }
            else{
                max=indice;
            }
        }
        else if(array[right].dato==array[indice].dato && array[left].dato<array[indice].dato){
            if(array[right].indice>array[indice].indice){
                max=right;
            }
            else{
                max=indice;
            }
        }
        else if(array[left].dato==array[right].dato && array[indice].dato<array[left].dato && array[indice].dato<array[right].dato){
            if(array[right].indice>array[left].indice){
                max=right;
            }
            else{
                max=left;
            }
        }
        else{
            if(array[left].dato>array[indice].dato){
                max=left;
            }   
            else{
                max=indice;
            }
            if(array[right].dato>array[max].dato){
                max=right;
            }
        }
    }
    else if(left>0 && left<dim && !(right>=0 && right<dim)){
        if(array[left].dato==array[indice].dato){
            if(array[left].indice>array[indice].indice){
                max=left;
            }
            else{
                max=indice;
            }
        }
        else if(array[left].dato!=array[indice].dato){
            if(array[left].dato>array[indice].dato){
                max=left;
            }
            else{
                max=indice;
            }
        }
    }
    else if(right>=0 && right<dim && !(left>=0 && left<dim)){
        if(array[right].dato==array[indice].dato){
            if(array[right].indice>array[indice].indice){
                max=right;
            }
            else{
                max=indice;
            }
        }
        else if(array[right].dato!=array[indice].dato){
            if(array[right].dato>array[indice].dato){
                max=right;
            }
            else{
                max=indice;
            }
        }
    }
    else{
        max=indice;
    }
    if(max!=indice){
        temp_value=array[indice].dato;
        temp_index=array[indice].indice;
        array[indice].dato=array[max].dato;
        array[indice].indice=array[max].indice;
        array[max].dato=temp_value;
        array[max].indice=temp_index;
        max_heapify(array,dim,max);
    }
}
void build_max_heap(elemento array[],int dim){
    for(int i=dim/2-1;i>=0;i--){
        max_heapify(array,dim,i);
    }
}
void aggiungi_valore(elemento array[],int k,int valore_cammini,int *indice){
    if(*(indice)<k){
        array[*(indice)].dato=valore_cammini;
        array[*(indice)].indice=*(indice);
    }
    if(*(indice)==k){
        build_max_heap(array,k);
        if(valore_cammini<array[0].dato){
            array[0].dato=valore_cammini;
            array[0].indice=*(indice);
            max_heapify(array,k,0);
        }
    }
    if(*(indice)>k){
        if(valore_cammini<array[0].dato){
            array[0].dato=valore_cammini;
            array[0].indice=*(indice);
            max_heapify(array,k,0);
        }
    }
}