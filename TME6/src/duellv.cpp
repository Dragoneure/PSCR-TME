#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "rsleep.h"

	int pv = 3;
	std::string nom = "luke";

static void degats(int) {
	--pv;
	std::cout << nom << " : point de vie restant = " << pv << std::endl;
	if (!pv) {
		std::cout << nom << " :NOOOOOON !" << std::endl;
		exit(1);
	}
}

void attaque (pid_t adversaire){
	struct sigaction sa;
	sa.sa_handler = &degats;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction (SIGINT, &sa, NULL);
	if (kill(adversaire,SIGINT) == -1) {
		std::cout << nom << " : C'est fini !" << std::endl;
		exit(0);
	}
	randsleep();
}

void defense() {
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction (SIGINT, &sa, NULL);
	randsleep();
}

void combat(pid_t adversaire) {
	while(1) {
		attaque(adversaire);
		defense();
	}
}

int main() {

	pid_t luke;
	pid_t vador;
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction (SIGINT, &sa, NULL);
	switch (luke = fork()) {
		case -1 :
			perror("fork");
			break;
		case 0 :
			vador = getppid();
			combat(vador);
			break;
		default :
			nom = "vador";
			combat(luke);
			break;
	}
	return 0;
}
