#include "city.hh"


namespace Interface
{

City::City(std::shared_ptr<BetterMainWindow> main_window):
    main_window_(main_window)
{

}


City::~City()
{

}


void City::addStop(std::shared_ptr<IStop> stop)
{
    int x = stop->getLocation().giveX();
    int y = stop->getLocation().giveY();

    std::shared_ptr<GraphicStop> new_graphic_stop = main_window_->addGraphicStop(x, y);
    stop_dict_[stop] = new_graphic_stop;
}


void City::addActor(std::shared_ptr<IActor> newactor)
{
    int x = newactor->giveLocation().giveX();
    int y = newactor->giveLocation().giveY();

    // Actor to be added is a bus.
    if (dynamic_cast<CourseSide::Nysse*>(newactor.get())) {
        std::shared_ptr<GraphicBus> new_graphic_bus = main_window_->addGraphicBus(x, y);
        bus_dict_[newactor] = new_graphic_bus;

    // Actor to be added is a passenger.
    } else if (CourseSide::Passenger* passenger_pointer = dynamic_cast<CourseSide::Passenger*>(newactor.get())) {

        // Makes sure new passenger is at stop. Practically always this is the case.
        if (std::shared_ptr<Interface::IStop> stop = passenger_pointer->getStop()) {

            std::shared_ptr<PassengerInfo> new_passenger_info = std::make_shared<PassengerInfo>(x, y);
            std::shared_ptr<GraphicHost> host_stop = stop_dict_[stop];

            // Update number shown on stop.
            host_stop->addPassenger(new_passenger_info);

            // Store pointers to map.
            passenger_dict_[newactor] = std::pair< std::shared_ptr<PassengerInfo>, std::shared_ptr<GraphicHost>>(new_passenger_info, host_stop);
        }
    }
}


void City::actorMoved(std::shared_ptr<IActor> actor)
{
    // Actor to be moved is a bus.
    if (dynamic_cast<CourseSide::Nysse*>(actor.get())) {
        Location loc = actor->giveLocation();
        int x = loc.giveX();
        int y = loc.giveY();

        std::shared_ptr<GraphicBus> bus_to_be_updated = bus_dict_[actor];
        bus_to_be_updated->setGraphicCoords(x, y);

        // If bus moved close enough it should be visible to player, it will be
        // drawn.
        if (loc.isClose(main_window_->givePlayerLocation(), main_window_->width()/2)) {
            bus_to_be_updated->updateGraphicPosition();
        }

    // Actor to be moved is a passenger.
    } else if (CourseSide::Passenger* passenger_pointer = dynamic_cast<CourseSide::Passenger*>(actor.get())) {
        std::shared_ptr<GraphicHost> current_host;
        std::shared_ptr<PassengerInfo> graphic_passenger = passenger_dict_[actor].first;
        std::shared_ptr<GraphicHost> prev_host = passenger_dict_[actor].second;

        // Find current host (bus/stop) and compare it to previous one.
        if (std::shared_ptr<IVehicle> current_bus = passenger_pointer->getVehicle()) {
            current_host = bus_dict_[current_bus];

        } else if (std::shared_ptr<IStop> current_stop = passenger_pointer->getStop()) {
            current_host = stop_dict_[current_stop];
        }

        if (current_host == prev_host) {
            return;

        // Only if host is changed, graphic corresponds will be updated.
        } else {
            prev_host->removePassenger(graphic_passenger);
            current_host->addPassenger(graphic_passenger);
            passenger_dict_[actor].second = current_host;
        }
    }
}


std::vector<std::shared_ptr<IActor> > City::getNearbyActors(Location loc) const
{
    std::vector<std::shared_ptr<IActor> > nearby_actors;
    for (auto actor : bus_dict_) {

        if (loc.isClose(actor.first->giveLocation())) {
            nearby_actors.push_back(actor.first);
        }
    }
    return nearby_actors;
}


bool City::findActor(std::shared_ptr<IActor> actor) const
{
    // Actor to be find is a passenger.
    if (dynamic_cast<CourseSide::Passenger*>(actor.get())) {
        if (passenger_dict_.find(actor) == passenger_dict_.end()) {
            return false;
        }

    // Actor to be find is a bus.
    } else if (dynamic_cast<CourseSide::Nysse*>(actor.get())) {
        if (bus_dict_.find(actor) == bus_dict_.end()) {
            return false;
        }

    } else {
        return false;
    }

    return true;
}


void City::removeActor(std::shared_ptr<IActor> actor)
{
    // Graphic passengers don't need to be removed from scene, since they are
    // only represented as numbers on their hosts.
    if (dynamic_cast<CourseSide::Nysse*>(actor.get())) {
        main_window_->removeGraphicActor(bus_dict_[actor]);
        bus_dict_.erase(actor);
        actor->remove();

    } else if (dynamic_cast<CourseSide::Passenger*>(actor.get())) {
        passenger_dict_.erase(actor);
    }
}


void City::actorRemoved(std::shared_ptr<IActor> actor)
{
    actor->isRemoved();
}


bool City::isGameOver() const
{
    return main_window_->gameIsOver();
}


void City::startGame()
{

}


void City::setClock(QTime clock)
{
    // We don't need clock in our implementation
    Q_UNUSED(clock)
}


void City::setBackground(QImage &basicbackground, QImage &bigbackground)
{
    qDebug() <<"setBackground was called, Don't use this func";
    Q_UNUSED(basicbackground)
    Q_UNUSED(bigbackground)
}

}
