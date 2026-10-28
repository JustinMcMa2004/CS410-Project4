// Status class should show the package's initial status (ex. registered, unregistered)

#ifndef STATUS_H
#define STATUS_H

#include <string>

using std::string;

class Status {
private:
    // package's current delivery status
    string statusMessage;

    // date and time of the status
    string date;
    string time;

public:
    Status(
        const string& statusMessage,
        const string& date,
        const string& time
    );

    // return true if the package is ready to ship
    bool readyToShip() const;

    // will need a way to update the time associated with status
    void setTime(const string& time);

    // package will need a way to read current status without directly accessing status's private fields...
    string getStatusMessage() const;
};

#endif
