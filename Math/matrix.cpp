
#include "matrix.hpp"
#include "fortran2c.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using std::map;
using std::make_pair;
using std::copy;
using std::vector;
using std::gslice;

Matrix::Matrix()
   :width(0), height(0), vdata(0)
{
}

Matrix::Matrix(size_t height, size_t width)
   : vdata(width * height)
{
    this->height = height;
    this->width = width;
}
/*
Matrix::Matrix(size_t width, initializer_list<double> list)
   :width(width)
{
   size_t temp = list.size() / width;
   height = temp * width == list.size() ? temp : temp + 1;
   vdata.resize(width * height);
   vdata[gslice(0, {height, width}, {1, height})] = valarray<double>(list);
}
*/
Matrix::Matrix(Matrix &&other) noexcept
:width(other.width), height(other.height), vdata(other.vdata)
{   
}

Matrix::Matrix(const Matrix &other)
   :width(other.GetWidth()), height(other.GetHeight()), vdata(other.vdata)
{
}

Matrix::~Matrix()
{
}

Matrix::Matrix(const Matrix &other, const slice &row, const slice &col)
   :Matrix(row.size(), col.size())
{
   assert(row.start() + row.stride() * (row.size() - 1) < other.GetHeight());
   assert(col.start() + col.stride() * (col.size() - 1) < other.GetWidth());
   
   vdata = other.vdata[gslice(col.start() * other.GetHeight() + row.start(), 
   {col.size(), row.size()}, {col.stride() * other.GetHeight(), row.stride()})];
}
gslice_array<double> Matrix::operator()(const slice &row, const slice &col)
{
   assert(row.start() + row.stride() * (row.size() - 1) < height);
   assert(col.start() + col.stride() * (col.size() - 1) < width);
   
   return vdata[gslice(col.start() * height + row.start(), 
		       {col.size(), row.size()}, {col.stride() * height, row.stride()})];
}

Matrix Matrix::operator*(const Matrix &other) const
{
   assert(width == other.height);
   
   Matrix result(height, other.width);
   for(size_t row = 0; row < height; row++)
   {
      for(size_t col = 0; col < other.width; col++)
      {	 
	 result.vdata[col * height + row] =
	    (vdata[slice(row, width, height)] 
	     * other.vdata[slice(col * other.height, other.height, 1)]).sum();
      }
   }
   return result;
}

bool Matrix::HasNoZeros()
{
    for(auto iter : vdata)
    {
        if(iter == 0)
            return false;
    }
    return true;
}

Matrix& Matrix::operator+=(const Matrix &other)
{
    assert(IsSameSizeWith(other));
    vdata[slice(0, vdata.size(), 1)] += other.vdata[slice(0, vdata.size(), 1)];
    return *this;
}

Matrix& Matrix::operator=(const Matrix &other)
{
   width = other.GetWidth();
   height = other.GetHeight();
   vdata = other.vdata;
   return *this;
}

Matrix& Matrix::operator=(Matrix &&other) noexcept
{
   width = other.GetWidth();
   height = other.GetHeight();
   vdata = std::move(other.vdata);
   return *this;
}

Matrix& Matrix::operator=(double value)
{
   vdata = value;
   return *this;
}

slice_array<double> Matrix::row(size_t indexOfRow)
{
   assert(indexOfRow < height);
   return vdata[slice(indexOfRow, width, height)];
}

valarray<double> Matrix::row(size_t indexOfRow) const
{
   assert(indexOfRow < height);
   return vdata[slice(indexOfRow, width, height)];
}

slice_array<double> Matrix::col(size_t indexOfCol)
{
   assert(indexOfCol < width);
   return vdata[slice(indexOfCol * height, height, 1)];
}

valarray<double> Matrix::col(size_t indexOfCol) const
{
   assert(indexOfCol < width);
   return vdata[slice(indexOfCol * height, height, 1)];
}

bool Matrix::IsSquared() const
{
   if(width == height)
      return true;
   else
      return false;
}

bool Matrix::IsSymmetric() const
{
   if(width != height)
      return false;
   for(size_t i = 0; i < width; i++)
   {
      if(abs(row(i) - col(i)).sum() != 0)
	 return false;
   }
   return true;
}

bool Matrix::IsSameSizeWith(const Matrix &other) const
{
   if(this->width == other.GetWidth() &&
      this->height == other.GetHeight())
      return true;
   else
      return false;
}


Matrix Matrix::FilterByValid(const Matrix &window)
{
   assert(window.GetWidth() <= GetWidth() &&
	  window.GetHeight() <= GetHeight());

   Matrix result(GetWidth() - window.GetWidth() + 1,
		 GetHeight() - window.GetHeight() + 1);
   
   for(size_t row = 0; row < result.GetHeight(); row++)
   {
      for(size_t col = 0; col < result.GetWidth(); col++)
      {
	 double sum = 0;
	 for(size_t row_w = 0; row_w < window.GetHeight(); row_w++)
	 {
	    for(size_t col_w = 0; col_w < window.GetWidth(); col_w++)
	    {
	       sum += (*this)(row + row_w, col + col_w);
	    }
	 }
	 result(row, col) = sum;
      }
   }
   return result;
}

void Matrix::Print(string name) const
{
   std::cout<<"---------------------------"<<std::endl;
   std::cout<<name<<" : "<<std::endl;
   for(size_t row = 0; row < GetHeight(); row++)
   {
      for(size_t col = 0; col < GetWidth(); col++)
      {
	 std::cout<<(*this)(row, col)<<" ";
      }
      std::cout<<std::endl;
   }
}

