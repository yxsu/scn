#ifndef GNUPLOT_H
#define GNUPLOT_H
#include "math_global.h"
#include<string>
#include<vector>
#include<utility>
using std::string;
using std::vector;
using std::pair;

class MATHSHARED_EXPORT Gnuplot
{
public:
    Gnuplot();

    string DrawHistogram(vector<pair<double, size_t>>& data);

private:
    string result_base_path;

};

#endif // GNUPLOT_H
