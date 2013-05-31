#include "main_window.h"
#include "ui_main_window.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <network_generator.hpp>
#include <network_measurements.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    distance_thread(parent),
    lambda_thread(parent)
{
    ui->setupUi(this);
    //set property
    setWindowTitle(tr("Complex Network Analysis System"));
    showMaximized();
    statusBar()->showMessage(tr("Welcome to a complex network world!"));
    //set connection
    connect(&distance_thread, &DistanceComputation::HasResult,
            [&](size_t diameter, double average_distance)
    {
        if(ui->checkBox_diameter->isChecked())
            ui->lineEdit_diameter->setText(tr("%1").arg(diameter));
        if(ui->checkBox_average_distance->isChecked())
            ui->lineEdit_average_distance->setText(tr("%1").arg(average_distance));
    });
    connect(&lambda_thread, &LambdaComputation::HasResult,
            [&](double lambda2, double lambda_ratio)
    {
        if(ui->checkBox_lambda2->isChecked())
            ui->lineEdit_lambda2->setText(tr("%1").arg(lambda2));
        if(ui->checkBox_lambda_ratio->isChecked())
            ui->lineEdit_lambda_ratio->setText(tr("%1").arg(lambda_ratio));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DrawAndCompute()
{
    if(unetwork->GetTopology()->GetNumberOfNodes() < 1000)
    {
        unetwork->DrawOnScene();
        UpdateGraphivsView();
    }

    ComputeBasicInfo(unetwork->GetTopology());
}

void MainWindow::UpdateGraphivsView()
{
    ui->graphivsView_main->setScene(unetwork->GetScene());
}

void MainWindow::ComputeBasicInfo(scn::UGraph::pGraph graph)
{
    scn::Ruler ruler(graph);
    auto size = graph->size();
    //node
    if(ui->checkBox_node->isChecked())
        ui->lineEdit_node->setText(tr("%1").arg(size.first));
    else
        ui->lineEdit_node->clear();
    //edge
    if(ui->checkBox_edge->isChecked())
        ui->lineEdit_edge->setText(tr("%1").arg(size.second));
    else
        ui->lineEdit_edge->clear();
    //average degree
    if(ui->checkBox_average_degree->isChecked())
        ui->lineEdit_average_degree->setText(tr("%1").arg(ruler.ComputeAverageDegree()));
    else
        ui->lineEdit_average_degree->clear();
    //clustering coefficient
    if(ui->checkBox_clustering_coefficient->isChecked())
        ui->lineEdit_clustering_coefficient->setText(tr("%1").arg(ruler.GetClusteringCoeff()));
    else
        ui->lineEdit_clustering_coefficient->clear();
    //degree distribution
    QImage image_degree_dist(QString::fromStdString(ruler.DrawDegreeDistribution()));
    ui->label_image_degree_dist->setPixmap(
                QPixmap::fromImage(image_degree_dist.scaledToWidth(ui->dockWidget_ruler->width(), Qt::SmoothTransformation)));
    //diameter and average distance
    if(!ui->checkBox_diameter->isChecked())
        ui->lineEdit_diameter->clear();
    if(!ui->checkBox_average_distance->isChecked())
        ui->lineEdit_average_distance->clear();
    if(ui->checkBox_diameter->isChecked() || ui->checkBox_average_distance->isChecked())
    {
        ui->lineEdit_diameter->setText("Computing...");
        ui->lineEdit_average_distance->setText("Computing...");
        distance_thread.StartCompute(graph);
    }
    //lambda
    if(!ui->checkBox_lambda2->isChecked())
        ui->lineEdit_lambda2->clear();
    if(!ui->checkBox_lambda_ratio->isChecked())
        ui->lineEdit_lambda_ratio->clear();
    if(ui->checkBox_lambda2->isChecked() || ui->checkBox_lambda_ratio->isChecked())
    {
        ui->lineEdit_lambda2->setText("Computing...");
        ui->lineEdit_lambda_ratio->setText("Computing...");
        lambda_thread.StartCompute(graph);
    }
}

void MainWindow::on_action_open_net_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open a Net File"), "",
                                                    tr("Pajak Network Files(*.net)"));
    QFile file(filename);
    if(filename.isNull() || !file.open(QFile::ReadOnly))
    {
        QMessageBox::critical(this, tr("Error!"), tr("Could not open file!"));
    }
    QTextStream reader(&file);
    //test whether the network is undirected
    //if *Arcs list is empty, the network is undirected
    QString line = reader.readLine();
    while(!line.isNull())
    {
        if(line == "*Arcs")
        {
            line = reader.readLine();
            if(line != "*Edges")
            {
                QMessageBox::warning(this, tr("Warning!"), tr("This network is not undirected!"));
                file.close();
                return;
            }
            else
            {
                file.close();
                unetwork = QUNetwork::ReadFromNetFile(filename);
                DrawAndCompute();
                return;
            }
        }
        line = reader.readLine();
    }
    QMessageBox::warning(this, tr("Warning!"), tr("The structure of this file is not supported!"));
    file.close();
}

void MainWindow::on_action_save_net_file_triggered()
{
    if(unetwork)
    {
        QString filename = unetwork->GetPathName();
        if(filename.isNull())
        {
            filename = QFileDialog::getSaveFileName(this, tr("Save as a Net File"), "",
                                                    tr("Pajek Network Files(*.net)"));
            if(!filename.endsWith(".net"))
                filename += ".net";
        }
        unetwork->WriteToNetFile(filename);
        statusBar()->showMessage(tr("Write to ") + filename, 2000);
    }
}

void MainWindow::on_action_regular_network_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t numberOfNodes = QInputDialog::getInt(this, tr("Regular Network"),
                                                tr("Number Of Nodes:"),
                                                100, 0, 100000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
    }
    size_t k = QInputDialog::getInt(this, tr("Regular Network"),
                                    tr("Values of K: (The degree of each node will be 2*K)"),
                                    5, 0, (numberOfNodes - 1)/2, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
    }
    //create
    UGraph::pGraph graph = scn::GenKNearestNetwork(numberOfNodes, k);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_pushButton_recompute_clicked()
{
    if(unetwork)
        ComputeBasicInfo(unetwork->GetTopology());
    else
        QMessageBox::critical(this, tr("Error !"), tr("Please choose a network first!"));
}

void MainWindow::on_action_ws_small_world_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t numberOfNodes = QInputDialog::getInt(this, tr("WS Small World"),
                                                tr("Number Of Nodes:"),
                                                100, 0, 100000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    size_t k = QInputDialog::getInt(this, tr("WS Small World"),
                                    tr("Values of K: (The degree of each node will be 2*K)"),
                                    5, 0, (numberOfNodes - 1)/2, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    double probability = QInputDialog::getDouble(this, tr("WS Small World"),
                                                 tr("Probability of re-connection:"),
                                                 0.1, 0, 1, 5, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenSmallWorldNetworkByWS(numberOfNodes, k, probability);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_nw_small_world_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t numberOfNodes = QInputDialog::getInt(this, tr("NW Small World"),
                                                tr("Number Of Nodes:"),
                                                100, 0, 100000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    size_t k = QInputDialog::getInt(this, tr("NW Small World"),
                                    tr("Values of K: (The init degree of each node will be 2*K)"),
                                    20, 0, (numberOfNodes - 1)/2, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    double probability = QInputDialog::getDouble(this, tr("NW Small World"),
                                                 tr("Probability of additive connection:"),
                                                 0.1, 0, 1, 5, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenSmallWorldNetworkByNW(numberOfNodes, k, probability);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_random_network_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t numberOfNodes = QInputDialog::getInt(this, tr("Random Network"),
                                                tr("Number Of Nodes:"),
                                                100, 0, 100000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    double probability = QInputDialog::getDouble(this, tr("Random Network"),
                                                 tr("Probability of connection:"),
                                                 0.1, 0, 1, 5, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenRandomNetwork(numberOfNodes, probability);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_scale_free_network_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t numberOfNodes = QInputDialog::getInt(this, tr("Scale-free Network"),
                                                tr("Number Of Nodes:"),
                                                100, 0, 100000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    size_t k = QInputDialog::getInt(this, tr("Scale-free Network"),
                                    tr("Degree of new node : "),
                                    5, 0, numberOfNodes - 1, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenScaleFreeNetwork(numberOfNodes, k);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_small_world_from_edge_iteration_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t times = QInputDialog::getInt(this, tr("Edge Iteration Small World"),
                                        tr("Iteration times: "),
                                        3, 0, 1000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenSmallWorldByEdgeIteration(times);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_uniform_recursive_tree_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t numberOfNode = QInputDialog::getInt(this, tr("Uniform Recursive Tree"),
                                               tr("Number Of Nodes :"),
                                               100, 0, 1000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenUniformRecursiveTree(numberOfNode);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_durt_network_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t times = QInputDialog::getInt(this, tr("DURT"),
                                        tr("Iteration times :"),
                                        3, 0, 1000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenDURT(times);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_small_world_from_durt_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t times = QInputDialog::getInt(this, tr("Small World From DURT"),
                                        tr("Iteration times :"),
                                        3, 0, 1000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenSmallWorldNetworkFromDURT(times);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_community_network_triggered()
{
    //input the number of node
    bool ok = false;
    size_t numberOfNodes = QInputDialog::getInt(this, tr("Community Network"),
                                                tr("Number Of Nodes:"),
                                                100, 0, 100000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //input number of communities
    size_t numberOfCommunity = QInputDialog::getInt(this, tr("Community Network"),
                                                    tr("Number Of Community:"),
                                                    2, 0, numberOfNodes, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //inner probability
    double inner_prob = QInputDialog::getDouble(this, tr("Community Network"),
                                                tr("Probability of connection within a community:"),
                                                0.9, 0, 1, 5, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //outer probability
    double outer_prob = QInputDialog::getDouble(this, tr("Community Network"),
                                                tr("Probability of connection between communities:"),
                                                0.05, 0, 1, 5, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenCommunityNetwork(numberOfNodes, numberOfCommunity,
                                                    inner_prob, outer_prob);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_tsd_sfsw_network_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t times = QInputDialog::getInt(this, tr("Tree-Structured Scale-Free Small World"),
                                        tr("Number Of Nodes :"),
                                        35, 1, 2000000, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenTreeStructuredSFSW(times);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}

void MainWindow::on_action_tsd_sw_network_triggered()
{
    //input the number of node and edge
    bool ok = false;
    size_t times = QInputDialog::getInt(this, tr("Tree-Structured Small World"),
                                        tr("Iteration Times :"),
                                        5, 1, 30, 1, &ok);
    if(!ok)
    {
        QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
        return;
    }
    //create
    UGraph::pGraph graph = scn::GenTreeStructuredSW(times);
    unetwork.reset(new QUNetwork(graph));
    DrawAndCompute();
}
