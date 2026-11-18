#ifndef DRIVER_ASSIGNMENT_MANAGER_H
#define DRIVER_ASSIGNMENT_MANAGER_H

#include <string>
#include <vector>
#include <chrono>
#include "Driver.h"
#include "PackageRegistry.h"

using std::string;
using std::vector;

// stores which driver was assigned to a specific package and when they will depart
struct DriverAssignment {
    string packageID;
    string driverID;
    std::chrono::system_clock::time_point departureTime;
};

class DriverAssignmentManager {
private:
    PackageRegistry& registry;
    vector<Driver> drivers;
    vector<DriverAssignment> assignments;

public:
    DriverAssignmentManager(PackageRegistry& registry);

    bool addDriver(const Driver& driver);

    bool assignPackage(const string& packageID, string& errorMessage, std::chrono::system_clock::time_point now = std::chrono::system_clock::now()
);

    const vector<DriverAssignment>& getAssignments() const;
};

#endif