#include "dialog_enhance_sync.h"
#include "ui_dialog_enhance_sync.h"
#include <gnuplot.h>
#include <QMessageBox>

DialogEnhanceSync::DialogEnhanceSync(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEnhanceSync),
    optimize_thread(parent)
{
    ui->setupUi(this);
    //set connect
    connect(&optimize_thread, &SAOptimalGraph::UpdateRatio, this,
            &DialogEnhanceSync::update_ratio);

    connect(&optimize_thread, &SAOptimalGraph::OptimalGraph,
            [&](scn::UGraph::pGraph graph)
    {
        this->graph = graph;
        QMessageBox::information(this, tr("Result"), tr("Optimizing process finished!"),QMessageBox::Ok);
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
    optimize_thread.StartCompute(graph);
}

void DialogEnhanceSync::update_ratio(double ratio, double progress)
{
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
}
