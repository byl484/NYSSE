#include "statistics.hh"


Statistics::Statistics(QGraphicsItem* parent)
{
    Q_UNUSED(parent)
    humans_eliminated_ = 0;
    busses_eliminated_ = 0;
    player_lives_ = 3;
    // Statistics will be shown at the top layer of scene.
    setZValue(6);
}


Statistics::~Statistics()
{

}


QRectF Statistics::boundingRect() const
{
    return QRectF(0, 0, 210, 120);
}


void Statistics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QRectF bounds = boundingRect();
    QFont font;
    QPen pen;
    QColor bg_color = QColor(0, 0, 0);
    QColor font_color = QColor(0, 0, 0);

    bg_color.setAlpha(70);

    font.setPixelSize(15);
    pen.setColor(font_color);

    painter->setFont(font);
    painter->setPen(pen);

    painter->setBrush(QBrush(bg_color));
    painter->drawRect(bounds);

    painter->drawText(bounds, Qt::AlignCenter, QString("Difficulty: " + difficulty_.difficulty_name + "\n" +
                                      "Humans eliminated: " + QString::number(humans_eliminated_) + "\n" +
                                      "Busses eliminated: " + QString::number(busses_eliminated_) + "\n" +
                                      "Lives left: " + QString::number(player_lives_) + "\n" +
                                      "Time left: " + QString::number(game_timer_->remainingTime()/1000) + "s"));

}


void Statistics::setTimer(QTimer* game_timer)
{
    game_timer_ = game_timer;
}


void Statistics::setDifficulty(int difficulty)
{
    difficulty_ = difficulties_[difficulty];
}


Difficulty Statistics::giveDifficulty()
{
    return difficulty_;
}


void Statistics::addEliminatedHumans(unsigned int amount)
{
    humans_eliminated_ += amount;
}


void Statistics::addEliminatedBusses(unsigned int amount)
{
    busses_eliminated_ += amount;
}


void Statistics::removeLife()
{
    if (player_lives_ > 0) {
        --player_lives_;
    }
}


int Statistics::givePlayerLives()
{
    return player_lives_;
}


bool Statistics::playerIsDead()
{
    if (player_lives_ == 0) {
        return true;
    }
    return false;
}
