#ifndef CONDITION_IS_MORE_THAN_H
#define CONDITION_IS_MORE_THAN_H

#include "../condition.h"

class isMoreThan : public condition{
private:
	double* var;
	double* target;

public:

	isMoreThan(double* var,double* target) :
		var(var),
		target(target){
	}

	bool isMet(){
		return *var > *target;
	}

};




#endif // CONDITION_IS_MORE_THAN_H
