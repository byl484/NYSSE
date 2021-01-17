#include "engine.hh"


Engine::Engine(QObject* parent):
    QObject(parent),
    start_window_(new StartWindow),
    statistics_(std::make_shared<Statistics> (new Statistics)),
    main_window_(std::make_shared<BetterMainWindow>(new BetterMainWindow)),
    city_(std::make_shared<Interface::City>(main_window_)),
    logic_(new CourseSide::Logic),
    game_timer_(new QTimer)
{
    connect(start_window_, SIGNAL(startButtonClicked(unsigned int)), this, SLOT(startGame(unsigned int)));
    connect(main_window_.get(), SIGNAL(dropBomb(Interface::Location)), this, SLOT(bombDropped(Interface::Location) ) );
    connect(main_window_.get(), SIGNAL(gameOver()), this, SLOT(endGame()));
    connect(game_timer_, SIGNAL(timeout()), this, SLOT(endGame()));

    logic_->fileConfig();
    logic_->takeCity(city_);
    logic_->setTime(16, 30); // Time is always se to this time cause it fits the game the best.

    start_window_->show();
}


Engine::~Engine()
{
    delete start_window_;
    start_window_ = nullptr;
    delete logic_;
    logic_ = nullptr;
    delete game_timer_;
    game_timer_ = nullptr;
}


void Engine::bombDropped(Interface::Location loc)
{
    std::vector<std::shared_ptr<Interface::IActor> > nearby_actors = city_->getNearbyActors(loc);

    int humans_eliminated = 0;
    int busses_eliminated = 0;

    for ( std::shared_ptr<Interface::IActor> actor : nearby_actors) {
        if (CourseSide::Nysse* bus_pointer = dynamic_cast<CourseSide::Nysse*>(actor.get())) {
            int passengers_inside = bus_pointer->getPassengers().size();
            humans_eliminated += passengers_inside;
            ++busses_eliminated;
        }
        actor->remove();
    }

    statistics_->addEliminatedHumans(humans_eliminated);
    statistics_->addEliminatedBusses(busses_eliminated);
}


void Engine::startGame(unsigned int difficulty)
{
    start_window_->hide();
    statistics_->setDifficulty(difficulty);
    statistics_->setTimer(game_timer_);
    main_window_->takeStatistics(statistics_);
    main_window_->startGameGraphics();

    logic_->finalizeGameStart();

    // Game time is defined as seconds -> will be transformed into ms.
    game_timer_->start(statistics_->giveDifficulty().time*1000);
    main_window_->show();
}


void Engine::endGame()
{
    game_timer_->stop();
    main_window_->setGameOver();

    // Show correct ending banner.
    if (statistics_->playerIsDead()) {
        main_window_->showYouDiedBanner();

    } else {
        main_window_->showYouWonBanner();
    }
}
