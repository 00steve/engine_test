/* The view class is basically a way to split the window up
into different renerable areas. Each view is rendered to a different
part of the screen. When the screen is resized, the view is scaled
with it.
*/
#ifndef td_view_h
#define td_view_h
#include "../core/double2.h"
#include "../core/int2.h"
#include "../graphics/drawable.h"
#include "../core/camera.h"
#include "../ui/indui.h"
#include "../graphics/shader.h"

#define VIEW_SIZEBY_PIXEL 0
#define VIEW_SIZEBY_SCALE 1

#define VIEW_LOCATION_TOP_LEFT 0
#define VIEW_LOCATION_TOP 1
#define VIEW_LOCATION_TOP_RIGHT 2
#define VIEW_LOCATION_LEFT 3
#define VIEW_LOCATION_CENTER 4
#define VIEW_LOCATION_RIGHT 5
#define VIEW_LOCATION_BOTTOM_LEFT 6
#define VIEW_LOCATION_BOTTOM 7
#define VIEW_LOCATION_BOTTOM_RIGHT 8




class view : protected node{
private:

    /*the pixels or precent offset that the view
    should be from its location. This is stored
    when the view is created and used to calculate
    calculated_offset*/
    double2 offset;
    /*the actual pixel offset of the view*/
    double2 calculated_offset;

    double2 scale;
    int size_method;
    int location;
    int2 windowSize;


    /*store a shader program to use when using
    the default draw function of the view*/
    shader* shader_program;

    /*store the pointer to a possible ui that could be used in this
    view.*/
	IndUI *ui;

	/*the name of the camera that the view would like to use. This
	is mostly used when loading views and cameras. Say... a view is
	loaded, but the camera that it wants has not been loaded yet...
	the preferredCameraName string is set with the name of the camera
	that it wants, so that when each new camera is loaded, the loading
	function can check to see if each new camera is the camera that
	each view is looking for. */
	string preferredCameraName;

    virtual void recalculateBounds() {
        #if EVENT_VERBOSE_OUTPUT
            cout << "RECALCULATE view " << this->getId() << "\n";
        #endif
        switch(size_method){
            case VIEW_SIZEBY_PIXEL:
                calculated_offset = offset;
                break;
            case VIEW_SIZEBY_SCALE:
                dimensions = int2(windowSize.x*scale.x,windowSize.y*scale.y);
                raw_dimensions = double2((float)windowSize.x*scale.x,(float)windowSize.y*scale.y);
                calculated_offset = double2(windowSize.x*offset.x,windowSize.y*offset.y);
                break;
        }
        //calculate aspect ratio
        aspectRatio = (float)dimensions.x / (float)dimensions.y;
        //find the bounds of the view based on its location setting
        switch(location) {
            case VIEW_LOCATION_TOP_LEFT:
                bottom_left = int2(windowSize.x*calculated_offset.x,windowSize.y-dimensions.y-windowSize.y*calculated_offset.y);
                break;
            case VIEW_LOCATION_TOP:
                bottom_left = int2((windowSize.x - dimensions.x) * .5f +windowSize.x*calculated_offset.x , windowSize.y-dimensions.y-windowSize.y*calculated_offset.y);
                break;
            case VIEW_LOCATION_TOP_RIGHT:
                bottom_left = int2(windowSize.x-dimensions.x-windowSize.x*calculated_offset.x,windowSize.y-dimensions.y-windowSize.y*calculated_offset.y);
                break;
            case VIEW_LOCATION_LEFT:
                bottom_left = int2( windowSize.x*calculated_offset.x, (windowSize.y - dimensions.y) * .5f);
                break;
            case VIEW_LOCATION_CENTER:
                bottom_left = int2((windowSize.x - dimensions.x) * .5f +windowSize.x*calculated_offset.x , (windowSize.y - dimensions.y) * .5f);
                break;
            case VIEW_LOCATION_RIGHT:
                bottom_left = int2( windowSize.x-dimensions.x-windowSize.x*calculated_offset.x, (windowSize.y - dimensions.y) * .5f);
                break;
            case VIEW_LOCATION_BOTTOM_LEFT:
                bottom_left = int2( windowSize.x*calculated_offset.x, windowSize.y*calculated_offset.y);
                break;
            case VIEW_LOCATION_BOTTOM:
                bottom_left = int2((windowSize.x - dimensions.x) * .5f , windowSize.y*calculated_offset.y);
                break;
            case VIEW_LOCATION_BOTTOM_RIGHT:
                bottom_left = int2(windowSize.x-dimensions.x-windowSize.x*calculated_offset.x, windowSize.y*calculated_offset.y);
                break;
        }
    }

protected:

