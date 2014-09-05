/*
    It's don't work. Need use linean regression
*/

#include <stdio.h>
#include <iostream>
#include <vector>

    class MOLS
    {
    public:
    	MOLS(){};
    	MOLS(std::vector<float> x, std::vector<float> y);
    	void SetXValues(std::vector<float> x);
    	void SetYValues(std::vector<float> y);

    	/* y = kx + b */
    	bool defK();
    	bool defB();
    	bool defY(float x);
    	bool defX(float y);
    	bool defFullY(float x);

    	float getK() { return this->_k; }
    	float getB() { return this->_b; }
    	float getY() { return this->_newY; }
    	float getX() { return this->_newX; }

    private:
    	std::vector<float> _x;
    	std::vector<float> _y;
    	float _k;
    	float _b;
    	float _newY;
    	float _newX;
    };

