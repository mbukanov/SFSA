#include "mols.h"
#include <cmath>

MOLS::MOLS(std::vector<double> x, std::vector<double> y)
{
    _x = x;
    _y = y;
}

void MOLS::defW()
{
    Matrix<double> A(_x.size(), 2);
    Matrix<double> At(2, _x.size());
    int i;

    // Create Matrix A
    for(i = 1; i < _x.size(); i++)
    {
        A.setElement(i, 1, 1);
        A.setElement(i, 2, _x[i-1]);
    }

    _X = A;

    // At is transposition A
    At = A.Transposition();

    // def W
    _w = ((At.mulMatrix(A)).Reverse()).mulMatrix(At.mulVector(_y)); // lisp? 
}

void MOLS::defY()
{
    int i;
    for(i = 1; i <= _X.getSize().rows; i++)
    {
        //double value = _w.getElement(3, 1) * _X.getElement(i, 3) + _w.getElement(2, 1) * _X.getElement(i, 2) + _w.getElement(1, 1);
        double value = _w.getElement(1,1) + _w.getElement(2,1)*_X.getElement(i, 2);
        _Y.addElement(i, 1, value);
    }

    //_Y = _X.mulMatrix(_w);
}


// fix this
double MOLS::defTimeLimit(double sizeLimit)
{
    double x = (sizeLimit - _w.getElement(1,1)) / _w.getElement(2,1);
    return x;

    /*
    // desciminant
    double D = _w.getElement(2,1) * _w.getElement(2,1) - 4 * (_w.getElement(3,1) * (_w.getElement(1,1) - sizeLimit));
    std::cout<<"D: "<<D<<std::endl;
    std::cout<<"sqrt: "<<sqrt(D)<<std::endl;
    double x1 = (-_w.getElement(2,1) + sqrt(D)) / (2 * _w.getElement(3,1));
    double x2 = (-_w.getElement(2,1) - sqrt(D)) / (2 * _w.getElement(3,1));

    std::cout<<"X1: "<<x1<<std::endl;
    if(x2 > x1)
        return x2;
    else
        return x1;
*/
}