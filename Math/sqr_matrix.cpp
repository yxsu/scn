#include "sqr_matrix.hpp"
/*
SqrMatrix::SqrMatrix()
   :Matrix()
{
}

SqrMatrix::SqrMatrix(size_t size)
{
  Create(size);
}

SqrMatrix::SqrMatrix(const SqrMatrix &other)
{
  Create(other);
}

SqrMatrix::~SqrMatrix()
{
}

void SqrMatrix::Create(size_t size)
{
  this->size = size;
  Matrix::Create(size, size);
}

void SqrMatrix::Create(const SqrMatrix &other)
{
   this->size = other.GetSize();
   Matrix::Create(other);
}

Matrix::pVector SqrMatrix::GetDiagonal()
{
   pVector result(new Vector(GetSize()));

   for(size_t i = 0; i < GetSize(); i++)
   {
      (*result)[i] = GetElement(i, i);
   }
   return result;
}

void SqrMatrix::SetDiagonal(const pVector source)
{
   assert(source->size() == GetSize());

   for(size_t i = 0; i < GetSize(); i++)
   {
      SetElement(i, i, (*source)[i]);
   }
}
*/
