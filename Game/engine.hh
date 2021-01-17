#ifndef ENGINE_HH
#define ENGINE_HH
#include "core/logic.hh"
#include "city.hh"
#include "startwindow.hh"

#include <QObject>

/**
 * @brief Sets up games main components, runs them.
 *
 * @pre -
 * @post startwindow, statistics, mainwindow, logic is created
 */

class Engine : public QObject
{
    Q_OBJECT

public:
    explicit Engine(QObject* parent = nullptr);
    ~Engine();

private:
    StartWindow* start_window_;
    std::shared_ptr<Statistics> statistics_;
    std::shared_ptr<BetterMainWindow> main_window_;
    std::shared_ptr<Interface::City> city_;
    CourseSide::Logic* logic_;
    QTimer* game_timer_; // Indicates how long a game lasts.

public slots:

    // Will activate when Space is pressed. Checks if any actors are found close
    // enough to the dropped bomb and removes.
    void bombDropped(Interface::Location);

    // Stops timers and shows winning/losing banner to player.
    void endGame();

private slots:

    // StartWindow activates this when user presses start game button. Difficulty
    // is given as parameter.
    void startGame(unsigned int);

};

#endif // ENGINE_HH
