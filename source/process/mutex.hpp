#pragma once

class Mutex{
    private:
        volatile bool locked;
    
    public:
        void Lock();
        void Unlock();
        bool GetLock();
};