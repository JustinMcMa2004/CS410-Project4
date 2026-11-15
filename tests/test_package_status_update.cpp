#include <gtest/gtest.h>
#include "PackageRegistry.h"
#include "Status.h"
#include <string>

// Test 1: A valid delivery status can be updated.
TEST(PackageStatusUpdateTest, UpdateStatusSuccessfully) {
    PackageRegistry registry;
    Location location("123 Main St", "", "Boston", "MA", "02125");
    Package package("John Smith", location, "PKG001",
                    {10, 8, 6}, 5.0, location);

    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    EXPECT_TRUE(registry.updatePackageStatus(
        "PKG001", "Ready to be Delivered", errorMessage));

    int index = registry.getIndex("PKG001");
    ASSERT_NE(index, -1);
    EXPECT_EQ(registry.getPackages()[index]
                  .getCurrentStatus().getStatusMessage(),
              "Ready to be Delivered");
    EXPECT_TRUE(errorMessage.empty());
}

// Test 2: The updated status is retained when viewed again.
TEST(PackageStatusUpdateTest, RetainUpdatedStatus) {
    PackageRegistry registry;
    Location location("123 Main St", "", "Boston", "MA", "02125");
    Package package("John Smith", location, "PKG002",
                    {10, 8, 6}, 5.0, location);

    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    ASSERT_TRUE(registry.updatePackageStatus(
        "PKG002", "Delivered", errorMessage));

    int index = registry.getIndex("PKG002");
    ASSERT_NE(index, -1);

    EXPECT_EQ(registry.getPackages()[index]
                  .getCurrentStatus().getStatusMessage(),
              "Delivered");
}

// Test 3: Invalid delivery statuses are rejected.
TEST(PackageStatusUpdateTest, RejectInvalidStatus) {
    PackageRegistry registry;
    Location location("123 Main St", "", "Boston", "MA", "02125");
    Package package("John Smith", location, "PKG003",
                    {10, 8, 6}, 5.0, location);

    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    EXPECT_FALSE(registry.updatePackageStatus(
        "PKG003", "Unknown Status", errorMessage));

    EXPECT_EQ(errorMessage, "invalid delivery status.");

    int index = registry.getIndex("PKG003");
    ASSERT_NE(index, -1);
    EXPECT_EQ(registry.getPackages()[index]
                  .getCurrentStatus().getStatusMessage(),
              "Registered");
}

// Test 4: Unknown tracking IDs are rejected.
TEST(PackageStatusUpdateTest, RejectUnknownTrackingID) {
    PackageRegistry registry;
    std::string errorMessage;

    EXPECT_FALSE(registry.updatePackageStatus(
        "PKG999", "Delivered", errorMessage));

    EXPECT_EQ(errorMessage, "package not found.");
}

// Test 5: Updating the status preserves its history.
TEST(PackageStatusUpdateTest, PreserveStatusHistory) {
    PackageRegistry registry;
    Location location("123 Main St", "", "Boston", "MA", "02125");
    Package package("John Smith", location, "PKG004",
                    {10, 8, 6}, 5.0, location);

    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    ASSERT_TRUE(registry.updatePackageStatus(
        "PKG004", "Ready to be Delivered", errorMessage));
    ASSERT_TRUE(registry.updatePackageStatus(
        "PKG004", "Out for Delivery", errorMessage));

    int index = registry.getIndex("PKG004");
    ASSERT_NE(index, -1);

    auto history = registry.getPackages()[index].getStatusHistory();

    ASSERT_EQ(history.size(), 3u);
    EXPECT_EQ(history[0].getStatusMessage(), "Registered");
    EXPECT_EQ(history[1].getStatusMessage(), "Ready to be Delivered");
    EXPECT_EQ(history[2].getStatusMessage(), "Out for Delivery");
}
