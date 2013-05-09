#pragma once
#include "math_global.h"
#include<cassert>
#include<vector>
#include<valarray>
#include<string>
#include<utility>
extern "C++" class Matrix;
using namespace std;

class MATHSHARED_EXPORT Matrix
{
public:

   typedef std::vector<pair<double, valarray<double>>> ValueVectorList;

public:
/**
 * @name Matrix - Empty constructor
 * @return Matrix
 */
   Matrix();
/**
 * @name Matrix - Constructor with given height and width
 * @param height -  also the number of rows
 * @param width - also the number of cols
 * @return Matrix
 */
   Matrix(size_t height, size_t width);
/**
 * @name Matrix - Constructor with an initializer list, in which
 * elements are arranged by row-major order(same with C/C++
 * principal). If the number of list is less than the actual size of actual
 * matirx, zeros will be added at the end of matrix
 * @param width - specify width of the matrix represented by the
 * initializer list.
 * @param list -  list 
 * @return Matrix
 */
 //  Matrix(size_t width, initializer_list<double> list);
/**
 * @name Matrix - Copy constructor
 * @param other -  other 
 * @return Matrix
 */
   Matrix(const Matrix &other);
/**
 * @name Matrix - Move constructor
 * @param other - rvalue
 * @return Matrix
 */
   Matrix(Matrix &&other) noexcept;
/**
 * @name Matrix - Construct a sub-matrix from other super-matrix.
 * @param other -  the super-matrix
 * @param row -  row
 * @param col -  col 
 * @return Matrix
 */
   Matrix(const Matrix &other, const slice &row, const slice &col);
/**
 * @name Matrix - Destructor
 * @return void
 */
   ~Matrix();
public://operator overload
/**
 * @name () - get the reference of a element in the matrix
 * @param row -  row 
 * @param col -  col 
 * @return double
 */
   inline double& operator()(size_t row, size_t col);
/**
 * @name () - get the copy of a element in the matrix
 * @param row -  row 
 * @param col -  col 
 * @return double
 */
   inline double operator()(size_t row, size_t col) const;
/**
 * @name () - get the reference of a sub-matrix from this matrix
 * @param row -  row 
 * @param col -  col 
 * @return gslice_array
 */
   gslice_array<double> operator()(const slice &row, const slice &col); 
/**
 * @name = - assignment
 * @param other -  other 
 * @return Matrix
 */
   Matrix& operator=(const Matrix &other);
/**
 * @name = - assignment
 * @param other -  other 
 * @return Matrix
 */
   Matrix& operator=(Matrix &&other) noexcept;
/**
 * @name = - the all element of this matrix is assigned to the same value
 * @param value - the same value
 * @return Matrix
 */
   Matrix& operator=(double value);
/**
 * @name * - the real matrix production
 * @param other - the right matrix in this matrix production. A assert
 * will test whether the height of the right matrix is equal to the
 * width of this matrix
 */
   Matrix operator*(const Matrix &other) const;

   Matrix& operator /=(double value);

public://public basic operation
/**
 * @name row - get the reference of a row in this matrix
 * @param indexOfRow -  index Of Row 
 * @return slice_array
 */
   slice_array<double> row(size_t indexOfRow);
/**
 * @name row - get the copy of a row in this matrix
 * @param indexOfRow -  index Of Row 
 * @return valarray
 */
   valarray<double> row(size_t indexOfRow) const;
/**
 * @name col - get the reference of a column in this matrix
 * @param indexOfCol -  index Of Col 
 * @return slice_array
 */
   slice_array<double> col(size_t indexOfCol);
/**
 * @name col - get the copy of a column in this matrix
 * @param indexOfCol -  index Of Col 
 * @return valarray
 */
   valarray<double> col(size_t indexOfCol) const;
/**
 * @name GetWidth - get the width(also the number of columns) of this matrix
 * @return size_t
 */
   inline size_t GetWidth() const;
/**
 * @name GetHeight - get the height(also the number of rows) of this matrix
 * @return size_t
 */
   inline size_t GetHeight() const;
/**
 * @name Print - Prints out the element of this matrix through the
 * standard console
 * @param name - Name of this matrix showed in the console
 * @return void
 */
   void Print(string name) const;
/**
 * @name IsSquared - test whether this matrix is square
 * @return bool
 */
   bool IsSquared() const;
/**
 * @name IsSymmetric - test whether this matrix is symmetric
 * @return bool
 */
   bool IsSymmetric() const;
/**
 * @name IsSameSizeWith - test whether a given matrix is same size
 * with this matrix
 * @param other -  other 
 * @return bool
 */
   bool IsSameSizeWith(const Matrix &other) const;
public://advanced functions
/**
 * @name FilterByValid - symmetric filtering
 * @param window -  window 
 * @return Matrix - filtered result
 */
   Matrix FilterByValid(const Matrix &window);
/**
 * @name GetEigenvalueList - Get all eigenvalue with ascending
 * order. The implementation is based on DSYEVR in lapack
 * @return valarray
 */
   valarray<double> GetEigenvalueList() ;

   ValueVectorList GetEigenValueAndVector() const;
/**
 * @name DrawSpectralDensity - draw the spectral density of
 * eigenvalues in this matrix
 * @param slide - the width of slice in this drawing
 * @return string - the path of generated image
 */
   string DrawSpectralDensity(double slide);
private://real data
   size_t width;
   size_t height;
   valarray<double> vdata;
};

double& Matrix::operator()(size_t row, size_t col)
{
   return vdata[col * height + row];
}

double Matrix::operator()(size_t row, size_t col) const
{
   return vdata[col * height + row];
}

size_t Matrix::GetWidth() const
{
   return width;
}

size_t Matrix::GetHeight() const
{
   return height;
}
