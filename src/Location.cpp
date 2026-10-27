#include "Location.h"

using std::string;

// initialize location
Location::Location(
    const string& address,
    const string& apartment,
    const string& city,
    const string& state,
    const string& zipCode
    ) {
            this->address = address;
            this->apartment = apartment;
            this->city = city;
            this->state = state;
            this->zipCode = zipCode;
    }

bool Location::isValid() const {
    return !address.empty() && !city.empty() && !state.empty() && !zipCode.empty();
}
