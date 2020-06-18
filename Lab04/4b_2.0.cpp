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

struct memorija {
    pthread_mutex_t mon;
    pthread_cond_t uv[2];
    int br[2] = { 0,0 };
    int vrsta;
    int ceka[2] = { 0,0 };
    int SITI = 0;
} *podaci;

pthread_mutexattr_t mon_atr;
pthread_condattr_t uv_atr;

int shmID;
unsigned int N,M;

void ispis(int vrsta) 
{
    cout<<"Red Linux: ";
    for(int i=0;i<M;i++) 
		if (i<podaci->ceka[0])
			cout<<"L";
		else
			cout<<"-";
    
    cout<<" Red Microsoft: ";
    for(int i=0;i<M;i++)
        if (i<podaci->ceka[1])
           	cout<<"M";
        else
           	cout<<"-";
		
    cout<<" Restoran: ";
    if(vrsta==0 || podaci->br[0]>0) 
        for(int i=0;i<podaci->br[0];i++) 
            cout<<"L";
    if(vrsta==1 || podaci->br[1]>0) 
		for(int i=0;i<podaci->br[1];i++) 
		cout<<"M";
 
    cout<<" --> ";
    if(vrsta==0)
        cout<<"L";
    else 
         cout<<"M";
}

void udji(int vrsta) {
        pthread_mutex_lock(&podaci->mon);
        podaci->ceka[vrsta]++;
        while(podaci->br[1-vrsta]>0 || (podaci->SITI>=N && podaci->ceka[1-vrsta]>0)) 
		{
                ispis(vrsta);
				cout<<" u red cekanja"<<endl;
                pthread_cond_wait(&podaci->uv[vrsta], &podaci->mon);
        }
        podaci->br[vrsta]++;
        podaci->ceka[vrsta]--;

        if(podaci->ceka[1-vrsta]>0) 
			podaci->SITI++;
		
        ispis(vrsta);
		cout<<" u restoran"<<endl;
        pthread_mutex_unlock(&podaci->mon);
}

void izadji(int vrsta) {
        pthread_mutex_lock(&podaci->mon);
        podaci->br[vrsta]--;
        if(podaci->br[vrsta]==0) 
		{
                podaci->SITI=0;
                pthread_cond_broadcast(&podaci->uv[1-vrsta]);
        }
        
        ispis(vrsta);
		cout<<" iz restorana"<<endl;
        pthread_mutex_unlock(&podaci->mon);
}

void funkcija(int vrsta) {
        usleep(rand()%100);
        udji(vrsta);
        usleep(rand()%100);
        izadji(vrsta);
}

void brisi(int sig) {
        pthread_mutexattr_destroy(&mon_atr);
        pthread_mutex_destroy(&podaci->mon);
        pthread_condattr_destroy(&uv_atr);
        for(int i=0;i<2;i++) 
                pthread_cond_destroy(&podaci->uv[i]);
        
        shmdt(podaci);
        shmctl(shmID, IPC_RMID, 0);
        cout<<endl<<"Brisem dijeljenu memoriju i monitor"<<endl;
        exit(0);
}

int main(int argc, char *argv[]) {
        if(argc!=3) {
                cout<<endl<<"Krivi parametri!"<<endl;
                return 0;
        }
        
        N=atoi(argv[1]);
        M=atoi(argv[2]);
        srand(time(0));
        
        shmID=shmget(IPC_PRIVATE, sizeof(memorija), 0600);
        if(shmID<0) {
                cout<<endl<<"Nema djeljene memorije!"<<endl;
                exit(0);
        }
        podaci=(memorija*)shmat(shmID, NULL, 0);
		
        pthread_mutexattr_init(&mon_atr);
        pthread_mutexattr_setpshared(&mon_atr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&podaci->mon, &mon_atr);
        pthread_condattr_init(&uv_atr);
        pthread_condattr_setpshared(&uv_atr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&podaci->uv[0], &uv_atr);
        pthread_cond_init(&podaci->uv[1], &uv_atr);
        
        int i;
        bool vrsta = true;
        for(i=0;i<2*M;i++) 
        	if (fork()==0)
	        	if (vrsta)
	        	{
	        		funkcija(vrsta);
	        		vrsta = false;
				}
				else
				{
					funkcija(vrsta);
	        		vrsta = true;
				}
		
        sigset(SIGINT, brisi);
        while(i!=0) 
		{
                wait(NULL);
                i--;
        }
        cout<<"U restoranu vise nema nikoga i nitko ne ceka na ulazak!"<<endl;
        brisi(0);
        return 0;
}

