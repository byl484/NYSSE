#ifndef GRAPHICHOST_HH
#define GRAPHICHOST_HH
#include "graphicactor.hh"
#include "interfaces/ipassenger.hh"
#include "passengerinfo.hh"

/**
 * @brief Acts as parent class for graphical objects,
 * that can host passengers (bus stops and busses at the moment)
 */

class GraphicHost : public GraphicActor
{

public:
    GraphicHost() = default;

    // Adds given passenger to private vector.
    void addPassenger(std::shared_ptr<PassengerInfo> passenger);

    // Removes given passenger from private vector if it is found.
    void removePassenger(std::shared_ptr<PassengerInfo> passenger);

    // Size of private vector containing passengers.
    int amountOfPassengersHosted();

private:
    // Used to show number of passengers onboard a bus / at stop.
    std::vector<std::shared_ptr<PassengerInfo> > hosted_passengers_;

};

#endif // GRAPHICHOST_HH
