#ifndef INT2_H
#define INT2_H


class int2
{
    public:
        int x,y;

        int2(){x=0;y=0;}
        int2(int newX,int newY){x=newX;y=newY;}

        /*simple arithmatic operators*/
        int2 operator + (const int2 &i){return int2(x+i.x,y+i.y);}
        int2 operator - (const int2 &i){return int2(x-i.x,y-i.y);}

        /*comparison operators*/
        bool operator < (const int &i){if(x*x+y*y < i*i)return true; return false;}
        bool operator > (const int &i){if(x*x+y*y > i*i)return true; return false;}

};
#endif
