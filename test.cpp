#include <stdio.h>
#include <signal.h>
#include <time.h>

#define N 4    /* broj razina prekida */

int PRIORITET[N];
int TEKUCI_PRIORITET;

void obrada_prekida(int j)
{
   printf ("Poceo obradu prekida %d\n", j);
   /* obrada se simulira trošenjem vremena - 10 s */
   printf ("Zavrsio obradu prekida %d\n", j);
}

void prekidna_rutina (int sig)
{
   int i;
   time_t t;

   time(&t);
   printf("Prekidna rutina pozvana u: %s", ctime(&t));

   switch (sig) {
   case SIGINT:
      i = 1;
      break;
   case SIGQUIT:
      i = 2;
      break;
   case SIGTSTP:
      i = 3;
      break;
   default:
      return;
   }
   printf ("Razina prekida: %d\n", i);

   if(i > TEKUĆI_PRIORITET) {
     PRIORITET[i] = TEKUĆI_PRIORITET;
     TEKUĆI_PRIORITET = i;
     sigrelse(i);
     obrada_prekida(i);
     sighold(i);
     TEKUĆI_PRIORITET = PRIORITET[i];
  }
}

int main (void)
{
   sigset (SIGINT, prekidna_rutina);
   sigset (SIGQUIT, prekidna_rutina);
   sigset (SIGTSTP, prekidna_rutina);

   printf ("Poceo osnovni program\n");
   /* troši vrijeme da se ima šta prekinuti - 10 s */
   for(int i=1;i<=20;i++){
     cout << i << "/20" << endl;
     sleep(1);
   }
   printf ("Zavrsio osnovni program\n");

   return 0;
}
