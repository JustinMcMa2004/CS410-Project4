#include "Location.h"
#include "isBlank.h"

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
    return !isBlank(address) && !isBlank(city) && !isBlank(state) && !isBlank(zipCode);
}
