#include "graphicbus.hh"

#include <QDebug>
GraphicBus::GraphicBus(int x, int y):
    x_(x),
    y_(y)

{
    addImage(new QImage(BUS_IMAGE)); // No animation for bus, so only one pic ;(
    setZValue(2);

    // Redraw every bus at construction, so they appear in right places.
    updateGraphicPosition();
}


GraphicBus::~GraphicBus()
{

}

void GraphicBus::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // Set busses graphical position on scene if needed.
    updateGraphicPosition();

    QRectF bounds = boundingRect();
    QFont font;
    QPen pen;

    font.setPixelSize(8);
    pen.setColor(QColor(0, 0, 0));

    painter->setFont(font);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(255, 0, 0)));

    int rotation_degree_ = giveRotationDegree();

    // If bus wont be mirrored, this will be passenger amount x coordinate.
    int passenger_amount_x_coord = 10;

    // Check if bus needs to be mirrored.
    if ((rotation_degree_ == 45 ) || (rotation_degree_ == 90) || (rotation_degree_ == 135)) {
       mirror_bus_ = true;
       passenger_amount_x_coord = 51;
    } else if ((rotation_degree_ == 255) || (rotation_degree_ == 270) || (rotation_degree_ == 315)) {
        mirror_bus_ = false;
    }

    painter->drawImage(bounds, giveActorImages()[0]->mirrored(mirror_bus_, false));

    // Shows amount of passengers onboard. Will be drawn on window.
    painter->drawText(QRect(passenger_amount_x_coord, 7, bounds.width(), bounds.height()),
                      QString::number(amountOfPassengersHosted()));
}
