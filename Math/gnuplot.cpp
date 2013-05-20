#include "gnuplot.h"
#include<fstream>
#include<chrono>
#include<sstream>
using namespace std;
Gnuplot::Gnuplot()
{
    result_base_path = "figure/";
#ifdef Q_OS_MAC
    gnuplot_path = "/usr/local/bin/gnuplot";
#else
    gnuplot_path = "gnuplot";
#endif
}

string Gnuplot::DrawHistogram(vector<pair<double, size_t> > &data)
{
    //get current time
    using namespace std::chrono;
    time_t date = chrono::system_clock::to_time_t(system_clock::now());
    //get filename
    stringstream ss;
    ss<<"histogram_file_"<<date;
    string data_filename = ss.str() + ".dat";
    string command_filename = ss.str() + ".gp";
    string figure_filename = ss.str() + ".png";
    //write data
    ofstream out_data(data_filename, ios_base::trunc);
    for(auto iter = data.begin(); iter != data.end(); iter++)
    {
        out_data<<iter->first<<"  "<<iter->second<<endl;
    }
    out_data.close();
    //write command
    ofstream outfile(command_filename, ios_base::trunc);
    outfile<<"set terminal png"<<endl;
    outfile<<"set output \""<<figure_filename<<"\""<<endl;
    outfile<<"set style fill solid"<<endl;
    outfile<<"plot \""<<data_filename<<"\" notitle with boxes"<<endl;
    outfile.close();
    //execute
    if(system(string(gnuplot_path + " " + command_filename).c_str()) == 0)
        return figure_filename;
    else
        return "";
}
