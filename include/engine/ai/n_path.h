#ifndef N_PATH_H
#define N_PATH_H

/*the n-path class utilizes the ODE api to generate
a 3d grid representing what areas things can pass though.
It will probably take a while to run, because the general thought
is that a ray will be drawn down each axis of a trimesh at set
intervals. Wherever there is a collision, the block is set to solid
which means that a player can't move through it.

Right now, this is only also set up to work with things that move
on the ground. If you are making something that flies, you will have
to do something else.*/


#include "../physics/ode.h"
#include "../core/physical.h"

class nPath{
private:

	/*store the resolution of what is searched*/
	double res;

	double3 area_min;
	double3 area_max;

	bool *grid;
	int grid_width;
	int grid_height;
	int grid_depth;
	int cell_count;


	void buildGrid(){
		grid_width 	= ceil((area_max.x-area_min.x)/res);
		grid_height = ceil((area_max.y-area_min.y)/res);
		grid_depth 	= ceil((area_max.z-area_min.z)/res);

		cell_count = grid_width * grid_height * grid_depth;

		grid = new bool[cell_count]();

	}

	bool &gridCell(int x,int y,int z){
		return grid[x*grid_width*grid_height + y*grid_height + grid_depth];
	}

	/*return the cell that a x,y,z coordinate exists in*/
	bool gridCell(double3 offset){
        return false;
	}

public:


    nPath(double3 area_min,double3 area_max,double resolution) :
    	res(resolution),
    	area_min(area_min),
    	area_max(area_max),
		grid(NULL),
		grid_width(0),
		grid_height(0),
		grid_depth(0) {
		buildGrid();
		//reset();
    }

	//bool


	//void reset(){
		//int i=cell_count;
		//while(i-->0){
		//	grid[i] = false;
		//}
	//}


};




#endif // N_PATH_H
