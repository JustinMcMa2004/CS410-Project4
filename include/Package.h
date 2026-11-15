// Package class should store the package's information (ex. trackingID, recipient, destination, weight, dimensions, etc)

#ifndef PACKAGE_H
#define PACKAGE_H

#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include "Location.h"
#include "Status.h"

using std::string;
using std::vector;

class Package {
private:
    // store the information needed to identify and register a package
    string recipient;
    Location destination;
    string packageID;
    vector<double> dimensions;
    double weight;
    bool requiresSignature;
    bool isFragile;
    Location initialLocation;

    vector<Location> locationHistory;
    vector<Status> statusHistory;
    std::chrono::system_clock::time_point intakeTime;
    // add later:
    // vector<DeliveryAttempt> deliveryAttempts;

public:
    // create a package with its register info
    Package(
        const string& recipient,
        const Location& destination,
        const string& packageID,
        const vector<double>& dimensions,
        double weight,
        const Location& currentLocation,
        bool requiresSignature = false,
        bool isFragile = false,
        std::chrono::system_clock::time_point intakeTime = std::chrono::system_clock::now()
    );

    bool isValid() const;

    // returns the package's tracking ID
    string getPackageID() const;

    // return the time when the package was registered
    std::chrono::system_clock::time_point getIntakeTime() const;


    // return package's delivery destination
    Location getDestination() const;

    // returns current status
    Status getCurrentStatus() const;

    // returns package's status history
    vector<Status> getStatusHistory() const;

    //adds new status to working status history
    void updateStatus(const Status&);
 
    // returns current Location
    Location getCurrentLocation() const;

    // returns package's status history
    vector<Location> getLocationHistory() const;

    // Adds new location to working location history
    void addLocation(const Location&);
    
    // Prints the currrent information of a package. 
    string getPackageInfo() const; 

    bool isDelivered() const;
};

#endif
