// Registry class should be used to register packages, reject invalid info, prevent duplicate tracking IDs

#ifndef PACKAGE_REGISTRY_H
#define PACKAGE_REGISTRY_H

#include <string>
#include <vector>
#include "Package.h"

using std::string;
using std::vector;

class PackageRegistry {
private:
    // store all packages that have been registered
    vector<Package> packages;

public:
    // register a package if it has valid info and a unique ID
    bool registerPackage(const Package& package, string& errorMessage);

    // check whether the tracking ID is already registered
    bool isRegistered(const string& packageID) const;

    // return all registered packages
    const vector<Package>& getPackages() const;

    // Finds the matching index of a package with the same provided id, if it exists. Returns -1 otherwise. 
    int getIndex(string) const; 

    // Update a registered package's delivery status.
    // Return false if the package or status is invalid.
    bool updatePackageStatus(
        const string& packageID,
        const string& newStatus,
        string& errorMessage
    );
};

#endif
