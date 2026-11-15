#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include "PackageRegistry.h"
#include "DriverAssignmentManager.h"

#include <ctime>


namespace {
// Read and trim user input.
bool readText(const std::string& prompt, std::string& value) {
    std::cout << prompt;
    if (!std::getline(std::cin, value)) return false;
    const auto first = value.find_first_not_of(" \t\r\n");
    value = first == std::string::npos ? "" :
        value.substr(first, value.find_last_not_of(" \t\r\n") - first + 1);
    return true;
}

// Keep prompting until the user enters a valid positive double.
bool readPositiveDouble(const std::string& prompt, double& value) {
    std::string line;
    while (readText(prompt, line)) {
        std::istringstream input(line);
        char extra;
        if ((input >> value) && !(input >> extra) &&
            std::isfinite(value) && value > 0) return true;
        std::cout << "Enter a positive, finite number.\n";
    }
    return false;
}

// Keep prompting until the user enters a valid positive int.
bool readPositiveInt(const std::string& prompt, int& value) {
    std::string line;
    while (readText(prompt, line)) {
        std::istringstream input(line);
        char extra;
        if ((input >> value) && !(input >> extra) &&
            std::isfinite(value) && value >= 0) return true;
        std::cout << "Enter a nonnegative, finite number.\n";
    }
    return false;
}

}
int main() {
    PackageRegistry registry;
    DriverAssignmentManager manager(registry);

    std::cout << "Package Management System\n";
    int choice;
    while (readPositiveInt(
        "4. Assign package to driver\n"
        "3. Add delivery driver\n"
        "2. View existing package\n"
        "1. Register package\n" 
        "0. Exit\nChoice: ", choice)) {
        if (choice > 4 || choice < 0) {
            std::cout << "Please select one of the values listed.\n";
            continue;
        }
        if (choice == 0) break;
        if (choice == 1) {
            std::string id, recipient, address, apartment, city, state, zip, facility, currentCity, currentState, currentZip;
            double length, width, height, weight;
            if (!readText("Tracking ID: ", id) ||
                !readText("Recipient: ", recipient) ||
                !readText("Destination Address:\nStreet address: ", address) ||
                !readText("Apartment (optional): ", apartment) ||
                !readText("City: ", city) ||
                !readText("State: ", state) ||
                !readText("ZIP code: ", zip) ||
                !readText("Intial Package Location:\nFacility: ", facility) ||
                !readText("City: ", currentCity) ||
                !readText("State: ", currentState) ||
                !readText("ZIP code: ", currentZip) ||
                !readPositiveDouble("Length (inches): ", length) ||
                !readPositiveDouble("Width (inches): ", width) ||
                !readPositiveDouble("Height (inches): ", height) ||
                !readPositiveDouble("Weight (pounds): ", weight)) break;
            // Create a package using the provided information.
            Location destination(address, apartment, city, state, zip);
            Location initialLocation(facility, "", currentCity, currentState, currentZip);
            Package package(recipient, destination, id, {length, width, height}, weight, initialLocation);
            
            std::string error;
            
            // Register the package and display the result.
            if (registry.registerPackage(package, error)) {
                std::cout << "Registered package: " << package.getPackageID()
                        << "\nStatus: " << package.getCurrentStatus().getStatusMessage()
                        << "\nTotal packages: " << registry.getPackages().size() << '\n';
            } else {
                std::cout << "Registration failed: " << error << '\n';
            }
        }
        if (choice == 2) {
            std::string id;
            readText("Tracking ID: ", id);
            int index = registry.getIndex(id);
            std::string message;
            if (index == -1) message = "Invalid Tracking ID or Package does not exist.";
            else message = registry.getPackages()[index].getPackageInfo();
            std::cout << message << std::endl;
        }
        if (choice == 3) {
            std::string driverID, city, state;
            int hour, minute;

            if (!readText("Driver ID: ", driverID) ||
                !readText("Delivery City: ", city) ||
                !readText("Delivery State: ", state) ||
                !readPositiveInt("Departure Hour (0-23): ", hour) ||
                !readPositiveInt("Departure Minute (0-59): ", minute)) break;

            if (driverID.empty() || city.empty() || state.empty() ||
                hour > 23 || minute > 59) {
                std::cout << "Invalid driver information.\n";
                continue;
            }

            Driver driver(driverID, city, state, hour, minute);
            if (manager.addDriver(driver)) {
                std::cout << "Driver added successfully!\n";
            } else {
                std::cout << "Failed to add driver: Driver ID already exists.\n";
            }
        }

        if (choice == 4) {
            std::string packageID;
            std::string errorMessage;

            if (!readText("Enter Tracking ID: ", packageID)) break;

            if (manager.assignPackage(packageID, errorMessage)) {

                const DriverAssignment& assignment = manager.getAssignments().back();

                std::time_t departure = std::chrono::system_clock::to_time_t(
                    assignment.departureTime
                );

                std::cout << "Package assigned successfully!\n";
                std::cout << "Tracking ID: " << assignment.packageID << "\n";
                std::cout << "Assigned Driver: " << assignment.driverID << "\n";
                std::cout << "Scheduled Departure: " << std::ctime(&departure);
            }
            else {
                std::cout << "Assignment failed: " << errorMessage << "\n";
            }
        }
    }
    return 0;
}
