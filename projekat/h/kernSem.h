#ifndef _kernSem_h_
#define _kernSem_h_

#include "common.h"
#include "list.h"
#include "pcb.h"

class KernelSem;

extern List<KernelSem*> semList;


class KernelSem {
public:
	KernelSem(int init = 1);
	int wait(Time maxTimeToWait);
	void signal();
	int getVal() const { return val; }
	void reduceWaitTime();
private:
	int val;
	void block();
	void deblock();
	List<PCB*> blocked;
	List<int> waitTime;
};


#endif
