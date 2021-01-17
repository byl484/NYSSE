#ifndef GRAPHICACTOR_HH
#define GRAPHICACTOR_HH

#include <QGraphicsItem>
#include <QPainter>

/**
 * @brief Acts as base class for every graphical actor (= busses, stops, enemies and player).
 */

class GraphicActor : public QGraphicsItem
{

public:
    GraphicActor() = default;
    ~GraphicActor();

    // boundingrect is determinated with given first image.
    QRectF boundingRect() const;

    // Default paint method animates actor with given pictures.
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    // Saves given coordinates to private section. DOES NOT REDRAW ACTOR!
    void setGraphicCoords(int x, int y);

    // Redraws actor by calling setPos on current coordinates.
    void updateGraphicPosition();

    // Used to add image/images that represent the actor.
    void addImage(QImage* image);

    // Returns current graphical position, x_ or y_. This isn't necessarily the
    // position on scene, if the actor haven't been redrawn yet. It should be
    // also noted that since actors are always drawn so that their graphics
    // are centered around x_ and y_, these will actually give the center point
    // of the graphics.
    int giveGraphicX();
    int giveGraphicY();

    std::vector<QImage*> giveActorImages();
    int giveRotationDegree();

private:
    int x_;
    int y_;
    int rotation_degree_ = 0;
    unsigned long int current_actor_image_index_ = 0;
    std::vector<QImage*> actor_images_;

};

#endif // GRAPHICACTOR_HH
