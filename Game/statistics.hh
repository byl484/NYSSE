#ifndef STATISTICS_HH
#define STATISTICS_HH

#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>


struct Difficulty {
    QString difficulty_name;
    int time;
    int amount_of_enemies;
    // Speed scaling gives diminishing increase in speed, more we increase it
    // So it doesnt scale up linearly.
    int enemy_speed;
    int enemy_vision;
};

const Difficulty EASY = {
    "Easy", 200, 500, 2, 60
};

const Difficulty MEDIUM = {
    "Medium", 200, 600, 3, 60
};

const Difficulty HARD = {
    "Hard", 200, 700, 6, 60
};

const Difficulty INSANE = {
    "Insane", 200, 700, 36, 60
};

/**
 * @brief Manages statistics window in gamewindow
 * keeps track of game stats
 */

class Statistics : public QGraphicsItem
{

public:
    Statistics(QGraphicsItem* parent = nullptr);
    ~Statistics();

    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    void setTimer(QTimer* game_timer);
    void setDifficulty(int difficulty);
    Difficulty giveDifficulty();
    void addEliminatedHumans(unsigned int amount);
    void addEliminatedBusses(unsigned int amount);
    void removeLife(); // Called when an enemy touches player.
    int givePlayerLives();
    bool playerIsDead();


private:
    std::vector<Difficulty> difficulties_ = {EASY, MEDIUM, HARD, INSANE};
    Difficulty difficulty_;
    int humans_eliminated_;
    int busses_eliminated_;
    int player_lives_;
    QTimer* game_timer_;

};

#endif // STATISTICS_HH
