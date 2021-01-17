#include "bettermainwindow.hh"
#include "ui_bettermainwindow.h"


BetterMainWindow::BetterMainWindow(QWidget* parent) :
    QMainWindow(parent),
    map_image_(new QImage(TAMPERE_22862x15770)),
    you_died_banner_pixmap_(new QGraphicsPixmapItem(YOU_DIED_BANNER)),
    you_won_banner_pixmap_(new QGraphicsPixmapItem(YOU_WON_BANNER)),
    ui_(new Ui::BetterMainWindow),
    player_(new PlayerLogic(274, 234)) // Player starts at the city center (x = 274, y = 234).
{
    current_bomb_image_index_ = 0;
    player_x_change_ = 0;
    player_y_change_ = 0;
    ui_->setupUi(this);
    map_ = new QGraphicsScene(this);
    map_->setBackgroundBrush(*map_image_);
    map_->addItem(player_->giveGraphicActor());
    ui_->gameView->setScene(map_);
    map_->setSceneRect(0, 0, map_image_->width(), map_image_->height());

    tick_timer_ = new QTimer(this);
    bomb_animation_timer_ = new QTimer(this);
    connect(bomb_animation_timer_, SIGNAL(timeout()), this, SLOT(bombAnimationHandler()));
    connect(tick_timer_, SIGNAL(timeout()), this, SLOT(gameTickHandler()));

    // Bomb animation.
    for (int i = 0; i < BOMB_FILE_NAMES.size(); ++i) {
        bomb_images_.push_back(std::make_shared<QPixmap>(QPixmap(BOMB_IMAGE_LOCATION + BOMB_FILE_NAMES[i])));
    }
    current_bomb_image_ = map_->addPixmap(*bomb_images_[0]);
    current_bomb_image_->hide();

    // Banners are shown at the end of the game.
    you_died_banner_pixmap_->setZValue(7);
    you_won_banner_pixmap_->setZValue(7);
    you_died_banner_pixmap_->hide();
    you_won_banner_pixmap_->hide();
    map_->addItem(you_died_banner_pixmap_);
    map_->addItem(you_won_banner_pixmap_);

}


BetterMainWindow::~BetterMainWindow()
{
    delete map_image_;
    map_image_ = nullptr;
    delete you_died_banner_pixmap_;
    you_died_banner_pixmap_ = nullptr;
    delete you_won_banner_pixmap_;
    you_won_banner_pixmap_ = nullptr;
    delete ui_;
    ui_ = nullptr;
    delete player_;
    player_ = nullptr;
    delete map_;
    map_ = nullptr;
    delete tick_timer_;
    tick_timer_ = nullptr;
    delete bomb_animation_timer_;
    bomb_animation_timer_ = nullptr;
    for (unsigned long long int i = 0; i < enemy_list_.size(); i++) {
        delete enemy_list_.at(i);
        enemy_list_.at(i) = nullptr;
    }
}


void BetterMainWindow::takeStatistics(std::shared_ptr<Statistics> statistics)
{
    statistics_ = statistics;
    map_->addItem(statistics_.get());

    // Center graphicsview and statistics to player location.
    ui_->gameView->centerOn(player_->giveGraphicActor()->giveGraphicX(),
                            player_->giveGraphicActor()->giveGraphicY());
    statistics_->setPos(ui_->gameView->mapToScene(0, 0).x(),
                        ui_->gameView->mapToScene(0, 0).y());
}


std::shared_ptr<GraphicBus> BetterMainWindow::addGraphicBus(int x, int y)
{
    std::shared_ptr<GraphicBus> new_bus = std::make_shared<GraphicBus>(x, y);
    map_->addItem(new_bus.get());
    return new_bus;
}


std::shared_ptr<GraphicStop> BetterMainWindow::addGraphicStop(int x, int y)
{
    std::shared_ptr<GraphicStop> new_stop = std::make_shared<GraphicStop>(x, y);
    map_->addItem(new_stop.get());
    return new_stop;
}


void BetterMainWindow::removeGraphicActor(std::shared_ptr<GraphicActor> actor_to_be_removed)
{
    map_->removeItem(actor_to_be_removed.get());
}


