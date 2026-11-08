// Package class should store the package's information (ex. trackingID, recipient, destination, weight, dimensions, etc)

#ifndef PACKAGE_H
#define PACKAGE_H

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

    vector<Location> locationHistory;
    vector<Status> statusHistory;
    string intakeTime;
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
        bool requiresSignature = false,
        bool isFragile = false
    );

    bool isValid() const;

    // returns the package's tracking ID
    string getPackageID() const;

    // returns current status
    Status getCurrentStatus() const;

    void addLocation(const Location&);
    // void addLocation(Location);

    // Prints the currrent information of a package. 
    string getPackageInfo() const; 
};

#endif
