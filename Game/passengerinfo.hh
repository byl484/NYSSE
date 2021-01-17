#ifndef GRAPHICPASSENGER_HH
#define GRAPHICPASSENGER_HH


/**
 * @brief Stores info to set passenger amounts on
 * busses and busstops in real time
 */

class PassengerInfo
{

public:
    PassengerInfo(int x, int y);
    void setCoordinates(int x, int y);

private:
    int x_;
    int y_;

};

#endif // GRAPHICPASSENGER_HH
