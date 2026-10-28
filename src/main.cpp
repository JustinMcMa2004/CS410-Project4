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
// Keep prompting until the user enters a valid positive number.
bool readPositive(const std::string& prompt, double& value) {
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
}
int main() {
    PackageRegistry registry;
    std::cout << "Package Management System\n";
    std::string choice;
    while (readText("\n1. Register package\n0. Exit\nChoice: ", choice)) {
        if (choice == "0") break;
        if (choice != "1") {
            std::cout << "Choose 1 or 0.\n";
            continue;
        }
        std::string id, recipient, address, apartment, city, state, zip;
        double length, width, height, weight;
        if (!readText("Tracking ID: ", id) ||
            !readText("Recipient: ", recipient) ||
            !readText("Street address: ", address) ||
            !readText("Apartment (optional): ", apartment) ||
            !readText("City: ", city) ||
            !readText("State: ", state) ||
            !readText("ZIP code: ", zip) ||
            !readPositive("Length (inches): ", length) ||
            !readPositive("Width (inches): ", width) ||
            !readPositive("Height (inches): ", height) ||
            !readPositive("Weight (pounds): ", weight)) break;
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
    return 0;
}
