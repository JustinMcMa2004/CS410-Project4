/*
Test checks if

1. trying to register a new valid package works
2. trying to register a new invalid package fails
3. trying to register a package with a duplicate ID fails

*/



#include <gtest/gtest.h>
#include "PackageRegistry.h"

// Test that a valid package is registered successfully.
TEST(PackageRegistrationTest, RegistersValidPackage) {
    PackageRegistry registry;
    string errorMessage;

    Location destination(
        "123 Main Street", "3B", "Boston", "MA", "02125"
    );

    Package package(
        "Alex Smith", destination, "PKG001",
        {10.0, 5.0, 4.0}, 2.5
    );

    EXPECT_TRUE(registry.registerPackage(package, errorMessage));
    EXPECT_TRUE(errorMessage.empty());
    EXPECT_TRUE(registry.isRegistered("PKG001"));
    EXPECT_EQ(registry.getPackages().size(), 1);
}

// Test that a package missing required information is rejected.
TEST(PackageRegistrationTest, RejectsInvalidPackage) {
    PackageRegistry registry;
    string errorMessage;

    // An empty recipient makes this package invalid.
    Location destination(
        "123 Main Street", "3B", "Boston", "MA", "02125"
    );

    Package package(
        "", destination, "PKG002",
        {10.0, 5.0, 4.0}, 2.5
    );

    EXPECT_FALSE(registry.registerPackage(package, errorMessage));
    EXPECT_EQ(
        errorMessage,
        "package could not be registered, missing information."
    );
    EXPECT_FALSE(registry.isRegistered("PKG002"));
    EXPECT_TRUE(registry.getPackages().empty());
}

// Test that a tracking ID cannot be registered twice.
TEST(PackageRegistrationTest, RejectsDuplicateTrackingID) {
    PackageRegistry registry;
    string errorMessage;

    Location destination(
        "123 Main Street", "3B", "Boston", "MA", "02125"
    );

    Package firstPackage(
        "Alex Smith", destination, "PKG003",
        {10.0, 5.0, 4.0}, 2.5
    );

    Package duplicatePackage(
        "Jordan Lee", destination, "PKG003",
        {8.0, 4.0, 3.0}, 1.5
    );

    ASSERT_TRUE(registry.registerPackage(firstPackage, errorMessage));

    EXPECT_FALSE(registry.registerPackage(duplicatePackage, errorMessage));
    EXPECT_EQ(errorMessage, "package already registered.");
    EXPECT_EQ(registry.getPackages().size(), 1);
}

/*
Added regression test for whitespace-only input cases
*/


// Reject a package when the recipient contains only whitespace.
TEST(PackageRegistrationTest, RejectsWhitespaceOnlyRecipient) {
    PackageRegistry registry;
    string errorMessage;

    Location destination(
        "123 Main Street", "3B", "Boston", "MA", "02125"
    );

    Package package(
        "   ", destination, "PKG004",
        {10.0, 5.0, 4.0}, 2.5
    );

    EXPECT_FALSE(registry.registerPackage(package, errorMessage));
    EXPECT_EQ(
        errorMessage,
        "package could not be registered, missing information."
    );
}

// Reject a package when the destination address contains only whitespace.
TEST(PackageRegistrationTest, RejectsWhitespaceOnlyStreetAddress) {
    PackageRegistry registry;
    string errorMessage;

    Location destination(
        "   ", "3B", "Boston", "MA", "02125"
    );

    Package package(
        "Alex Smith", destination, "PKG005",
        {10.0, 5.0, 4.0}, 2.5
    );

    EXPECT_FALSE(registry.registerPackage(package, errorMessage));
    EXPECT_EQ(
        errorMessage,
        "package could not be registered, missing information."
    );
}


 // Reject a package when its tracking ID contains only whitespace.
TEST(PackageRegistrationTest, RejectsWhitespaceOnlyTrackingID) {
    PackageRegistry registry;
    string errorMessage;

    Location destination(
        "123 Main Street", "3B", "Boston", "MA", "02125"
    );

    Package package(
        "Alex Smith", destination, "   ",
        {10.0, 5.0, 4.0}, 2.5
    );

    EXPECT_FALSE(registry.registerPackage(package, errorMessage));
    EXPECT_EQ(
        errorMessage,
        "package could not be registered, missing information."
    );
    EXPECT_TRUE(registry.getPackages().empty());
}
