#pragma once

extern "C"
{
//blas

//lapack
   void dsyevr_(char* jobz, char* range, char* uplo, int* n, double* a, 
		int* lda, double* vl, double* vu, int* il, int* iu,
		double* abstol, int* m, double* w, double* z, int* ldz,
		int* isuppz, double* work, int* lwork, int* iwork, 
		int* liwork, int* info);
//user-defined

}
