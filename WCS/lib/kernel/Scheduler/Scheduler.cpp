#include "Scheduler.h"

void Scheduler::init(uint8_t basePeriod) {
  	this->basePeriod = basePeriod;
	this->nTasks = 0;
}

bool Scheduler::addTask(Task* task) {
  	if (this->nTasks < MAX_TASKS-1) {
		this->taskList[this->nTasks] = task;
		this->nTasks++;
		return true;
  	} else {
		return false;
  	}
}

void Scheduler::schedule() {
  	for(uint8_t i = 0; i < this->nTasks; i++) {
		if (this->taskList[i]->updateAndCheckTime(this->basePeriod)) {
		  	this->taskList[i]->tick();
		}
  	}
}
