#include "dialog_enhance_sync.h"
#include "ui_dialog_enhance_sync.h"
#include <gnuplot.h>
#include <QMessageBox>
#include <fstream>

DialogEnhanceSync::DialogEnhanceSync(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEnhanceSync),
    optimize_thread(this)
{
    ui->setupUi(this);

    //set connect
    connect(&optimize_thread, &SAOptimalGraph::UpdateRatio, this,
            &DialogEnhanceSync::update_ratio);

    connect(&optimize_thread, &SAOptimalGraph::OptimalGraph,
            [&](scn::UGraph::pGraph graph)
    {
        this->graph = graph;
        ui->pushButton_begin->setText(tr("Finished"));
       // QMessageBox::information(this, tr("Result"), tr("Optimizing process finished!"),QMessageBox::Ok);
        //save ratio sequence to file
        std::ofstream outfile("ratio_sequence.txt", ios_base::trunc);
        outfile<<"iteration_time    ratio    clustering_coefficient    diameter    average_distance"<<endl;
        for(size_t i = 0; i < ratio_record.size(); i++)
        {
            outfile<<ratio_record[i].first<<"   "<<ratio_record[i].second<<"   "<<cc_record[i].second<<"   "
                  <<diameter_record[i].second<<"   "<<average_distance_record[i].second<<endl;
        }
        outfile.close();
    });
}

DialogEnhanceSync::~DialogEnhanceSync()
{
    delete ui;
}

void DialogEnhanceSync::SetInitialGraph(scn::UGraph::pGraph graph)
{
    this->graph = graph;
}

scn::UGraph::pGraph DialogEnhanceSync::GetFinalGraph()
{
    return graph;
}

void DialogEnhanceSync::on_pushButton_begin_clicked()
{
    ui->pushButton_begin->setText(tr("Computing..."));
    ui->pushButton_begin->setEnabled(false);
    optimize_thread.StartCompute(graph);
}

void DialogEnhanceSync::update_ratio(double progress, double ratio, double cc, double diameter, double average_distance)
{
    //ratio
    ratio_record.push_back(make_pair(progress,ratio));
    ui->lineEdit_lambda_ratio->setText(tr("%1").arg(ratio));
    //draw
    Gnuplot plot;
    plot.SetFileName("ratio_record");
    plot.SetXYData(ratio_record);
    //set image
    QImage image_ratio(QString::fromStdString(plot.DrawXYCurve()));
    ui->label_lambda_ratio_graph->setPixmap(
                QPixmap::fromImage(image_ratio.scaledToHeight(
                                       ui->label_lambda_ratio_graph->height(),
                                       Qt::SmoothTransformation)));
    //clustering coefficient
    cc_record.push_back(make_pair(progress, cc));
    ui->lineEdit_clustering_coefficient->setText(tr("%1").arg(cc));
    plot.SetFileName("clustering_coefficient_record");
    plot.SetXYData(cc_record);
    QImage image_cc(QString::fromStdString(plot.DrawXYCurve()));
    ui->label_clustering_coefficient_graph->setPixmap(
                QPixmap::fromImage(image_cc.scaledToHeight(
                                       ui->label_clustering_coefficient_graph->height(),
                                       Qt::SmoothTransformation)));
    //diameter
    diameter_record.push_back(make_pair(progress, diameter));
    ui->lineEdit_diameter->setText(tr("%1").arg(diameter));
    plot.SetFileName("diameter_record");
    plot.SetXYData(diameter_record);
    QImage image_diameter(QString::fromStdString(plot.DrawXYCurve()));
    ui->label_diameter_graph->setPixmap(
                QPixmap::fromImage(image_diameter.scaledToHeight(
                                       ui->label_diameter_graph->height(),
                                       Qt::SmoothTransformation)));
    //average distance
    average_distance_record.push_back(make_pair(progress, average_distance));
    ui->lineEdit_average_distance->setText(tr("%1").arg(average_distance));
    plot.SetFileName("average_distance");
    plot.SetXYData(average_distance_record);
    QImage image_average_distance(QString::fromStdString(plot.DrawXYCurve()));
    ui->label_average_distance_graph->setPixmap(
                QPixmap::fromImage(image_average_distance.scaledToHeight(
                                       ui->label_average_distance_graph->height(),
                                       Qt::SmoothTransformation)));
}
