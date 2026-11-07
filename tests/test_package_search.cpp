/*
Test checks if

1. Trying to search for a package that does not exist fails. 
2. Trying to search for a package with an empty ID fails. 
3. Trying to serach for a package with a matching ID and no apartment # works. 
4. Trying to search for a package with a matching ID an apartment # works. 

*/


#include <gtest/gtest.h>
#include "PackageRegistry.h"

// Test that a non-existent package cannot be searched and the searcher returns -1. 
TEST(PackageSearchTest, SearchNonExistentPackage) {
    PackageRegistry registry; 
    EXPECT_EQ(registry.getIndex("123456"), -1);
}

// Test that a package with an ID that is blank cannot be searched and the searcher returns -1. 
TEST(PackageSearchTest, SearchWithEmptyID) {
    PackageRegistry registry;

    Location destination(
        "296 Washington St", "", "Boston", "MA", "02108"
    );

    Package package(
        "John Smith", destination, "500001", {5.0, 6.0, 7.0}, 55.5
    );
    std::string errorMessage;
    registry.registerPackage(package, errorMessage);

    EXPECT_EQ(registry.getIndex(""), -1);
}

// Test that a pacakage with an apartment can be searched and has the correct message. 
TEST(PackageSearchTest, validSearchWithApartment) {
    PackageRegistry registry;

    Location destination(
        "296 Washington St", "1", "Boston", "MA", "02108"
    );

    Package package(
        "John Smith", destination, "500001", {5.0, 6.0, 7.0}, 55.5
    );
    std::string errorMessage;
    registry.registerPackage(package, errorMessage);
    std::string correctString = "Recipient: John Smith\n";
    correctString += "Destination: 296 Washington St, Apt. 1 Boston, MA 02108\n";
    correctString += "ID: 500001\n";
    correctString += "Length x Width x Height (inches): 5.000000 x 6.000000 x 7.000000\n";
    correctString += "Weight (pounds): 55.500000\n";
    correctString += "Requires Signature?: No\n";
    correctString += "Is Fragile?: No\n";
    correctString += "Current Location: 123 Placeholder Street, Apt. 2 Boston, MA 02118\n";
    correctString += "Status: Registered\n";
    correctString += "Time (EST): ";
    correctString += registry.getPackages()[0].getCurrentStatus().getDateAndTime();
    EXPECT_EQ(registry.getIndex("500001"), 0);
    EXPECT_EQ(registry.getPackages()[0].getPackageInfo(), correctString);
}

// Test that a pacakage without an apartment can be searched and has the correct message. 
TEST(PackageSearchTest, validSearchWithoutApartment) {
    PackageRegistry registry;

    Location destination(
        "296 Washington St", "", "Boston", "MA", "02108"
    );

    Package package(
        "John Smith", destination, "500001", {5.0, 6.0, 7.0}, 55.5
    );
    std::string errorMessage;
    registry.registerPackage(package, errorMessage);
    std::string correctString = "Recipient: John Smith\n";
    correctString += "Destination: 296 Washington St Boston, MA 02108\n";
    correctString += "ID: 500001\n";
    correctString += "Length x Width x Height (inches): 5.000000 x 6.000000 x 7.000000\n";
    correctString += "Weight (pounds): 55.500000\n";
    correctString += "Requires Signature?: No\n";
    correctString += "Is Fragile?: No\n";
    correctString += "Current Location: 123 Placeholder Street, Apt. 2 Boston, MA 02118\n";
    correctString += "Status: Registered\n";
    correctString += "Time (EST): ";
    correctString += registry.getPackages()[0].getCurrentStatus().getDateAndTime();
    EXPECT_EQ(registry.getIndex("500001"), 0);
    EXPECT_EQ(registry.getPackages()[0].getPackageInfo(), correctString);
}
