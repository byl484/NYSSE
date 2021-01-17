#ifndef PLAYERLOGIC_HH
#define PLAYERLOGIC_HH
#include "interfaces/iactor.hh"
#include "graphicactor.hh"

#include <QDebug>

const QString HELICOPTER_IMAGE1 = ":/images/helicopter1.png";
const QString HELICOPTER_IMAGE2 = ":/images/helicopter2.png";
const int STEP_SIZE = 1;


/**
 * @brief Playerlogic executes players actions
 * @pre Bettermainwindow must exist
 * @post Player is created and is stationary somewhere in the gamemap.
 */

class PlayerLogic : public Interface::IActor
{
public:
    PlayerLogic(int start_x, int start_y);
    ~PlayerLogic();

    // Use movePlayer when moving player ingame with keyboard keys.
    void setNewCoords(int x_change, int y_change);

    GraphicActor* giveGraphicActor();
    Interface::Location giveLocation() const;

    // Unused methods.
    void move(Interface::Location loc);
    bool isRemoved() const;
    void remove();

private:
    GraphicActor* graphicPlayer_;
    Interface::Location location_;

};

#endif // PLAYERLOGIC_HH