void BetterMainWindow::keepPlayerInBounds()
{
    if (player_->giveLocation().giveY() >= border_y_max_ and player_y_change_ == 1) {
        player_y_change_ = 0;
    }
    if (player_->giveLocation().giveY() <= border_y_min_ and player_y_change_ == -1) {
        player_y_change_ = 0;
    }
    if (player_->giveLocation().giveX() >= border_x_max_ and player_x_change_ == 1) {
        player_x_change_ = 0;
    }
    if (player_->giveLocation().giveX() <= border_x_min_ and player_x_change_ == -1) {
        player_x_change_ = 0;
    }
}


void BetterMainWindow::showYouWonBanner()
{
    int pixmap_width = you_won_banner_pixmap_->pixmap().width();
    int pixmap_height = you_won_banner_pixmap_->pixmap().height();

    // Show banner in the center of the screen.
    you_won_banner_pixmap_->setPos(player_->giveGraphicActor()->giveGraphicX() - pixmap_width/2,
                                   player_->giveGraphicActor()->giveGraphicY() - pixmap_height/2);
    you_won_banner_pixmap_->show();
}


void BetterMainWindow::showYouDiedBanner()
{
    int pixmap_width = you_died_banner_pixmap_->pixmap().width();
    int pixmap_height = you_died_banner_pixmap_->pixmap().height();

    // Show banner in the center of the screen.
    you_died_banner_pixmap_->setPos(player_->giveGraphicActor()->giveGraphicX() - pixmap_width/2,
                                    player_->giveGraphicActor()->giveGraphicY() - pixmap_height);
    you_died_banner_pixmap_->show();
}


bool BetterMainWindow::gameIsOver()
{
    return game_over_;
}


void BetterMainWindow::setGameOver()
{
    tick_timer_->stop();
    game_over_ = true;
}


Interface::Location BetterMainWindow::givePlayerLocation()
{
    return player_->giveLocation();
}


void BetterMainWindow::startGameGraphics()
{
    spawnEnemiesAmount(statistics_->giveDifficulty().amount_of_enemies);
    tick_timer_->start(tick_rate_);
}


void BetterMainWindow::keyPressEvent(QKeyEvent* event)
{
    // Can't move player if game ended.
    if (game_over_) {
        return;
    }

    // Positive changes are to north and east.
    // Negative changes are to south and west.
    if (event->key() == Qt::Key_W ) {
        w_down_ = true;
        player_y_change_ = 1;

    } else if (event->key() == Qt::Key_A) {
        a_down_ = true;
        player_x_change_ = -1;

    } else if (event->key() == Qt::Key_S) {
        s_down_ = true;
        player_y_change_ = -1;

    } else if (event->key() == Qt::Key_D) {
        d_down_ = true;
        player_x_change_ = 1;

    } else if (event->key() == Qt::Key_Space) {
        emit dropBomb(player_->giveLocation());
        playBombAnimation();

    } else {
        return;
    }
}


void BetterMainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // Automatically repeated key releases are not counted, since if a key is held
    // down, it will produce these.
    if (event->isAutoRepeat() || game_over_) {
        return;
    }

    // Stopping player movement to certain direction if opposite direction key
    // is not held down.
    if (event->key() == Qt::Key_W) {
        w_down_ = false;
        if (s_down_) {
            player_y_change_ = -1;
        } else {
            player_y_change_ = 0;
        }

    } else if (event->key() == Qt::Key_A) {
        a_down_ = false;
        if (d_down_) {
            player_x_change_ = 1;
        } else {
            player_x_change_ = 0;
        }

    } else if (event->key() == Qt::Key_S) {
        s_down_ = false;
        if (w_down_) {
            player_y_change_ = 1;
        } else {
            player_y_change_ = 0;
        }

    } else if (event->key() == Qt::Key_D ) {
        d_down_ = false;
        if (a_down_) {
            player_x_change_ = -1;
        } else {
            player_x_change_ = 0;
        }

    } else {
        return;
    }
}


void BetterMainWindow::bombAnimationHandler()
{
    // Switches to different image everytime bomb_timer ticks. When last image
    // has been showed, stops bomb_timer.
    if (current_bomb_image_index_ != bomb_images_.size() - 1) {
        current_bomb_image_->setPixmap(*bomb_images_[current_bomb_image_index_ + 1]);
        ++current_bomb_image_index_;

    } else {
        current_bomb_image_->hide();
        current_bomb_image_->setPixmap(*bomb_images_[0]);
        current_bomb_image_index_ = 0;
        bomb_animation_timer_->stop();
    }
}


