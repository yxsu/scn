#include "qnetwork.hpp"
#include<QStyleOptionGraphicsItem>
#include<QGraphicsSceneMouseEvent>
#include<QPainter>
#include<QInputDialog>
#include<QLineEdit>
#include<QApplication>
#include<QMessageBox>
#include<QProgressDialog>
#include<QtCore/QDebug>
#include<QtCore/QFile>
#include<QtCore/QTextStream>
#include<QtCore/QString>
#include<cmath>
#include<ctime>
using namespace scn;
using namespace std;

QNodeItem::QNodeItem(QUNetwork* parent)
   :wparent(parent)
{
   text_color = Qt::darkGreen;
   outline_color = Qt::darkBlue;
   background_color = Qt::white;
   setFlags(ItemIsMovable | ItemIsSelectable);
}

QNodeItem::~QNodeItem()
{

}

void QNodeItem::SetText(const QString &text)
{
   prepareGeometryChange();
   this->text = text;
   update();
}

QString QNodeItem::GetText() const
{
   return text;
}

void QNodeItem::SetTextColor(const QColor &color)
{
   text_color = color;
   update();
}

QColor QNodeItem::GetTextColor() const
{
   return text_color;
}

void QNodeItem::SetOutlineColor(const QColor &color)
{
   outline_color = color;
   update();
}

QColor QNodeItem::GetOutlineColor() const
{
   return outline_color;
}

void QNodeItem::SetBackgroundColor(const QColor &color)
{
   background_color = color;
   update();
}

QColor QNodeItem::GetBackgroundColor() const
{
   return background_color;
}

QRectF QNodeItem::boundingRect() const
{
   return outlineRect().adjusted(-1, -1, 1, 1);
}

QPainterPath QNodeItem::shape() const
{
   QRectF rect = outlineRect();

   QPainterPath path;
   path.addRoundRect(rect, roundness(rect.width()),
		     roundness(rect.height()));
   return path;
}

void QNodeItem::paint(QPainter *painter,
		      const QStyleOptionGraphicsItem *option,
		      QWidget*  widget)
{
    QPen pen(outline_color);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(1);
    }
    painter->setPen(pen);
    painter->setBrush(background_color);

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    painter->setPen(text_color);
    painter->drawText(rect, Qt::AlignCenter, text);
}

void QNodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QString text = QInputDialog::getText(event->widget(),
                           "Edit Text", "Enter new text:",
                           QLineEdit::Normal, this->text);
    if (!text.isEmpty())
        SetText(text);
}

QVariant QNodeItem::itemChange(GraphicsItemChange change,
			       const QVariant &value)
{
    if (change == ItemPositionHasChanged) 
    {
       //report to qnetwork
       if(wparent)
       {
	  wparent->RedrawNode(indexOfNode);
       }
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF QNodeItem::outlineRect() const
{
    const int Padding = 6;
    QFontMetricsF metrics = static_cast<QFontMetricsF>(qApp->font());
    QRectF rect = metrics.boundingRect(text);
    rect.adjust(-Padding, -Padding, +Padding, +Padding);
    rect.translate(-rect.center());
    return rect;
}

int QNodeItem::roundness(double size) const
{
    const int Diameter = 8;
    return 50 * Diameter / int(size);
}

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

size_t QUNetwork::CreateScene(size_t numberOfNode)
{
   size_t size = 100 * static_cast<size_t>(sqrt(numberOfNode));
   scene.reset(new QGraphicsScene(0, 0, size, size));
   return size;
}

QUNetwork::QUNetwork(UGraph::pGraph &graph)
   :UNetwork(graph)
{
   CreateScene(graph->GetNumberOfNodes());
   //create draw node
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      pNode data = new QNodeItem(this);
      data->indexOfNode = *node;
      data->SetText(QString("%1").arg(*node));

      SetNodeData(node, data);
   }
   //create position
   CreateCirclePosition();
   //add draw edge
   pEdge data;
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 data = new QEdgeItem(GetNodeData(node)->pos(),
                  GetNodeData(*other)->pos());
	 
	 SetEdgeData(node, *other, data);
      }
   }
}

void QUNetwork::SetNodeMoveable()
{
   for(auto node = node_data.begin(); node != node_data.end(); node++)
   {
      node->second->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
   }
}

QUNetwork::~QUNetwork()
{

}

void QUNetwork::CreateCirclePosition()
{
   QPointF center(scene->width() / 2, scene->height() / 2);
   double radius = min(center.x(), center.y());
   double angle = 0;
   double delta = 2* 3.14 / graph->GetNumberOfNodes();
   
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      pNode data = GetNodeData(node);
      data->setPos(center.x() + radius * cos(angle),
		   center.y() + radius * sin(angle));
      data->setZValue(0);
      angle += delta;
   }
}

void QUNetwork::CreateRandomPosition()
{
   srand(time(00));

   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      pNode data = GetNodeData(node);
      data->setPos(rand() % int(scene->width()),
		   rand() % int(scene->height()));
      data->setZValue(0);
   }
}

void QUNetwork::DrawOnScene()
{
   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      scene->addItem(GetNodeData(node));//add node
      for(auto other = node->begin(); other != node->end(); other++)
      {
	 if(*node < *other)//avoid draw twice
	    scene->addItem(GetEdgeData(node, *other));//add edge
      }
   }
   SetNodeMoveable();
}

QGraphicsScene* QUNetwork::GetScene()
{
   if(scene)
      return scene.get();
   else
      return nullptr;
}

void QUNetwork::CreatePosition()
{
   
}

void QUNetwork::RedrawNode(size_t indexOfNode)
{
   auto node = graph->find(indexOfNode);
   for(auto other = node->begin(); other != node->end(); other++)
   {
       GetEdgeData(node, *other)->UpdatePosition(
                   GetNodeData(node)->pos(), GetNodeData(*other)->pos());
   }
}

QString QUNetwork::GetPathName()
{
   return path_name;
}

std::shared_ptr<QUNetwork> QUNetwork::ReadFromNetFile(QString &path)
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
      return nullptr;
   }
   //create graph and network
   UGraph::pGraph graph(new UGraph());
   shared_ptr<QUNetwork> network(new QUNetwork(graph));
   size_t numberOfNodes;
   reader>>numberOfNodes;
   size_t scene_size = network->CreateScene(numberOfNodes);
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
      node_data = new QNodeItem(network.get());
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
      network->SetNodeData(indexOfNode, node_data);
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
      edge_data = new QEdgeItem(network->GetNodeData(indexOfNode)->pos(),
				network->GetNodeData(indexOfNode2)->pos());
      graph->AddEdge(indexOfNode, indexOfNode2);
      network->SetEdgeData(indexOfNode, indexOfNode2, edge_data);
      line = reader.readLine();
   }
   file.close();
   assert(graph->GetNumberOfNodes() == numberOfNodes);
   network->path_name = path;
   return network;
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
