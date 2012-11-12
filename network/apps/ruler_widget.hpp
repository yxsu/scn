#pragma once
#include<QtGui/QWidget>
#include<QtGui/QLabel>
#include<QtGui/QFormLayout>
#include<QtGui/QVBoxLayout>
#include<QtGui/QImage>
#include<QtGui/QPicture>
#include"../include/graph.hpp"
#include"../include/network_measurements.hpp"

class URulerWidget : public QWidget
{
   Q_OBJECT;
public:
   
   URulerWidget();

   ~URulerWidget();
public:

   void ComputeBasicInfo(scn::UGraph::pGraph graph);
   
private:

   void CreateWidget();

private:
   
   QVBoxLayout* layout_main;
   QFormLayout* layout_basic_info;
   QLabel* label_numberOfNode;
   QLabel* label_numberOfEdge;
   QLabel* label_average_degree;
   QLabel* label_image_degree_dist;
   QLabel* label_image_spectral_density;
   QLabel* label_average_distance;
   QLabel* label_diameter;
   QLabel* label_cluster_coefficient;
   QLabel* label_lambda2;
   QLabel* label_lambda_ratio;
   QLabel* label_mfpt;
   QLabel* label_mrt;
};
