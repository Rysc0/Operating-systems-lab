#include <iostream>

using namespace std;


int main(){
	
	programer(vrsta){	- svaki programer je predstavljen jednim procesom
		programiraj;
		uđi(vrsta);
		jedi;
		izađi(vrsta);
	}
	
	a) sinkronizacija programera monitorom
	b) najviše N programera ulazi u restoran i jede (ako je guzva idu jedni pa drugi)
		- ako programeri jedne vrste ne čekaju, tad svi programeri druge mogu unutra
	
	- dva ulazna argumenta
		- prvi: N programera koji mogu ući u restoran
		- drugi: broj programera svake vrste
		
	- procesi/programeri se generiraju naizmjenice
		- u fazi programiranja i fazi jela svaki proces spava do 100ms (usleep(rand()%100)) 
		
	Trenutno stanje u restoranu i redovima čekanja je potrebno ispisivati neposredno prije 
	stavljanja procesa u red čekanja i neposredno prije otključavanja monitorskog semafora. 
	


	PSEUDOKOD
	
	M -varijabla zaključavanja (monitorski semafor)
	2 reda uvjeta uv[2]
	br[2]={0,0}// dva brojača koliko programera je u restoranu
	vrsta = 0(MS)ili 1(Linux)
	čeka[2] = {0,0}// dvabrojača koliko programera čeka
	SITI = 0; // brojač koliko se programera jedne vrste najelo
	
	m-funkcija udji(vrsta) {
		mutex_lock(&M);
		čeka[vrsta]++
		dok je (br[1-vrsta]>0 || ((SITI>=N)&& (ceka[1-vrsta]>0)))
			cond_wait(&uv[vrsta], &M);
		br[vrsta]++;
		čeka[vrsta]--;
		ako je (čeka[1-vrsta]>0) SITI++;
		mutex_unlock(&M);
	}
	
	m-funkcija izadji(vrsta) {
		mutex_lock(&M);
		br[vrsta]--;
		ako je (br[vrsta] == 0){
			SITI = 0;
			cond_broadcast(&uv[1-vrsta]);
		}
		mutex_unlock(&m);
	}


	return 0;
}
