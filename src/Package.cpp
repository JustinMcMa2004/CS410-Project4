
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
    const Location& currentLocation,
    bool requiresSignature,
    bool isFragile
    ) : destination(destination), currentLocation(currentLocation) {
        this->recipient = recipient;
        this->packageID = packageID;
        this->dimensions = dimensions;
        this->weight = weight;
        this->requiresSignature = requiresSignature;
        this->isFragile = isFragile;


        // record the initial stat        ) : destination(destination), currentLocation(currentLocation) {us in history when the package is created!
        // The program assumes the system time is in EST. If machines in different time zones are used this will need to be changed. 
        auto systemTime = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(systemTime);
        string currentTimeString = ctime(&currentTime);
        statusHistory.push_back(Status("Registered", currentTimeString));
        locationHistory.push_back(currentLocation);
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

vector<Status> Package::getStatusHistory() const {
    return statusHistory;
}

Location Package::getCurrentLocation() const {
    // back returns the last element of a vector, and since new statuses are appended to history, the last element should represent the current status
    return locationHistory.back(); 
}

vector<Location> Package::getLocationHistory() const {
    return locationHistory;
}

void Package::updateStatus(const Status& status){
    statusHistory.push_back(status);
}

void Package::addLocation(const Location& location) {
    locationHistory.push_back(location);
}

string Package::getPackageInfo() const {
    // Main Package Info
    string message = "================================\n";
    message += "        PACKAGE INFORMATION\n";
    message += "================================\n";
    message += "Recipient: " + recipient + "\n";
    message += "Destination: " + destination.getLocation() + "\n";
    message += "ID: " + packageID + "\n";
    message += "Length x Width x Height (inches): "
        + std::to_string(dimensions[0])
        + " x " + std::to_string(dimensions[1])
        + " x " + std::to_string(dimensions[2]) + "\n";
    message += "Weight (pounds): " + std::to_string(weight) + "\n";
    message += requiresSignature
        ? "Requires Signature?: Yes\n"
        : "Requires Signature?: No\n";
    message += isFragile
        ? "Is Fragile?: Yes\n"
        : "Is Fragile?: No\n";

    // Current delivery status
    message += "\n================================\n";
    message += "       CURRENT DELIVERY STATUS\n";
    message += "================================\n";
    if (!locationHistory.empty()) {
        message += "Current Location: "
            + locationHistory.back().getLocation() + "\n";
    }
    else {
        message += "Current Location: Not yet available\n";
    }
    if (!statusHistory.empty()) {
        message += "Status: "
            + statusHistory.back().getStatusMessage() + "\n";

        message += "Time: "
            + statusHistory.back().getDateAndTime() + "\n";
    }
    else {
        message += "Status: Not yet available\n";
    }
    message += "Delivered?: ";
    message += isDelivered() ? "Yes\n" : "No\n";
    // Complete status history
    message += "\n================================\n";
    message += "          STATUS HISTORY\n";
    message += "================================\n";

    if (!statusHistory.empty()) {
        for (size_t i = 0; i < statusHistory.size(); i++) {
            message += std::to_string(i + 1) + ". "
                + statusHistory[i].getStatusMessage() + "\n";

            message += "   Time: "
                + statusHistory[i].getDateAndTime() + "\n";
        }
    }
    else {
        message += "No status history recorded.\n";
    }

    // Complete location history
    message += "\n================================\n";
    message += "         LOCATION HISTORY\n";
    message += "================================\n";

    if (!locationHistory.empty()) {
        for (size_t i = 0; i < locationHistory.size(); i++) {
            message += std::to_string(i + 1) + ". "
                + locationHistory[i].getLocation() + "\n";
        }
    }
    else {
        message += "No location history recorded.\n";
    }
    return message;
}

bool Package::isDelivered() const {
    if (statusHistory.size() > 0) { 
        Status currentStatus = statusHistory[statusHistory.size() - 1];
        string currentStatusMessage = statusHistory[statusHistory.size() - 1].getStatusMessage();
        std::transform(
            currentStatusMessage.begin(),
            currentStatusMessage.end(),
            currentStatusMessage.begin(),
            [](unsigned char c) {
                return std::tolower(c);
            }
        );
        return currentStatusMessage == "delivered";
    }
    return false;
}