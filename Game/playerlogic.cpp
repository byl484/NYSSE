#include "playerlogic.hh"


PlayerLogic::PlayerLogic(int start_x, int start_y):
    graphicPlayer_(new GraphicActor)

{
    // GraphicPlayers Destructor removes these images.
    graphicPlayer_->addImage(new QImage(HELICOPTER_IMAGE1));
    graphicPlayer_->addImage(new QImage(HELICOPTER_IMAGE2));

    graphicPlayer_->setGraphicCoords(start_x, start_y);
    graphicPlayer_->setZValue(4);
    graphicPlayer_->updateGraphicPosition();

    location_.setXY(start_x, start_y);
}


PlayerLogic::~PlayerLogic()
{
    delete graphicPlayer_;
    graphicPlayer_ = nullptr;
}


Interface::Location PlayerLogic::giveLocation() const
{
    return location_;
}


void PlayerLogic::setNewCoords(int x_change, int y_change)
// Player is able to move simulatenously on both axis
// So diagonal movement is faster
// This game mechanic is intentional and necessary
// On INSANE difficulty
// Enemies don't move faster diagonally.
{
    y_change = y_change * STEP_SIZE;
    x_change = x_change * STEP_SIZE;

    int y = location_.giveY();
    int x = location_.giveX();

    y += y_change;
    x += x_change;

    location_.setXY(x, y);
    graphicPlayer_->setGraphicCoords(x, y);
}


GraphicActor* PlayerLogic::giveGraphicActor()
{
    return graphicPlayer_;
}


// Move() func is to set player to specific coordinate, but it does nothing,
// since we don't move the player to specific coordinate but rather
// change the current coordinates.
void PlayerLogic::move(Interface::Location loc)
{   qDebug() <<&loc;
    qDebug() << "Never call move func on player"
                "use movePlayer instead ";
}


// Never  call remove functions on players.
// These exists only because we inherited playerlogic from IActor.
bool PlayerLogic::isRemoved() const
{
    qDebug() << "Player's isRemoved was called!!!"
                "player object's isRemoved should never be called!!!";
    return false;
}


void PlayerLogic::remove()
{
    qDebug() << "Player object's remove method was called!!!"
                "Don't ever try to remove the player!!!!";
}
