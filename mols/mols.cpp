#include "mols.h"

MOLS::MOLS(std::vector<float> x, std::vector<float> y)
{
    _x = x;
    _y = y;
}

void MOLS::defW()
{
    Matrix<float> A(_x.size(), 3);
    Matrix<float> At(3, _x.size());
    int i;

    // Create Matrix A
    for(i = 1; i < _x.size(); i++)
    {
        A.setElement(i, 1, 1);
        A.setElement(i, 2, _x[i-1]);
        A.setElement(i, 3, _x[i-1]*_x[i-1]);
    }

    //_A.addElements(A.getElements());
    _X = A;

    // At is transposition A
    //At.setElements((A.Transposition()).getElements());
    At = A.Transposition();

    // def W
    _w = ((At.mulMatrix(A)).Reverse()).mulMatrix(At.mulVector(_y)); // lisp? 
    debugMatrix(_w);
}

void MOLS::defY()
{
    int i;
    for(i = 1; i <= _X.getSize().rows; i++)
    {
        float value = _w.getElement(3, 1) * _X.getElement(i, 3) + _w.getElement(2, 1) * _X.getElement(i, 2) + _w.getElement(1, 1);
        _Y.addElement(i, 1, value);
    }
    //_Y = _X.mulMatrix(_w);
}
