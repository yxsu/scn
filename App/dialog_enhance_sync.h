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

    void update_ratio(double ratio, double progress);

private:
    Ui::DialogEnhanceSync *ui;
    scn::UGraph::pGraph graph;
    SAOptimalGraph optimize_thread;
    vector<pair<double, double>> ratio_record;//pair<progress, ratio>
};

#endif // DIALOG_ENHANCE_SYNC_H
