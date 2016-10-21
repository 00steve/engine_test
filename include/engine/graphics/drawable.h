#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "../core/node.h"
#include "../graphics/mesh.h"

class drawable : public node {
private:
	list<drawable*> drawableChildren;
	drawable* drawableParent;

	/*if the node is enabled or not. This can be used in any way that
	the programmer would like, but by default any disabled nodes are
	not updated or drawn*/
	bool should_draw;

	static double3 currentCameraPosition;


public:

	drawable() :
		drawableChildren(list<drawable*>()),
		drawableParent(NULL){
	}

	~drawable(){
		#if DEBUG_DELETES
			cout << " drawable >";
		#endif
		if(drawableParent){
			drawableParent->removeDrawable(this);
		}
	}

	virtual void draw() = 0;


	drawable* addDrawable(drawable* newDrawable){
		this->addChild((node*)newDrawable);
		newDrawable->drawableParent = this;
		return drawableChildren.push(newDrawable);
	}

	drawable* removeDrawable(drawable* oldDrawable){
		this->removeChild((node*)oldDrawable);
		oldDrawable->drawableParent = NULL;
		drawableChildren.remove(oldDrawable);
		return oldDrawable;
	}

	list<drawable*>& getDrawables(){ return drawableChildren; }

	drawable* getDrawableChild(int index){
		return drawableChildren[index];
	}

	bool handleMessage(node_message* m){
		switch(m->code){
		case MESSAGE_ADD_DRAWABLE:
			addDrawable((drawable*)m->data);
			return true;
		case MESSAGE_REMOVE_DRAWABLE:
			removeDrawable((drawable*)m->data);
			return true;
		}
		return node::handleMessage(m);
	}



	/*a function to return a reference to any camera associated
	with a drawable. For a player, this could be a 3rd person
	cam, fps cam, on gun cam, following cam, etc. It is up to
	the drawable to add its own cameras for use.*/
	//camera* getCameraReference(const int camId){
	//}

	static void setCurrentCameraPosition(double3 cameraPosition){
		currentCameraPosition = cameraPosition;
	}

	static double3 getCurrentCameraPosition(){
		return currentCameraPosition;
	}

};


double3 drawable::currentCameraPosition;


#endif // DRAWABLE
