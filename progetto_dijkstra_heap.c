#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#define INF INT_MAX

typedef struct{
    int dato;
    int indice;
}elemento;

typedef struct{
    int nodo;
    int distanza;
}elemento_dijkstra;


int atoi_custom_1(char *stringa);
int atoi_custom_2(char stringa[]);
void min_heapify(elemento_dijkstra array[],int dim,int indice);//per dijkstra
int Dijkstra(int n,int matrice[][n],int sorgente);
void max_heapify(elemento array[],int dim,int indice);
void build_max_heap(elemento array[],int dim);
void build_min_heap(elemento_dijkstra array[],int dim);
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

//lettura di d e k
puntatore=fgets(comando,100,stdin);
if(puntatore!=NULL){
    token=strtok(comando,delimitatore);
    d=atoi_custom_1(token);
    token=strtok(NULL,delimitatore);
    k=atoi_custom_2(token);
}

elemento array[k];
char buffer[2*k];

while(flag==0){
    puntatore=fgets(comando,100,stdin);
    if(puntatore==NULL){
        flag=1;
        break;
    }
    else{
        if(comando[0]=='A'){ //lettura comando aggiungi_grafo
            aggiungi_grafo(array,d,k,temp);
            *(temp)=*(temp)+1;
        }
        if(comando[0]=='T'){ //lettura comando topk
            setvbuf(stdout,buffer,_IOFBF,k);
            if(*(temp)!=0){
                //stampa classifica
                if(*(temp)<k){ //caso in cui la classifica è piu grande degli effettivi elementi aggiunti
                    for(int i=0;i<*(temp);i++){
                        //printf("%d",array[i].indice);
                        fprintf(stdout,"%d",array[i].indice);
                        if(i<*(temp)-1){
                            //printf(" ");
                            fprintf(stdout," ");
                        }
                    }
                }
                else{
                    for(int i=0;i<k;i++){
                        // printf("%d",array[i].indice);
                        fprintf(stdout,"%d",array[i].indice);
                        if(i<k-1){
                            // printf(" ");
                            fprintf(stdout," ");
                        }
                    }
                }
            }
            // printf("\n");
            fprintf(stdout,"\n");
            fflush(stdout);
        }
    }
}

    return 0;
}