    /*the actual pixel position in the screen of the view where it
    starts drawing from. glViewport uses this and the dimensions
    variable to determine where to draw.*/
    int2 bottom_left;

	/*this is the value that was passed into the construtor*/
	double2 raw_dimensions;

	/*the calculated dimensions of the view, based on the sizing
	method that was used to create it.*/
    int2 dimensions;

    camera *cam;

    float aspectRatio;

public:

    view(string name,double2 offset,double2 dimensions,int size_method,int location) :
				offset(offset),
				size_method(size_method),
				location(location),
				shader_program(NULL),
				ui(NULL),
				preferredCameraName("default"),
				raw_dimensions(dimensions),
				cam(NULL)
			{
        #if EVENT_OUTPUT
            cout << "CREATE view\n";
        #endif
        this->offset = offset;
        switch(size_method){
		case VIEW_SIZEBY_PIXEL:
			this->dimensions = int2((int)dimensions.x,(int)dimensions.y);
			break;
		case VIEW_SIZEBY_SCALE:
			scale = dimensions;
			break;
		default:
			#if EVENT_OUTPUT
				cout << "FAILURE could not create view (incorrect VIEW_SIZEBY argument)\n";
			#endif
			return;
        }
		setName(new string("view_" + name));

    }

	~view(){
		#if DEBUG_DELETES
			cout << " view >";
		#endif
	}

	virtual bool handleMessage(node_message* m){
		switch(m->code){
		case MESSAGE_SET_CAMERA:
			cam = (camera*) m->data;
			cout << "- set view camera\n";
			return true;
		}
		return node::handleMessage(m);
	}


    /**\brief set openGL to draw to this view. If there is no camera, it will
    return false**/
    virtual bool orientToCamera(){
        if(!cam) return false;
        cam->update();
        cam->translateView(aspectRatio);
        glMatrixMode(GL_MODELVIEW);
        drawable::setCurrentCameraPosition(cam->getPosition());
        return true;
    }

    /**\brief force the view to draw a node however that node should be drawn.
    This will usually involve calling the node's draw() function.

    This involves pushing the matrix state, setting the viewport (glViewport),
	orienting the view matrix to the camera (if a camera is set), setting the
	shader (if a shader is set), and calling the argument's draw() function.
    **/
    virtual void draw(drawable *subject){
        glClear(GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glViewport(bottom_left.x,bottom_left.y,dimensions.x,dimensions.y);
			orientToCamera();
			if(shader_program){
				shader_program->makeCurrent();
				if(subject) subject->draw();
				shader_program->clearCurrent();
			} else {
				if(subject) subject->draw();
			}
			if(ui){
				ui->draw();
			}
			glPopAttrib();
        glPopMatrix();
    }

    string type(){
        return "view";
    }


	/*get various properties of the view*/
	int2 getDimensions(){ return dimensions; }
	double2 getRawDimensions(){ return raw_dimensions; }
	double2 getRawOffset(){ return offset; }
	int2 getOffset(){return int2((int)offset.x,(int)offset.y);}
	int getSizeMethod(){ return size_method; }
	int getLocation(){ return location; }
	int2* getSizePtr(){ return &dimensions; }
	int* getWidthPtr(){ return &dimensions.x; }
	int* getHeightPtr(){ return &dimensions.y; }
	string getPreferredCameraName(){
		return preferredCameraName;
	}

    /**\brief returns a reference to the current camera that is being
    used to update the view's starting drawing position.**/
    camera* getCamera(){ return cam; }
	float getAspectRatio(){ return aspectRatio; }



    /**\brief change the area of the window that the
    view draws to.

    If the size_method is VIEW_SIZEBY_PIXEL,
    the view will retain it's size but be moved to the location
    it should be positioned at, based on the size of the window.
    If the size_method is VIEW_SIZEBY_PIXEL.**/
    void setWindowSize(int windowWidth,int windowHeight){
        this->windowSize = int2(windowWidth,windowHeight);
        //cout << "setting new window size ( " << windowWidth << "," << windowHeight << ")\n";
        recalculateBounds();
    }
    /**\brief sets the camera that will be used to set the view
    when it needs to be drawn to.**/
    void setCamera(camera* currentCamera){
        cam = currentCamera;
        //cout << "view cam address: " << cam << endl;
    }
    void setShader(shader* newShader){
        shader_program = newShader;
    }
	void setUI(IndUI *newUI){
		ui = newUI;
	}
	string setPreferredCameraName(string newName){
		return preferredCameraName = newName;
	}



};

#endif // td_view_h
