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
	Size() { rows = 0; cols = 0; }
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
	Matrix<T> Transposition();
	Matrix<T> Reverse();
	Matrix<T> mulMatrix(Matrix m);
	Matrix<T> mulVector(std::vector<T> v);
	T Determinant();
	int Search(T item, bool match, unsigned &uI, unsigned &uJ, unsigned starti, unsigned startj);
	void swapRows(int row1, int rows2);
	void swapCols(int col1, int col2);

	/*
	void operator =(Matrix<T> b)
	{
		setSize(b.getSize().rows, b.getSize().cols);
		setElements(b.getElements());
	}
	*/

	void addRow();
	void addCol();

	bool isSquare();
	bool isIdentity();

private:
	Size size;
	Elements<T> elements;
};

template<class T>
int Matrix<T>::Search(T what, bool match, unsigned &uI, unsigned &uJ, unsigned starti, unsigned startj)
{
	//width = rows
	// height = cols
	if( (starti >= size.cols) || (startj >= size.rows) ) return 0;
	for (unsigned i = starti; i < size.cols; i++)
		for (unsigned j = startj; j < size.rows; j++)
			if (match == true) 
			{
				if (getElement(i, i) == what) 
				{
					uI = i;
					uJ = j;
					return 1;
				}
			}else
				if (getElement(i, j) != what) 
				{
					uI = i;
					uJ = j;
					return 1;
				}
	return 0;
}

template<class T>
void Matrix<T>::addRow()
{
	size.rows++;
}

template<class T>
void Matrix<T>::addCol()
{
	size.cols++;
}

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
	return this->size;
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
Matrix<T> Matrix<T>::Transposition()
{
	typename Elements<T>::iterator it;
	Matrix<T> srcMatrix(size.rows, size.cols);
	Matrix<T> newMatrix(size.cols, size.rows);

	srcMatrix.setElements(getElements());

	Elements<T> el = getElements();
	for(it = el.begin(); it != el.end(); it++)
	{
		newMatrix.setElement((*it)->getCol(), (*it)->getRow(), (*it)->getValue()); /* i = j; j = i; */
		//this->setElement((*it)->getCol(), (*it)->getRow(), (*it)->getValue()); /* i = j; j = i; */
	}
	return newMatrix;
}

template<class T>
void Matrix<T>::swapRows(int row1, int row2)
{
	T temp = 0;
	int col;
	for( col = 1; col <= size.cols; col++ )
	{
		temp = getElement(row1, col);
		setElement( row1, col, getElement(row2, col) );
		setElement( row2, col, temp);
	}
}

template<class T>
void Matrix<T>::swapCols(int col1, int col2)
{
	T temp = 0;
	int row;
	for( row = 1; row <= size.rows; row++)
	{
		temp = getElement(row, col1);
		setElement( row, col1, getElement(row, col2) );
		setElement( row, col2, temp );
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
	if(size.rows < i)
		size.rows++;
	if(size.cols < j)
		size.cols++;
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

template<class T>
Matrix<T> Matrix<T>::mulVector(std::vector<T> v)
{
	// 1 column , v.size() rows
	Matrix<T> newMatrix(v.size(), 1);
	Matrix<T> srcMatrix(size.rows, size.cols);
	srcMatrix.setElements(getElements());

	unsigned i;
	for(i = 1; i <= v.size(); i++)
	{
		newMatrix.setElement(i, 1, v[i-1]);
	}

	return srcMatrix.mulMatrix(newMatrix);
}

template<class T>
Matrix<T> Matrix<T>::Reverse()
{
	Matrix<T> rok;
	if(!isSquare())
	{
		std::cout<<"NOT SQUARE"<<std::endl;
		return rok;
	}

	Matrix<T> srcMatrix(size.rows, size.cols);
	Matrix<T> dstMatrix(size.rows, size.cols);
	srcMatrix.setElements(getElements());

	for(int i = 1; i <= size.rows; i++)
	{
		dstMatrix.setElement(i,i, 1);
	}

	long double tmp;
	unsigned i, j;
	unsigned x, y;

	for( unsigned i = 1; i <= size.rows; i++ )
	{
		if(srcMatrix.getElement(i, i) == 0)		
		{
			if(!srcMatrix.Search(0, false, y, x, i, i)) break;
			if( i != y )
			{
				srcMatrix.swapRows(i, y);
				dstMatrix.swapRows(i, y);
			}
			if(i != x)
			{
				srcMatrix.swapCols(i, x);
				dstMatrix.swapCols(i, x);
			}
		}
		tmp = srcMatrix.getElement(i, i);
		for(x = 1; x <= size.cols; x++)
		{
			srcMatrix.setElement(i, x, srcMatrix.getElement(i, x) / tmp);
			dstMatrix.setElement(i, x, dstMatrix.getElement(i, x) / tmp);
		}

		for(y = 1; y <= size.rows; y++)
		{
			if(y == i) continue;
			tmp = srcMatrix.getElement(y, i);
			for( x = 1; x <= size.cols; x++)
			{
				srcMatrix.setElement(y, x, (srcMatrix.getElement(y, x) - (srcMatrix.getElement(i, x) * tmp)));
				dstMatrix.setElement(y, x, (dstMatrix.getElement(y, x) - (dstMatrix.getElement(i, x) * tmp)));
			}
		}
	}

	return dstMatrix;
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