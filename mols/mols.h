/*
qeed.
I think it work. dunno.. :/
I dont know
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include "matrix.h"

class MOLS
{
public:
    MOLS() {}
    MOLS(std::vector<float> x, std::vector<float> y);
    void defW();
    void defY();
    float defTimeLimit(float sizeLimit);

    Matrix<float> getW() { return _w; }
    Matrix<float> getX() { return _X; }   
    Matrix<float> getY() { return _Y; }


private:
    Matrix<float> _X;
    Matrix<float> _Y;
    Matrix<float> _w;

    std::vector<float> _x;
    std::vector<float> _y;
};
 