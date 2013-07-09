#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <foursquare.hpp>
#include "qnetwork.hpp"
#include "distance_computation.h"
#include "lambda_computation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    void DrawAndCompute();

    void UpdateGraphivsView();

    void ComputeBasicInfo(scn::UGraph::pGraph graph);

private slots:
    void on_action_open_net_file_triggered();

    void on_action_save_net_file_triggered();

    void on_action_regular_network_triggered();

    void on_pushButton_recompute_clicked();

    void on_action_ws_small_world_triggered();

    void on_action_nw_small_world_triggered();

    void on_action_random_network_triggered();

    void on_action_scale_free_network_triggered();

    void on_action_small_world_from_edge_iteration_triggered();

    void on_action_uniform_recursive_tree_triggered();

    void on_action_durt_network_triggered();

    void on_action_small_world_from_durt_triggered();

    void on_action_community_network_triggered();

    void on_action_tsd_sfsw_network_triggered();

    void on_action_tsd_sw_network_triggered();

    void on_action_stimulated_annealing_triggered();

    void on_actionSetup_Data_Source_triggered();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<QUNetwork> unetwork;
    DistanceComputation distance_thread;
    LambdaComputation lambda_thread;
    Foursquare foursquare;
};

#endif // MAIN_WINDOW_H
