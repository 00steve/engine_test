#include "include/engine/engine.h"

int main() {


	engine e = engine();
	if(!e.init()){
		cout << "Engine failed to initiate!\n" << endl;
		return 0;
	}
	e.run();


    return 0;
};
