

#include "engine.h"
#include "assetlibrary/assetlibrary.h"

Engine::Engine()
    //horde3dInitialized(false),
    //model(0),
    //_cam(0),
    //t(0)
    {
	glfwInit();
}

Engine::~Engine(){
	//if(horde3dInitialized){
    //    h3dRelease();
	//}
	glfwTerminate();
	cout << "Kill engine\n";
}

bool Engine::Init(){
    //load asset library


	//load default settings
	VarMap settings = Assets().LoadSettings(this,"default.est");
	settings = settings.GetGroup("default");

    if(settings.IsSet("engine-control.type")){
		EngineControl* ec = Assets().LoadEngineControl(this,settings.GetGroup("engine-control"));
		engineControlStack.Push(ec);
		Node::RegisterGlobal((Node*)ec,"engine-control");
    }

    //load any windows
    //VarMap windowsSettings =settings.GetGroup("windows");
    //if(windowsSettings.GetCount()){
    //    List<string> windowNames = windowsSettings.GroupNames();
    //    for(int i=0;i<windowNames.GetCount();i++){
    //        VarMap windowSettings = windowsSettings.GetGroup(windowNames[i]);
    //        Window* window = new Window(windowSettings);
    //        if(window->Handle()){
    //            windows.Push(window);
    //        } else {
    //            cout << "Couldn't create the window: " << windowNames[i] << "\n";
    //        }
    //    }
    //}

    //if(windows.GetCount() == 0){
    //    cout << "engine can't continue initialization without a window\n";
    //    return false;
    //}
    //horde3dInitialized = h3dInit();

    //if(!horde3dInitialized){
    //    cout << "failed to initialize horde3d\n";
    //    return false;
    //}







    //load any cameras
    /*
    VarMap cameras = settings.GetGroup("cameras");
    List<string> cameraNames = cameras.GroupNames();
    for(int i=0;i<cameraNames.GetCount();i++){
        Assets().LoadCamera(this,cameras.GetGroup(cameraNames[i]+""));
    }


	//load any views
    VarMap views = settings.GetGroup("views");
    List<string> viewNames = views.GroupNames();
    for(int i=0;i<viewNames.GetCount();i++){
        Assets().LoadView(this,views.GetGroup(viewNames[i]+""));
    }
    */
/*
	//try to load any misc. objects that have a name and a type and in the
	//  custom {
	//      object {
	// 		    type objectType
	//      }
	//  }
	//This should load the .dll with the name engine.object.objectType.dll
	//from the engine/assets/extensions folder
	VarMap customNodes = settings.GetGroup("custom");
	List<string> customNodeNames = customNodes.GroupNames();
	for(int i=0;i<customNodeNames.GetCount();i++){
		AssetLibrary::LoadCustom(customNodeNames[i],customNodes.GetGroup(customNodeNames[i]));
	}
    */

      //  cout << "Engine::AssetLibrary id = " << &Assets() << endl;



    //Node::RegisterGlobal((Node*)this,"engine");

	return true;
}

void Engine::Run(){
    //if(!horde3dInitialized) return;
	//while(windows.GetCount()){

        //if(engineControlStack.GetCount()){
        //    engineControlStack.Last()->Update();
        //}

        //glfwMakeContextCurrent(windows[0]->Handle());

            glfwPollEvents();
            //t += .4;
            // Set camera parameters
            //h3dSetNodeTransform( _cam, 0, 10, 0, -34 ,t, 0, 1, 1, 1 );

            // Show stats
            //h3dutShowFrameStats( _fontMatRes, _panelMatRes, _statMode );

            //h3dutShowText( "Pipeline: forward", 0.03f, 0.24f, 0.026f, 1, 1, 1, _fontMatRes );


            // Show logo
            //const float ww = (float)h3dGetNodeParamI( _cam, H3DCamera::ViewportWidthI ) /
            //                 (float)h3dGetNodeParamI( _cam, H3DCamera::ViewportHeightI );
            //const float ovLogo[] = { ww-0.4f, 0.8f, 0, 1,  ww-0.4f, 1, 0, 0,  ww, 1, 1, 0,  ww, 0.8f, 1, 1 };
            //h3dShowOverlays( ovLogo, 4, 1.f, 1.f, 1.f, 1.f, _logoMatRes, 0 );

            // Render scene
            //h3dRender( _cam );


        /*
        for(int i=0;i<windows.GetCount();i++){
            Window* window = windows[i];
            window->Update();
            window->Draw();
            if(window->ShouldClose()){
                cout << "closed window\n";
                windows.Cut(i);
                delete window;
            }
        }*/







        //glfwSwapBuffers(windows[0]->Handle());

        /*
        for(int i=0;i<windows.GetCount();i++){
            Window* window = windows[i];
            window->Update();
            window->Draw();
            if(window->ShouldClose()){
                cout << "closed window\n";
                windows.Cut(i);
                delete window;
            }
        }*/


	//}

}
