#include "graphicstop.hh"


GraphicStop::GraphicStop(int x, int y):
    x_(x),
    y_(y)
{
    addImage(new QImage(STOP_IMAGE));
    setZValue(1);
    setPos(4461 + (x_*5), (15770 - ((15770-4941) + (y_*5))) - giveActorImages()[0]->height());
}


GraphicStop::~GraphicStop()
{

}


void GraphicStop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QRectF bounds = boundingRect();
    QFont font;
    QPen pen;

    font.setPixelSize(8);
    pen.setColor(QColor(0, 0, 0));

    painter->setFont(font);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(100, 180, 230)));
    painter->drawImage(bounds, *giveActorImages()[0]);
    painter->drawText(QRectF(7, 9, 20, 30),  QString::number(amountOfPassengersHosted()));

}
