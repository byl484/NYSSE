#ifndef CITY_HH
#define CITY_HH
#include "interfaces/icity.hh"
#include "actors/nysse.hh"
#include "actors/passenger.hh"

#include "bettermainwindow.hh"


/**
 * @brief  Works as an interface between game logic and mainwindow/engine
 */


namespace Interface
{

class City : public ICity
{

public:
    City(std::shared_ptr<BetterMainWindow> main_window);
    ~City();

    // Tells main window to create new graphic stop and stores its pointer with the IStop given.
    void addStop(std::shared_ptr<IStop> stop);

    // Tells main window to create either an graphic bus or passenger and stores
    // its pointer to corresponding map.
    void addActor(std::shared_ptr<IActor> newactor);

    // This is used to move a busses graphics or move graphical passenger to either
    // a new bus or stop. Additionally the maps that hold said actor are updated.
    void actorMoved(std::shared_ptr<IActor> actor);

    // Finds all busses nearby given location.
    std::vector<std::shared_ptr<IActor> > getNearbyActors(Location loc) const;

    // Checks if there exists a graphical version of given actor.
    bool findActor(std::shared_ptr<IActor> actor) const;

    // Removes graphical version of given actor from its map and sets given IActor->remove()
    void removeActor(std::shared_ptr<IActor> actor);


    // These methods are used by logic. They are defined just because it is necessary.
    // These aren't used by any other classes than logic.
    void actorRemoved(std::shared_ptr<IActor> actor);
    bool isGameOver() const;
    void startGame();
    void setClock(QTime clock);
    void setBackground(QImage& basicbackground, QImage& bigbackground);

private:
    std::shared_ptr<BetterMainWindow> main_window_;

    // Private maps hold key-value pairs such that for every IActor (IPassenger or IVehicle aka Nysse)
    // there is corresponding graphical representation of that object.

    std::map<std::shared_ptr<Interface::IActor>, std::shared_ptr<GraphicBus> > bus_dict_;
    std::map<std::shared_ptr<Interface::IStop>, std::shared_ptr<GraphicStop> > stop_dict_;
    // For every passenger, graphical bus and graphical passenger is stored as a pair.
    std::map<std::shared_ptr<Interface::IActor>, std::pair< std::shared_ptr<PassengerInfo>, std::shared_ptr<GraphicHost>> > passenger_dict_;

};

}

#endif // CITY_HH
