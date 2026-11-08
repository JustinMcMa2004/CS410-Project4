
#include "Package.h"
#include "isBlank.h"
#include <iostream>
#include <chrono>
#include <ctime>

using std::string;
using std::vector;

Package::Package(
    const string& recipient,
    const Location& destination,
    const string& packageID,
    const vector<double>& dimensions,
    double weight,
    bool requiresSignature,
    bool isFragile
    ) : destination(destination) {
        this->recipient = recipient;
        this->packageID = packageID;
        this->dimensions = dimensions;
        this->weight = weight;
        this->requiresSignature = requiresSignature;
        this->isFragile = isFragile;

        // record the initial status in history when the package is created!
        // The program assumes the system time is in EST. If machines in different time zones are used this will need to be changed. 
        auto systemTime = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(systemTime);
        string currentTimeString = ctime(&currentTime);
        statusHistory.push_back(Status("Registered", currentTimeString));
}

bool Package::isValid() const {
    return !isBlank(recipient) && 
            destination.isValid() &&
            !isBlank(packageID) &&
            dimensions.size() == 3 &&
            dimensions[0] > 0 &&
            dimensions[1] > 0 &&
            dimensions[2] > 0 &&
            weight > 0;
}

string Package::getPackageID() const {
    return packageID;
}

Status Package::getCurrentStatus() const {
    // back returns the last element of a vector, and since new statuses are appended to history, the last element should represent the current status
    return statusHistory.back(); 
}

void Package::addLocation(const Location& location) {
    locationHistory.push_back(location);
}

string Package::getPackageInfo() const {
    string message = "Recipient: " + recipient + "\n";
    message += "Destination: " + destination.getLocation() + "\n";
    message += "ID: " + packageID + "\n";
    message += "Length x Width x Height (inches): " + std::to_string(dimensions[0]) 
        + " x " + std::to_string(dimensions[1]) + " x " + std::to_string(dimensions[2]) + "\n";
    message += "Weight (pounds): " + std::to_string(weight) + "\n";
    if (requiresSignature) message += "Requires Signature?: Yes\n";
    else message += "Requires Signature?: No\n";
    if (isFragile) message += "Is Fragile?: Yes\n";
    else message += "Is Fragile?: No\n";
    if (locationHistory.size() > 0) message += "Current Location: " + locationHistory[locationHistory.size() - 1].getLocation() + "\n";
    else message += "Current Location: Not yet available\n" ;
    if (statusHistory.size() > 0) { 
        message += "Status: " + statusHistory[statusHistory.size() - 1].getStatusMessage() + "\n";
        message += "Time (EST): " + statusHistory[statusHistory.size() - 1].getDateAndTime();
    }
    return message;
}