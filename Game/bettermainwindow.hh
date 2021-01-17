#ifndef BETTERMAINWINDOW_HH
#define BETTERMAINWINDOW_HH
#include "interfaces/ipassenger.hh"
#include "statistics.hh"
#include "graphicbus.hh"
#include "graphicstop.hh"
#include "playerlogic.hh"
#include "enemylogic.hh"

#include <QMainWindow>
#include <QKeyEvent>
#include <map>
#include <random>


const QString TAMPERE_22862x15770 = ":/images/better_map_images/tampere_22862x15770.png";
const QString YOU_DIED_BANNER = ":/images/game_over_banner.png";
const QString YOU_WON_BANNER = ":/images/win_banner.png";
const QString BOMB_IMAGE_LOCATION = ":/images/bomb_images/";
const QList<QString> BOMB_FILE_NAMES = {"exp1.png", "exp2.png", "exp3.png", "exp4.png"};

/**
 * @brief Holds games graphicscene that is used to draw all of games graphics.
 * Handels game ticks. Used to create graphic objects on scene,
 * but does not move busses or passengers.
 * Moves enemies and handels player movement and bomb animation. Scrolls map.
 * @post - Game is starting
 */

namespace Ui
{
class BetterMainWindow;
}

class BetterMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BetterMainWindow(QWidget* parent = 0);
    virtual ~BetterMainWindow();

    // Stores statistics on private pointer and gives it to scene.
    void takeStatistics(std::shared_ptr<Statistics> statistics);

    // These create new graphicactors and add them to the graphicscene.
    std::shared_ptr<GraphicBus> addGraphicBus(int x, int y);
    std::shared_ptr<GraphicStop> addGraphicStop(int x, int y);

    // Removes actor from scene.
    void removeGraphicActor(std::shared_ptr<GraphicActor>);

    // Starts tick_timer and spawns enemies.
    void startGameGraphics();

    // Used by city when calculating distance between busses and player for
    // determinating if buss needs to be redrawn.
    Interface::Location givePlayerLocation();

    // Denies movement of player if it would put player out of the map.
    void keepPlayerInBounds();

    // Show banner image at end of the game. These are called by engine.
    void showYouWonBanner();
    void showYouDiedBanner();

    // Checked every tick by gameTickHandler.
    bool gameIsOver();

    // Used to end game if time ran out. Called by engine.
    void setGameOver();

signals:
    // Used to notify engine that bomb is dropped at given location.
    void dropBomb(Interface::Location);

    // Used to notify engine that game ended when player died.
    void gameOver();

private slots:
    // These handle player movement by listening keypresses.
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    // Sets correct bomb image to be shown. Will be activated with bomb_timer_.
    void bombAnimationHandler();

    // Updates player and enemy positions on screen. Used to redraw scene if needed.
    // Checks if player died and ends game if so. Ceeps the scene focussed on player.
    void gameTickHandler();

private:

    // Call this funktion only AFTER player has been created,
    // because this func checks player coordinates.
    void spawnEnemiesAmount(int amount);

    // This memberfunction is basically the same as Locations-class's
    // isClose memberfunc however we need this, because when creating enemies,
    // we DO NOT want to first create enemy object or location object
    // and then check if the randomised location is too close to the player and
    // then delete the object immediately if it's too close.
    // With this func, we check the locations before the object is created!
    bool enemyIsTooCloseToBeSpawned(int x, int y);

    // Moves enemies every tick and redraws them on scene if needed.
    void moveEnemies();

    // Plays bomb animation at players position.
    void playBombAnimation(int z = 3);

    bool game_over_ = false;
    const int tick_rate_ = 8;

    // These are the border coordinates. They are weird, because we are using
    // a bigger map and the location class has to work with the coordinates.
    // We cannot change the location class.
    const int border_x_min_ = -890;
    const int border_x_max_ = 3660;
    const int border_y_min_ = -2145;
    const int border_y_max_ = 980;

    // From how far away the enemy sees player and starts chasing it
    const int enemy_vision_ = 60;
    const int enemy_speed = 1;
    const int min_enemy_distance_at_start = 100;

    // These determinate the players next moving direction.
    int player_x_change_;
    int player_y_change_;

    // Information on what keys are held down. These need to be stored so that
    // the game understands multiable keypresses and releases simultaniously correctly.
    bool w_down_ = false;
    bool a_down_ = false;
    bool s_down_ = false;
    bool d_down_ = false;

    std::shared_ptr<Statistics> statistics_;
    QImage* map_image_;
    QGraphicsPixmapItem* you_died_banner_pixmap_;
    QGraphicsPixmapItem* you_won_banner_pixmap_;
    Ui::BetterMainWindow* ui_;
    QGraphicsScene* map_;
    PlayerLogic* player_;
    std::vector<EnemyLogic*> enemy_list_;

    QTimer* tick_timer_;
    QTimer* bomb_animation_timer_;

    // Used for bomb animation.
    std::vector<std::shared_ptr<QPixmap> > bomb_images_;
    QGraphicsPixmapItem* current_bomb_image_;
    unsigned long int current_bomb_image_index_;

};

#endif // BETTERMAINWINDOW_HH
