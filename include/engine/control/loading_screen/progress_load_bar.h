#ifndef PROGRESS_LOAD_BAR_H
#define PROGRESS_LOAD_BAR_H

#include "loading_screen_progress.h"

class progress_load_bar : public loading_screen_progress{
private:

	double lp;
public:

	progress_load_bar(double* progress,varMap settings) :
		loading_screen_progress(progress),
		lp(0){
		cout << " shittake\n";
	}

	void update(){
		//cout << " progress : " <<
		if(getProgress() - lp > .01){
			lp = getProgress();
			cout << "- ";
		}
	}

	void draw(){

	}

};


#endif // PROGRESS_LOAD_BAR_H
