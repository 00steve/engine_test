#ifndef WINDOW_H
#define WINDOW_H

#define GLEW_STATIC
#include "../glew.c"
#include <GL/glfw.h>
#include <GL/glu.h>

#include "../core/list.h"
#include "../core/view.h"

#include "../core/node.h"


class window{
private:


    static float aspectRatio;
    static bool open;
    static list<view*> views;

    static void (*resizeCallbackFunction)(int width,int height) ;
    static void GLFWCALL onResize(int width,int height){
        window::width = width;
        window::height = height;
        window::aspectRatio = (float)height / (float)width;
        #if EVENT_VERBOSE_OUTPUT
            cout << "WINDOW resize to " << width << "x" << height << endl;
        #endif
        view** tV;
        while((tV = views.iterate())) {
            (*tV)->setWindowSize(width,height);
        }
        if(resizeCallbackFunction){
            resizeCallbackFunction(width,height);
        }
    }

    static int GLFWCALL onClose(){
        open = false;
        return GL_TRUE;
    }

public:

    static int width;
    static int height;


    window(int width,int height,int screentype) {
        #if EVENT_OUTPUT
            cout << "CREATE window\n";
        #endif
        //store the resolution of the window
        window::width = width;
        window::height = height;
        aspectRatio = (float)height / (float)width;
        //try to initialize GLFW
        if (glfwInit() != GL_TRUE)
            return;
        /*send hints to the graphics card about how we want to
        draw everything*/
        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);
        //try to create
        if ((open = glfwOpenWindow(width, height, 8, 8, 8, 0, 8, 0, screentype)) != GL_TRUE) {
            cout << "could not create glfw window";
            return;
        }
        resizeCallbackFunction = NULL;
        glfwSetWindowSizeCallback(window::onResize);
        glfwSetWindowCloseCallback(window::onClose);
        /*lets the engine render as fast as it wants. Can cause screen
        tearing. This should be removed and rendering should be done
        asynchronously at some later date.*/
        glfwSwapInterval(0);
        //set up the basic openGL settings
        glEnable(GL_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable (GL_BLEND);
		glEnable( GL_TEXTURE_2D );
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glClearColor(0,0,1,1);

        GLenum err = glewInit();

        if (GLEW_OK != err) {
            cout << "GLEW FAILED!\n";
        }
        if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader){
            cout << "Ready for GLSL\n";
        }else {
            cout << "Not totally ready for GLSL :(  \n";
        }
        if (glewIsSupported("GL_VERSION_2_0")){
            cout << "Ready for OpenGL 2.0\n";
        }else {
            cout << "OpenGL 2.0 not supported\n";
        }
    }

    ~window() {
        #if EVENT_OUTPUT
            cout << "DESTROY window\n";
        #endif
        glfwTerminate();
    }

    void addView(view* newView){
        #if EVENT_OUTPUT
            cout << "ADD view to window\n";
        #endif
        views.push(newView);
        views.last()->setWindowSize(width,height);
    }

    bool isOpen(){
        return open;
    }

    float getAspectRatio(){return aspectRatio;}

    int getWidth(){return width;}

    int getHeight(){return height;}

    int2 getSize(){return int2(width,height);}

    list<view*>* getViews(){ return &views; }
    /*
    bool CaptureScreen()
    {
       nWidth = 1;
       nHeight = 1;
       unsigned char *pRGB = new unsigned char [3 * (nWidth+1) * (nHeight + 1) + 3];

       glReadBuffer( GL_BACK );
       glReadPixels( nStartX, nStartY, nWidth, nHeight, GL_RGB, GL_UNSIGNED_BYTE, pRGB);
    }
    */

    /*allow an external callback to be added when the window
    is resized. It will pass the new size as an array of two
    short unsigned ints.*/

    void resizeCallback(void(* func)(int width,int height)){
        resizeCallbackFunction = func;
    }



};

int window::height;
int window::width;
float window::aspectRatio;
bool window::open = false;
list<view*> window::views;
void (*window::resizeCallbackFunction)(int width,int height);

#endif
