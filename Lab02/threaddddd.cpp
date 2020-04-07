#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#define DRED 1000

int zadnji=0;
int ulaz[DRED]={0};
int broj[DRED]={0};
int br=0;

void pocetak_kritickog(int i){
    int j;
    ulaz[i] = 1;
    broj[i] = zadnji + 1;
    zadnji = broj[i];
    ulaz[i] = 0;
    for(j = 0; j < br; j++){
    while(ulaz[j] == 1){
     }
    while((broj[j] != 0)&&(broj[j] < broj[i] || (broj[j] == broj[i] && j < i))){
     }
    }
}

void kraj_kritickog(int i){
     broj[i] = 0;
}

void *dretvica(void *i){
     int j, k;
     int pretvorba=*(int *)i;
     for(j = 1; j <= 5; j++){
        pocetak_kritickog(pretvorba);
        for(k = 1; k <= 5; k++){
             printf("Dretva: %d, K.O. br: %d (%d/5)\n",pretvorba+1, j, k);
             usleep(1000000);
         }
             kraj_kritickog(pretvorba);           }
}

int main(int argc, char *argv[]){

    sigset(SIGINT, SIG_DFL);
    int i;

    br = atoi(argv[1]);
    pthread_t th[br];
    int brojac12[br];
    for(i = 0; i < br; i++) {
	brojac12[i]=i;
        if(pthread_create(&th[i], NULL, &dretvica, (void*)&brojac12[i])){
            printf("Greska!!!\n");
            exit(1);   }
        }
    int k;
    for(k = 0; k < br; k++){
        pthread_join(th[k], NULL);
    }
    return 0;
}
