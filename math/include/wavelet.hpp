#pragma once

#include<vector>
#include<memory>
#include"matrix.hpp"
/*
class Wavelet
{
public://enumerate
   typedef std::vector<double>  Vector;
   typedef std::shared_ptr<Vector> pVector;

   enum WaveletType
   {
      //haar wavelet
      HAAR_2,
      //bspline wavelet
      BSPLINE_103, BSPLINE_105, BSPLINE_202, BSPLINE_204, BSPLINE_206,
      BSPLINE_208, BSPLINE_301, BSPLINE_303, BSPLINE_305, BSPLINE_307,
      BSPLINE_309,
      //daubechies wavelet
      DAUBECHIES_2, DAUBECHIES_4, DAUBECHIES_6, DAUBECHIES_8, DAUBECHIES_10,
      DAUBECHIES_12, DAUBECHIES_14, DAUBECHIES_16, DAUBECHIES_18,
      DAUBECHIES_20,
   };
public:
*/
/**
 * @name Wavelet - Default constructor
 * @return Wavelet
 */
/*
   Wavelet();
   ~Wavelet();

public:
 
   void LoadData(pVector source);

   void InitWavelet(Wavelet::WaveletType type, bool isCentered);

   void Transform(bool isForward);
*/
/**
 * @name GetData - Get the coefficient of transformed data
 * @param level - Number of level, if level = -1, the data is
 * smoothing coefficient 
 * @return pVector - result
 */
/*
   pVector GetData(int level);
 
protected://protected function

   bool IsPowerOf2(size_t number);

   void DWT_step(size_t length, bool isForward);

protected://protected data
   //data used in transform
   pVector  process_data;*/	/**< process_data. copy the
				 * original data to it, then
				 * run the transform, the
				 * result will replace the
				 * original data in it. */
/*  int max_level;
   size_t  nc;
   size_t  offset;
   pVector  h1;
   pVector  g1;
   pVector  h2;
   pVector  g2;
};


class Wavelet2D
: public Wavelet
{
public://enumerate
   enum SubBand
   {        //   -------------- 
      LL,   //   |  LL  |  LH |   
      HL,   //   --------------
      LH,   //   |  HL  |  HH |
      HH    //   --------------
   };
public:
   Wavelet2D();

   ~Wavelet2D();

public:
   
   void LoadData(const Matrix::pMatrix source);
   
   void Transform(bool isForward, bool isStandard);
   
   Matrix::pMatrix GetData(int level, Wavelet2D::SubBand type);

protected:

   Matrix::pMatrix data;
};
*/
