#ifndef GRAPHICS_STRUCTS_H
#define GRAPHICS_STRUCTS_H
#include <cstdlib>
#include <fstream>

struct rgba{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;

    rgba() :
    	r(0), g(0), b(0), a(0) {}
    rgba(float r,float g,float b) :
        r(r), g(g), b(b), a(1) {}
    rgba(float r,float g,float b,float a) :
        r(r), g(g), b(b), a(a) {}
};

struct point{
    vertex position;
    rgba color;

    point() {}

    point (float x,float y,float z) :
        position(vertex(x,y,z)),
        color(rgba(1,1,1)) {}

    point(float x,float y,float z,float r,float g,float b) :
        position(vertex(x,y,z)),
        color(rgba(r,g,b)) {}

};

struct locking_point : public point{
    bool locked;
};

struct face {
    int vertex[3];
    int texture[3];
    int normal[3];

    face(){}
    face(int v1,int v2,int v3){
        vertex[0] = v1;
        vertex[1] = v2;
        vertex[2] = v3;
    }

};

/*stores the indexes of vertices in a list. The
short type is used with the *assinine* assumption
that no mesh will ever have more than 65535 unique
vertices. Let's hope so.*/
struct tri {
    vertex a;
    vertex b;
    vertex c;

    tri() {}
    tri(vertex a,vertex b,vertex c) :
        a(a), b(b), c(c) {}
};

struct vbo {
private:
    /*keep track of the total number of triangles that
    are being stored in the vbo*/
    unsigned int triangle_size;

    GLuint positionBuffer;
    GLsizeiptr positionSize;// = 6 * 2 * sizeof(GLfloat);
    GLdouble *positionData;

    GLuint colorBuffer;
    GLsizeiptr colorSize;// = 6 * 3 * sizeof(GLubyte);
    GLubyte *colorData;

    tri l_face;
    unsigned int l_face_index;

    void init_data_structures(){
        //build buffers array
        //glGenBuffers(1,buffers);

        //build arrays to hold data
        positionData = new GLdouble[triangle_size*9]();
        glGenBuffers(1,&positionBuffer);
        colorData = new GLubyte[triangle_size*3]();
        glGenBuffers(1,&colorBuffer);
        //send the initial data structures to the graphics card in
        //case nothing else is ever sent
        send();
        l_face_index = -1;
    }

    /*
    void store_current_face(){
        if(l_face_index != -1u){
            positionData[l_face_index] = l_face.a.x;
            positionData[l_face_index+1] = l_face.a.y;
            positionData[l_face_index+2] = l_face.a.z;
            positionData[l_face_index+3] = l_face.b.x;
            positionData[l_face_index+4] = l_face.b.y;
            positionData[l_face_index+5] = l_face.b.z;
            positionData[l_face_index+6] = l_face.c.x;
            positionData[l_face_index+7] = l_face.c.y;
            positionData[l_face_index+8] = l_face.c.z;
        }
    }*/

public:
    /*create a blank vbo with all of the data set to
    zero. This is probably not good, so don't do it.*/
    vbo() :
        triangle_size(0),
        positionSize(0),
        colorSize(0) {
        init_data_structures();
    }

    /*construct the vbo giving it a starting
    size. If the size is known ahead of time, this
    should be used to prevent the resizing of the arrays
    that contain the data at runtime.*/
    vbo(unsigned int startingTriangleSize) :
        triangle_size(startingTriangleSize),
        positionSize(startingTriangleSize * 9 * sizeof(GLdouble)),
        colorSize(startingTriangleSize * 3 * sizeof(GLubyte)) {
        init_data_structures();
        for(unsigned int i=0;i<startingTriangleSize *3;i++){
            colorData[i] = (double)(rand()%1000) *.001f;
        }
    }



    /*Sends the current buffer data to the graphics card*/
    bool send(){
        /*make sure that if any vertices have been edited, they
        have been saved to the array before sending the array to
        graphics*/
        //store_current_face();
        return true;
    }

    void draw(){

        glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
        glBufferData(GL_ARRAY_BUFFER,positionSize,positionData,GL_STREAM_DRAW);
        glVertexPointer(3, GL_DOUBLE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER,colorSize,colorData,GL_STREAM_DRAW);
        glColorPointer(3, GL_UNSIGNED_BYTE, 0, 0);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glDrawArrays(GL_TRIANGLES,0,triangle_size*3);

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

    }

    /*send a temporary triangle object out to be
    edited and saved.*/
    /*
    face& operator [] (const unsigned int index) {
        if(l_face_index == index) return l_face;
        store_current_face();
        //prepare new return object to reference and update
        //using the data the coorisponds in the positionData
        //object
        l_face_index = index;
        l_face.a.x = positionData[index];
        l_face.a.y = positionData[index+1];
        l_face.a.z = positionData[index+2];
        l_face.b.x = positionData[index+3];
        l_face.b.y = positionData[index+4];
        l_face.b.z = positionData[index+5];
        l_face.c.x = positionData[index+6];
        l_face.c.y = positionData[index+7];
        l_face.c.z = positionData[index+8];
        return l_face;
    }*/



};

#endif // GRAPHICS_STRUCTS_H
