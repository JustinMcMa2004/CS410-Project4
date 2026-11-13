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
        "John Smith", destination, "500001", {5.0, 6.0, 7.0}, 55.5,
        destination
    );
    package.addLocation(Location("123 Placeholder Street", "2", "Boston", "MA", "02118"));
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
    Location currentLocation(
        "123 Placeholder Street", "2", "Boston", "MA", "02118"
    );

    Package package(
        "John Smith", destination, "500001", {5.0, 6.0, 7.0}, 55.5,
        currentLocation
    );
    std::string errorMessage;
    registry.registerPackage(package, errorMessage);
    const std::string statusTime =
        registry.getPackages()[0].getCurrentStatus().getDateAndTime();
    std::string correctString = "================================\n";
    correctString += "        PACKAGE INFORMATION\n";
    correctString += "================================\n";
    correctString += "Recipient: John Smith\n";
    correctString += "Destination: 296 Washington St, Apt. 1 Boston, MA 02108\n";
    correctString += "ID: 500001\n";
    correctString += "Length x Width x Height (inches): 5.000000 x 6.000000 x 7.000000\n";
    correctString += "Weight (pounds): 55.500000\n";
    correctString += "Requires Signature?: No\n";
    correctString += "Is Fragile?: No\n";
    correctString += "\n================================\n";
    correctString += "       CURRENT DELIVERY STATUS\n";
    correctString += "================================\n";
    correctString += "Current Location: 123 Placeholder Street, Apt. 2 Boston, MA 02118\n";
    correctString += "Status: Registered\n";
    correctString += "Time: " + statusTime;
    correctString += "\n";
    correctString += "Delivered?: No\n";
    correctString += "\n================================\n";
    correctString += "          STATUS HISTORY\n";
    correctString += "================================\n";
    correctString += "1. Registered\n";
    correctString += "   Time: " + statusTime;
    correctString += "\n\n================================\n";
    correctString += "         LOCATION HISTORY\n";
    correctString += "================================\n";
    correctString += "1. 123 Placeholder Street, Apt. 2 Boston, MA 02118\n";
    EXPECT_EQ(registry.getIndex("500001"), 0);
    EXPECT_EQ(registry.getPackages()[0].getPackageInfo(), correctString);
}

// Test that a pacakage without an apartment can be searched and has the correct message. 
TEST(PackageSearchTest, validSearchWithoutApartment) {
    PackageRegistry registry;

    Location destination(
        "296 Washington St", "", "Boston", "MA", "02108"
    );
    Location currentLocation(
        "123 Placeholder Street", "2", "Boston", "MA", "02118"
    );

    Package package(
        "John Smith", destination, "500001", {5.0, 6.0, 7.0}, 55.5,
        currentLocation
    );
    std::string errorMessage;
    registry.registerPackage(package, errorMessage);
    const std::string statusTime =
        registry.getPackages()[0].getCurrentStatus().getDateAndTime();
    std::string correctString = "================================\n";
    correctString += "        PACKAGE INFORMATION\n";
    correctString += "================================\n";
    correctString += "Recipient: John Smith\n";
    correctString += "Destination: 296 Washington St Boston, MA 02108\n";
    correctString += "ID: 500001\n";
    correctString += "Length x Width x Height (inches): 5.000000 x 6.000000 x 7.000000\n";
    correctString += "Weight (pounds): 55.500000\n";
    correctString += "Requires Signature?: No\n";
    correctString += "Is Fragile?: No\n";
    correctString += "\n================================\n";
    correctString += "       CURRENT DELIVERY STATUS\n";
    correctString += "================================\n";
    correctString += "Current Location: 123 Placeholder Street, Apt. 2 Boston, MA 02118\n";
    correctString += "Status: Registered\n";
    correctString += "Time: " + statusTime;
    correctString += "\n";
    correctString += "Delivered?: No\n";
    correctString += "\n================================\n";
    correctString += "          STATUS HISTORY\n";
    correctString += "================================\n";
    correctString += "1. Registered\n";
    correctString += "   Time: " + statusTime;
    correctString += "\n\n================================\n";
    correctString += "         LOCATION HISTORY\n";
    correctString += "================================\n";
    correctString += "1. 123 Placeholder Street, Apt. 2 Boston, MA 02118\n";
    EXPECT_EQ(registry.getIndex("500001"), 0);
    EXPECT_EQ(registry.getPackages()[0].getPackageInfo(), correctString);
}

TEST(PackageSearchTest, validSearchWithoutAdditionalLocation) {
    PackageRegistry registry;

    Location destination(
        "296 Washington St", "", "Boston", "MA", "02108"
    );

    Package package(
        "John Smith", destination, "500001", {5.0, 6.0, 7.0}, 55.5,
        destination
    );
    std::string errorMessage;
    registry.registerPackage(package, errorMessage);
    EXPECT_EQ(registry.getIndex("500001"), 0);
    EXPECT_EQ(registry.getPackages()[0].getPackageID(), "500001");
}

TEST(PackageSearchTest, validSearchWithMultiplePackages) {
    PackageRegistry registry;

    Location destination1(
        "296 Washington St", "", "Boston", "MA", "02108"
    );

    Package package1(
        "John Smith", destination1, "500001", {5.0, 6.0, 7.0}, 55.5,
        destination1
    );
    package1.addLocation(Location("123 Placeholder Street", "2", "Boston", "MA", "02118"));
    std::string errorMessage;
    registry.registerPackage(package1, errorMessage);

    Location destination2(
        "1731 Massachusetts Ave", "", "Cambridge", "MA", "02138"
    );
    Location currentLocation2(
        "456 Placeholder Avenue", "3", "Boston", "MA", "02118"
    );

    Package package2(
        "Alice Bob", destination2, "P21000", {2.0, 3.0, 4.0}, 9.5,
        currentLocation2
    );
    registry.registerPackage(package2, errorMessage);
    const std::string statusTime =
        registry.getPackages()[1].getCurrentStatus().getDateAndTime();
    std::string correctString = "================================\n";
    correctString += "        PACKAGE INFORMATION\n";
    correctString += "================================\n";
    correctString += "Recipient: Alice Bob\n";
    correctString += "Destination: 1731 Massachusetts Ave Cambridge, MA 02138\n";
    correctString += "ID: P21000\n";
    correctString += "Length x Width x Height (inches): 2.000000 x 3.000000 x 4.000000\n";
    correctString += "Weight (pounds): 9.500000\n";
    correctString += "Requires Signature?: No\n";
    correctString += "Is Fragile?: No\n";
    correctString += "\n================================\n";
    correctString += "       CURRENT DELIVERY STATUS\n";
    correctString += "================================\n";
    correctString += "Current Location: 456 Placeholder Avenue, Apt. 3 Boston, MA 02118\n";
    correctString += "Status: Registered\n";
    correctString += "Time: " + statusTime;
    correctString += "\n";
    correctString += "Delivered?: No\n";
    correctString += "\n================================\n";
    correctString += "          STATUS HISTORY\n";
    correctString += "================================\n";
    correctString += "1. Registered\n";
    correctString += "   Time: " + statusTime;
    correctString += "\n\n================================\n";
    correctString += "         LOCATION HISTORY\n";
    correctString += "================================\n";
    correctString += "1. 456 Placeholder Avenue, Apt. 3 Boston, MA 02118\n";
    EXPECT_EQ(registry.getIndex("P21000"), 1);
    EXPECT_EQ(registry.getPackages()[1].getPackageInfo(), correctString);
}