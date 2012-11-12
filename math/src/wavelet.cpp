#include "../include/wavelet.hpp"
/*
using namespace std;

Wavelet::Wavelet()
   :h1(new Vector()), g1(new Vector()), h2(new Vector()), g2(new Vector())
{
   max_level = 0;
   nc = 0;
   offset = 0;
}

Wavelet::~Wavelet()
{
}

void Wavelet::DWT_step(size_t length, bool isForward)
{
   vector<double> buffer(length, 0.0);

   size_t nmod = this->nc * length;
   size_t n1 = length - 1;
   size_t nh = length >> 1;
   nmod -= offset;
   
   if(isForward)
   {
      size_t index = 0;
      for(size_t i = 0; i < length; i += 2)
      {
	 double h = 0, g = 0;

	 for(size_t j = 0; j < nc; j++)
	 {
	    h += (*h1)[j] * (*process_data)[n1 & (i + nmod + j)];
	    g += (*g1)[j] * (*process_data)[n1 & (i + nmod + j)];
	 }
	 buffer[index] += h;
	 buffer[index++ + nh] += g;
      }
   }
   else
   {//inverse direction
      double ai, ai1;
      for(size_t i = 0; i < length; i += 2)
      {
	 ai = (*process_data)[i];
	 ai1 = (*process_data)[i + nh];
	 
	 for(size_t j = 0; j < nc; j++)
	 {
	    buffer[n1 & (i + nmod + j)] += 
	       (*h2)[j] * ai + (*g2)[j] * ai1;
	 }
      }
   }
   //copy
   for(size_t i = 0; i < length; i++)
   {
      (*process_data)[i] = buffer[i];
   }
}

void Wavelet::Transform(bool isForward)
{
   if(process_data->size() < 2)
      return;
   //test whether the length is a power of 2
   if(!IsPowerOf2(process_data->size()))
      return;

   //run
   if(isForward)
   {//forward
      for(size_t i = process_data->size(); i >= 2; i >>=1)
      {
	 DWT_step(i, true);
      }
   }
   else
   {//inverse
      for(size_t i = 2; i <= process_data->size(); i <<= 1)
      {
	 DWT_step(i, false);
      }
   }
}

bool Wavelet::IsPowerOf2(size_t number)
{
   size_t k = 1;
   max_level = -1;
   while(k < number)
   {
      k *= 2;
      max_level++;
   }
   if(k != number)
   {
      max_level = 0;
      return false;
   }
   return true;
}

Wavelet::pVector Wavelet::GetData(int level)
{
   //test
   if(level > max_level || level < -1)
      return pVector();
   //decide the size of data
   if(level == -1)
   {
      pVector result(new Vector(1));
      (*result)[0] = (*process_data)[0];
   }
   else if(level == 0)
   {
      pVector result(new Vector(1));
      (*result)[0] = (*process_data)[1];
   }
   else
   {
      pVector result(new Vector( (1<<level)));
      size_t begin = 1 << level;
      for(size_t i = 0; i < result->size(); i++)
      {
	 (*result)[i] = (*process_data)[begin + i];
      }
   }
}

void Wavelet::LoadData(pVector source)
{
   process_data->assign(source->begin(), source->end());
}


Wavelet2D::Wavelet2D()
   : Wavelet()
{

}

Wavelet2D::~Wavelet2D()
{
}

void Wavelet2D::LoadData(const Matrix::pMatrix source)
{
   if(!source->IsSquared())
      return;
   else if(!IsPowerOf2(source->GetWidth()))
      return;
   else
      data.reset(new Matrix(*source));
}

void Wavelet2D::Transform(bool isForward, bool isStandard)
{
   if(isStandard)
   {
      for(size_t row = 0; row < data->GetHeight(); row++)
      {
	 process_data = data->GetRow(row);
	 Wavelet::Transform(isForward);
	 data->SetRow(row, process_data);
      }
      for(size_t col = 0; col < data->GetWidth(); col++)
      {
	 process_data = data->GetColumn(col);
	 Wavelet::Transform(isForward);
	 data->SetColumn(col, process_data);
      }
   }
   else
   {//non-standard
      if(isForward)
      {
	 for(size_t i = data->GetWidth(); i >= 2; i>>=1)
	 {//level

	    //row
	    for(size_t row = 0; row < data->GetHeight(); row++)
	    {
	       process_data = data->GetRow(row);
	       DWT_step(i, true);
	       data->SetRow(row, process_data);
	    }
	    //col
	    for(size_t col = 0; col < data->GetWidth(); col++)
	    {
	       process_data = data->GetColumn(col);
	       DWT_step(i, true);
	       data->SetColumn(col, process_data);
	    }
	 }
      }
      else
      {//inverse
	 for(size_t i = 2; i <= process_data->size(); i<<=1)
	 {//level

	    //row
	    for(size_t row = 0; row < data->GetHeight(); row++)
	    {
	       process_data = data->GetRow(row);
	       DWT_step(i, false);
	       data->SetRow(row, process_data);
	    }
	    //col
	    for(size_t col = 0; col < data->GetWidth(); col++)
	    {
	       process_data = data->GetColumn(col);
	       DWT_step(i, false);
	       data->SetColumn(col, process_data);
	    }
	 }
      }
   }
}

Matrix::pMatrix Wavelet2D::GetData(int level, Wavelet2D::SubBand type)
{
   Matrix::pMatrix result(new Matrix());
   if(level == -1)
   {//smooth coefficient
      result->Create(1,1);
      result->SetElement(0, 0, data->GetElement(0,0));
      return result;
   }
   size_t low_limit = 1<<level;
   size_t high_limit = 1 <<(level + 1);
    switch(type)
   {
      case LL:
	 result = data->GetSubMatrix(make_pair(0,0), make_pair(low_limit, low_limit));
	 break;
      case HL:
	 result = data->GetSubMatrix(make_pair(low_limit, 0),
				     make_pair(high_limit, low_limit));
	 break;
      case LH:
	 result = data->GetSubMatrix(make_pair(0, low_limit), 
				     make_pair(low_limit, high_limit));
	 break;
      case HH:
	 result = data->GetSubMatrix(make_pair(low_limit, low_limit), 
				     make_pair(high_limit, high_limit));
	 break;
      default:
	 break;
   }
    return result;
}
*/
