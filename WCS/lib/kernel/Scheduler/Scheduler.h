#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Task.h"
#include <stdint.h>

#define MAX_TASKS 50

class Scheduler {
	public:
	  	void init(uint8_t basePeriod);
	  	bool addTask(Task* task);
	  	void schedule();

	private:
		uint8_t basePeriod;
  		uint8_t nTasks;
  		Task* taskList[MAX_TASKS];
};

#endif
