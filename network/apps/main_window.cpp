#include "main_window.hpp"
#include "../include/network_generator.hpp"
#include "../include/network_measurements.hpp"
#include <gnuplot.hpp>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QKeySequence>
#include <QtGui/QMenuBar>
#include <QtGui/QInputDialog>
#include <QtGui/QProgressDialog>
#include <QtCore/QTextStream>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
using namespace scn;
using namespace std;
MainWindow::MainWindow()
{
   //set windows
   setWindowTitle(tr("Complex Network Analysis System"));
   showMaximized();
   statusBar()->showMessage(tr("Welcome to a complex network world!"));
   CreateActions();
   CreateMenus();
   //view
   graphics_view = new QGraphicsView;
//   graphics_view->setDragMode(QGraphicsView::RubberBandDrag);
//   graphics_view->setRenderHint(QPainter::Antialiasing | QPainter::TextAntialiasing);
//   graphics_view->setContextMenuPolicy(Qt::ActionsContextMenu);
   //set layout and widget
   setCentralWidget(graphics_view);
   CreateDockWidgets();
   //set pointer state
   unetwork = nullptr;
}

MainWindow::~MainWindow()
{
   delete action_open_file;
   delete action_save_file;
   delete action_exit;
   
   delete measurement;
   delete dock_basic_info;
   delete graphics_view;
   
   delete menu_file;
   delete menu_create_network;
}

void MainWindow::CreateActions()
{
//action of file menu
   action_exit = new QAction(tr("E&xit"), this);
   action_exit->setShortcut(QString("Ctrl+W"));
   connect(action_exit, SIGNAL(triggered()), this, SLOT(close()));
   
   action_open_file = new QAction(tr("&Open a Net File"), this);
   action_open_file->setShortcut(QString("Ctrl+O"));
   action_open_file->setStatusTip(tr("Open a Pajek Net file..."));
   connect(action_open_file, SIGNAL(triggered()), this, SLOT(OpenFile()));

   action_save_file = new QAction(tr("&Save to Net File"), this);
   action_save_file->setShortcut(QString("Ctrl+S"));
   connect(action_save_file, SIGNAL(triggered()), this, SLOT(SaveFile()));
   
//action of create network
   action_create_regular_network = new QAction(tr("&Regular Network"), this);
   connect(action_create_regular_network, SIGNAL(triggered()), this, 
	   SLOT(CreateRegularNetwork()));

   action_create_small_world_ws = new QAction(tr("&WS Small World"), this);
   connect(action_create_small_world_ws, SIGNAL(triggered()), this,
	   SLOT(CreateWSSmallWorld()));

   action_create_small_world_nw = new QAction(tr("&NW Small World"), this);
   connect(action_create_small_world_nw, SIGNAL(triggered()), this,
	   SLOT(CreateNWSmallWorld()));

   action_create_random_network = new QAction(tr("R&andom Network"), this);
   connect(action_create_random_network, SIGNAL(triggered()), this,
	   SLOT(CreateRandomNetwork()));

   action_create_scale_free = new QAction(tr("&Scale-free Network"), this);
   connect(action_create_scale_free, SIGNAL(triggered()), this,
	   SLOT(CreateScaleFree()));

   action_small_world_edge_iter = new QAction(tr("Small World From &Edge Iteration"), this);
   connect(action_small_world_edge_iter, SIGNAL(triggered()), this,
	   SLOT(CreateSmallWorldByEdgeIter()));

   action_uniform_recursive_tree = new QAction(tr("&Uniform Recursive Tree"), this);
   connect(action_uniform_recursive_tree, SIGNAL(triggered()), this,
	   SLOT(CreateUniformRecursiveTree()));

   action_create_durt = new QAction(tr("&DURT"), this);
   connect(action_create_durt, SIGNAL(triggered()), this, 
	   SLOT(CreateDURT()));

   action_small_world_from_durt = new QAction(tr("Small World From DUR&T"), this);
   connect(action_small_world_from_durt, SIGNAL(triggered()), this,
	   SLOT(CreateSmallWorldFromDURT()));

   action_create_community_network = new QAction(tr("Community Network"), this);
   connect(action_create_community_network, SIGNAL(triggered()), this,
	   SLOT(CreateCommunityNetwork()));

   action_create_tsd_sfsw = new QAction(tr("TSD-SFSW Network"), this);
   action_create_tsd_sfsw->setStatusTip(tr("Create a Tree-Structured Deterministic Scale-free Small World Network"));
   connect(action_create_tsd_sfsw, SIGNAL(triggered()), this,
	   SLOT(CreateTreeStructuredSFSW()));

   action_create_tsd_sw = new QAction(tr("TSD-SW Network"), this);
   action_create_tsd_sw->setStatusTip(tr("Create a Tree-Structured Deterministic Small World Network"));
   connect(action_create_tsd_sw, SIGNAL(triggered()), this,
	   SLOT(CreateTreeStructuredSW()));

//action of model analysis
   //action of average degree
   action_curve_avg_degree_tsd_sfsw = new QAction(tr("TSD-SFSW Network"), this);
   connect(action_curve_avg_degree_tsd_sfsw, SIGNAL(triggered()), this,
	   SLOT(DrawCurveOfAvgDegreeTSD_SFSW()));
   //action of average distance and diameter
   action_curve_diameter_tsd_sfsw = new QAction(tr("TSD-SFSW Network"), this);
   connect(action_curve_diameter_tsd_sfsw, SIGNAL(triggered()), this,
	   SLOT(DrawCurveOfDiameterTSD_SFSW()));
   //action of clustering coefficient
   action_curve_cc_tsd_sfsw = new QAction(tr("TSD-SFSW Network"), this);
   connect(action_curve_cc_tsd_sfsw, SIGNAL(triggered()), this,
	   SLOT(DrawCurveOfClusteringCoeffTSD_SFSW()));
}

