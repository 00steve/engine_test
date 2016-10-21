#ifndef DEPTH_CLICK_H
#define DEPTH_CLICK_H

#include "../core/double3.h"
#include "../graphics/gl_util.h"
#include "../core/camera.h"

struct depth_click{

	double3 	origin;
	double3 	direction;
	bool 		active;


	static depth_click* get(int2 mousePosition,camera* cam,int2 viewOffset,int2 viewSize,float aspectRatio){

		double matModelView[16], matProjection[16];
		double3 m_start,m_end;
		int viewport[4];
		viewport[0] = viewOffset.x;
		viewport[1] = viewOffset.y;
		viewport[2] = viewSize.x;
		viewport[3] = viewSize.y;

		glPushMatrix();
			cam->translateView(aspectRatio);
			// get matrix and viewport:
			glGetDoublev( GL_MODELVIEW_MATRIX, matModelView );
			glGetDoublev( GL_PROJECTION_MATRIX, matProjection );
			//glGetIntegerv( GL_VIEWPORT, viewport );
		glPopMatrix();
		// window pos of mouse, Y is inverted on Windows
		double winX = (double)mousePosition.x;
		double winY = viewport[3] - (double)mousePosition.y;

		if(winX < 0){
			winX = 0;
		} else if(winX > viewport[2]){
			winX = viewport[2];
		}
		if(winY < 0){
			winY = 0;
		} else if(winY > viewport[3]){
			winY = viewport[3];
		}

		// get point on the 'near' plane (third param is set to 0.0)
		gluUnProject(winX, winY, 0.0, matModelView, matProjection,
				 viewport, &m_start.x, &m_start.y, &m_start.z);

		// get point on the 'far' plane (third param is set to 1.0)
		gluUnProject(winX, winY, 1.0, matModelView, matProjection,
				 viewport, &m_end.x, &m_end.y, &m_end.z);


		depth_click *dc = new depth_click();
		dc->origin = m_start;
		dc->direction = (m_end-m_start).normalize();
		return dc;
	}


};




#endif // 3D_CLICK_H
