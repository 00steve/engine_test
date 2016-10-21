#ifndef THREAD_H
#define THREAD_H


#include <windows.h>
#include <process.h>



class thread{
private:
    //the handle for this thread
    HANDLE thread_handle;

    /*create a static list of thread handles that
      will be referenced by static member functions*/
    static thread** threads;
    static unsigned int threadsCount;
    static unsigned int threadsLength;

    /*add a handle to the list of thread handles
      if the list is not large enough, increase the
      size of the list and then add it.*/
    static void addThread(thread* new_thread){
        //cout << "-->THREADS LIST\n";
        //for(unsigned int i=0;i<threadsCount;i++){
        //    cout << " - " << (i+1) << ": " << threads[i] << endl;
        //}
        //cout << " - new thread\n";
		//cout << "--<THREADS LIST\n";
        if(threadsCount == threadsLength){
            threadsLength += threadsLength + 1;
            thread** tempThreads = new thread*[threadsLength];
            memcpy(tempThreads,threads,sizeof(thread)*threadsCount);
            threads = tempThreads;
        }
        threads[threadsCount++] = new_thread;
        cout << "add thread\n";
    }



public:

    thread(void(* func)(void*),void* args){
        thread_handle = (HANDLE) _beginthread( func , 0 , args);
        addThread(this);
    }

    ~thread(){
        removeThread(this);
        TerminateThread(thread_handle,0);
    }

    HANDLE handle(){
        return thread_handle;
    }

    /*remove a handle from the list of thread handles*/
    static void removeThread(thread* old_thread){
        for(unsigned int i=0;i<threadsCount;i++){
            if(old_thread != threads[i]) continue;
            threads[i] = threads[--threadsCount];
            cout << "remove thread\n";
            return;
        }
        return;
    }


    /*try to kill all of the running threads. If the thread has
    finished running before it has been removed from the list,
    its handle will still be removed.*/
    static void killAll() {
        unsigned int t = 0;
        while(t < threadsCount){
            delete threads[t++];
        }
        threadsCount = 0;
    }

    static void waitForAll() {
        //create thread object
        HANDLE* handles = new HANDLE[threadsCount];
        for(unsigned int t=0;t<threadsCount;t++){
            handles[t] = threads[t]->thread_handle;
        }
        WaitForMultipleObjects(threadsCount,handles,TRUE,INFINITE);
    }

	static bool running() {
		return threadsCount > 0 ? true : false;
	}

	static int getCount(){ return threadsCount; }

};


thread** thread::threads = new thread*[0];
unsigned int thread::threadsCount = 0;
unsigned int thread::threadsLength = 0;
#endif // THREAD_H
