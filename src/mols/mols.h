/*
qeed.
I think it don't work. dunno.. :/
I dont know.
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include "../matrix.h"
#include "../notification/notification.h"

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
 