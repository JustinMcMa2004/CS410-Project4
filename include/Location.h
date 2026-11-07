// Location class should store the destination address and valiate the required address fields

#ifndef LOCATION_H
#define LOCATION_H

#include <string>

using std::string;

class Location {
private:
    // store the destination's address info
    string address;
    string apartment;
    string city;
    string state;
    string zipCode;

public:
    // initialize location with its address info
    Location(
        const string& address, 
        const string& apartment,
        const string& city,
        const string& state,
        const string& zipCode
    );

    // return true if all required address fields are present
    bool isValid() const;

    // Returns a string that represents a properly formatted address. 
    string getLocation() const;
};

#endif