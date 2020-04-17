#include<iostream>
#include<semaphore.h>
#include<ctime>
#include<sys/shm.h>
#include<sys/ipc.h>
using namespace std;

int *broj;

void generator(int prvi_arg){
    for(int i = 0; i < prvi_arg; i++){
        broj[0] = rand()%10;
        cout << broj[0] << endl;
    }
}

void sum(int drugi_arg){
    int zbroj = 0;
    for(int i = 0; i < broj[0]; i++){
        zbroj += i;
    }
    cout << "Zbroj je: " << zbroj << endl;
}

int main(int argc, char *argv[]){
    
    if(argc != 3){
        cout << "ERROR, must have 3 arguments!" << endl;
        exit(-1);
    }    

    srand(time(NULL));

    int prvi_argument = atoi(argv[1]);
    int drugi_argument = atoi(argv[2]);

    int id = shmget(IPC_PRIVATE,sizeof(int),0600);
    if(id == -1){
        cout << "ERROR; NO SHARED MEMORY!" << endl;
        exit(-1);
    }

    broj = (int*) shmat(id,NULL,0);


    
    generator(prvi_argument);
    sum(drugi_argument);

    
    return 0;
}