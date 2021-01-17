#include "passengerinfo.hh"


PassengerInfo::PassengerInfo(int x, int y):
    x_(x),
    y_(y)
{

}


void PassengerInfo::setCoordinates(int x, int y)
{
    x_ = x;
    y_ = y;
}