void MainWindow::CreateMenus()
{
   menu_file = menuBar()->addMenu(tr("&File"));
   menu_file->addAction(action_open_file);
   menu_file->addAction(action_save_file);
   menu_file->addSeparator();
   menu_file->addAction(action_exit);

   menu_create_network = menuBar()->addMenu(tr("&Create Network"));
   menu_create_network->addAction(action_create_regular_network);
   menu_create_network->addAction(action_create_random_network);
   menu_create_network->addAction(action_create_community_network);
   menu_create_network->addAction(action_create_small_world_ws);
   menu_create_network->addAction(action_create_small_world_nw);
   menu_create_network->addAction(action_create_scale_free);
   menu_create_network->addAction(action_small_world_edge_iter);
   menu_create_network->addAction(action_small_world_from_durt);
   menu_create_network->addAction(action_uniform_recursive_tree);
   menu_create_network->addAction(action_create_durt);
   menu_create_network->addAction(action_create_tsd_sfsw);
   menu_create_network->addAction(action_create_tsd_sw);

   menu_model_analysis = menuBar()->addMenu(tr("&Model Analysis"));
   menu_curve_average_degree = menu_model_analysis->addMenu(tr("Average D&egree"));
   menu_curve_diameter = menu_model_analysis->addMenu(tr("&Diameter and Average Distance"));
   menu_curve_clustering_coefficient = menu_model_analysis->addMenu(tr("&Clustering Coefficient"));
  
   menu_curve_average_degree->addAction(action_curve_avg_degree_tsd_sfsw);
   menu_curve_diameter->addAction(action_curve_diameter_tsd_sfsw);
   menu_curve_clustering_coefficient->addAction(action_curve_cc_tsd_sfsw);
}

void MainWindow::OpenFile()
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
	    DrawAndComputing();
	    return;
	 }
      }
      line = reader.readLine();
   }
   QMessageBox::warning(this, tr("Warning!"), tr("The structure of this file is not supported!"));
   file.close();
}

