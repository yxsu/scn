#include "qnetwork.hpp"

#include<cmath>
#include<ctime>
using namespace scn;
using namespace std;

QEdgeItem::QEdgeItem(QPointF &&head, QPointF &&tail, bool isDirected)
   :weight(1)
{
   isDashed = false;
   this->isDirected = isDirected;
   this->head = head;
   this->tail = tail;
   setFlags(QGraphicsItem::ItemIsSelectable);
   setZValue(-1);
}

QEdgeItem::~QEdgeItem()
{
}

void QEdgeItem::paint(QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget*  widget)
{
    painter->setPen(QPen(Qt::darkRed, 1.0));
    //draw line
    painter->drawLine(head, tail);
    //draw triangle
    if(this->isDirected)
    {
        QLineF line(head, tail);
        line.setP1(QPointF((head.x() + tail.x())/2, (head.y() + tail.y())/2));
        line.setLength(5);
        QPointF points[3];
        points[0] = line.p2();
        line.setAngle(line.angle() + 90);
        points[1] = line.p2();
        line.setAngle(line.angle() + 180);
        points[2] = line.p2();
        painter->drawPolygon(points, 3);
    }
}

QRectF QEdgeItem::boundingRect() const
{

    return QRectF(min(head.x(), tail.x()), min(head.y(), tail.y()),
                  abs(head.x() - tail.x()), abs(head.y() - tail.y())).adjusted(-1, -1, 1, 1);
}

void QEdgeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
   isDashed = ! isDashed;
   /*
   QPen pen;
   if(isDashed)
      //setPen(QPen(Qt::darkRed, 1.0, Qt::DashDotLine));
   else
      //setPen(QPen(Qt::darkRed, 1.0));
   
   setLine(line());
   */
}

void QEdgeItem::UpdatePosition(QPointF &&head, QPointF &&tail)
{
    this->head = head;
    this->tail = tail;
    update(boundingRect());
}

QUNetwork::QUNetwork()
    :QNetwork<UGraph>()
{
}

QUNetwork::QUNetwork(UGraph::pGraph &graph)
      :QNetwork<UGraph>(graph)
{
   //add draw edge
   pEdge data;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      for(auto other = node->begin(); other != node->end(); other++)
      {
     data = new QEdgeItem(pnetwork->GetNodeData(node)->pos(),
                  pnetwork->GetNodeData(*other)->pos());
	 
     pnetwork->SetEdgeData(node, *other, data);
      }
   }
}

QUNetwork::~QUNetwork()
{

}

void QUNetwork::RedrawNode(size_t indexOfNode)
{
   auto node = pnetwork->GetTopology()->find(indexOfNode);
   for(auto other = node->begin(); other != node->end(); other++)
   {
       pnetwork->GetEdgeData(node, *other)->UpdatePosition(
                   pnetwork->GetNodeData(node)->pos(),
                   pnetwork->GetNodeData(*other)->pos());
   }
}

void QUNetwork::DrawOnScene()
{
    auto graph = pnetwork->GetTopology();
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
       pNode data = pnetwork->GetNodeData(node);
      scene->addItem(data);//add node
      for(auto other = node->begin(); other != node->end(); other++)
      {
     if(*node < *other)//avoid draw twice
        scene->addItem(pnetwork->GetEdgeData(node, *other));//add edge
      }
   }
   SetNodeMoveable();
}

QUNetwork&& QUNetwork::ReadFromNetFile(QString &path)
{
   //open file
   QFile file(path);
   file.open(QFile::ReadOnly);
   QTextStream reader(&file);
   //read header
   QString flag;
   reader>>flag;
   if(flag != "*Vertices")
   {
      QMessageBox::critical(nullptr, "Error !", "This file does not begin with *Vertices");
      file.close();
      return QUNetwork();
   }
   //create graph and network
   UGraph::pGraph graph(new UGraph());
   QUNetwork network = QUNetwork(graph);
   size_t numberOfNodes;
   reader>>numberOfNodes;
   size_t scene_size = network.CreateScene(numberOfNodes);
   //read nodes
   QString line = reader.readLine();
   size_t indexOfNode, indexOfNode2;
   double position, weight;
   QString label;
   QTextStream line_reader;
   pNode node_data;
   while(line != "*Arcs")
   {
      if(line == "")
      {
	 line = reader.readLine();
	 continue;
      }  
      line_reader.setString(&line);
      //read index and label
      line_reader>>indexOfNode>>label;
      node_data = new QNodeItem<UGraph>(&network);
      node_data->indexOfNode = indexOfNode;
      node_data->SetText(label);
      //read position
      line_reader>>position;
      node_data->setX(position * scene_size);
      line_reader>>position;
      node_data->setY(position * scene_size);
      if((line_reader>>position).status() != QTextStream::ReadCorruptData)
	 node_data->setZValue(position);
      else
	 node_data->setZValue(0);
      //add node and data
      graph->AddNode(indexOfNode);
      network(indexOfNode) = node_data;
      //read next line
      line = reader.readLine();
   }
   line = reader.readLine();//line = *Edges
   line = reader.readLine();
   pEdge edge_data;
   while(!line.isNull())
   {
      line_reader.setString(&line);
      line_reader>>indexOfNode>>indexOfNode2>>weight;
      edge_data = new QEdgeItem(network.GetNodeData(indexOfNode)->pos(),
				network->GetNodeData(indexOfNode2)->pos());
      graph->AddEdge(indexOfNode, indexOfNode2);
      network.SetEdgeData(indexOfNode, indexOfNode2, edge_data);
      line = reader.readLine();
   }
   file.close();
   assert(graph->GetNumberOfNodes() == numberOfNodes);
   network.path_name = path;
   return move(network);
}

void QUNetwork::WriteToNetFile(QString &path)
{
   QFile file(path);
   if(!file.open(QFile::WriteOnly))
   {
      QMessageBox::critical(nullptr, "Error !", "The filename is not writable!");
   }
   float scene_size = scene->width();
   QTextStream writer(&file);
   writer<<"*Vertices "<<graph->GetNumberOfNodes()<<endl;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      auto data = GetNodeData(node);
      writer<<*node + 1<<"  "<<data->GetText()<<"    "<<data->x() / scene_size<<" "
	    <<data->y() / scene_size<<" "<<data->zValue()<<endl;
   }
   writer<<"*Arcs"<<endl;
   writer<<"*Edges"<<endl;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(*other < *node)
	 {
	    writer<<*node + 1<<" "<<*other + 1<<" "<<GetEdgeData(node, *other)->weight<<endl;
	 }
      }
   }
   file.close();
}

UGraph::pGraph QUNetwork::GetTopology()
{
    return pnetwork->GetTopology();
}