void aggiungi_grafo(elemento array[],int n,int k,int *indice){ // n è il numero di nodi del grafo
    int matrice_grafo[n][n];
    int i=0,t=0,z=0;
    char riga[1000000];
    char *puntatore;
    int valore_cammini_minimi=0;
    int cnt=0;
    int numero=0;
    //lettura della matrice
    while(cnt<n){ //appena leggo n righe esco
        puntatore=fgets(riga,1000000,stdin);
        if(puntatore==NULL){
            break;
        }
        z=0; //colonna della matrice
        i=0;
        while(riga[i]!='\n'){
            if(riga[i]==','){
                matrice_grafo[t][z]=numero;
                numero=0;
                z++;
            }
            else{
                numero=(10*numero) + ((int)riga[i] - 48);
            }
            i++;
        }
        matrice_grafo[t][z]=numero;
        numero=0;
        t++; //incremento la riga della matrice
        cnt++;
    }
    //stampa la matrici questo doppio for

    // for(int s=0;s<n;s++){
    //     for(int r=0;r<n;r++){
    //         printf("   %d   ",matrice_grafo[s][r]);
    //     }
    //     printf("\n");
    // }
    //a questo punto ho letto la matrice e sono pronto ad applicare dijkstra
    valore_cammini_minimi=Dijkstra(n,matrice_grafo,0); // mi salvo in una variabile il valore dei cammini minimi del grafo appena inserito
    //stampo le somme di ogni grafo
    //printf("Il grafo %d ha somme %d!\n",*(indice),valore_cammini_minimi);
    // //inserimento valore in classifica
    aggiungi_valore(array,k,valore_cammini_minimi,indice);
}
int Dijkstra(int n,int matrice[][n],int sorgente){
    int i,j,cnt=n;
    elemento_dijkstra coda[n];
    int distanze[n];
    int next_node=0;
    int somma_cammini_minimi=0;

    //inizializzazione coda e distanze
    for(i=0;i<n;i++){
        if(i!=sorgente){
            coda[i].distanza=INF;
            distanze[i]=INF;
        }
        coda[i].nodo=i;
    }
    coda[sorgente].distanza=0; // distanza sorgente-sorgente=0
    distanze[sorgente]=0;
    //build_min_heap(coda,n); // a questo punto ho il min-heap pronto
    //cnt inizializzato a n,mi conta i nodi che devo analizzare 
    while(cnt>0){
        next_node=coda[0].nodo;
        for(j=0;j<n;j++){
            if(matrice[next_node][j]!=0 && distanze[next_node]!=INF && (distanze[next_node] + matrice[next_node][j])<distanze[j]){
                distanze[j]=distanze[next_node] + matrice[next_node][j];
                for(int z=0;z<cnt;z++){
                    if((coda[z].nodo)==j){
                        coda[z].distanza=distanze[j];
                        break;
                    }
                }
            }
        }
        build_min_heap(coda,cnt);
        coda[0].distanza=coda[cnt-1].distanza;
        coda[0].nodo=coda[cnt-1].nodo;
        coda[cnt-1].distanza=0;
        coda[cnt-1].nodo=0;
        if((cnt-1)>1){
            min_heapify(coda,cnt-1,0);
        }
        cnt--;
    }
    for(i=0;i<n;i++){
        //grazie a questo if se il grafo è fatto solo di nodi irraggiungibili la somma è 0!
        if(distanze[i]!=INF){
            somma_cammini_minimi=somma_cammini_minimi+distanze[i];
        }
    }
    return somma_cammini_minimi;

}
void min_heapify(elemento_dijkstra array[],int dim,int indice){
    int left=(2*indice)+1;
    int right=(2*indice)+2;
    int min;
    int temp_value,temp_index;

    if(left<dim && right<dim && left>=0 && right>=0){
        if(left<dim && array[left].distanza<array[indice].distanza){
            min=left;
        }
        else{
            min=indice;
        }
        if(right<dim && array[right].distanza<array[min].distanza){
            min=right;
        }
    }
    else if(left>=0 && left<dim && !(right>=0 && right<dim)){
        if(array[left].distanza<array[indice].distanza){
            min=left;
        }
        else{
            min=indice;
        }
    }
    else if(right>=0 && right<dim && !(left>=0 && left<dim)){
        if(array[right].distanza<array[indice].distanza){
            min=right;
        }
        else{
            min=indice;
        }
    }
    else{
        min=indice;
    }
    if(min!=indice){
        temp_value=array[indice].distanza;
        temp_index=array[indice].nodo;
        array[indice].distanza=array[min].distanza;
        array[indice].nodo=array[min].nodo;
        array[min].distanza=temp_value;
        array[min].nodo=temp_index;
        min_heapify(array,dim,min);
    }
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
    else if(left>=0 && left<dim && !(right>=0 && right<dim)){
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
void aggiungi_valore(elemento array[],int k,int valore_cammini,int *indice){
    if(*(indice)<k){ // finche non riempio la classifica butto dentro a caso
        array[*(indice)].dato=valore_cammini;
        array[*(indice)].indice=*(indice);
    }
    if(*(indice)==k){ //quando ho i k elementi costruisco il max_heap e inizio a lavorarci
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
void build_min_heap(elemento_dijkstra array[],int dim){
    for(int i=dim/2-1;i>=0;i--){
        min_heapify(array,dim,i);
    }
}
void build_max_heap(elemento array[],int dim){
    for(int i=dim/2-1;i>=0;i--){
        max_heapify(array,dim,i);
    }
}