#include "PackageRegistry.h"

using std::string;
using std::vector;

// check whether a package with this tracking ID already exists
bool PackageRegistry::isRegistered(const string& packageID) const {
    for (const Package& package : packages) {
        if (package.getPackageID() == packageID) {
            return true;
        }
    }

    return false;
}

// register package only if its info is valid and ID is unique
bool PackageRegistry::registerPackage(
    const Package& package,
    string& errorMessage
) {
    // reject packages with missing or invalid info
    if (!package.isValid()) {
        errorMessage = "package could not be registered, missing information.";
        return false;
    }

    // reject duplicate tracking IDs (already registered ID)
    if (isRegistered(package.getPackageID())) {
        errorMessage = "package already registered.";
        return false;
    }

    // store the package if both validation checks passed
    packages.push_back(package);
    errorMessage.clear();
    return true;
}

// read-only access to all registered packages
const vector<Package>& PackageRegistry::getPackages() const {
    return packages;
}

// Return the index of a package with a matching ID, if it exists. Returns -1 otherwise. 
int PackageRegistry::getIndex(string id) const {
    for (int i = 0; i < packages.size(); i++) {
        if (packages[i].getPackageID().compare(id) == 0) return i;
    }
    return -1;
}

// Update the delivery status of a registered package.
bool PackageRegistry::updatePackageStatus(
    const string& packageID,
    const string& newStatus,
    string& errorMessage
) {
    // Find the package by its tracking ID.
    int index = getIndex(packageID);

    if (index == -1) {
        errorMessage = "package not found.";
        return false;
    }

    // Only allow valid delivery statuses.
    if (newStatus != "Ready to be Delivered" &&
        newStatus != "Out for Delivery" &&
        newStatus != "Delivered" &&
        newStatus != "Delivery Failed") {
        errorMessage = "invalid delivery status.";
        return false;
    }

    // Create the new status and record its timestamp.
    Status status(newStatus, "");
    status.setTime();

    // Save the updated status in the package's history.
    packages[index].updateStatus(status);

    errorMessage.clear();
    return true;
}