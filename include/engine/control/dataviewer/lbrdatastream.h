#ifndef LBR_GRAPH_DATA_STREAM_H
#define LBR_GRAPH_DATA_STREAM_H

#include "../../graphics/drawable.h"
#include <limits>

class lbrDataStream : public drawable {
private:

	int rows;
	unsigned short channels;
	double* xData;
	double* yData;

	double2 minData;
	double2 maxData;
	double2 scale;
	double2 bounds;

public:


	lbrDataStream(int rows,int channels) :
			rows(rows),
			channels(channels),
			xData(new double[rows]),
			yData(new double[rows*channels])
			{

		double minLimit = numeric_limits<double>::min();
		double maxLimit = numeric_limits<double>::max();
		minData = double2(maxLimit,maxLimit);
        maxData = double2(minLimit,minLimit);
	}


	void draw(){
		glPushMatrix();
		glScaled(scale.x,scale.y,1);
		for(int c=0;c<channels;c++){
			glColor3f(1,1,0);
			glBegin(GL_LINE_STRIP);
			for(int i=0;i<rows;i++){
				glVertex3d((double)xData[i],yData[c*rows+i],0);
			}
			glEnd();
		}
		glPopMatrix();
		glColor3f(1,1,1);
		glBegin(GL_LINE_LOOP);
			glVertex3d(0,bounds.y*-.5,0);
			glVertex3d(0,bounds.y*.5,0);
			glVertex3d(bounds.x,bounds.y*.5,0);
			glVertex3d(bounds.x,bounds.y*-.5,0);
		glEnd();

/*		double pos[3];
		pos[0] = 0; pos[1] = 0; pos[2] = 0;
		double acc[3];
		acc[0] = 0; acc[1] = 0; acc[2] = 0;
		double vel[3];
		vel[0] = 0; vel[1] = 0; vel[2] = 0;



		glPointSize(3);
		glBegin(GL_POINTS);
		for(int r=0;r<rows;r++){
			glColor3f(r-.3,r+.3,r);
			for(int c=0;c<channels;c++){
				acc[c] = yData[channels*r+c]-yData[c];
				vel[c] = vel[c] + acc[c]*.22;
				pos[c] = pos[c] + vel[c]*.22;
			}
			glVertex3d(pos[0]*.01f,pos[1]*.01f,pos[2]*.01f);
		}
		glEnd();
*/

	}

	void setXData(double* data){
		for(int i=0;i<rows;i++){
			xData[i] = data[i];
			if(minData.x > data[i]) minData.x = data[i];
			if(maxData.x < data[i]) maxData.x = data[i];
		}
	}

	void setYData(double* data,int channelIndex){
		if(!data){
			cout << "could not set the data for the stream\n";
			return;
		}
		for(int i=0;i<rows;i++){
			yData[channelIndex*rows + i] = data[i];
			if(minData.y > data[i]) minData.y = data[i];
			if(maxData.y < data[i]) maxData.y = data[i];
		}
	}

	void fitToBounds(double2 bounds){
		scale = (this->bounds = bounds) / (maxData-minData);
	}

};

#endif // LBR_GRAPH_DATA_STREAM_H
