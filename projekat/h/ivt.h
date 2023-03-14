#ifndef _ivt_h_
#define _ivt_h_

#include "common.h"
#include "list.h"

#define IVTENTRY_COUNT 256

#define PREPAREENTRY(ivtNo, callOld)                        \
    extern IVTEntry ivtEntry##ivtNo;                        \
                                                            \
	void interrupt eventInterrupt##ivtNo(...) {             \
		ivtEntry##ivtNo.signal();                           \
		if (callOld) ivtEntry##ivtNo.callOldInterrupt();    \
		dispatch();											\
	}                                                       \
                                                            \
    IVTEntry ivtEntry##ivtNo(ivtNo, eventInterrupt##ivtNo);

class KernelEv;

class IVTEntry {
public:
	static IVTEntry* getEntry(IVTNo ivtNo) { return entryMap[ivtNo]; }

	IVTEntry(IVTNo ivtNo, InterruptRoutine newInterrupt);
	~IVTEntry();

	void signal();

	void assign(KernelEv* ev) { events.addToTail(ev); }
	void unassign(KernelEv* ev) { events.erase(ev); }

	void callOldInterrupt() { oldInterrupt(); }

private:
	static IVTEntry* entryMap[IVTENTRY_COUNT];

	IVTNo myIVTNo;
	List<KernelEv*> events;
	InterruptRoutine oldInterrupt;
};

#endif
