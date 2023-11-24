#include <iostream>
#include <unistd.h>
#include <sys/wait.h>


int main () {
	const int N = 3;
	int nbfils = 0;
	int j = N;
	int i = 1;
	std::cout << "main pid=" << getpid() << std::endl;

	for (; i<=N && j==N && fork()==0 ; i++ ) {
		nbfils = 0;
		std::cout << "je suis " << getpid() << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				nbfils = 0;
				j=0;
				std::cout << "je suis " << getpid() << " k:j " << k << ":" << j << std::endl;
			} else {
				++nbfils;
			}
		}
	}
	if (j != 0 && i < N) {
		++nbfils;
	}
	for (int l = 0; l < nbfils; l++) {
		if (wait(NULL) == -1) {
			std::cerr << "trop de wait de " << getpid() << std::endl;;
		}
	}
	//while (wait(NULL) != -1 ) {}
	return 0;
}
