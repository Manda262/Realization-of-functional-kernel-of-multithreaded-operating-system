//MOJ PROJEKAT

#include "common.h"
#include "pcb.h"
#include "thread.h"
#include "kernSem.h"
#include <dos.h>
#include <iostream.h>

InterruptRoutine oldTimer = NULL;

//Liste
List<PCB*> pcbList;
List<KernelSem*> semList;

//PCBovi
PCB* runningPCB;
PCB* idlePCB;
PCB* mainPCB;

unsigned int volatile lock = 0;
bool volatile fromDispatch = false;
bool volatile dispatchAfterUnlock = false;
Register volatile tsp, tss, tbp;



void interrupt timer(...) {

	//provera da li je pozvan timer iz dispatcha i potrebne provere
	if (!fromDispatch) {
		oldTimer();
		tick();

		KernelSem* ker;

		for (List<KernelSem*>::Iterator it = semList.begin();
			it != semList.end();
			++it)
		{
			(*it)->reduceWaitTime();
		}

		if (runningPCB->getPcbTimeSlice() == 0) return;
		if (runningPCB->getPcbTime() > 0) --(runningPCB->getPcbTime());
		if (lock && runningPCB->getPcbTime() == 0) dispatchAfterUnlock = true;

	}

	if (fromDispatch && dispatchAfterUnlock) dispatchAfterUnlock = false;


	//priprema za menjanje runningPCB-a
	if (fromDispatch || (runningPCB->getPcbTime() == 0 && lock == 0)) {

		asm{
			mov tss,ss;
			mov tsp, sp;
			mov tbp, bp;
		}
		runningPCB->setSs(tss);
		runningPCB->setSp(tsp);
		runningPCB->setBp(tbp);

		if (runningPCB->isRunning()) {
			runningPCB->setReady();
			Scheduler::put(runningPCB);
		}

		//promena runninga i namestanje njegovih registara
		runningPCB = Scheduler::get();
		if (!runningPCB) {
			runningPCB = idlePCB;
			runningPCB->setIdle();
		}
		else {
			runningPCB->setRunning();
		}
		runningPCB->setPcbTime(runningPCB->getPcbTimeSlice());

		tss = runningPCB->getSs();
		tsp = runningPCB->getSp();
		tbp = runningPCB->getBp();
		asm{
			mov ss,tss;
			mov sp, tsp;
			mov bp, tbp;
		}
		fromDispatch = false;
	}

}



//Korisnicki main
int userMain(int argc, char* argv[]);

//glavni program
int main(int argc, char* argv[]) {

	cout << "Pocetak programa" << endl;

	mainPCB = new PCB(0, defaultTimeSlice);
	mainPCB->setRunning();
	runningPCB = mainPCB;
	idlePCB = new PCB(0x400UL, 1U, true);

	//inicijalizacija prekida;

	INT_DISABLE;

		oldTimer = getvect(0x8); // cuvamo staru prekidnu rutinu za timer
		setvect(0x8, timer); // smestamo nasu prekidnu rutinu za timer

	INT_ENABLE;

	INT_DISABLE;

		cout << "Pocetak korisnickog programa" << endl;

	INT_ENABLE;

	int rez = userMain(argc, argv);

	//restauracija prekida;

	INT_DISABLE;

		setvect(0x8, oldTimer); // vracamo staru prekidnu rutinu za timer na mesto

	INT_ENABLE;


	delete mainPCB;
	delete idlePCB;

	cout << "Kraj programa" << endl;

	return rez;


}
