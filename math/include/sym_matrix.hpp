#pragma once

#include "sqr_matrix.hpp"


/** SymMatrix
 *
 * The symmetric matrix, which storage is based on upper-triangle
 * forms, not band forms. Thus, the parent's SetElement and GetElement functions need
 * to be overload.
 */
/*
class SymMatrix : public SqrMatrix
{
public:
   typedef std::shared_ptr<SymMatrix>  pMatrix;
   typedef std::pair<double, pVector>  ValueVectorPair;
   typedef std::vector<ValueVectorPair> ValueVectorList;
   typedef std::shared_ptr<ValueVectorList> pValueVectorList;
public:

   SymMatrix();

   SymMatrix(size_t size);

   SymMatrix(const SymMatrix &other);

   ~SymMatrix();

public:
*/
/**
 * @name GetEigenvalueList - Get all eigenvalue. The implementation is
 * based on DSYEVR in lapack
 * @return pVector - return the list of eigenvalues
 */
/*
   pVector GetEigenvalueList();

   pValueVectorList GetEigenValueAndVector();
   
   pVector GetEigenvalueRange(int indexOfLow, int indexOfHigh);
public:
   void Print();
   
   virtual inline void SetElement(size_t row, size_t col, double value);

   virtual inline void SetElement(Point point, double value);
   
   virtual inline double GetElement(size_t row, size_t col);

   virtual inline double GetElement(Point point);

   virtual inline pVector GetRow(size_t indexOfRow);

   virtual inline pVector GetColumn(size_t indexOfCol);

   virtual inline void SetRow(size_t indexOfRow, const pVector source);
   
   virtual inline void SetColumn(size_t indexOfCol, const pVector source);

   virtual inline void SetSubMatrix(Point point_begin, const pMatrix source);
   
   virtual inline Matrix::pMatrix GetSubMatrix(Point point_begin, Point point_end);
};

void SymMatrix::SetElement(size_t row, size_t col, double value)
{
   assert(row < GetSize() && col < GetSize());

   Matrix::SetElement(row, col, value);
   Matrix::SetElement(col, row, value);
}

void SymMatrix::SetElement(Point point, double value)
{
   assert(point.first < GetSize() && point.second < GetSize());

   Matrix::SetElement(point.second, point.first, value);
   Matrix::SetElement(point.first, point.second, value);
}


double SymMatrix::GetElement(size_t row, size_t col)
{
   assert(row < GetSize() && col < GetSize());

   if(row > col)
   {//change to upper triangle
      return Matrix::GetElement(col, row);
   }
   else
   {
      return Matrix::GetElement(row, col);
   }
}

double SymMatrix::GetElement(Point point)
{
   assert(point.first < GetSize() && point.second < GetSize());

   if(point.first > point.second)
   {//change to upper triangle
      return Matrix::GetElement(point.second, point.first);
   }
   else
   {
      return Matrix::GetElement(point.first, point.second);
   }
}

Matrix::pVector SymMatrix::GetRow(size_t indexOfRow)
{
   assert(indexOfRow < GetSize());
   
   pVector result(new Vector(GetSize()));

   for(size_t col = 0; col < GetSize(); col++)
   {
      (*result)[col] = GetElement(indexOfRow, col);
   }
   return result;
}

void SymMatrix::SetRow(size_t indexOfRow, const pVector source)
{
   assert(indexOfRow < GetSize());
   assert(source->size() == GetSize());

   for(size_t col = 0; col < GetSize(); col++)
   {
      SetElement(indexOfRow, col,  (*source)[col]);
   }
}

Matrix::pVector SymMatrix::GetColumn(size_t indexOfCol)
{
   assert(indexOfCol < GetSize());
   pVector result(new Vector(GetSize()));

   for(size_t row = 0; row < GetSize(); row++)
   {
      (*result)[row] = GetElement(row, indexOfCol);
   }
   return result;
}

void SymMatrix::SetColumn(size_t indexOfCol, const pVector source)
{
   assert(indexOfCol < GetSize());
   assert(source->size() == GetSize());

   for(size_t row = 0; row < GetSize(); row++)
   {
      SetElement(row, indexOfCol, (*source)[row]);
   }
}

void SymMatrix::SetSubMatrix(Point point_begin, const pMatrix source)
{
   assert(point_begin.first + source->GetHeight() <= GetHeight());
   assert(point_begin.second + source->GetWidth() <= GetWidth());
   
   for(size_t row = 0; row < source->GetHeight(); row++)
   {
      for(size_t col = 0; col < source->GetWidth(); col++)
      {
	 SetElement(row + point_begin.first, col + point_begin.second, 
		    source->GetElement(row, col));
      }
   }
}

Matrix::pMatrix SymMatrix::GetSubMatrix(Point point_begin, Point point_end)
{
   assert(point_end.first > point_begin.first);
   assert(point_end.second > point_begin.first);
   assert(point_end.first <= GetHeight());
   assert(point_end.second <= GetWidth());

   Matrix::pMatrix target(new Matrix(point_end.second - point_begin.second,
				     point_end.first - point_begin.first));
   
   for(size_t row = 0; row < target->GetHeight(); row++)
   {
      for(size_t col = 0; col < target->GetWidth(); col++)
      {
	 target->SetElement(row, col, 
			    GetElement(row + point_begin.first,
				       row + point_begin.second));
      }
   }
   return target;
}
*/
