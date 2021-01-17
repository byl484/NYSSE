#ifndef GRAPHICSTOP_HH
#define GRAPHICSTOP_HH
#include "graphichost.hh"
#include <QImage>

const QString STOP_IMAGE = ":/images/stop.png";

/**
 * @brief Draws graphical stops to the map
 */

class GraphicStop : public GraphicHost
{
public:
    GraphicStop(int x, int y);
    ~GraphicStop();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    int x_;
    int y_;

};
#endif // GRAPHICSTOP_HH
