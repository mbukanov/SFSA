/*
qeed.
I think it don't work. dunno.. :/
I dont know.
*/

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "matrix.h"
#include "notification/notification.h"

/*
You can choose notification type
*/
 
template<class NotificationClass>
class MOLS  : public Notification< NotificationClass > 
{
public:
    MOLS() {}
    MOLS(std::vector<double> x, std::vector<double> y);
    void defW();
    void defY();
    double defTimeLimit(double sizeLimit);

    Matrix<double> getW() { return _w; }
    Matrix<double> getX() { return _X; }   
    Matrix<double> getY() { return _Y; }


private:
    Matrix<double> _X;
    Matrix<double> _Y;
    Matrix<double> _w;

    std::vector<double> _x;
    std::vector<double> _y;
};

template<class T>
MOLS<T>::MOLS(std::vector<double> x, std::vector<double> y)
{
    _x = x;
    _y = y;
}

template<class T>
void MOLS<T>::defW()
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

template<class T>
void MOLS<T>::defY()
{
    int i;
    for(i = 1; i <= _X.getSize().rows; i++)
    {
        double value = _w.getElement(1,1) + _w.getElement(2,1)*_X.getElement(i, 2);
        _Y.addElement(i, 1, value);
    }
}


template<class T>
double MOLS<T>::defTimeLimit(double sizeLimit)
{
    double x = (sizeLimit - _w.getElement(1,1)) / _w.getElement(2,1);
    return x;
}