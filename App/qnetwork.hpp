#pragma once
#include<network.hpp>
#include<string>
#include<memory>
#include<QColor>
#include<QGraphicsItem>
#include<QGraphicsScene>

using std::string;
using std::shared_ptr;
using std::weak_ptr;
using scn::UNetwork;
using scn::UGraph;

class QUNetwork;

class QNodeItem : public QGraphicsItem
{
public:

    QNodeItem(QUNetwork* parent);
    ~QNodeItem();

    void SetText(const QString &text);
    QString GetText() const;
    void SetTextColor(const QColor &color);
    QColor GetTextColor() const;
    void SetOutlineColor(const QColor &color);
    QColor GetOutlineColor() const;
    void SetBackgroundColor(const QColor &color);
    QColor GetBackgroundColor() const;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
public://public data

    size_t indexOfNode;

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value);

private:
    QRectF outlineRect() const;
    int roundness(double size) const;

    QUNetwork* wparent;

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

class QUNetwork
        :public UNetwork<QNodeItem, QEdgeItem>
{
public:

    QUNetwork(UGraph::pGraph &graph);

    QUNetwork(std::string net_file_name);

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

    size_t CreateScene(size_t numberOfNode);

    void RedrawNode(size_t indexOfNode);

    void CreatePosition();

    QGraphicsScene* GetScene();
      
    QString GetPathName();

protected:

    void CreateRandomPosition();

    void CreateCirclePosition();

    void SetNodeMoveable();

protected:

    shared_ptr<QGraphicsScene> scene;

    QString path_name;
};
