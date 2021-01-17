#include "graphichost.hh"


void GraphicHost::addPassenger(std::shared_ptr<PassengerInfo> passenger)
{
    hosted_passengers_.push_back(passenger);
}


void GraphicHost::removePassenger(std::shared_ptr<PassengerInfo> passenger)
{
    // Find passenger in vector and remove it if found.
    std::vector<std::shared_ptr<PassengerInfo> >::iterator it;
    it = std::find(hosted_passengers_.begin(), hosted_passengers_.end(), passenger);

    if (it != hosted_passengers_.end()) {
        int i = std::distance(hosted_passengers_.begin(), it);
        hosted_passengers_.erase(hosted_passengers_.begin() + i);
    }
}


int GraphicHost::amountOfPassengersHosted()
{
    return hosted_passengers_.size();
}