void MainWindow::SaveFile()
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

void MainWindow::CreateRegularNetwork()
{
   //input the number of node and edge
   bool ok = false;
   size_t numberOfNodes = QInputDialog::getInteger(this, tr("Regular Network"),
						   tr("Number Of Nodes:"), 
						   100, 0, 100000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   size_t k = QInputDialog::getInteger(this, tr("Regular Network"),
				       tr("Values of K: (The degree of each node will be 2*K)"),
				       5, 0, (numberOfNodes - 1)/2, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenKNearestNetwork(numberOfNodes, k);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateCommunityNetwork()
{
   //input the number of node
   bool ok = false;
   size_t numberOfNodes = QInputDialog::getInteger(this, tr("Community Network"),
						   tr("Number Of Nodes:"), 
						   100, 0, 100000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
      return;
   }
   //input number of communities
   size_t numberOfCommunity = QInputDialog::getInteger(this, tr("Community Network"),
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
   DrawAndComputing();
}

void MainWindow::CreateWSSmallWorld()
{
   //input the number of node and edge
   bool ok = false;
   size_t numberOfNodes = QInputDialog::getInteger(this, tr("WS Small World"),
						   tr("Number Of Nodes:"), 
						   100, 0, 100000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
      return;
   }
   size_t k = QInputDialog::getInteger(this, tr("WS Small World"),
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
   DrawAndComputing();
}

void MainWindow::CreateNWSmallWorld()
{
   //input the number of node and edge
   bool ok = false;
   size_t numberOfNodes = QInputDialog::getInteger(this, tr("NW Small World"),
						   tr("Number Of Nodes:"), 
						   100, 0, 100000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   size_t k = QInputDialog::getInteger(this, tr("NW Small World"),
				       tr("Values of K: (The init degree of each node will be 2*K)"),
				       20, 0, (numberOfNodes - 1)/2, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   double probability = QInputDialog::getDouble(this, tr("NW Small World"),
						tr("Probability of additive connection:"),
						0.1, 0, 1, 5, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenSmallWorldNetworkByNW(numberOfNodes, k, probability);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateRandomNetwork()
{
   //input the number of node and edge
   bool ok = false;
   size_t numberOfNodes = QInputDialog::getInteger(this, tr("Random Network"),
						   tr("Number Of Nodes:"), 
						   100, 0, 100000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   double probability = QInputDialog::getDouble(this, tr("Random Network"),
						tr("Probability of connection:"),
						0.1, 0, 1, 5, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenRandomNetwork(numberOfNodes, probability);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateScaleFree()
{
   //input the number of node and edge
   bool ok = false;
   size_t numberOfNodes = QInputDialog::getInteger(this, tr("Scale-free Network"),
						   tr("Number Of Nodes:"), 
						   100, 0, 100000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   size_t k = QInputDialog::getInteger(this, tr("Scale-free Network"),
				       tr("Degree of new node : "),
				       5, 0, numberOfNodes - 1, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenScaleFreeNetwork(numberOfNodes, k);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateSmallWorldByEdgeIter()
{
   //input the number of node and edge
   bool ok = false;
   size_t times = QInputDialog::getInteger(this, tr("Edge Iteration Small World"),
						   tr("Iteration times: "), 
						   3, 0, 1000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenSmallWorldByEdgeIteration(times);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateUniformRecursiveTree()
{
   //input the number of node and edge
   bool ok = false;
   size_t numberOfNode = QInputDialog::getInteger(this, tr("Uniform Recursive Tree"),
						  tr("Number Of Nodes :"), 
						  100, 0, 1000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenUniformRecursiveTree(numberOfNode);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateDURT()
{
   //input the number of node and edge
   bool ok = false;
   size_t times = QInputDialog::getInteger(this, tr("DURT"),
					   tr("Iteration times :"), 
					   3, 0, 1000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenDURT(times);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateSmallWorldFromDURT()
{
   //input the number of node and edge
   bool ok = false;
   size_t times = QInputDialog::getInteger(this, tr("Small World From DURT"),
					   tr("Iteration times :"), 
					   3, 0, 1000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenSmallWorldNetworkFromDURT(times);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateTreeStructuredSFSW()
{
   //input the number of node and edge
   bool ok = false;
   size_t times = QInputDialog::getInteger(this, tr("Tree-Structured Scale-Free Small World"),
					   tr("Number Of Nodes :"), 
					   35, 1, 2000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenTreeStructuredSFSW(times);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::CreateTreeStructuredSW()
{
   //input the number of node and edge
   bool ok = false;
   size_t times = QInputDialog::getInteger(this, tr("Tree-Structured Small World"),
					   tr("Iteration Times :"), 
					   5, 1, 30, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
   }
   //create
   UGraph::pGraph graph = scn::GenTreeStructuredSW(times);
   unetwork.reset(new QUNetwork(graph));
   DrawAndComputing();
}

void MainWindow::DrawAndComputing()
{  
   if(thread_compute && thread_compute->joinable())
      thread_compute->join();
   
   thread_compute.reset(new std::thread([&]()
		    {
		       measurement->ComputeBasicInfo(unetwork->GetTopology());
   		    }));
   if(unetwork->GetTopology()->GetNumberOfNodes() < 1000)
   {
      unetwork->DrawOnScene();
      UpdateGraphivsView();
   }
}

void MainWindow::UpdateGraphivsView()
{
   graphics_view->setScene(unetwork->GetScene());
}

void MainWindow::CreateDockWidgets()
{
   //basic info
   dock_basic_info = new QDockWidget(tr("Basic Infomation"), this);
   dock_basic_info->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
   measurement = new URulerWidget;
   measurement->setMinimumWidth(300);
   dock_basic_info->setWidget(measurement);
   addDockWidget(Qt::LeftDockWidgetArea, dock_basic_info);
}

void MainWindow::DrawCurveOfAvgDegreeTSD_SFSW()
{
   bool ok = false;
   size_t max_numberOfNodes = QInputDialog::getInteger(this, tr("TSD-SFSW Network"),
						       tr("Maximum Number of Nodes :"), 
						       1000, 1, 1000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
      return;
   }
   //run
   QProgressDialog progress("Computing Average Degree...", "Abort Computing",
			    1, max_numberOfNodes, this);
   vector<double> node_number, average_degree;
 
   for(size_t i = 2; i <= max_numberOfNodes; i++)
   {
      progress.setValue(i);
      if(progress.wasCanceled())
	 break;
      //run
      statusBar()->showMessage(tr("Generating network with %1 nodes").arg(i));
      Ruler ruler(scn::GenTreeStructuredSFSW(i));
      node_number.push_back(i);
      statusBar()->showMessage(tr("Computing Average Degree with %1 nodes").arg(i));
      average_degree.push_back(ruler.ComputeAverageDegree());
   }
   statusBar()->showMessage(tr("Finished"));
   //draw
   scn::Gnuplot plot_file("lines");
   plot_file.set_title("Average Degree");
   plot_file.savetopng("curve_average_degree");
   plot_file.plot_xy(node_number, average_degree);
   scn::Gnuplot plot_screen("lines");
   plot_screen.set_title("Average Degree");
   plot_screen.plot_xy(node_number, average_degree);
}

void MainWindow::DrawCurveOfClusteringCoeffTSD_SFSW()
{
   
   bool ok = false;
   size_t max_numberOfNodes = QInputDialog::getInteger(this, tr("TSD-SFSW Network"),
						       tr("Maximum Number of Nodes :"), 
						       1000, 1, 1000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
      return;
   }
    size_t numberOfSample = QInputDialog::getInteger(this, tr("TSD-SFSW Network"),
    						    tr("Number Of Samples :"), 
   						    10, 1, max_numberOfNodes, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
      return;
   }


   //run
   QProgressDialog progress("Computing Clustering Coefficient...", "Abort Computing",
   			    1, max_numberOfNodes, this);
   progress.setWindowModality(Qt::WindowModality::WindowModal);
   vector<double> node_number, cc;
   size_t steps = max_numberOfNodes / numberOfSample;
   ofstream outfile("data_clustering coefficient.txt", ios_base::trunc);
   outfile<<"#NumberOfNodes     #ClusteringCoefficient"<<endl;
   for(size_t i = 2; i <= max_numberOfNodes; i += steps)
   {
      progress.setValue(i);
      if(progress.wasCanceled())
    	 break;
      //run
      Ruler ruler(scn::GenTreeStructuredSFSW(i));
      node_number.push_back(i);
      double temp = ruler.GetClusteringCoeff();
      cc.push_back(temp);
      outfile<<i<<"    "<<temp<<endl;
   }
   outfile.close();
   //draw
   scn::Gnuplot plot_file("lines");
   //  plot_file.set_title("Clustering Coefficient");
   plot_file.set_xlabel("Number of Nodes");
   plot_file.set_ylabel("Clustering Coefficient");
   plot_file.set_xlogscale();
   plot_file.savetopng("curve_clustering_coefficient");
   plot_file.plot_xy(node_number, cc);
   scn::Gnuplot plot_screen("lines");
   plot_screen.set_title("Clustering Coefficient");
   plot_screen.set_xlogscale();
   plot_screen.plot_xy(node_number, cc);
}

void MainWindow::DrawCurveOfDiameterTSD_SFSW()
{
   
   bool ok = false;
   size_t max_numberOfNodes = QInputDialog::getInteger(this, tr("TSD-SFSW Network"),
						       tr("Maximum Number of Nodes :"), 
						       1000, 1, 1000000, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
      return;
   }
    size_t numberOfSample = QInputDialog::getInteger(this, tr("TSD-SFSW Network"),
    						    tr("Number Of Samples :"), 
   						    10, 1, max_numberOfNodes, 1, &ok);
   if(!ok)
   {
      QMessageBox::critical(this, tr("Error !"), tr("The input value is invalid!"));
      return;
   }

   //run
   QProgressDialog progress("Computing Average Distance and Diameter...", "Abort Computing",
   			    1, max_numberOfNodes, this);
   progress.setWindowModality(Qt::WindowModality::WindowModal);
   vector<double> node_number, diameter, average_distance;
   size_t steps = max_numberOfNodes / numberOfSample;
   //save data to text file
   ofstream outfile("data_diameter_average_distance.txt", ios_base::trunc);
   outfile<<"#NumberOfNodes   #Diameter    #AverageDistance"<<endl;

   for(size_t i = 2; i <= max_numberOfNodes; i += steps)
   {
      progress.setValue(i);
      if(progress.wasCanceled())
    	 break;
      //run
      Ruler ruler(scn::GenTreeStructuredSFSW(i));
      node_number.push_back(i);
      auto distance = ruler.GetDiameterAndAverageDistance();
      diameter.push_back(distance.first);
      average_distance.push_back(distance.second);
      outfile<<i<<"   "<<distance.first<<"   "<<distance.second<<endl;
   }
   outfile.close();
   //draw
   scn::Gnuplot plot_file("lines");
   plot_file.savetopng("curve_diameter_average distance");
   plot_file.set_xlabel("Number of Nodes");
   plot_file.set_xlogscale();
   plot_file.plot_xy(node_number, diameter, "Diameter");
   plot_file.plot_xy(node_number, average_distance, "Average Distance");
   scn::Gnuplot plot_screen("lines");
   plot_file.set_xlabel("Number of Nodes");
   plot_file.set_xlogscale();
   plot_screen.plot_xy(node_number, diameter, "Diameter");
   plot_screen.plot_xy(node_number, average_distance, "Average Distance");
}
