#ifndef ENGINE_H
#define ENGINE_H
//#include <Horde3D.h>
//#include <Horde3DUtils.h>
#define GLFW_INCLUDE_GLU

#include <GLFW/glfw3.h>

#include "core/node.h"
#include "core/list.h"
#include "core/double3.h"


//#include "core/enginecontrol.h"
//#include "graphics/window.h"

//#include "physics/physical_single_body.h"

class Engine : public Node{
private:

	//List<EngineControl*> engineControlStack;
	//List<Window*> windows;
	//bool horde3dInitialized;

	//H3DNode model;
	//H3DNode _cam;
	//float t;
    //H3DRes pipeRes;
    //H3DRes modelRes;
    //H3DRes animRes;
    //H3DNode light;

	//H3DRes _forwardPipeRes;
	//H3DRes _deferredPipeRes;
	// Overlays
	//H3DRes _fontMatRes;
	//H3DRes _panelMatRes;
	//H3DRes _logoMatRes;


public:

    Engine();
    ~Engine();

	bool Init();


	void Run();

};




#endif // ENGINE_H
