#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include "PackageRegistry.h"

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
    std::cout << "Package Management System\n";
    int choice;
    while (readPositiveInt("2. View existing package\n1. Register package\n0. Exit\nChoice: ", choice)) {
        if (choice > 2 || choice < 0) {
            std::cout << "Please select one of the values listed.\n";
            continue;
        }
        if (choice == 0) break;
        if (choice == 1) {
            std::string id, recipient, address, apartment, city, state, zip;
            double length, width, height, weight;
            if (!readText("Tracking ID: ", id) ||
                !readText("Recipient: ", recipient) ||
                !readText("Street address: ", address) ||
                !readText("Apartment (optional): ", apartment) ||
                !readText("City: ", city) ||
                !readText("State: ", state) ||
                !readText("ZIP code: ", zip) ||
                !readPositiveDouble("Length (inches): ", length) ||
                !readPositiveDouble("Width (inches): ", width) ||
                !readPositiveDouble("Height (inches): ", height) ||
                !readPositiveDouble("Weight (pounds): ", weight)) break;
            // Create a package using the provided information.
            Location destination(address, apartment, city, state, zip);
            Package package(recipient, destination, id, {length, width, height}, weight);
            
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
            std:string message;
            if (index == -1) message = "Invalid Tracking ID or Package does not exist.";
            else message = registry.getPackages()[index].getPackageInfo();
            std::cout << message << std::endl;
        }
    }
    return 0;
}
