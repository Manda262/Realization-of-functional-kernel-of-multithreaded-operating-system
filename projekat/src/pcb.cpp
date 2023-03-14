#include "pcb.h"
#include <dos.h>

int PCB::pos_id = 0;
StackSize const PCB::MAX_STACK_SIZE = 0x1000;
Word const PCB::PSW_INIT = 0x0200;

PCB::PCB(StackSize stackSize, Time timeSlice, bool isIdl, Thread* thr)
{
	if (stackSize > MAX_STACK_SIZE) pcbStackSize = MAX_STACK_SIZE;
	else pcbStackSize = stackSize;
	pcbStackLenght = pcbStackSize / sizeof(Word);
	pcbTimeSlice = timeSlice;
	pcbTime = timeSlice;
	id = ++pos_id;
	isIdleFlag = isIdl;
	myThread = thr;
	waitResult = 0;

	if (!isIdleFlag) setNotStarted();
	else setIdle();

	LOCK;

		pcbList.addToTail(this);

	UNLOCK;



	if (isIdleFlag) initializeStack(idleWrapper);
	else if (myThread) initializeStack(normalWrapper);
}

PCB::~PCB()
{
	LOCK;

		pcbList.erase(this);

	UNLOCK;


	INT_DISABLE;

		if (stack) delete[] stack;

	INT_ENABLE;
}

void PCB::startPCB()
{
	LOCK;

	if (isNotStarted()) { // ne sme 2 put da se pozove start
		setReady();
		Scheduler::put(this);
	}


	UNLOCK;
}

void PCB::waitToComplete()
{
	LOCK;

	if (this != runningPCB && !this->isFinished() && !this->isIdle()) {
		runningPCB->setBlocked();
		blockedList.addToTail(runningPCB);
		UNLOCK;
		dispatch();
	}
	else {
		UNLOCK;
	}



}

PCB* PCB::getById(int id)
{
	LOCK;

	for (List<PCB*>::Iterator it = pcbList.begin(); it != pcbList.end(); ++it) {
		if ((*it)->getId() == id) {
			UNLOCK;
			return *it;
		}
	}

	UNLOCK;

	return NULL;

}

void PCB::initializeStack(WrapperFunc wrapper)
{
	stack = new Word[pcbStackLenght];

	stack[pcbStackLenght - 1] = PSW_INIT;
	stack[pcbStackLenght - 2] = FP_SEG(wrapper);
	stack[pcbStackLenght - 3] = FP_OFF(wrapper);

	ss = FP_SEG(stack + pcbStackLenght - 12);
	sp = bp = FP_OFF(stack + pcbStackLenght - 12);
}



void PCB::setNotStarted()
{
	LOCK;

		clearFlags();
		isNotStartedFlag = true;

	UNLOCK;
}

void PCB::setBlocked()
{
	LOCK;

		clearFlags();
		isBlockedFlag = true;

	UNLOCK;
}

void PCB::setFinished()
{
	LOCK;

		clearFlags();
		isFinishedFlag = true;

	UNLOCK;
}

void PCB::setRunning()
{
	LOCK;

		clearFlags();
		isRunningFlag = true;

	UNLOCK;
}

void PCB::setReady()
{
	LOCK;

		clearFlags();
		isReadyFlag = true;

	UNLOCK;
}

void PCB::setIdle()
{
	LOCK;

		clearFlags();
		isIdleFlag = true;

	UNLOCK;
}


void PCB::normalWrapper()
{

	runningPCB->myThread->run();

	LOCK;

	runningPCB->setFinished();
	int sz = runningPCB->blockedList.getSize();
	for (int i = 0; i < sz; i++) {
		PCB* pcb = runningPCB->blockedList.getHead();
		pcb->setReady();
		Scheduler::put(pcb);
		runningPCB->blockedList.deleteHead();
	}

	UNLOCK;

	dispatch();
}

void PCB::idleWrapper()
{
	while (true) {
		dispatch();
	}
}

void PCB::clearFlags()
{
	isNotStartedFlag = false;
	isBlockedFlag = false;
	isReadyFlag = false;
	isRunningFlag = false;
	isFinishedFlag = false;
	isIdleFlag = false;
}
