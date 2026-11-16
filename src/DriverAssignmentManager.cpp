#include "DriverAssignmentManager.h"
#include <stdexcept>


using std::string;
using std::vector;

DriverAssignmentManager::DriverAssignmentManager(
    const PackageRegistry& registry
) : registry(registry) {
}

// adds a driver to the list of available drivers
bool DriverAssignmentManager::addDriver(const Driver& driver) {
    if (!driver.isValid()) {
        return false;
    }
    
    // prevent duplicate driver IDs
    for (const Driver& existingDriver : drivers) {
        if (existingDriver.getDriverID() == driver.getDriverID()) {
            return false;
        }
    }

    drivers.push_back(driver);
    return true;
}

// attempts to assign a registered package to a driver. 
// will return true if success, or false with an error message
bool DriverAssignmentManager::assignPackage(
    const string& packageID,
    string& errorMessage,
    std::chrono::system_clock::time_point now
) {
    // find the package in the registry using its trackingID
    int index = registry.getIndex(packageID);

    // returns -1 if package doesnt exist
    if (index == -1) {
        errorMessage = "package not found.";
        return false;
    }

    const Package& package = registry.getPackages()[index];



    // checks whether this package was already assigned to a driver
    // prevents assigning the same package twice
    for (const DriverAssignment& assignment : assignments) {
        if (assignment.packageID == packageID) {
            errorMessage = "package already assigned.";
            return false;
        }
    }

    // only packages marked "Ready to be Delivered" can be assigned
    if (package.getCurrentStatus().getStatusMessage() != "Ready to be Delivered") {
        errorMessage = "package is not ready to be delivered.";
        return false;
    }
    
    // earliest date/time a driver is allowed to leave with the package is the current time (since we can't assign a driver to a departure in the past)
    auto earliestTime = now;

    // if the package's intake time is later than the current time,
    // then the earliest time a driver can leave with the package is the package's intake time
    // makes sure the driver doesnt leave before the package arrives
    if (package.getIntakeTime() > earliestTime) {
        earliestTime = package.getIntakeTime();
    }

    // keep track of the best driver
    const Driver* bestDriver = nullptr;

    // departure time of the best driver
    std::chrono::system_clock::time_point bestDeparture;

    // goes through every available driver and finds the best match
    for (const Driver& driver : drivers) {
        if (!driver.servesArea(package.getDestination())) {
            continue;
        }

        // find driver's next depature after our earliest allowed time
        std::chrono::system_clock::time_point departure;

        try {
            departure = driver.getNextDeparture(earliestTime);
        } catch (const std::runtime_error& error) {
            errorMessage = error.what();
            return false;
        }

        // select this driver if no driver assigned yet, OR this driver departs earlier than our current best driver
        if (bestDriver == nullptr || departure < bestDeparture) {
            bestDriver = &driver;
            bestDeparture = departure;
        }
    }

    // if no driver serves the delivery area, we couldn't assign a driver to this package
    if (bestDriver == nullptr) {
        errorMessage = "no driver serves this delivery area.";
        return false;
    }

    // Save the selected driver and departure time
    assignments.push_back({
        packageID,
        bestDriver->getDriverID(),
        bestDeparture
    });

    errorMessage.clear();
    return true;
}

const vector<DriverAssignment>& DriverAssignmentManager::getAssignments() const {
    return assignments;
}