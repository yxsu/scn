#include "ruler_widget.hpp"

using namespace std;
using namespace scn;

URulerWidget::URulerWidget()
{
   CreateWidget();
}

URulerWidget::~URulerWidget()
{

   delete label_numberOfNode;
   delete label_numberOfEdge;
   delete label_average_degree;
   delete label_average_distance;
   delete label_diameter;
   delete label_cluster_coefficient;

   delete layout_basic_info;
   delete layout_main;

}

void URulerWidget::CreateWidget()
{
   auto common_style = QFrame::Panel | QFrame::Sunken;
   //create widget
   label_numberOfNode = new QLabel(this);
   label_numberOfNode->setFrameStyle(common_style);
   label_numberOfEdge = new QLabel(this);
   label_numberOfEdge->setFrameStyle(common_style);
   label_average_degree = new QLabel(this);
   label_average_degree->setFrameStyle(common_style);
   label_average_distance = new QLabel(this);
   label_average_distance->setFrameStyle(common_style);
   label_diameter = new QLabel(this);
   label_diameter->setFrameStyle(common_style);
   label_cluster_coefficient = new QLabel(this);
   label_cluster_coefficient->setFrameStyle(common_style);
   label_lambda2 = new QLabel(this);
   label_lambda2->setFrameStyle(common_style);
   label_lambda_ratio = new QLabel(this);
   label_lambda_ratio->setFrameStyle(common_style);
   label_mfpt = new QLabel(this);
   label_mfpt->setFrameStyle(common_style);
   label_mrt = new QLabel(this);
   label_mrt->setFrameStyle(common_style);
   label_image_degree_dist = new QLabel(this);
   label_image_degree_dist->setMaximumWidth(400);
   label_image_spectral_density = new QLabel(this);
   label_image_spectral_density->setMaximumHeight(400);
   //create layout
   layout_basic_info = new QFormLayout;
   layout_basic_info->addRow(tr("Node :"), label_numberOfNode);
   layout_basic_info->addRow(tr("Edge :"), label_numberOfEdge);
   layout_basic_info->addRow(tr("Cluster Coefficient:"), label_cluster_coefficient);
   layout_basic_info->addRow(tr("Average Degree :"), label_average_degree);
   layout_basic_info->addRow(tr("Average Distance :"), label_average_distance);
   layout_basic_info->addRow(tr("Diameter :"), label_diameter);
   layout_basic_info->addRow(tr("Lambda 2 :"), label_lambda2);
   layout_basic_info->addRow(tr("Lambda Ratio:"), label_lambda_ratio);
   layout_basic_info->addRow(tr("MRT of Random Walk:"), label_mrt);
   layout_basic_info->addRow(tr("MFPT of Random Walk:"), label_mfpt);
   
   layout_main = new QVBoxLayout;
   layout_main->addLayout(layout_basic_info);
   layout_main->addWidget(label_image_degree_dist);
   layout_main->addWidget(label_image_spectral_density);
   setLayout(layout_main);
}

void URulerWidget::ComputeBasicInfo(UGraph::pGraph graph)
{
   Ruler ruler(graph);
   QImage image(QString::fromStdString(ruler.DrawDegreeDistribution()));
   auto size = graph->size();
   label_numberOfNode->setText(tr("%1").arg(size.first));
   label_numberOfEdge->setText(tr("%1").arg(size.second));
   label_average_degree->setText(tr("%1").arg(ruler.ComputeAverageDegree()));
   //generate picture
   label_image_degree_dist->setPixmap(
   QPixmap::fromImage(image.scaledToWidth(width(),Qt::SmoothTransformation)));
   //
   label_cluster_coefficient->setText(tr("Computing..."));
   label_cluster_coefficient->setText(tr("%1").arg(ruler.GetClusteringCoeff()));
/*   label_diameter->setText(tr("Computing..."));
   label_average_distance->setText(tr("Computing..."));
    auto distance = ruler.GetDiameterAndAverageDistance();
   label_diameter->setText(tr("%1").arg(distance.first));
   label_average_distance->setText(tr("%1").arg(distance.second));
*/
/*
   label_lambda2->setText(tr("Computing..."));
   label_lambda_ratio->setText(tr("Computing..."));
   auto lambda = ruler.GetLambda2AndRatio();
   label_lambda2->setText(tr("%1").arg(lambda.first));
   label_lambda_ratio->setText(tr("%1").arg(lambda.second));
*/
/*
   label_mrt->setText(tr("Computing..."));
   label_mrt->setText(tr("%1").arg(ruler.MRTofRandomWalk()));

   label_mfpt->setText(tr("Computing..."));
   label_mfpt->setText(tr("%1").arg(ruler.MFPTofRandomWalk()));
*/
 //
/*
   Matrix shortest_path = ruler.GetGeodesicMatrix();
   QImage image_spectral_density(
      QString::fromStdString(shortest_path.DrawSpectralDensity(1)));
   label_image_spectral_density->setPixmap(
      QPixmap::fromImage(image_spectral_density.scaledToWidth(width(), 
							      Qt::SmoothTransformation)));
*/ 

}
