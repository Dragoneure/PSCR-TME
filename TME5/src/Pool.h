#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>


namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize) ;
	void start (int nbthread);
	void submit (Job * job) ;
	void stop() ;
	void poolWorker(Queue<Job>queue);
	~Pool() ;


	Pool(int qsize) {
		this->queue = new Queue<Job>(qsize);
		this->threads = new std::vector<std::thread>;
	}

	void poolWorker(Queue<Job>queue) {
		while(true) {}
	}


	void start(int nbthread) {
		threads.reserve(nbthread);
		for(int i =0;i<nbthread;i++) {
			threads.emplace_back(poolWorker,std::ref(queue));
		}
	}
	void stop() {
		queue.setBlocking(false);
		for(auto &t_threads : threads) {
			t_threads.join();
		}
		threads.clear();
	}

	void submit(Job * job) {
		queue.push(job);
	}

	~Pool() {
		stop();
	}
};
}
