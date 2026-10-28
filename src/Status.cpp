#include "Status.h"

using std::string;

Status::Status(
    const string& statusMessage,
    const string& date,
    const string& time
    ) {
        this->statusMessage = statusMessage;
        this->date = date;
        this->time = time;
    }

// check whether package's status is ready to ship
bool Status::readyToShip() const {
    return statusMessage == "Ready to Ship";
}

// update the time associated with this status.
void Status::setTime(const string& time) {
    this->time = time;
}

// returns the current status message
string Status::getStatusMessage() const {
    return statusMessage;
}
