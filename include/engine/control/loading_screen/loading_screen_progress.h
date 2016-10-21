#ifndef LOADING_SCREEN_PROGRESS_H
#define LOADING_SCREEN_PROGRESS_H

class loading_screen_progress : public drawable{
private:

	/*store a reference to a double that exists
	in whatever is creating this object, so it
	can be updated in the other object and this
	will update as well.*/
	double* progressPtr;

public:

	loading_screen_progress(double* progress) :
		progressPtr(progress){
	}


	double getProgress(){ return *progressPtr; }

	virtual void update() = 0;
};


#endif // LOADING_SCREEN_PROGRESS_H
