#include "kernEv.h"
#include "common.h"
#include "ivt.h"
#include <dos.h>

KernelEv::KernelEv(IVTNo ivtNo)
{
	LOCK;

		free = true;
		myIVTNo = ivtNo;
		pcbCreator = runningPCB;
		IVTEntry::getEntry(myIVTNo)->assign(this);

	UNLOCK;
}

KernelEv::~KernelEv()
{
	LOCK;

		IVTEntry::getEntry(myIVTNo)->unassign(this);

	UNLOCK;
}

void KernelEv::wait()
{

	LOCK;

	if(pcbCreator == runningPCB) {
		pcbCreator->setBlocked();

		free = false;

		UNLOCK;
		dispatch();
	}else{
		UNLOCK;
	}


}

void KernelEv::signal()
{

	LOCK;

	if(!free){

		free = true;
		pcbCreator->setReady();
		Scheduler::put(pcbCreator);
	}

	UNLOCK;
}
