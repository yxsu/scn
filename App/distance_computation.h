#ifndef DISTANCE_COMPUTATION_H
#define DISTANCE_COMPUTATION_H

#include <QObject>
#include <network_measurements.hpp>
#include <QtCore>
#include <memory>

class DistanceComputation : public QThread
{
    Q_OBJECT
public:
    explicit DistanceComputation(QObject *parent = 0);
    ~DistanceComputation();

    void StartCompute(scn::UGraph::pGraph graph);

signals:

    void HasResult(size_t diameter, double average_distance);

public slots:

protected:
    void run();
private:
    std::shared_ptr<scn::Ruler> ruler;
};

#endif // DISTANCE_COMPUTATION_H
