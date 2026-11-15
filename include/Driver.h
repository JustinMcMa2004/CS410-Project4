#ifndef DRIVER_H
#define DRIVER_H

#include "Location.h"

#include <chrono>
#include <string>

using std::string;

class Driver {
private:
    string driverID;
    string deliveryCity;
    string deliveryState;
    int departureHour;
    int departureMinute;

public:
    Driver(
        const string& driverID,
        const string& deliveryCity,
        const string& deliveryState,
        int departureHour,
        int departureMinute
    );
    
    bool isValid() const;

    string getDriverID() const;
    string getDeliveryCity() const;
    string getDeliveryState() const;
    int getDepartureHour() const;
    int getDepartureMinute() const;


    // check whether the driver serves the package's destination
    bool servesArea(const Location& destination) const;

    // returns the driver's next available departure time after the given earliest time
    std::chrono::system_clock::time_point getNextDeparture(
        std::chrono::system_clock::time_point earliestTime
    ) const;

};

#endif