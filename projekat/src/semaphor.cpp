#include "semaphor.h"
#include "kernSem.h"

Semaphore::Semaphore(int init)
{
    LOCK;

        myImpl = new KernelSem(init);

    UNLOCK;
}

Semaphore::~Semaphore()
{
    INT_DISABLE;

        delete myImpl;

    INT_ENABLE;
}

int Semaphore::wait(Time maxTimeToWait)
{
        return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal()
{
        myImpl->signal();
}

int Semaphore::val() const
{
    LOCK;

        int ret = myImpl->getVal();

    UNLOCK;

    return ret;
}
