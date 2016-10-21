#ifndef TIMER_H
#define TIMER_H

#include <windows.h>


#define TARGET_RESOLUTION 1         // 1-millisecond target resolution



class timer{
private:

	LARGE_INTEGER frequency;        // ticks per second
	LARGE_INTEGER t1, t2;           // ticks
	double elapsedTime;
	double deltaTime;
	double tickLength;
	double tickLeft;
	bool isTick;

	int ticksPerSecond;
	int secondTicks;
	double secondLeft;

	int callsPerSecond;
	int secondCalls;

	/* internal function to figure out stuff */
	void updateTick(){
		QueryPerformanceCounter(&t2);
		deltaTime = elapsedTime;
		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		deltaTime = elapsedTime - deltaTime;
		tickLeft -= deltaTime;
		secondLeft -= deltaTime;

		while(tickLeft < 0){
			isTick = true;
			tickLeft += tickLength;
			++secondTicks;
		}
		while(secondLeft < 0){
			ticksPerSecond = secondTicks;
			secondTicks = 0;
			callsPerSecond = secondCalls;
			secondCalls = 0;
			secondLeft += 1000;
			if(ticksPerSecond < 100){
				cout << "fps:" << ticksPerSecond << " cps: " << callsPerSecond << endl;
			}
		}
	}



public:


	timer() :
				tickLength(10), //in milliseconds (10ms = .01s)
				tickLeft(0),
				isTick(false),
				ticksPerSecond(100),
				secondLeft(0)
			{

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&t1);

	}

	~timer(){
	}

	double getTimeSinceLastTick(){
		return elapsedTime;
	}

	bool tick(){
		updateTick();
		++secondCalls;
		return !(!isTick ? !isTick : isTick = false);
	}

	int fps(){
		return ticksPerSecond;
	}

};



#endif // TIMER_H
