
#include "Package.h"

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
        statusHistory.push_back(Status("Registered", "", ""));
}

bool Package::isValid() const {
    return !recipient.empty() && 
            destination.isValid() &&
            !packageID.empty() &&
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
