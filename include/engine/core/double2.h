
#ifndef DOUBLE2_H
#define DOUBLE2_H
#include <math.h>

class double2 {
    public:
        double x,y;

        double2(){x=0;y=0;}
        double2(double newX,double newY){x=newX;y=newY;}

        double2 normalize() {
            double length = sqrt(x*x + y*y);
            if(length == 0) return double2(0,0);
            return double2(x/length,y/length);
        }

        double length(){
            return sqrt(x*x + y*y);
        }

        /*assignment operators*/
        double2 operator += (const double2 &f){x += f.x;y += f.y;return *this;}
        double2 operator -= (const double2 &f){x -= f.x;y -= f.y;return *this;}

		double2 operator *= (const double &f){x *= f;y *= f;return *this;}

        /*simple arithmatic operators*/
        double2 operator * (const double2 &i){return double2(x*i.x,y*i.y);}
        double2 operator / (const double2 &i){return double2(x/i.x,y/i.y);}
        double2 operator + (const double2 &i){return double2(x+i.x,y+i.y);}
        double2 operator - (const double2 &i){return double2(x-i.x,y-i.y);}

        double2 operator * (const double &f){return double2(x*f,y*f);}

        /*comparison operators*/
        bool operator < (const double &i){if(x*x+y*y < i*i)return true; return false;}
        bool operator > (const double &i){if(x*x+y*y > i*i)return true; return false;}
};
#endif

