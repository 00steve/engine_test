#include <iostream>
using namespace std;

#include <engine/engine.h>


int main(int argc, char* argv[])
{

	Engine engine;
	if(!engine.Init()){
		cout << "engine failed to initialize\n";
		return 0;
	}
	engine.Run();
    return 0;
}
