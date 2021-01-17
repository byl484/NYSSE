#include "enemylogic.hh"


EnemyLogic::EnemyLogic(int start_x, int start_y, int speed, int vision,
                       int border_x_max, int border_x_min, int border_y_max, int border_y_min):
   graphic_enemy_(new GraphicActor)
{
   //graphicPlayers Destructor removes these images.
   graphic_enemy_->addImage(new QImage(ENEMY_HELICOPTER_IMAGE1));
   graphic_enemy_->addImage(new QImage(ENEMY_HELICOPTER_IMAGE2));
   graphic_enemy_->setGraphicCoords(start_x, start_y);
   graphic_enemy_->setZValue(4);
   graphic_enemy_->updateGraphicPosition();

   vision_ = vision;
   speed_ = speed;
   border_x_max_ = border_x_max;
   border_x_min_ = border_x_min;
   border_y_max_ = border_y_max;
   border_y_min_ = border_y_min;

   // At the start, every enemy is inside of the map because the randomiser
   // at the bettermainwindow takes map size and coords into the account
   bounds_ = true;
   moving_ = false;
   location_.setXY(start_x, start_y);
   amount_move_commands_received_ = 0;
}


EnemyLogic::~EnemyLogic()
{
    delete graphic_enemy_;
}


Interface::Location EnemyLogic::giveLocation() const
{
    return location_;
}


GraphicActor* EnemyLogic::giveGraphics() const
{
    return graphic_enemy_;
}


bool EnemyLogic::isMoving() const
{
    return moving_;
}


bool EnemyLogic::IsBounds() const
{
    return bounds_;
}


void EnemyLogic::checkBounds()
{
    if (location_.giveX() >= border_x_max_ or location_.giveX() <=border_x_min_
            or location_.giveY() >= border_y_max_ or location_.giveY() <= border_y_min_) {
        bounds_ = false;
    } else {
        bounds_ = true;
      }
}


void EnemyLogic::checkLegalMoveDirections()
{
    // This moving algorhitm cheks if the enemy is at a border of the game area.
    // If the object is at the border, dont cross it.
    // Instead of listing illegal moves, we list legal moves so
    // the enemy bounces off the border and doesnt drag on it this way.
    std::vector<int> legal_moves;

    // 1 = south, 2 = east, 3 = north, 4 = west
    if (location_.giveY() <= border_y_min_) {
        legal_moves.push_back(3);
    }

    if (location_.giveX() <= border_x_min_) {
        legal_moves.push_back(2);
    }

    if (location_.giveY() >= border_y_max_) {
        legal_moves.push_back(1);
    }

    if (location_.giveX() >= border_x_max_) {
        legal_moves.push_back(4);
    }

    // If legal move list is empty, every move is legal.
   if (legal_moves.size() == 0) {
    moving_dir_sign_ = (rand() % 4) + 1;

   } else {
       // If the move is not on the legal moves vector, randomise until the move is on the list.
       while (std::find(legal_moves.begin(), legal_moves.end(), moving_dir_sign_)
              == legal_moves.end()) {
       moving_dir_sign_ = (rand() % 4) + 1;
      }
   }
}


void EnemyLogic::calculateNextStep()
{
    // Set these to zero from previous move.
    x_to_be_changed_ = 0;
    y_to_be_changed_ = 0;

    if (moving_dir_sign_ == 1) {
        y_to_be_changed_ = step_size_ * -1;
    } else if (moving_dir_sign_ ==2) {
        x_to_be_changed_ = step_size_ * 1;
    } else if (moving_dir_sign_ ==3) {
        y_to_be_changed_ = step_size_ * 1;
    } else if (moving_dir_sign_==4) {
        x_to_be_changed_ = step_size_ * -1;
    }
}


void EnemyLogic::setNewCoords(int new_x, int new_y)
{
    // Depending on the speed (which comes from difficulty)
    // skip moves in cretain intervals to slow enemies down.
    // We are using this implementation
    // because we wanted to have one gamewide high tick rate and a smooth experience.
    amount_move_commands_received_++;
    amount_speed_normalized_move_commands++;

    if (amount_move_commands_received_ % speed_ == 0) {
        new_x = 0;
        new_y = 0;
        amount_speed_normalized_move_commands--;
    }
    // If moving diagonally, slow the speed by a third
    // Third is not exactly 1/sqrt(2), which would be the real
    // number to multiply the movement speed by in order to have same
    // speed diagonally as vertically and horizontally.
    // We can't use that, since we are dealing with integers
    // The difference is unnoticeable
    if (move_is_diagonal and amount_speed_normalized_move_commands % 3 == 0) {
        new_x = 0;
        new_y = 0;
    }

    int current_y = location_.giveY();
    int current_x = location_.giveX();

    int next_x = current_x + new_x;
    int next_y = current_y + new_y;

    // Move both, logical and graphical object
    location_.setXY(next_x, next_y);
    graphic_enemy_->setGraphicCoords(next_x, next_y);
}


void EnemyLogic::startMoving()
{
    moving_ = true;

    // Randomise, how many steps enemy takes to the next direction.
    amount_steps_left_ = rand() % 500 + 1;
    checkLegalMoveDirections();
    bounds_ = true;
    keepMoving();
}


void EnemyLogic::keepMoving()
{
    checkBounds();
    --amount_steps_left_;
    calculateNextStep();
    setNewCoords(x_to_be_changed_, y_to_be_changed_);

    // If there is 0 steps left, the enemy randomises a new direction and amount of steps.
    if (amount_steps_left_ <= 0) {
        moving_ = false;
    }
}


void EnemyLogic::chasePlayer(Interface::Location player_loc)
{
    // Only the randomized sequence tracks steps.
    amount_steps_left_ = 0;
    moving_ = true;
    move_is_diagonal = false;

    int player_x = player_loc.giveX();
    int player_y = player_loc.giveY();

    int difference_x = abs(location_.giveX() - player_x);
    int difference_y = abs(location_.giveY() - player_y);

    int x_change = 0;
    int y_change = 0;
    // only move on the specific axis if the difference is great enough
    if (difference_x > 6){
         if (player_x > location_.giveX()) {
             x_change = 1 * step_size_;
         } else {
             x_change = -1 * step_size_;
         }
    }

    if (difference_y > 6){
        if (player_y > location_.giveY()) {
            y_change = 1 * step_size_;
        } else {
            y_change = -1 * step_size_;
        }
    }

        if (y_change != 0 and x_change != 0){
            move_is_diagonal = true;
        }

        setNewCoords(x_change, y_change);
}


bool EnemyLogic::seePlayer(Interface::Location player_loc) const
{
    return location_.isClose(player_loc, vision_);

}


bool EnemyLogic::hitPlayer(Interface::Location player_loc) const
{
    return location_.isClose(player_loc, 10);
}


// The bottom 3 aren't used.
bool EnemyLogic::isRemoved() const
{
    return false;
}


void EnemyLogic::remove()
{
    qDebug() << "EnemyLogic remove() was called"
                "it does nothing, don't use it";
}


void EnemyLogic::move(Interface::Location loc)
{
    Q_UNUSED(loc)
    qDebug() << "Ne need to move enemy to specific coordinate "
                "so this func does nothing";
}
