#ifndef _kernEv_h_
#define _kernEv_h_

#include "common.h"
#include "list.h"
#include "pcb.h"

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();

private:
	IVTNo myIVTNo;
	PCB* pcbCreator;
	bool free;
};


#endif
