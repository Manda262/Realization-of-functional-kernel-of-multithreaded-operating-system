#include "thread.h"
#include "pcb.h"


void Thread::start()
{
    myPCB->startPCB();
}

void Thread::waitToComplete()
{
    myPCB->waitToComplete();
}

Thread::~Thread()
{

    waitToComplete();

    INT_DISABLE;

    	delete myPCB;

    INT_ENABLE;
}

ID Thread::getId()
{
    return myPCB->getId();
}

ID Thread::getRunningId()
{
    return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id)
{
    LOCK;

    PCB* pcb = PCB::getById(id);
    if (pcb) {
        UNLOCK;
        return pcb->getMyThread();
    }

    UNLOCK;
    return NULL;
}

Thread::Thread(StackSize stackSize, Time timeSlice)
{

    INT_DISABLE;

    	myPCB = new PCB(stackSize, timeSlice, false, this);

    INT_ENABLE;
}

void dispatch()
{
    INT_DISABLE;

    	fromDispatch = true;
    	timer();

    INT_ENABLE;
}
