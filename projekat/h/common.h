#ifndef _common_h_
#define _common_h_

#include <stddef.h>

//create bool type
typedef int bool; // ne postoji bool u Borlandu
#define false 0
#define true  1


//Stvari uvezene iz maina
extern unsigned int volatile lock;
extern bool volatile fromDispatch;
extern bool volatile dispatchAfterUnlock;


//LOCKS

#define LOCK ++lock;
#define UNLOCK --lock; if(lock == 0 && dispatchAfterUnlock) { dispatch(); }

#define INT_DISABLE asm{ pushf; cli; }
#define INT_ENABLE asm popf


//Thread stvari
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

//interrupts
typedef void interrupt(*InterruptRoutine)(...);
void interrupt timer(...);

//to make it more easy
typedef unsigned int Register;
typedef unsigned int  Word;
typedef void (*WrapperFunc)();
typedef unsigned char IVTNo;



void tick();

#endif // !_COMMON_H_

