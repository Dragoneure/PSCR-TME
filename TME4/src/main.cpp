#include "Banque.h"
#include <random>
#include<unistd.h>
#include <iostream>


using namespace std;
using namespace pr;


const int NB_THREAD = 10;

void boucle_thread(Banque &b) {
	random_device r;
    default_random_engine e1(r());
    uniform_int_distribution<int> compte(0, b.size()-1);
    uniform_int_distribution<int> montant(1, 100);
    uniform_int_distribution<unsigned int> temps(1, 20);


	for (int k = 0; k < 1000; ++k) {
		    int i;
		    int j;
		    int m;

		    m = montant(e1);
		    i = compte(e1);
		    do {
		    	j = compte(e1);
		    } while (j == i);
		    b.transfert(i, j, m);
		    usleep(temps(e1));
			b.comptabiliser(20*10);

	}
}

int main () {
	vector<thread> threads;
	Banque b(20,10);
	for (int i = 0; i < NB_THREAD; ++i) {
		threads.emplace_back(boucle_thread,ref(b));
	}


	for (auto & t : threads) {
		t.join();
	}

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
