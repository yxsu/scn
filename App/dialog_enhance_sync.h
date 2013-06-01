#ifndef DIALOG_ENHANCE_SYNC_H
#define DIALOG_ENHANCE_SYNC_H

#include <QDialog>
#include <graph.hpp>
#include <sa_optimal_graph.h>
#include <vector>

namespace Ui {
class DialogEnhanceSync;
}

class DialogEnhanceSync : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogEnhanceSync(QWidget *parent = 0);
    ~DialogEnhanceSync();

    void SetInitialGraph(scn::UGraph::pGraph graph);
    scn::UGraph::pGraph GetFinalGraph();
    
public slots:
    void on_pushButton_begin_clicked();

    void update_ratio(double progress, double ratio, double cc, double diameter, double average_distance);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogEnhanceSync *ui;
    scn::UGraph::pGraph graph;
    SAOptimalGraph optimize_thread;
    vector<pair<double, double>> ratio_record;//pair<progress, ratio>
    vector<pair<double, double>> cc_record;//clustering coefficient
    vector<pair<double, double>> diameter_record;
    vector<pair<double, double>> average_distance_record;
};

#endif // DIALOG_ENHANCE_SYNC_H
