#pragma once
#include<QtGui/QMainWindow>
#include<QtGui/QGraphicsView>
#include<QtGui/QMenu>
#include<QtGui/QAction>
#include<QtGui/QPushButton>
#include<QtGui/QDockWidget>
#include<QtGui/QStatusBar>
#include<thread>
#include "qnetwork.hpp"
#include "ruler_widget.hpp"

class MainWindow : public QMainWindow
{
   Q_OBJECT;
public:

   MainWindow();

   ~MainWindow();

private://private function
   
   void CreateActions();

   void CreateMenus();

   void CreateDockWidgets();
   
   void UpdateGraphivsView();

   void DrawAndComputing();
			
private slots://private slot
   
   void OpenFile();
   
   void SaveFile();
   
   void CreateRegularNetwork();

   void CreateWSSmallWorld();

   void CreateNWSmallWorld();

   void CreateRandomNetwork();
   
   void CreateScaleFree();

   void CreateSmallWorldByEdgeIter();

   void CreateUniformRecursiveTree();

   void CreateDURT();

   void CreateSmallWorldFromDURT();

   void CreateTreeStructuredSFSW();

   void CreateTreeStructuredSW();

   void CreateCommunityNetwork();
/**
 * @name DrawCurveOfAvgDegreeTSD_SFSW - Draw the curve of Average
 * Degree of Tree-Structured Scale-free Small-world network
 * @return void
 */
   void DrawCurveOfAvgDegreeTSD_SFSW();

   void DrawCurveOfClusteringCoeffTSD_SFSW();

   void DrawCurveOfDiameterTSD_SFSW();
   
private://action and menu
   QMenu* menu_file;
   QMenu* menu_create_network;
   QMenu* menu_model_analysis;

   //sub menu of menu_model_analysis
   QMenu* menu_curve_diameter;
   QMenu* menu_curve_average_distance;
   QMenu* menu_curve_average_degree;
   QMenu* menu_curve_clustering_coefficient;

   QAction* action_open_file;
   QAction* action_save_file;
   QAction* action_exit;

   QAction* action_create_regular_network;
   QAction* action_create_small_world_ws;
   QAction* action_create_small_world_nw;
   QAction* action_create_random_network;
   QAction* action_create_scale_free;
   QAction* action_small_world_edge_iter;
   QAction* action_uniform_recursive_tree;
   QAction* action_create_durt;
   QAction* action_small_world_from_durt;
   QAction* action_create_community_network;
   QAction* action_create_tsd_sfsw;
   QAction* action_create_tsd_sw;

   QAction* action_curve_avg_degree_tsd_sfsw;/**< Average degree of
					       * Tree-Structured
					       * deterministic scale-free
					       * small world network */
   QAction* action_curve_diameter_tsd_sfsw;/**<  Diameter curve of
					     * Tree-Structured
					     * scale-free small world network */
   QAction* action_curve_cc_tsd_sfsw;/**<  Clustering Coefficient
				       * curve of Tree-Structured
				       * scale-free small world network */
private://components
   QDockWidget*   dock_basic_info;
   URulerWidget*  measurement;
   QGraphicsView* graphics_view;
   std::shared_ptr<std::thread>        thread_compute;
   std::shared_ptr<scn::QUNetwork>     unetwork;
};
