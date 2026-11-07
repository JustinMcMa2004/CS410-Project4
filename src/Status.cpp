#include "Status.h"
#include <chrono>
#include <ctime>

using std::string;

Status::Status(
    const string& statusMessage,
    const string& dateAndTime
    ) {
        this->statusMessage = statusMessage;
        this->dateAndTime = dateAndTime;
    }

// check whether package's status is ready to ship
bool Status::readyToShip() const {
    return statusMessage == "Ready to Ship";
}

// update the time associated with this status.
void Status::setTime() {
    auto systemTime = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(systemTime);
    string currentTimeString = ctime(&currentTime);
    dateAndTime = currentTimeString;
}

// returns the current status message
string Status::getStatusMessage() const {
    return statusMessage;
}

string Status::getDateAndTime() const {
    return dateAndTime;
}