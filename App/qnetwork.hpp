#pragma once
#include<network.hpp>
#include<string>
#include<memory>
#include<QColor>
#include<QGraphicsItem>
#include<QGraphicsScene>
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

using std::string;
using std::shared_ptr;
using std::weak_ptr;
using scn::UNetwork;
using scn::UGraph;

template<class GraphType> class QNodeItem;
class QEdgeItem;
///////////////////////////////////////////////////////////////////
template<class GraphType>
class QNetwork
        :virtual public scn::Network<GraphType, QNodeItem<GraphType>, QEdgeItem>
{
public:
    using typename scn::Network<GraphType, QNodeItem<GraphType>, QEdgeItem>::pEdge;
    using typename scn::Network<GraphType, QNodeItem<GraphType>, QEdgeItem>::pNode;

    QNetwork(typename GraphType::pGraph graph);

    ~QNetwork();
public:

    size_t CreateScene(size_t numberOfNode);

    void CreatePosition();

    QGraphicsScene* GetScene();

    QString GetPathName();

    virtual void RedrawNode(size_t indexOfNode) = 0;

protected:

    void CreateRandomPosition();

    void CreateCirclePosition();

    void SetNodeMoveable();

protected:

    shared_ptr<QGraphicsScene> scene;

    QString path_name;

    using scn::Network<GraphType, QNodeItem<GraphType>, QEdgeItem>::graph;
    using scn::Network<GraphType, QNodeItem<GraphType>, QEdgeItem>::node_data;
};

class QUNetwork
        :public QNetwork<UGraph>, public UNetwork<QNodeItem<UGraph>, QEdgeItem>
{
public:

    QUNetwork(UGraph::pGraph &graph);

    ~QUNetwork();
public:
 /**
 * @name ReadFromNetFile - read network information from a Pajek net
 * file. This function is a member of QUNetwork, thus, the given file
 * must represent a undirected network
 * @param path -  path
 * @return - new object of QUNetwork
 */
    static std::shared_ptr<QUNetwork> ReadFromNetFile(QString &path);

    void WriteToNetFile(QString &path);

    void DrawOnScene();

    virtual void RedrawNode(size_t indexOfNode);

protected:

     using scn::Network<UGraph, QNodeItem<UGraph>, QEdgeItem>::graph;
};

template<class GraphType>
class QNodeItem : public QGraphicsItem
{
public:

    QNodeItem(QNetwork<GraphType>* parent)
        :wparent(parent)
     {
        text_color = Qt::darkGreen;
        outline_color = Qt::darkBlue;
        background_color = Qt::white;
        setFlags(ItemIsMovable | ItemIsSelectable);
    }

    ~QNodeItem(){}

    void SetText(const QString &text)
    {
       prepareGeometryChange();
       this->text = text;
       update();
    }

    QString GetText() const
    {
       return text;
    }

    void SetTextColor(const QColor &color)
    {
       text_color = color;
       update();
    }

    QColor GetTextColor() const
    {
       return text_color;
    }

    void SetOutlineColor(const QColor &color)
    {
       outline_color = color;
       update();
    }

    QColor GetOutlineColor() const
    {
       return outline_color;
    }

    void SetBackgroundColor(const QColor &color)
    {
       background_color = color;
       update();
    }
    QColor GetBackgroundColor() const
    {
       return background_color;
    }

    QRectF boundingRect() const
    {
       return outlineRect().adjusted(-1, -1, 1, 1);
    }

    QPainterPath shape() const
    {
       QRectF rect = outlineRect();

       QPainterPath path;
       path.addRoundRect(rect, roundness(rect.width()),
                 roundness(rect.height()));
       return path;
    }

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget)
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

public://public data

    size_t indexOfNode;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        QString text = QInputDialog::getText(event->widget(),
                               "Edit Text", "Enter new text:",
                               QLineEdit::Normal, this->text);
        if (!text.isEmpty())
            SetText(text);
    }

    QVariant itemChange(GraphicsItemChange change,
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


private:
    QRectF outlineRect() const
    {
        const int Padding = 6;
        QFontMetricsF metrics = static_cast<QFontMetricsF>(qApp->font());
        QRectF rect = metrics.boundingRect(text);
        rect.adjust(-Padding, -Padding, +Padding, +Padding);
        rect.translate(-rect.center());
        return rect;
    }

    int roundness(double size) const
    {
        const int Diameter = 8;
        return 50 * Diameter / int(size);
    }

    QNetwork<GraphType>* wparent;

    QString text;
    QColor text_color;
    QColor background_color;
    QColor outline_color;
};

class QEdgeItem : public QGraphicsItem
{
public:

    QEdgeItem(QPointF &&pos1, QPointF &&pos2, bool isDirected = false);

    ~QEdgeItem();

    void UpdatePosition(QPointF &&pos1, QPointF &&pos2);

    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);

public://edge data
    double weight;
protected:

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    bool isDashed;
    bool isDirected;
    QPointF head;
    QPointF tail;
};

/////////////////////////////////////////////////////////////////////
template<class GraphType>
QNetwork<GraphType>::QNetwork(typename GraphType::pGraph graph)
    :scn::Network<GraphType, QNodeItem<GraphType>, QEdgeItem>(graph)
{
    CreateScene(graph->GetNumberOfNodes());
    //create draw node
    for(auto node = graph->begin(); node != graph->end(); node++)
    {
       pNode data = new QNodeItem<GraphType>(this);
       data->indexOfNode = *node;
       data->SetText(QString("%1").arg(*node));

       scn::Network<UGraph, QNodeItem<UGraph>, QEdgeItem>::SetNodeData(node, data);
    }
    //create position
    CreateCirclePosition();
}

template<class GraphType>
QNetwork<GraphType>::~QNetwork()
{
}

template<class GraphType>
size_t QNetwork<GraphType>::CreateScene(size_t numberOfNode)
{
   size_t size = 100 * static_cast<size_t>(sqrt(numberOfNode));
   scene.reset(new QGraphicsScene(0, 0, size, size));
   return size;
}

template<class GraphType>
void QNetwork<GraphType>::SetNodeMoveable()
{
   for(auto node = node_data.begin(); node != node_data.end(); node++)
   {
      node->second->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
   }
}

template<class GraphType>
void QNetwork<GraphType>::CreateCirclePosition()
{
   QPointF center(scene->width() / 2, scene->height() / 2);
   double radius = min(center.x(), center.y());
   double angle = 0;
   double delta = 2* 3.14 / graph->GetNumberOfNodes();

   for(auto node = graph->begin(); node != graph->end(); node++)
   {
      pNode data = scn::Network<GraphType, QNodeItem<GraphType>, QEdgeItem>::GetNodeData(node);
      data->setPos(center.x() + radius * cos(angle),
           center.y() + radius * sin(angle));
      data->setZValue(0);
      angle += delta;
   }
}

template<class GraphType>
void QNetwork<GraphType>::CreateRandomPosition()
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

template<class GraphType>
QGraphicsScene* QNetwork<GraphType>::GetScene()
{
   if(scene)
      return scene.get();
   else
      return nullptr;
}

template<class GraphType>
void QNetwork<GraphType>::CreatePosition()
{

}


template<class GraphType>
QString QNetwork<GraphType>::GetPathName()
{
   return path_name;
}
