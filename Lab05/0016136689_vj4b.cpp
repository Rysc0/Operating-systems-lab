#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

struct mem{
    pthread_mutex_t l;
    pthread_cond_t uv[2];
    int br[2] = { 0,0 };
    int type;
    int red[2] = { 0,0 };
    int SITI = 0;
} *pom;

pthread_mutexattr_t l_atr;
pthread_condattr_t uv_atr;

int ID;
unsigned int N,M;

void ispis(int type) {
	
    cout<<"Red Linux: ";
    for(int i=0;i<M;i++) 
		if (i<pom->red[0])
			cout<<"L";
			
		else
			cout<<"-";
    
    cout<<" Red Microsoft: ";
    for(int i=0;i<M;i++)
        if (i<pom->red[1])
           	cout<<"M";
           	
        else
           	cout<<"-";
		
    cout<<" Restoran: ";
    if(type==0 || pom->br[0]>0) 
        for(int i=0;i<pom->br[0];i++) 
            cout<<"L";
            
    if(type==1 || pom->br[1]>0) 
		for(int i=0;i<pom->br[1];i++) 
		cout<<"M";
 
    cout<<" --> ";
    if(type==0)
        cout<<"L";
        
    else 
         cout<<"M";
}

void ulazak(int type) {
	
        pthread_mutex_lock(&pom->l);
        pom->red[type]++;
        
        while(pom->br[1-type]>0 || (pom->SITI>=N && pom->red[1-type]>0)) 
		{
                ispis(type);
				cout<<"Ulazak u red cekanja!"<<endl;
                pthread_cond_wait(&pom->uv[type], &pom->l);
        }
        pom->br[type]++;
        pom->red[type]--;

        if(pom->red[1-type]>0) 
			pom->SITI++;
		
        ispis(type);
		cout<<"Ulazak u restoran!"<<endl;
        pthread_mutex_unlock(&pom->l);
}

void izlazak(int type) {
	
        pthread_mutex_lock(&pom->l);
        pom->br[type]--;
        
        if(pom->br[type]==0) 
		{
                pom->SITI=0;
                pthread_cond_broadcast(&pom->uv[1-type]);
        }
        
        ispis(type);
		cout<<"Izlazak iz restorana!"<<endl;
        pthread_mutex_unlock(&pom->l);
}

void func(int type) {
	
        usleep(rand()%100);
        ulazak(type);
        usleep(rand()%100);
        izlazak(type);
}

void del(int sig) {
	
        pthread_mutexattr_destroy(&l_atr);
        pthread_mutex_destroy(&pom->l);
        pthread_condattr_destroy(&uv_atr);
        
        for(int i=0;i<2;i++) 
                pthread_cond_destroy(&pom->uv[i]);
        
        shmdt(pom);
        shmctl(ID, IPC_RMID, 0);
        cout<<endl<<"Brisanje memorije i monitora"<<endl;
        exit(0);
}

int main(int argc, char *argv[]) {
	
        if(argc!=3) {
        	
                cout<<endl<<"Krivi unos!"<<endl;
                return 0;
        }
        
        N=atoi(argv[1]);
        M=atoi(argv[2]);
        srand(time(0));
        
        ID=shmget(IPC_PRIVATE, sizeof(mem), 0600);
        
        if(ID<0) {
        	
                cout<<endl<<"Ne postoji shared memorija!"<<endl;
                exit(0);
        }
        
        pom=(mem*)shmat(ID, NULL, 0);
		
        pthread_mutexattr_init(&l_atr);
        pthread_mutexattr_setpshared(&l_atr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&pom->l, &l_atr);
        pthread_condattr_init(&uv_atr);
        pthread_condattr_setpshared(&uv_atr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&pom->uv[0], &uv_atr);
        pthread_cond_init(&pom->uv[1], &uv_atr);
        
     
        bool type = true;
        int i;
        
        for(i=0;i<2*M;i++) {
        	
                switch(fork()) {
                	
                case 0: pom->type=1-pom->type;
                        func(pom->type);
                        exit(0);
                        
                case -1: cout<<endl<<"Pogreska!"<<endl;
                         exit(0);
                }
        }
		
        sigset(SIGINT, del);
        
        while(i!=0) 
		{
			
            wait(NULL);
            i--;
        }
        cout<<"Restoran i red cekanja su prazni!"<<endl;
        del(0);
        return 0;
}
