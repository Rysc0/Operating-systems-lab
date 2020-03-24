/*Ostvariti sustav paralelnih procesa/dretava. Struktura procesa/dretava dana je
sljedećim pseudokodom:*/
#include<iostream>
#include<cstdlib>        /*exit() */
#include<unistd.h>      /*fork()*/
#include<sys/wait.h>  /* wait()*/
using namespace std;

// zajedničke varijable: TRAŽIM[0..n-1], BROJ[0..n-1]
int proces[2],prioritet[2];

funkcija uđi_u_kritični_odsječak(i)
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

funkcija izađi_iz_kritičnog_odsječka(i)
{
  BROJ[i] = 0
}

int main(int argc, char **argv) {
  // extracting second argument - number of processes to be spawned
  int procesa = atoi(argv[1]);


  // TAKING SHARED MEMORY
  /* Ako se program rješava s procesima tada treba zajedničke varijable tako organizirati
  da se prostor za njih zauzme odjednom i podijeli među njima. Ovo je nužno zbog ograničenog
  broja segmenata i velikog broja korisnika.*/

  // STARTING PARALEL PROCCESES

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

  // FREE SHARED MEMORY AND CLEANUP

  return 0;
}
