#include "../include/wavelet.hpp"


//haar wavelet
const double haar_ch_2[2] = { 0.70710678118654752440084436210,
			      0.70710678118654752440084436210};
const double haar_cg_2[2] = { 0.70710678118654752440084436210,
			      -0.70710678118654752440084436210};
//bspline wavelet

/*

void Wavelet::InitWavelet(Wavelet::WaveletType type, bool isCentered)
{
   switch(type)
   {
      case HAAR_2:
	 h1->assign(haar_ch_2, haar_ch_2 + 2);
	 g1->assign(haar_cg_2, haar_cg_2 + 2);
	 h2->assign(haar_ch_2, haar_ch_2 + 2);
	 g2->assign(haar_cg_2, haar_cg_2 + 2);
	 nc = 2;
	 offset = isCentered ? 1 : 0;
	 break;
      case BSPLINE_301:

	 break;
   };
}
*/
