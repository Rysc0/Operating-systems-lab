/*Ostvariti sustav paralelnih procesa/dretava. Struktura procesa/dretava dana je
sljedećim pseudokodom:*/
#include<iostream>
#include<cstdlib>        /*exit() */
#include<unistd.h>      /*fork()*/
#include<sys/wait.h>  /* wait()*/
using namespace std;

zajedničke varijable: TRAŽIM[0..n-1], BROJ[0..n-1]

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

  proces proc(i)          /* i  [0..n-1] */
  {
    za k = 1 do 5 čini {
      uđi u kritični odsječak
      za m = 1 do 5 čini {
        ispiši (i, k, m)
      }
      izađi iz kritičnog odsječka
    }
  }

  /* Ako se program rješava s procesima tada treba zajedničke varijable tako organizirati
  da se prostor za njih zauzme odjednom i podijeli među njima. Ovo je nužno zbog ograničenog
  broja segmenata i velikog broja korisnika.*/

  /* Ovisno o opterećenju računala i broju procesa koji se pokreću, a da bi se vidjele razlike
  prilikom izvođenja programa može biti potrebno usporiti izvršavanje sa:*/
  sleep(1);

  nakon: ispisi (i, k, m).

  return 0;
}
