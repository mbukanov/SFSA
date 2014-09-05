/*
qeed
*/

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <set>

template<class T> class MatrixElement;
template<class T> class Matrix;
	
/* need c++11 */	
template<typename T>
using Elements = std::set<MatrixElement<T>*>;

struct Size 
{
	int rows; 
	int cols; 
};

template<class T> void debugMatrix(Matrix<T>);

template<class T>
class MatrixElement
{
public:
	MatrixElement(int i, int j, T value) { _i = i; _j = j; Element = value; }

	void setValue(T value) { Element = value; }

	int getRow(){ return _i; }
	int getCol(){ return _j; }
	void setRow(int row) { _i = row; }
	void setCol(int col) { _j = col; }
	T getValue(){ return Element; }

private:
	int _i;
	int _j;
	T Element;
};

template<typename T>
using Elements = std::set<MatrixElement<T>*>;

template<class T>
class Matrix
{
public:
	Matrix() {}
	Matrix(int i, int j);
	void setSize(int i, int j);
	struct Size getSize();
	void addElement(int i, int j, T value);
	void addElements(Elements<T> elmts);
	void setElements(Elements<T> elmts);
	void setElement(int i, int j, T value);
	T getElement(int i, int j);
	Elements<T> getElements();
	MatrixElement<T>* getElementPoint(int i, int j);
	void Transposition();
	Matrix<T> Reverse();
	Matrix<T> mulMatrix(Matrix m);

	bool isSquare();
	bool isIdentity();

private:
	Size size;
	Elements<T> elements;
};

template<class T>
bool Matrix<T>::isSquare()
{
	if(size.rows == size.cols)
		return true;
	return false;
}

template<class T>
bool Matrix<T>::isIdentity()
{
	if(size.rows != size.cols)
		return false;

	bool flag = false;
	int i, j;
	for(i = 1; i <= size.rows; i++)
	{
		for(j = 1; j < size.cols; j++)
		{
			if( (i != j) && (getElement(i,j) != 0) )
				return false;
			if( (i == j) && (getElement(i, j) != 1))
				return false;
		}
	}
	return true;
}

template<class T>
Size Matrix<T>::getSize()
{
	return size;
}

template<class T> 
Elements<T> Matrix<T>::getElements()
{
	return this->elements;
}

template<class T> 
void Matrix<T>::addElements(Elements<T> elmts)
{
	typename Elements<T>::iterator it;
	for(it = elmts.begin(); it != elmts.end(); it++)
	{
		this->addElement((*it)->getRow(), (*it)->getCol(), (*it)->getValue());
	}
}

template<class T>
void Matrix<T>::setElements(Elements<T> elmts)
{
	typename Elements<T>::iterator it;
	for(it = elmts.begin(); it != elmts.end(); it++)
	{
		this->setElement((*it)->getRow(), (*it)->getCol(), (*it)->getValue());
	}
}

template<class T> 
void Matrix<T>::Transposition()
{
	typename Elements<T>::iterator it;
	Matrix<T> tmp(size.rows, size.cols);
	tmp.addElements(this->elements);

	Elements<T> el = tmp.getElements();
	for(it = el.begin(); it != el.end(); it++)
	{
		this->setElement((*it)->getCol(), (*it)->getRow(), (*it)->getValue()); /* i = j; j = i; */
	}
}

template<class T> 
void Matrix<T>::setElement(int i, int j, T value)
{
	this->getElementPoint(i, j)->setValue(value);
}

template<class T> 
Matrix<T>::Matrix(int i, int j)
{
	setSize(i, j);
}

template<class T> 
void Matrix<T>::setSize(int i, int j)
{
	size.rows = i;
	size.cols = j;
	for(i = 1; i <= size.rows; i++)
	{
		for(j = 1; j <= size.cols; j++)
		{
			elements.insert(new MatrixElement<T>(i, j, 0));
		}
	}
}

template<class T> 
void Matrix<T>::addElement(int i, int j, T value)
{
	elements.insert(new MatrixElement<T>(i, j, value));
}

template<class T> 
T Matrix<T>::getElement(int i, int j)
{
	typename Elements<T>::iterator it;
	for( it = elements.begin(); it != elements.end(); it++)
	{
		if((*it)->getRow() == i && (*it)->getCol() == j)
			return (*it)->getValue();
	}

	T ok;
	return ok;
}

template<class T> 
MatrixElement<T>* Matrix<T>::getElementPoint(int i, int j)
{
	typename Elements<T>::iterator it;
	for( it = elements.begin(); it != elements.end(); it++)
	{
		if((*it)->getRow() == i && (*it)->getCol() == j)
			return (*it);
	}

	return NULL;
}

