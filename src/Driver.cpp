#include "Driver.h"
#include "isBlank.h"

#include <ctime>
#include <cctype>
#include <stdexcept>

using std::string;

static string convertToLowercase(string text) {
    for (int i = 0; i < text.length(); i++) {
        unsigned char character = text[i];
        text[i] = std::tolower(character);
    }

    return text;
}

Driver::Driver(
    const string& driverID,
    const string& deliveryCity,
    const string& deliveryState,
    int departureHour,
    int departureMinute
) {
    this->driverID = driverID;
    this->deliveryCity = deliveryCity;
    this->deliveryState = deliveryState;
    this->departureHour = departureHour;
    this->departureMinute = departureMinute;
}

// Check that the driver has valid information and a valid departure time.
bool Driver::isValid() const {
    if (isBlank(driverID) || isBlank(deliveryCity) || isBlank(deliveryState)) {
        return false;
    }

    if (departureHour < 0 || departureHour > 23) {
        return false;
    }

    if (departureMinute < 0 || departureMinute > 59) {
        return false;
    }

    return true;
}

string Driver::getDriverID() const {
    return driverID;
}

string Driver::getDeliveryCity() const {
    return deliveryCity;
}

string Driver::getDeliveryState() const {
    return deliveryState;
}

int Driver::getDepartureHour() const {
    return departureHour;
}

int Driver::getDepartureMinute() const {
    return departureMinute;
}

// Checks whether the driver serves the destination's city and state
bool Driver::servesArea(const Location& destination) const {
    string driverCity = convertToLowercase(deliveryCity);
    string driverState = convertToLowercase(deliveryState);

    string packageCity = convertToLowercase(destination.getCity());
    string packageState = convertToLowercase(destination.getState());

    return driverCity == packageCity && driverState == packageState;
}



std::chrono::system_clock::time_point Driver::getNextDeparture(
    std::chrono::system_clock::time_point earliestTime
) const {
    // get the date associated w/ earliest possible departure
    std::time_t time = std::chrono::system_clock::to_time_t(earliestTime);
    
    std::tm* localTime = std::localtime(&time);

    if (localTime == nullptr) {
        throw std::runtime_error("Failed to convert time to local date.");
    }
    
    std::tm departure = *localTime;

    // set the driver's scheduled departure time
    departure.tm_hour = departureHour;
    departure.tm_min = departureMinute;
    departure.tm_sec = 0;
    departure.tm_isdst = -1;

    std::time_t departureTime = std::mktime(&departure);

    if (departureTime == -1) {
        // if time conversion fails, throw an error
        throw std::runtime_error("Failed to calculate driver departure time.");
    }

    auto nextDeparture = std::chrono::system_clock::from_time_t(departureTime);

    // If today's departure has passed, recalcualte the departure using tomorrow's date
    // for ex. if the driver already departed today, schedule their next departure for tomorrow
    if (nextDeparture < earliestTime) {
        departure.tm_mday += 1;
        departure.tm_isdst = -1;

        
        std::time_t tomorrowTime = std::mktime(&departure);

        if (tomorrowTime == -1) {
            throw std::runtime_error("Failed to calculate driver departure time for tomorrow.");
        }


        nextDeparture = std::chrono::system_clock::from_time_t(tomorrowTime);
    }

    return nextDeparture;
}