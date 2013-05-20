#include "distance_computation.h"

DistanceComputation::DistanceComputation(QObject *parent) :
    QThread(parent)
{
}

DistanceComputation::~DistanceComputation()
{
    if(isRunning())
        terminate();
    wait();
}

void DistanceComputation::StartCompute(scn::UGraph::pGraph graph)
{
    ruler.reset(new scn::Ruler(graph));

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

void DistanceComputation::run()
{
    auto distance = ruler->GetDiameterAndAverageDistance();
    emit HasResult(distance.first, distance.second);
}
