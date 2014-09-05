MOLS::MOLS(std::vector<float> x, std::vector<float> y)
{
   	this->_x = x;
   	this->_y = y;
}

void MOLS::SetXValues(std::vector<float> x)
{
  	this->_x = x;
}

void MOLS::SetYValues(std::vector<float> y)
{
  	this->_y = y;
}

bool MOLS::defK()
{
    if(this->_x.empty() || this->_y.empty() || (this->_x.size() != this->_y.size()))
    	return false;

    int n = this->_x.size();
	float sum_xy = 0;
    float sum_x = 0;
	float sum_y = 0;
	float sum_x2 = 0;
	int i;
	for(i = 0; i < n; i++)
	{
	    sum_x += _x[i];
	    sum_y += _y[i];
	    sum_xy += _x[i]*_y[i];
	    sum_x2 += _x[i]*_x[i];
    }
	    
        printf("n = %d\n", n);
	    printf("sum_x = %f\n", sum_x);
	    printf("sum_y = %f\n", sum_y);
	    printf("sum_xy = %f\n", sum_xy);
	    printf("sum_x2 = %f\n", sum_x2);
	    
        printf("(n*sum_xy - sum_x*sum_y) = %f\n", ((float)n*sum_xy - sum_x*sum_y));
        printf("(n*sum_x2 - sum_x*sum_x) = %f\n", ((float)n*sum_x2 - sum_x*sum_x));
	    
	this->_k = (n*sum_xy - sum_x*sum_y) / (n*sum_x2 - sum_x*sum_x);
	return true;
}

bool MOLS::defB()
{
    if(this->_x.empty() || this->_y.empty() || (this->_x.size() != this->_y.size()))
        return false;

    int n = _x.size();
	int i = 0;
	float sum_x = 0;
	float sum_y = 0;
	for(i = 0 ; i < n; i++)
	{
	   sum_x += _x[i];
	   sum_y += _y[i];
	}

    this->_b = (sum_y - this->_k * sum_x) / n;

	return true;
}

bool MOLS::defY(float x)
{
    this->_newY = _k*x+_b;
    return true;
}

bool MOLS::defX(float y)
{
    this->_newX = (y - this->_b) / this->_k;
    return true;
}

bool MOLS::defFullY(float x)
{
    if(!defK()) return false;
    if(!defB()) return false;
    if(!defY(x)) return false;
    return true;
}