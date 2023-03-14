#include "kernSem.h"


KernelSem::KernelSem(int init)
{
	LOCK;

		val = init;
		semList.addToTail(this);

	UNLOCK;
}

int KernelSem::wait(Time maxTimeToWait)
{
	int ret = 0;

	LOCK;

		if (--val < 0) {
			waitTime.addToTail(maxTimeToWait);
			block();

			UNLOCK;
			dispatch();

			ret = runningPCB->getWaitResult();
		}else{
			UNLOCK;
		}


	return ret;
}

void KernelSem::signal()
{
	LOCK;

	if (++val <= 0) {
		deblock();
	}

	UNLOCK;
}

void KernelSem::reduceWaitTime()
{
	LOCK;

	List<int>::Iterator waitIt = waitTime.begin();
	int k = 0;

	for (List<PCB*>::Iterator it = blocked.begin();
		it != blocked.end();
		) {
		PCB* pcb = (*it);
		++it;
		int& timeLeftToWait = (*waitIt);
		++waitIt;
		k++;

		if (timeLeftToWait > 0) {
			if (--timeLeftToWait == 0) {

				++val;
				pcb->setReady();
				pcb->setWaitResult(0);
				blocked.erase(pcb);
				waitTime.eraseById(k);
				k--;
				Scheduler::put(pcb);

			}
		}
	}

	UNLOCK;
}

void KernelSem::block()
{
	LOCK;

		runningPCB->setBlocked();
		blocked.addToTail(runningPCB);

	UNLOCK;
}

void KernelSem::deblock()
{

	LOCK;

	PCB* pcb = blocked.getHead();



	if (pcb) {

		waitTime.deleteHead();
		blocked.deleteHead();
		pcb->setReady();
		pcb->setWaitResult(1);
		Scheduler::put(pcb);

	}

	UNLOCK;
}
