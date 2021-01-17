#ifndef GRAPHICBUS_HH
#define GRAPHICBUS_HH
#include "graphichost.hh"
#include <QDebug>

const QString BUS_IMAGE = ":/images/bus.png";

/**
 * @brief Draws busses and moves the graphical busses on the map
 */


class GraphicBus : public GraphicHost
{

class GraphicActor;

public:
    GraphicBus(int x, int y);
    ~GraphicBus();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int x_;
    int y_;
    bool mirror_bus_ = false;

};

#endif // GRAPHICBUS_HH
