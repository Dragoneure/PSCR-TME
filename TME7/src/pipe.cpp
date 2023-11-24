#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXPROC 10


int initproc(int argc, char **argv, int *stop) {
	int compteur = 1;
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i],"|") == 0) {
			if (i == argc) {
				perror("il faut une commande après le pipe");
				exit(1);
			}
			stop[compteur] = i+1;
			compteur++;
			argv[i] = 0x0;
		}
	}
	return compteur;
}


int main(int argc, char ** argv) {
	if (argc == 0) {
		perror("manque argument");
		return 1;
	}
		pid_t pids[MAXPROC];
		int stop[MAXPROC];
		stop[0] = 1;
		int nbproc = initproc(argc,argv,stop);
		if (nbproc > 10) {
			perror("max 10 pipes");
		}
		int base[2] = {dup(0),dup(1)};
		int fdentr = dup(0);
		for (int i = 0; i < nbproc; i++) {
			if (i != (nbproc-1)) {
				int pfd[2];
				if (pipe(pfd)) {
					perror("pipe");
					exit(1);
				}
				dup2(pfd[1],1);
				close(pfd[1]);
				dup2(fdentr,0);
				close(fdentr);
				fdentr = pfd[0];

			} else {
				dup2(fdentr,0);
				close(fdentr);
			}
			if (! (pids[i] =fork())) {
				execvp(argv[stop[i]],&argv[stop[i]]);
				return 0;
			} else {
				dup2(base[0],0);
				dup2(base[1],1);
			}
		}
		for (int i = 0; i < nbproc; i++) {
			wait(NULL);
		}

}

