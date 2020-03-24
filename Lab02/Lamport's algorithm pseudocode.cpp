/*Ostvariti sustav paralelnih procesa/dretava. Struktura procesa/dretava dana je
sljedećim pseudokodom:*/
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>        /*exit() */
#include <unistd.h>      /*fork()*/
#include <sys/wait.h>  /* wait()*/
using namespace std;

int *proces_id_ptr;
int *prioritet_ptr;
// FREE SHARED MEMORY AND CLEANUP
void brisi(int sig)
{
  /* oslobađanje zajedničke memorije */
  shmdt(prioritet_ptr);
  shmdt(proces_id_ptr);
  shmctl(prioritet_ptr, IPC_RMID, NULL);
  shmctl(proces_id_ptr, IPC_RMID, NULL);
  exit(0);
}
/*
void funkcija uđi_u_kritični_odsječak(i)
{
  TRAŽIM[i] = 1
  BROJ[i] = max(BROJ[0],...,BROJ[n-1]) + 1
  TRAŽIM[i] = 0

  za j = 0 do n-1 čini
  dok je TRAŽIM[j] <> 0 čini
  ništa
  dok je BROJ[j] <> 0 && (BROJ[j] < BROJ[i] || (BROJ[j] == BROJ[i] && j < i)) čini
  ništa
}

void funkcija izađi_iz_kritičnog_odsječka(i)
{
  BROJ[i] = 0
}
*/



int main(int argc, char **argv) {
  // extracting second argument - number of processes to be spawned
  int procesa = atoi(argv[1]);

  // zajedničke varijable: TRAŽIM[0..n-1], BROJ[0..n-1]
  int proces_id[argc];
  int prioritet[argc];
  proces_id_ptr = proces_id;
  prioritet_ptr = prioritet;

  // TAKING SHARED MEMORY
  /* zauzimanje zajedničke memorije */
  for(int i = 0; i < procesa; i++){
    proces_id[i] = shmget(IPC_PRIVATE, sizeof(int), 0600);

    if (proces_id[i] == -1){
      cout << "SHARED MEMORY ERROR!" << endl;
      exit(1);
    }  /* greška - nema zajedničke memorije */

    prioritet[i] = (int *) shmat(proces_id, NULL, 0);
    *prioritet[i] = 0;
    sigset(SIGINT, brisi);//u slučaju prekida briši memoriju

    /* Ako se program rješava s procesima tada treba zajedničke varijable tako organizirati
    da se prostor za njih zauzme odjednom i podijeli među njima. Ovo je nužno zbog ograničenog
    broja segmenata i velikog broja korisnika.*/
  }

  // STARTING PARALEL PROCCESES
  for(int i = 0; i < procesa; i++){
    if (fork() == 0) {
      // child code
       cout << "I'm child proccess with pid: " << getpid() << endl;
       exit(0);
    }
  }
  // WAITING FOR CHILD PROCCESES
  for(int i = 0; i < procesa; i++){
    wait(NULL);
  }
  brisi(0);
/*
  // LAMPORT'S ALGORITHM IMPLEMENTATION
  for(int i = 0; i < procesa; i++){
    for(int k = 1; k < 6; k++){
      uđi u kritični odsječak
      for(int m = 1; m < 6; i++){
        ispiši (i, k, m)
      }
      izađi iz kritičnog odsječka
    }
  }

  nakon: ispisi (i, k, m).

*/
  return 0;
}
