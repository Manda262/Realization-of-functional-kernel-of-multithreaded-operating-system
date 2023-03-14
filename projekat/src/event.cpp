#include "event.h"
#include "kernEv.h"

Event::Event(IVTNo ivtNo)
{
    LOCK;

    	myImpl = new KernelEv(ivtNo);

    UNLOCK;
}

Event::~Event()
{
    INT_DISABLE;

    	delete myImpl;

    INT_ENABLE;
}

void Event::wait()
{

    myImpl->wait();

}

void Event::signal()
{

    myImpl->signal();

}
