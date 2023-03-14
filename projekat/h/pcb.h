#ifndef _pcb_h_
#define _pcb_h_
#include "thread.h"
#include "common.h"
#include "schedule.h"
#include "list.h"



class PCB;

extern List<PCB*> pcbList;
extern PCB* runningPCB;
extern PCB* mainPCB;
extern PCB* idlePCB;


class PCB {
public:
	PCB(StackSize stackSize, Time timeSlice, bool isIdl = false, Thread* thr = NULL);
	~PCB();

	static StackSize const MAX_STACK_SIZE;
	static Word const PSW_INIT;

	//metodi potrebni za izradu thread metoda
	void startPCB();
	void waitToComplete();
	const int getId() const { return id; }
	static PCB* getById(int id);
	static int getRunningId() { return runningPCB->id; }

	//stack initialization
	void initializeStack(WrapperFunc wrapper);

	//flags setters
	void setNotStarted();
	void setBlocked();
	void setFinished();
	void setRunning();
	void setReady();

	void setIdle();

	//flags getters
	const bool isNotStarted() const { return isNotStartedFlag; }
	const bool isBlocked() const { return isBlockedFlag; }
	const bool isFinished() const { return isFinishedFlag; }
	const bool isRunning() const { return isRunningFlag; }
	const bool isReady() const { return isReadyFlag; }

	const bool isIdle() const { return isIdleFlag; }

	//getters
	Time& getPcbTime() { return pcbTime; }
	Time getPcbTimeSlice() const { return pcbTimeSlice; }
	Register getSs() const { return ss; }
	Register getSp() const { return sp; }
	Register getBp() const { return bp; }
	Thread* getMyThread() { return myThread; }
	int getWaitResult() const {return waitResult; }

	//setters
	void setSs(Register r) { ss = r; }
	void setSp(Register r) { sp = r; }
	void setBp(Register r) { bp = r; }

	void setPcbTime(Time t) { pcbTime = t; }
	void setWaitResult(int i) { waitResult = i; }


private:
	ID id;
	static int pos_id;

	Thread* myThread;

	List<PCB*> blockedList;

	Time pcbTimeSlice;
	Time pcbTime;
	Register ss;
	Register sp;
	Register bp;

	//stack
	Word* stack;
	StackSize pcbStackSize;
	StackSize pcbStackLenght;

	//wrappers
	static void normalWrapper();
	static void idleWrapper();

	//flags
	bool isNotStartedFlag;
	bool isBlockedFlag;
	bool isFinishedFlag;
	bool isRunningFlag;
	bool isReadyFlag;

	bool isIdleFlag;

	void clearFlags();

	int waitResult;


};

#endif // !_pcb_h_

