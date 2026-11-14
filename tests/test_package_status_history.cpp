/*
Test checks if

1. A new package starts with a registered status.
2. Status updates are kept in order and the latest status is current.
3. Searching for a package allows its status history to be viewed.
4. Delivery state follows the current status.
5. Location updates are kept in order and the latest location is current.

*/

#include <gtest/gtest.h>
#include "PackageRegistry.h"

// Test that a new package has its initial registered status.
TEST(PackageStatusHistoryTest, StartsWithRegisteredStatus) {
    Location destination(
        "123 Main Street", "", "Boston", "MA", "02125"
    );
    Location currentLocation(
        "10 Delivery Center", "", "Boston", "MA", "02125"
    );

    Package package(
        "Alex Smith", destination, "PKG100",
        {10.0, 5.0, 4.0}, 2.5, currentLocation
    );

    const std::vector<Status> history = package.getStatusHistory();
    ASSERT_EQ(history.size(), 1);
    EXPECT_EQ(history[0].getStatusMessage(), "Registered");
    EXPECT_FALSE(history[0].getDateAndTime().empty());
}

// Test that status updates are appended and the newest status is current.
TEST(PackageStatusHistoryTest, AppendsStatusUpdatesInOrder) {
    Location destination(
        "123 Main Street", "", "Boston", "MA", "02125"
    );
    Location currentLocation(
        "10 Delivery Center", "", "Boston", "MA", "02125"
    );

    Package package(
        "Alex Smith", destination, "PKG101",
        {10.0, 5.0, 4.0}, 2.5, currentLocation
    );
    package.updateStatus(Status("In Transit", "time 1"));
    package.updateStatus(Status("Out for Delivery", "time 2"));

    const std::vector<Status> history = package.getStatusHistory();
    ASSERT_EQ(history.size(), 3);
    EXPECT_EQ(history[0].getStatusMessage(), "Registered");
    EXPECT_EQ(history[1].getStatusMessage(), "In Transit");
    EXPECT_EQ(history[1].getDateAndTime(), "time 1");
    EXPECT_EQ(history[2].getStatusMessage(), "Out for Delivery");
    EXPECT_EQ(history[2].getDateAndTime(), "time 2");
    EXPECT_EQ(package.getCurrentStatus().getStatusMessage(), "Out for Delivery");
}

// Test that a package can be searched and its status history viewed.
TEST(PackageStatusHistoryTest, SearchShowsStatusHistoryInOrder) {
    PackageRegistry registry;
    std::string errorMessage;
    Location destination(
        "123 Main Street", "", "Boston", "MA", "02125"
    );
    Location currentLocation(
        "10 Delivery Center", "", "Boston", "MA", "02125"
    );

    Package package(
        "Alex Smith", destination, "PKG102",
        {10.0, 5.0, 4.0}, 2.5, currentLocation
    );
    package.updateStatus(Status("In Transit", "time 1"));
    package.updateStatus(Status("Out for Delivery", "time 2"));
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    const int packageIndex = registry.getIndex("PKG102");
    ASSERT_EQ(packageIndex, 0);
    const std::string packageInfo =
        registry.getPackages()[packageIndex].getPackageInfo();

    const std::size_t historyStart = packageInfo.find("STATUS HISTORY");
    ASSERT_NE(historyStart, std::string::npos);
    const std::size_t registered = packageInfo.find("1. Registered", historyStart);
    const std::size_t inTransit = packageInfo.find("2. In Transit", historyStart);
    const std::size_t outForDelivery =
        packageInfo.find("3. Out for Delivery", historyStart);
    EXPECT_NE(registered, std::string::npos);
    EXPECT_NE(inTransit, std::string::npos);
    EXPECT_NE(outForDelivery, std::string::npos);
    EXPECT_LT(registered, inTransit);
    EXPECT_LT(inTransit, outForDelivery);
    EXPECT_NE(packageInfo.find("   Time: time 1", inTransit), std::string::npos);
    EXPECT_NE(packageInfo.find("   Time: time 2", outForDelivery), std::string::npos);
}

// Test that delivery state reflects the latest status.
TEST(PackageStatusHistoryTest, DeliveryStateFollowsCurrentStatus) {
    Location destination(
        "123 Main Street", "", "Boston", "MA", "02125"
    );
    Location currentLocation(
        "10 Delivery Center", "", "Boston", "MA", "02125"
    );

    Package package(
        "Alex Smith", destination, "PKG103",
        {10.0, 5.0, 4.0}, 2.5, currentLocation
    );

    EXPECT_FALSE(package.isDelivered());
    package.updateStatus(Status("Delivered", "time 1"));
    EXPECT_TRUE(package.isDelivered());
    package.updateStatus(Status("Returned", "time 2"));
    EXPECT_FALSE(package.isDelivered());
}

// Test that location updates are appended and the newest location is current.
TEST(PackageStatusHistoryTest, AppendsLocationUpdatesInOrder) {
    Location destination(
        "123 Main Street", "", "Boston", "MA", "02125"
    );
    Location initialLocation(
        "10 Delivery Center", "", "Boston", "MA", "02125"
    );
    Location transitLocation(
        "20 Sorting Center", "", "Cambridge", "MA", "02138"
    );
    Location deliveryLocation(
        "30 Local Facility", "", "Somerville", "MA", "02143"
    );

    Package package(
        "Alex Smith", destination, "PKG104",
        {10.0, 5.0, 4.0}, 2.5, initialLocation
    );
    package.addLocation(transitLocation);
    package.addLocation(deliveryLocation);

    const std::vector<Location> history = package.getLocationHistory();
    ASSERT_EQ(history.size(), 3);
    EXPECT_EQ(history[0].getLocation(), initialLocation.getLocation());
    EXPECT_EQ(history[1].getLocation(), transitLocation.getLocation());
    EXPECT_EQ(history[2].getLocation(), deliveryLocation.getLocation());
    EXPECT_EQ(package.getCurrentLocation().getLocation(),
              deliveryLocation.getLocation());
}
