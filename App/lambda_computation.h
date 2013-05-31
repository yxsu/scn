#ifndef LAMBDA_COMPUTATION_H
#define LAMBDA_COMPUTATION_H

#include <QObject>
#include <network_measurements.hpp>
#include <QtCore>
#include <memory>

class LambdaComputation : public QThread
{
    Q_OBJECT
public:
    explicit LambdaComputation(QObject *parent = 0);
    ~LambdaComputation();

    void StartCompute(scn::UGraph::pGraph graph);
    
signals:
    void HasResult(double lambda2, double lambda_ratio);
public slots:

protected:
    void run();

private:
    std::shared_ptr<scn::Ruler> ruler;
    
};

#endif // LAMBDA_COMPUTATION_H
