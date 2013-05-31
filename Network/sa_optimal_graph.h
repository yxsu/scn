#ifndef SA_OPTIMAL_GRAPH_H
#define SA_OPTIMAL_GRAPH_H
#include "graph.hpp"
#include <QObject>
#include <QtCore>

//get optimal synchrozability graph by stimulated annealing
class SAOptimalGraph : public QThread
{
    Q_OBJECT
public:
    explicit SAOptimalGraph(QObject* parent = 0);
    ~SAOptimalGraph();

    void StartCompute(scn::UGraph::pGraph graph);

signals:
    void UpdateRatio(double lambda_ratio, double T);
    void OptimalGraph(scn::UGraph::pGraph graph);
public slots:

protected:
    void run();

    scn::UGraph::pGraph GenNextGraph(scn::UGraph::pGraph current_graph);
private:
    scn::UGraph::pGraph graph;
    double T_min;
    double T;
    double r;
};

#endif // SA_OPTIMAL_GRAPH_H
