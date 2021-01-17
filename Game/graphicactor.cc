#include "graphicactor.hh"


GraphicActor::~GraphicActor()
{
    for (QImage* actor_image : actor_images_) {
        delete actor_image;
        actor_image = nullptr;
    }
}


QRectF GraphicActor::boundingRect() const
{
    // BoundingRect is determinated by actor images, if they are given.
    if (actor_images_.size() != 0) {
        return QRectF(0, 0, actor_images_[0]->width(), actor_images_[0]->height());
    } else {
        return QRectF(0, 0, 50, 50);
    }
}


void GraphicActor::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QRectF bounds = boundingRect();

    // Rotate painter around the center of the bounding rect.
    painter->translate(bounds.width()/2, bounds.height()/2);
    painter->rotate(rotation_degree_);
    painter->translate(-bounds.width()/2, -bounds.height()/2);

    // By default, actor is animated with given pictures.
    painter->drawImage(bounds, *actor_images_[current_actor_image_index_]);

    if (current_actor_image_index_ == actor_images_.size()-1) {
        current_actor_image_index_ = 0;
    } else {
        ++current_actor_image_index_;
    }
}


void GraphicActor::setGraphicCoords(int x, int y)
{
    // Converts coordinates to fit bigger map.
    x_ = 4461 + (x*5);
    y_ = 15770 - ((15770-4941) + (y*5));
}


void GraphicActor::updateGraphicPosition()
{
    // Center coordinates to actors current image.
    int new_x = x_ - (actor_images_[current_actor_image_index_]->width()/2);
    int new_y = y_ - (actor_images_[current_actor_image_index_]->height()/2);

    // If position hasn't changed, no redraw.
    if ((x() == new_x) && (y() == new_y)) {
        return;
    }

    // Calculate needed rotation degree from previous graphical postion.
    if (new_x > x()) {
        if (new_y < y()) {
            rotation_degree_ = 45;
        } else if (new_y == y()) {
            rotation_degree_ = 90;
        } else if (new_y > y()) {
            rotation_degree_ = 135;
        }

    } else if (new_x < x()) {
        if (new_y > y()) {
            rotation_degree_ = 225;
        } else if (new_y == y()) {
            rotation_degree_ = 270;
        } else if (new_y < y()) {
            rotation_degree_ = 315;
        }

    } else if ((new_x == x()) && (new_y > y())) {
        rotation_degree_ = 180;

    } else {
        rotation_degree_ = 0;
    }

    // Change graphical postion on scene -> repaints actor.
    setPos(new_x, new_y);
}


void GraphicActor::addImage(QImage* image)
{
    actor_images_.push_back(image);
}


int GraphicActor::giveGraphicX()
{
    return x_;
}


int GraphicActor::giveGraphicY()
{
    return y_;
}


std::vector<QImage*> GraphicActor::giveActorImages()
{
    return actor_images_;
}


int GraphicActor::giveRotationDegree()
{
    return rotation_degree_;
}