valarray<double> Matrix::GetEigenvalueList()
{
   assert(IsSymmetric());
   vector<double> data(begin(vdata), end(vdata));
   //lapack parameter list
   char jobz = 'N';//compute eigenvalues only
   char range = 'A';//all eigenvalues will be found
   char uplo = 'U';//upper triangle of A is stored
   int n = GetWidth();
   int lda = GetWidth();
   double vl, vu;//no referenced when range = 'A' or 'I'
   int il, iu;//no referenced if range = 'A' or 'V'
   double abstol = 0;//absolute error tolerance for the eigenvalues
   int m;//the total number of eigenvalues found
   vector<double> w(n);//the first m elements contain the selected
                       //eigenvalues in ascending order
   vector<double> z(n*n);//not referenced when jobz = 'N', but this
			 //reserved space
   int ldz = n;//the leading dimension of the array z
   vector<int> isuppz(2 * n);//the support of the eigenvectors in Z
   int info;
   //got the optimal size of workspace
   int lwork = -1;//the dimension of the array work
   int liwork = -1;//the dimension of the array iwork
   vector<double> work(26 * n);//workspace
   vector<int> iwork(10 * n);//workspace
   //query the optimal size

   dsyevr_(&jobz, &range, &uplo, &n, data.data(), &lda, &vl, &vu, &il, &iu, &abstol,
	   &m, w.data(), z.data(), &ldz, isuppz.data(), work.data(), &lwork,
       iwork.data(), &liwork, &info);
   if(info == 0)
   {//succeed
      lwork = ceil(work[0]);
      liwork = iwork[0];
      //reallocate
      work.resize(lwork);
      iwork.resize(liwork);
   }
   else
   {
      return valarray<double>();
   }
   //compute
   dsyevr_(&jobz, &range, &uplo, &n, data.data(), &lda, &vl, &vu, &il, &iu, &abstol,
	   &m, w.data(), z.data(), &ldz, isuppz.data(), work.data(), &lwork,
       iwork.data(), &liwork, &info);
   if(info == 0)
   {
      valarray<double> result(n);
      copy(w.begin(), w.end(), begin(result));
      return result;
   }
   else
      return valarray<double>();
}

Matrix::ValueVectorList Matrix::GetEigenValueAndVector() const
{
   assert(IsSymmetric());
   vector<double> data(begin(vdata), end(vdata));
   //lapack parameter list
   char jobz = 'V';//compute eigenvalues only
   char range = 'A';//all eigenvalues will be found
   char uplo = 'U';//upper triangle of A is stored
   int n = GetWidth();
   int lda = GetWidth();
   double vl, vu;//no referenced when range = 'A' or 'I'
   int il, iu;//no referenced if range = 'A' or 'V'
   double abstol = 0;//absolute error tolerance for the eigenvalues
   int m;//the total number of eigenvalues found
   vector<double> w(n);//the first m elements contain the selected
                       //eigenvalues in ascending order
   vector<double> z(n * n);//eigenvector in each culumn
   int ldz = n;//the leading dimension of the array z
   vector<int> isuppz(2 * n);//the support of the eigenvectors in Z
   int info;
   //got the optimal size of workspace
   int lwork = -1;//the dimension of the array work
   int liwork = -1;//the dimension of the array iwork
   vector<double> work(26 * n);//workspace
   vector<int> iwork(10 * n);//workspace
   //query the optimal size
   dsyevr_(&jobz, &range, &uplo, &n, data.data(), &lda, &vl, &vu, &il, &iu, &abstol,
	   &m, w.data(), z.data(), &ldz, isuppz.data(), work.data(), &lwork,
       iwork.data(), &liwork, &info);
   if(info == 0)
   {//succeed
      lwork = ceil(work[0]);
      liwork = iwork[0];
      //reallocate
      work.resize(lwork);
      iwork.resize(liwork);
   }
   else
   {
      return ValueVectorList();
   }
   //compute

   dsyevr_(&jobz, &range, &uplo, &n, data.data(), &lda, &vl, &vu, &il, &iu, &abstol,
	   &m, w.data(), z.data(), &ldz, isuppz.data(), work.data(), &lwork,
       iwork.data(), &liwork, &info);
   if(info == 0)
   {
      ValueVectorList result;
      for(int i = 0; i < n; i++)
      {
	 valarray<double> temp(n);
	 copy(z.begin() + i * n, z.begin() + (i+1) * n, begin(temp));
	 result.push_back(make_pair(w[i], temp));
      }
      return result;
   }
   else
      return ValueVectorList();
}

string Matrix::DrawSpectralDensity(double slide)
{
   map<double, double> distribution;
   auto lambdas = GetEigenvalueList();
   int start = floor(lambdas[0]);
   int end = ceil(lambdas[lambdas.size() - 1]);
   for(int i = 0; i < (end - start) / slide; i++)
   {
      distribution[double(start) + i * slide] = 0;
   }
   for(auto& iter : lambdas)
   {
      distribution[floor(iter / slide) * slide]++;
   }
   //normalized
   vector<double> x,y;
   for(auto iter = distribution.begin(); iter != distribution.end(); iter++)
   {
      // if(abs(iter->first) < 5)
      // {
	 x.push_back(iter->first);
	 y.push_back(iter->second / lambdas.size());
	 // }
   }
   //draw
   string filename = "spectral_density_distribution";
   return filename + ".png";
}

Matrix& Matrix::operator/=(double value)
{
   vdata /= value;
   return *this;
}
