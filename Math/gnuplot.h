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

    string DrawHistogram();

    string DrawXYCurve();

    void SetFileName(string filename = "");

    void SetXYData(vector<pair<double, double>>& data);
private:
    string result_base_path;
    string gnuplot_path;
    string data_filename;
    string command_filename;
    string figure_filename;
    //data
    vector<pair<double, double>> data_xy;
};

#endif // GNUPLOT_H