void BetterMainWindow::gameTickHandler()
{
    moveEnemies();

    // Only move thing around if player position should be changed.
    if ((player_x_change_ != 0) || (player_y_change_ != 0)) {
        keepPlayerInBounds();
        player_->setNewCoords(player_x_change_, player_y_change_);
        ui_->gameView->centerOn(player_->giveGraphicActor()->giveGraphicX(), player_->giveGraphicActor()->giveGraphicY());
        statistics_->setPos(ui_->gameView->mapToScene(0, 0).x(),
                            ui_->gameView->mapToScene(0, 0).y());
        player_->giveGraphicActor()->updateGraphicPosition();
    }

    // Update everything show in gameView.
    map_->update(ui_->gameView->mapToScene(0, 0).x(),
                 ui_->gameView->mapToScene(0, 0).y(),
                 ui_->gameView->width(),
                 ui_->gameView->height());

    if (statistics_->playerIsDead()) {
        setGameOver();
        emit gameOver();
    }
}


void BetterMainWindow::spawnEnemiesAmount(int amount)
{
    int enemy_start_x_coord;
    int enemy_start_y_coord;

    for (int i = 0; i < amount; ++i) {
        // Randomise coordinates until enemy is not unfairly close to the player at the start.
        do {
            enemy_start_x_coord = (rand() % 4550) - 890; // Fit the coordinates to the different sized map.
            enemy_start_y_coord = (rand() % 3125) - 2145;
        } while (enemyIsTooCloseToBeSpawned(enemy_start_x_coord , enemy_start_y_coord) == true);

        EnemyLogic* new_enemy = new EnemyLogic(enemy_start_x_coord, enemy_start_y_coord,
                                    statistics_->giveDifficulty().enemy_speed,
                                    statistics_->giveDifficulty().enemy_vision,
                                    border_x_max_, border_x_min_,
                                    border_y_max_, border_y_min_);

        map_->addItem(new_enemy->giveGraphics());
        enemy_list_.push_back(new_enemy);
    }
}


bool BetterMainWindow::enemyIsTooCloseToBeSpawned(int enemy_x, int enemy_y)
{
    int dx = enemy_x - player_->giveLocation().giveX();
    int dy = enemy_y - player_->giveLocation().giveY();

    return dx*dx + dy*dy <= min_enemy_distance_at_start * min_enemy_distance_at_start;
}


void BetterMainWindow::moveEnemies()
{
    // Enemies have 3 different main actions:
    // 1. Determines it's next move sequence.
    // 2. Keep moving if the move sequence is unfinished.
    // 3. Starts chasing the player if the player is close enough.
    for (unsigned long int c = 0; c < enemy_list_.size(); c++) {
        EnemyLogic* enemy = enemy_list_.at(c);
        if (enemy->isMoving() and enemy->IsBounds() and
                enemy->seePlayer(player_->giveLocation()) ==false) {
            enemy->keepMoving();
        }

        else if (enemy->seePlayer(player_->giveLocation()) == false) {
          enemy->startMoving();

        } else {
              enemy->chasePlayer(player_->giveLocation());
        }

        // Only redraw enemy if it at scene.
        if (enemy->giveLocation().isClose(player_->giveLocation()), this->width()/2) {
            enemy->giveGraphics()->updateGraphicPosition();
        }

        if (enemy->hitPlayer(player_->giveLocation())) {
            playBombAnimation(7);
            enemy_list_.erase(enemy_list_.begin() + c);
            delete enemy;
            statistics_->removeLife();
        }
    }
}


void BetterMainWindow::playBombAnimation(int z)
{
    // z value is changeable so that it can be shown ontop of the ending screen.
    current_bomb_image_->setZValue(z);
    int x = player_->giveGraphicActor()->giveGraphicX();
    int y = player_->giveGraphicActor()->giveGraphicY();

    // Move bomb image at players current graphical position.
    current_bomb_image_->setPos(x - current_bomb_image_->pixmap().width()/2,
                                y - current_bomb_image_->pixmap().height()/2);
    current_bomb_image_->show();
    bomb_animation_timer_->start(85);
}
