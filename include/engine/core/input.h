#ifndef MONSTERS_INPUT_H
#define MONSTERS_INPUT_H
/*keep track of the input from the keyboard and mouse*/


/* XBOX controller layout
joystick axis 0/1 = left joystick x/y
joystick 2 0->1 = left trigger
joystick 2 0->-1 = right trigger
joystick axis 4/3 = left joystick x/y

using any of the controllers' joysticks would
be the following code...
		float joystickPosition[int controller_number][int joystic_number]

button 0 = button a
button 1 = button b
buttom 2 = buttom x

using any of the controllers' buttons would be
the following code...
		bool joystickButtons[int controller_number][int button number]
*/



#include "int2.h"
#include "double2.h"

class input {
    public:
        static int2 *screenResolution;
        static bool *keys;
        static int2 oldMousePosition;
        static int2 mousePosition;
        static int mousePositionX;
        static int mousePositionY;
        static int2 mousePixelMovement;
        static double2 mouseSmoothMovement;
        static int *mouseButton;
        static int mouseWheel;


        /*create an array of joystick positions, each
        of which is an array of the positions of each axis
        that the joystick supports*/
        static float **joystickPosition;
        static int *joystickAxisCount;
        static int joystickCount;
        static int *joystickId;
        static unsigned char **joystickButtons;
        static int *joystickButtonCount;



        /*the amount of window that the mouse has
        moved over. The total screen movement will
        add up to 1.*/
        static float screenMouseMoveX,screenMouseMoveY;




        static void init() {
            /*set a callback to record the key
            being pressed when it is*/
            glfwSetKeyCallback(input::key_callback);
            /*set all of the keys to be false by default*/
            resetKeys();
            /*set the mouse wheel to equal zero*/
            glfwSetMouseWheel(0);
            //get the size of the window and store it.
            glfwGetWindowSize( &screenResolution->x,&screenResolution->y);
            mousePixelMovement = int2(0,0);
            mouseSmoothMovement = double2(0,0);

            if(glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_PRESENT) == GL_TRUE){
                #if PRINT_LOW_LEVEL
                cout << "has a first JOYSTICK!\n";
                #endif
                joystickPosition = new float*[++joystickCount];
                joystickAxisCount = new int[joystickCount];
                joystickAxisCount[joystickCount-1] = glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_AXES);
                joystickPosition[joystickCount-1] = new float[joystickAxisCount[joystickCount-1]];
                #if PRINT_LOW_LEVEL
                cout << "joystick has " << joystickAxisCount[joystickCount-1] << " axes\n";
                #endif
                joystickId = new int[joystickCount];
                joystickId[joystickCount-1] = GLFW_JOYSTICK_1;
                joystickButtonCount = new int[joystickCount];
                joystickButtonCount[joystickCount-1] = glfwGetJoystickParam(GLFW_JOYSTICK_1,GLFW_BUTTONS);
                joystickButtons = new unsigned char*[joystickCount];
                joystickButtons[joystickCount-1] = new unsigned char[joystickButtonCount[joystickCount-1]];

            }
            if(glfwGetJoystickParam(GLFW_JOYSTICK_2,GLFW_PRESENT) == GL_TRUE){
            	#if PRINT_LOW_LEVEL
                cout << "has a second JOYSTICK!\n";
                #endif
            }

        }


        static void GLFWCALL key_callback(int key, int action){
            #if INPUT_OUTPUT
                if(action) cout << key << " = PRESSED\n";
                else cout << key << " = UN-PRESSED\n";
            #endif
            keys[key] = action;
        }

        static void updateMouse(){
            glfwGetMousePos(&mousePosition.x,&mousePosition.y);
            if(mousePosition - oldMousePosition < 200){
                mousePixelMovement = mousePosition - oldMousePosition;
                mouseSmoothMovement = (mouseSmoothMovement*3+double2(mousePixelMovement.x,mousePixelMovement.y)) * .25f;
                mousePositionX = mousePosition.x;
                //mousePositionY = mousePosition.y;
                mousePositionY = screenResolution->y - mousePosition.y;
            }

            oldMousePosition = mousePosition;
            mouseButton[0] = glfwGetMouseButton( GLFW_MOUSE_BUTTON_1 );
            mouseButton[1] = glfwGetMouseButton( GLFW_MOUSE_BUTTON_2 );
            mouseWheel = glfwGetMouseWheel();
        }

        static void updateJoystick(int pad_number){
            if(pad_number >= joystickCount) return;

            glfwGetJoystickPos(joystickId[pad_number],joystickPosition[pad_number],joystickAxisCount[pad_number]);
            glfwGetJoystickButtons(joystickId[pad_number],joystickButtons[pad_number],joystickButtonCount[pad_number]);
            #if PRINT_REALTIME
            cout << joystickPosition[0][0] << "," << joystickPosition[0][1] << endl;
            #endif
        }


        static void resetKeys() {

			cout << " KEYS RESET ON KEYBOARD, FUCKING FUCK FUCKER!\n";
            for(int i=0;i<325;i++)
                keys[i] = false;
        }


        static void setWindowSize(int width,int height){
            screenResolution->x = width;
            screenResolution->y = height;
        }
};

int2 *input::screenResolution = new int2(0,0);
bool *input::keys = new bool[325];
int2 input::mousePosition;
int input::mousePositionX;
int input::mousePositionY;
int2 input::oldMousePosition;
int2 input::mousePixelMovement;
double2 input::mouseSmoothMovement;
int *input::mouseButton = new int[8];
int input::mouseWheel = 0;


int input::joystickCount = 0;
float **input::joystickPosition = new float*[input::joystickCount];
int *input::joystickAxisCount = new int[input::joystickCount];
int *input::joystickId = new int[input::joystickCount];
unsigned char **input::joystickButtons = new unsigned char*[input::joystickCount];
int *input::joystickButtonCount = new int[input::joystickCount];





#endif
