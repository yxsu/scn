#include "lambda_computation.h"

LambdaComputation::LambdaComputation(QObject *parent) :
    QThread(parent)
{
}

LambdaComputation::~LambdaComputation()
{
    if(isRunning())
        terminate();
    wait();
}

void LambdaComputation::StartCompute(scn::UGraph::pGraph graph)
{
    ruler.reset(new scn::Ruler(graph));

    if(!isRunning())
        start();
    else
    {
        terminate();
        wait();
        start();
    }
}

void LambdaComputation::run()
{
    auto lambda = ruler->GetLambda2AndRatio();
    emit HasResult(lambda.first, lambda.second);
}
