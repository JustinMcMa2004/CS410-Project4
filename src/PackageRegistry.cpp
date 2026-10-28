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
