#ifndef BASEMOLS_H
#define BASEMOLS_H

class BaseMOLS
{
public:
    virtual void defW() = 0;
    virtual void defY() = 0;
    virtual double defTimeLimit(double sizeLimit) = 0;

    virtual Matrix<double> getW() = 0;
    virtual Matrix<double> getX() = 0;
    virtual Matrix<double> getY() = 0;
	virtual void Alert() = 0;
private:
};


#endif