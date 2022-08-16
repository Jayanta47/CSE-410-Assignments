#ifndef CAMERA_H
#define CAMERA_H

#include "Vector.h"
#include<string>


void change_camera_position(Point &pos, Point L, Point R, Point U, std::string command)
{
    if (command == "forward")
    {
        pos.addVector(L);
    }
    else if (command == "backward")
    {
        pos.addVector(L, -1);
    }
    else if (command == "right")
    {
        pos.addVector(R);
    }
    else if (command == "left")
    {
        pos.addVector(R, -1);
    }
    else if (command == "up")
    {
        pos.addVector(U);
    }
    else if (command == "down")
    {
        pos.addVector(U, -1);
    }
    else return;

}

void change_camera_orientation(Point &L, Point &R, Point &U, std::string command)
{
    if (command == "rotate_left")
    {
        // rotate L anticlockwise w.r.t U
        // rotate R anticlockwise w.r.t U
        rotate_vector(L, U, ANTICLOCKWISE);
        rotate_vector(R, U, ANTICLOCKWISE);
    }
    else if (command == "rotate_right")
    {
        // rotate L clockwise w.r.t U
        // rotate R clockwise w.r.t U
        rotate_vector(L, U, CLOCKWISE);
        rotate_vector(R, U, CLOCKWISE);
    }
    else if (command == "look_up")
    {
        // rotate L anticlockwise w.r.t R
        // rotate U anticlockwise w.r.t R
        rotate_vector(L, R, ANTICLOCKWISE);
        rotate_vector(U, R, ANTICLOCKWISE);
    }
    else if (command == "look_down")
    {
        // rotate L clockwise w.r.t R
        // rotate U clockwise w.r.t R
        rotate_vector(L, R, CLOCKWISE);
        rotate_vector(U, R, CLOCKWISE);
    }
    else if (command == "tilt_clockwise")
    {
        // rotate U anticlockwise w.r.t L
        // rotate R anticlockwise w.r.t L
        rotate_vector(U, L, ANTICLOCKWISE);
        rotate_vector(R, L, ANTICLOCKWISE);
    }
    else if (command == "tilt_counter_clockwise")
    {
        // rotate U clockwise w.r.t L
        // rotate R clockwise w.r.t L
        rotate_vector(U, L, CLOCKWISE);
        rotate_vector(R, L, CLOCKWISE);
    }
    else 
        return;
}


#endif 