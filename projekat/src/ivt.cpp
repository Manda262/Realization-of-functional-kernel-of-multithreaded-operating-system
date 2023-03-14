#include "ivt.h"
#include "thread.h"
#include "kernEv.h"
#include <dos.h>

IVTEntry* IVTEntry::entryMap[IVTENTRY_COUNT] = { 0 };

IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine newInterrupt)
{
	myIVTNo = ivtNo;

	INT_DISABLE;

		oldInterrupt = getvect(myIVTNo);
		setvect(myIVTNo, newInterrupt);

	INT_ENABLE;

	entryMap[myIVTNo] = this;
}

IVTEntry::~IVTEntry()
{
	INT_DISABLE;

		setvect(myIVTNo, oldInterrupt);

	INT_ENABLE;

	entryMap[myIVTNo] = NULL;
}

void IVTEntry::signal()
{
	LOCK;

	for (List<KernelEv*>::Iterator it = events.begin();
		it != events.end();
		++it)

		(*it)->signal();

	UNLOCK;

}
