#include "sa_optimal_graph.h"
#include "network_measurements.hpp"
#include <ctime>
using namespace scn;
using namespace std;
SAOptimalGraph::SAOptimalGraph(QObject* parent):
    QThread(parent),
    T_min(0.001), T(0.85), r(0.98)
{
}

SAOptimalGraph::~SAOptimalGraph()
{
}

scn::UGraph::pGraph SAOptimalGraph::GenNextGraph(scn::UGraph::pGraph current_graph)
{
    UGraph::pGraph next_graph;
    srand(size_t(time(NULL)));
/*
    do
    {
        size_t size = current_graph->GetNumberOfNodes();
        size_t index[4];
        do
        {
            index[0] = rand() % size;
            index[1] = rand() % size;//edge1: (index[0], index[1])
            index[2] = rand() % size;
            index[3] = rand() % size;//edge2: (index[2], index[3])
            if(index[0] == index[1] || index[2] == index[3])
                continue;
            if(current_graph->HasEdge(index[0], index[1]) && !current_graph->HasEdge(index[2], index[3]))
                break;
        }while(true);

        //remove edge1 and add edge2
        next_graph.reset(new UGraph(*current_graph));
        next_graph->RemoveEdge(index[0], index[1]);
        next_graph->AddEdge(index[2], index[3]);

    }while(!Ruler(next_graph).IsConnectedGraph());
    */

    do
    {
        auto matrix = current_graph->GetAdjacencyMatrix();
        size_t size = matrix.GetHeight();
        int row1, col1, row0, col0;
        do
        {
            row1 = rand() % size;
            col1 = rand() % size;
            row0 = rand() % size;
            col0 = rand() % size;
        }while(!(row1 != col1 && row0 != col0 && abs(matrix(row1, col1) - 1) < 1.0e-3) && abs(matrix(row0, col0)) < 1.0e-3);

        matrix(row1, col1) = 0;
        matrix(row0, col0) = 1;

        next_graph.reset(new UGraph(size));
        for(size_t i = 0; i < size; i++)
        {
            for(size_t j = 0; j < i; j++)
            {
                if(abs(matrix(i, j) - 1) < 1.0e-4)
                {
                    next_graph->AddEdge(i, j);
                }
            }
        }
    }while(!Ruler(next_graph).IsConnectedGraph());

    return next_graph;
}


void SAOptimalGraph::StartCompute(UGraph::pGraph graph)
{
    this->graph = graph;

    if(!isRunning())
    {
        start();
    }
    else
    {
        this->terminate();
        wait();
        start();
    }
}


void SAOptimalGraph::run()
{
    T = 0.85;
    auto current_graph = graph;
    srand(size_t(time(NULL)));
    int times = 0;
    double current_ratio = Ruler(current_graph).GetLambda2AndRatio().second;
    do
    {
        auto next_graph = GenNextGraph(current_graph);
        double ratio_next_graph = Ruler(next_graph).GetLambda2AndRatio().second;
        double dE = ratio_next_graph - current_ratio;
        if(dE <= 0)
        {
            current_graph = next_graph;
            current_ratio = ratio_next_graph;
        }
        else
        {
            if(exp(dE / T) < double(rand() % 10) / 10)
            {
                current_graph = next_graph;
                current_ratio = ratio_next_graph;
            }
        }
        T = r * T;
        //compute measurement
        Ruler ruler(current_graph);
        auto distance = ruler.GetDiameterAndAverageDistance();
        emit UpdateRatio(double(times++), ratio_next_graph, ruler.GetClusteringCoeff(),
                         double(distance.first), distance.second);

    }while(T > T_min);
    emit OptimalGraph(current_graph);
    graph = current_graph;
}
