#ifndef ENEMYLOGIC_HH
#define ENEMYLOGIC_HH
#include "interfaces/iactor.hh"
#include "graphicactor.hh"

#include <QDebug>
#include <random>


const QString ENEMY_HELICOPTER_IMAGE1 = ":/images/enemy_helicopter1.png";
const QString ENEMY_HELICOPTER_IMAGE2 = ":/images/enemy_helicopter2.png";


/**
 * @brief Enemylogic sets up enemies logic and calculates it's moves
 * also moves enemys graphical object on the gamemap
 *
 * @pre Before creating enemy, mainwindow, playerlogic must exist
 * @post Enemy is created and is stationary somewhere in the gamemap.
 */

class EnemyLogic : public Interface::IActor
{

public:

    // always set this object to be inside of bounds
    EnemyLogic(int start_x, int start_y, int step_size, int vision,
               int border_x_max, int border_x_min, int border_y_max, int border_y_min);
    ~EnemyLogic();

    Interface::Location giveLocation() const;
    GraphicActor* giveGraphics() const;

    bool isMoving() const;
    bool IsBounds() const;
    bool seePlayer(Interface::Location player_loc) const;
    bool hitPlayer(Interface::Location player_loc) const;

    void checkBounds();

    // these 5 are called by startMoving, keepMoving and chasePlayer functions:

    // check which directions are legal to move to
    void checkLegalMoveDirections();
    // this function simply calculates, how much x or y coordinates will be changed
    void calculateNextStep();
    void setNewCoords(int x, int y);
    void startMoving();
    void keepMoving();

    void chasePlayer(Interface::Location player_loc);

    // these functions exists because this class inherited them from IActor
    // we don't remove the enemies this way
    bool isRemoved() const;
    void remove();
    // Don't use move func, it exists because we inherited this class from IActor
    void move(Interface::Location loc);

private:
    GraphicActor* graphic_enemy_;
    Interface::Location location_;

    int vision_;
    const int step_size_ = 1;
    int speed_;

    int amount_move_commands_received_;
    int amount_speed_normalized_move_commands;

    int moving_dir_sign_;
    int amount_steps_left_;

    bool moving_;
    bool bounds_;
    bool move_is_diagonal;

    int border_x_min_;
    int border_y_min_;
    int border_x_max_;
    int border_y_max_;

    int x_to_be_changed_;
    int y_to_be_changed_;

};

#endif // ENEMYLOGIC_HH