template<class T>
Matrix<T> Matrix<T>::mulMatrix(Matrix m)
{
	Matrix<T> newMatrix(this->size.rows, m.getSize().cols);
	if(size.cols != m.getSize().rows)
		return newMatrix;
	
	typename Elements<T>::iterator its;
	//Elements<T> elmts = elements; //newMatrix.getElements();
	for(its = elements.begin(); its != elements.end(); its++)
	{
		//std::cout<<"row: "<<(*its)->getRow()<<" col: "<<(*its)->getCol()<<" value: "<<(*its)->getValue()<<std::endl;
	}
	

	int i = 1;
	int j = 1;

	for(i = 1; i <= size.rows; i++)
	{
		for(j = 1; j <= m.getSize().cols; j++)
		{
			int counter = 1;
			T value = 0;
			for(counter = 1; counter <= size.cols; counter++)
			{
				value += this->getElement(i, counter)*m.getElement(counter, j);
			}
			newMatrix.setElement(i, j, value);
		}
	}
	return newMatrix;
}

// fucking A^(-1)
// don't work! Fix it!
template<class T>
Matrix<T> Matrix<T>::Reverse()
{
	Matrix<T> rok;
	if(!isSquare())
	{
		std::cout<<"NOT SQUARE"<<std::endl;
		return rok;
	}

	Matrix<T> identityMatrix(size.rows, size.cols);
	Matrix<T> newMatrix(size.rows, size.cols);
	Matrix<T> reverseMatrix(size.rows, size.cols);
	newMatrix.setElements(getElements());
	reverseMatrix.setElements(getElements());

	debugMatrix(*this);
	std::cout<<std::endl;
	debugMatrix(newMatrix);
	std::cout<<std::endl;
	debugMatrix(reverseMatrix);

	int i, j, k; // counters
	float temp;
	// create identity matrix
	for(i = 1; i <= size.rows; i++)
	{
		identityMatrix.setElement(i,i, 1);
	}

	for (k = 1; k <= size.rows; k++)
    {
    	temp = newMatrix.getElement(k, k);
 
        for (j = 1; j <= size.rows; j++)
        {
        	newMatrix.setElement(k, j, (newMatrix.getElement(k, j) / temp ) );
        	identityMatrix.setElement(k, j, (identityMatrix.getElement(k, j) / temp ) );
        }
 
        for (i = k + 1; i <= size.rows; i++)
        {
        	temp = newMatrix.getElement(i, k);
 
            for (j = 1; j <= size.rows; j++)
            {
            	newMatrix.setElement(i, j, ( newMatrix.getElement(i, j) - newMatrix.getElement(k, j) * temp) );
            	identityMatrix.setElement(i, j, ( identityMatrix.getElement(i, j) - identityMatrix.getElement(k, j) * temp) );
            }
        }
    }

    for (k = size.rows - 1; k > 0; k--)
    {
        for (i = k - 1; i > 0; i--)
        {
        	temp = newMatrix.getElement(i, k);
 
            for (j = 1; j <= size.rows; j++)
            {
            	newMatrix.setElement(i, j, newMatrix.getElement(i,j) - newMatrix.getElement(k, j) * temp);
            	identityMatrix.setElement(i, j, identityMatrix.getElement(i, j) - identityMatrix.getElement(k, j) * temp);
            }
        }
    }
  
    for (i = 1; i < size.rows; i++)
        for (j = 1; j < size.rows; j++)
        	newMatrix.setElement(i, j, identityMatrix.getElement(i, j));
 
	return newMatrix;
}

template<class T> void debugMatrix(Matrix<T> m)
{
	std::cout<<std::endl;
	typename Elements<T>::iterator it;
	Elements<T> el = m.getElements();
	for(it = el.begin(); it != el.end(); it++)
	{
		std::cout<<"DEBUG: row: "<<(*it)->getRow()<<" col: "<<(*it)->getCol()<<" value: "<<(*it)->getValue()<<std::endl;
	}
	std::cout<<std::endl;
}

/*
int main(int argc, char **argv)
{
	Matrix<float> a(3,3);
	a.setElement(1, 1, 1);
	a.setElement(1, 2, 2);
	a.setElement(1, 3, 3);
	a.setElement(2, 1, 3);
	a.setElement(2, 2, 4);
	a.setElement(2, 3, 15);
	a.setElement(3, 1, 6);
	a.setElement(3, 2, 7);
	a.setElement(3, 3, 8);
	
	Matrix<float> b(3,2);
	b.setElement(1,1,6);
	b.setElement(1,2,4);
	b.setElement(2,1,3);
	b.setElement(2,2,4);
	b.setElement(3,1,5);
	b.setElement(3,2,2);

	debugMatrix(a);
	std::cout<<std::endl;

	Matrix<float> c = a.Reverse();
	debugMatrix(c);

	
	Matrix<float> c = a.mulMatrix(b);

	Elements<float>::iterator it;
	Elements<float> el = c.getElements();
	for(it = el.begin(); it != el.end(); it++)
	{
		std::cout<<(*it)->getValue()<<std::endl;
	}
	
 
	return 0;
}
*/