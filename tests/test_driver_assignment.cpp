#include <gtest/gtest.h>
#include "DriverAssignmentManager.h"
#include "Driver.h"
#include "Location.h"
#include <chrono>
#include <ctime>


// Creates a fixed local date and time for testing driver schedules.
static std::chrono::system_clock::time_point makeTestTime(
    int day, int hour, int minute
) {
    std::tm time = {};
    time.tm_year = 2026 - 1900;
    time.tm_mon = 0;  // January
    time.tm_mday = day;
    time.tm_hour = hour;
    time.tm_min = minute;
    time.tm_isdst = -1;

    return std::chrono::system_clock::from_time_t(std::mktime(&time));
}


// A driver should match a package in their assigned delivery area.
TEST(DriverAssignmentTest, MatchingDeliveryArea) {
    Driver driver("D001", "Boston", "MA", 11, 30);
    Location destination("123 Main St", "", "Boston", "MA", "02118");

    EXPECT_TRUE(driver.servesArea(destination));
}

// A driver should not match a package outside their delivery area.
TEST(DriverAssignmentTest, DifferentDeliveryArea) {
    Driver driver("D001", "Boston", "MA", 11, 30);
    Location destination("456 Main St", "", "Cambridge", "MA", "02139");

    EXPECT_FALSE(driver.servesArea(destination));
}

// Both the city and state must match.
TEST(DriverAssignmentTest, DifferentState) {
    Driver driver("D001", "Boston", "MA", 11, 30);
    Location destination("789 Main St", "", "Boston", "NY", "14025");

    EXPECT_FALSE(driver.servesArea(destination));
}


// Driver departs after the package arrives, so assign today.
TEST(DriverAssignmentTest, DepartureToday) {
    Driver driver("D001", "Boston", "MA", 11, 0);

    auto packageTime = makeTestTime(15, 9, 0);
    auto expected = makeTestTime(15, 11, 0);

    EXPECT_EQ(driver.getNextDeparture(packageTime), expected);
}

// Driver has already departed, so use tomorrow's departure.
TEST(DriverAssignmentTest, DepartureTomorrow) {
    Driver driver("D001", "Boston", "MA", 8, 0);

    auto packageTime = makeTestTime(15, 9, 0);
    auto expected = makeTestTime(16, 8, 0);

    EXPECT_EQ(driver.getNextDeparture(packageTime), expected);
}

// A departure at exactly the earliest allowed time is valid.
TEST(DriverAssignmentTest, DepartureAtExactTime) {
    Driver driver("D001", "Boston", "MA", 9, 0);

    auto packageTime = makeTestTime(15, 9, 0);

    EXPECT_EQ(driver.getNextDeparture(packageTime), packageTime);
}


// Verify that a ready package is assigned to a driver serving its destination.
TEST(DriverAssignmentTest, AssignPackageSuccessfully) {
    // Create a package going to Boston.
    Location destination("123 Main St", "", "Boston", "MA", "02118");

    Package package(
        "John Smith",
        destination,
        "PKG001",
        {10, 8, 6},
        5.0,
        destination
    );

    // Mark the package as ready for delivery.
    package.updateStatus(Status("Ready to be Delivered", "Test time"));

    // Register the package.
    PackageRegistry registry;
    std::string errorMessage;

    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    // Create the assignment manager and add a Boston driver.
    DriverAssignmentManager manager(registry);
    Driver driver("D001", "Boston", "MA", 11, 0);

    manager.addDriver(driver);

    // Attempt to assign the package.
    EXPECT_TRUE(manager.assignPackage("PKG001", errorMessage));

    // Verify that the correct package was assigned to the correct driver.
    ASSERT_EQ(manager.getAssignments().size(), 1u);
    EXPECT_EQ(manager.getAssignments()[0].packageID, "PKG001");
    EXPECT_EQ(manager.getAssignments()[0].driverID, "D001");
    EXPECT_TRUE(errorMessage.empty());
}


// A package that isn't ready for delivery should not be assigned.
TEST(DriverAssignmentTest, PackageNotReady) {
    Location destination("123 Main St", "", "Boston", "MA", "02118");
    Package package("John Smith", destination, "PKG002",
                    {10, 8, 6}, 5.0, destination);

    PackageRegistry registry;
    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    DriverAssignmentManager manager(registry);
    manager.addDriver(Driver("D001", "Boston", "MA", 11, 0));

    // The package still has its initial "Registered" status.
    EXPECT_FALSE(manager.assignPackage("PKG002", errorMessage));
    EXPECT_EQ(errorMessage, "package is not ready to be delivered.");
    EXPECT_TRUE(manager.getAssignments().empty());
}

// A package cannot be assigned if no driver serves its area.
TEST(DriverAssignmentTest, NoMatchingDriver) {
    Location destination("123 Main St", "", "Boston", "MA", "02118");
    Package package("John Smith", destination, "PKG003",
                    {10, 8, 6}, 5.0, destination);

    package.updateStatus(Status("Ready to be Delivered", "Test time"));

    PackageRegistry registry;
    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    DriverAssignmentManager manager(registry);

    // This driver only delivers to Cambridge, not Boston.
    manager.addDriver(Driver("D001", "Cambridge", "MA", 11, 0));

    EXPECT_FALSE(manager.assignPackage("PKG003", errorMessage));
    EXPECT_EQ(errorMessage, "no driver serves this delivery area.");
    EXPECT_TRUE(manager.getAssignments().empty());
}

// A package should not be assigned to multiple drivers.
TEST(DriverAssignmentTest, DuplicateAssignment) {
    Location destination("123 Main St", "", "Boston", "MA", "02118");
    Package package("John Smith", destination, "PKG004",
                    {10, 8, 6}, 5.0, destination);

    package.updateStatus(Status("Ready to be Delivered", "Test time"));

    PackageRegistry registry;
    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    DriverAssignmentManager manager(registry);
    manager.addDriver(Driver("D001", "Boston", "MA", 11, 0));

    // First assignment succeeds, but the second should fail.
    EXPECT_TRUE(manager.assignPackage("PKG004", errorMessage));
    EXPECT_FALSE(manager.assignPackage("PKG004", errorMessage));

    EXPECT_EQ(errorMessage, "package already assigned.");
    EXPECT_EQ(manager.getAssignments().size(), 1u);
}


// If today's departure has passed, schedule the package for tomorrow.
TEST(DriverAssignmentTest, AssignDriverTomorrow) {
    Location destination("123 Main St", "", "Boston", "MA", "02118");

    // Package arrives January 15 at 8:00 AM.
    auto intakeTime = makeTestTime(15, 8, 0);

    Package package("John Smith", destination, "PKG006",
                    {10, 8, 6}, 5.0, destination,
                    false, false, intakeTime);

    package.updateStatus(Status("Ready to be Delivered", "Test time"));

    PackageRegistry registry;
    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    DriverAssignmentManager manager(registry);

    // Driver departs at 8:00 AM every day.
    manager.addDriver(Driver("D001", "Boston", "MA", 8, 0));

    // It is now January 15 at 9:00 AM.
    auto testTime = makeTestTime(15, 9, 0);

    EXPECT_TRUE(manager.assignPackage("PKG006", errorMessage, testTime));

    ASSERT_EQ(manager.getAssignments().size(), 1u);
    EXPECT_EQ(manager.getAssignments()[0].driverID, "D001");

    // The next available departure should be tomorrow at 8:00 AM.
    EXPECT_EQ(manager.getAssignments()[0].departureTime,
              makeTestTime(16, 8, 0));
}

// Verify that drivers cannot be registered with duplicate IDs.
TEST(DriverAssignmentTest, RejectDuplicateDriverID) {
    PackageRegistry registry;
    DriverAssignmentManager manager(registry);

    Driver driver1("D001", "Boston", "MA", 11, 30);
    Driver driver2("D001", "Cambridge", "MA", 14, 0);

    EXPECT_TRUE(manager.addDriver(driver1));
    EXPECT_FALSE(manager.addDriver(driver2));
}


// Test that city and state names match regardless of capitalization.
TEST(DriverAssignmentTest, CaseInsensitiveAreaMatching) {
    Driver driver("D001", "BOSTON", "ma", 11, 30);

    Location destination(
        "123 Main St", "", "Boston", "MA", "02118"
    );

    EXPECT_TRUE(driver.servesArea(destination));
}

TEST(DriverAssignmentTest, RejectInvalidDriver) {
    PackageRegistry registry;
    DriverAssignmentManager manager(registry);

    Driver invalidTime("D001", "Boston", "MA", 25, 70);
    Driver missingID("", "Boston", "MA", 11, 30);
    Driver missingCity("D002", "   ", "MA", 11, 30);
    Driver validDriver("D003", "Boston", "MA", 11, 30);

    EXPECT_FALSE(manager.addDriver(invalidTime));
    EXPECT_FALSE(manager.addDriver(missingID));
    EXPECT_FALSE(manager.addDriver(missingCity));
    EXPECT_TRUE(manager.addDriver(validDriver));
}


// Assign the package to the driver with the earliest available departure.
TEST(DriverAssignmentTest, AssignEarliestAvailableDriver) {
    Location destination("123 Main St", "", "Boston", "MA", "02118");

    // The package was received at 8:00 AM on January 15.
    auto intakeTime = makeTestTime(15, 8, 0);

    Package package("John Smith", destination, "PKG007",
                    {10, 8, 6}, 5.0, destination,
                    false, false, intakeTime);

    package.updateStatus(Status("Ready to be Delivered", "Test time"));

    PackageRegistry registry;
    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    DriverAssignmentManager manager(registry);

    // All three drivers serve Boston, but depart at different times.
    manager.addDriver(Driver("D001", "Boston", "MA", 8, 0));
    manager.addDriver(Driver("D002", "Boston", "MA", 11, 0));
    manager.addDriver(Driver("D003", "Boston", "MA", 14, 0));

    // Pretend the current time is January 15 at 9:00 AM.
    auto testTime = makeTestTime(15, 9, 0);

    EXPECT_TRUE(manager.assignPackage("PKG007", errorMessage, testTime));

    ASSERT_EQ(manager.getAssignments().size(), 1u);
    EXPECT_EQ(manager.getAssignments()[0].driverID, "D002");
    EXPECT_EQ(manager.getAssignments()[0].departureTime,
              makeTestTime(15, 11, 0));
}

// A package that doesn't exist cannot be assigned to a driver.
TEST(DriverAssignmentTest, PackageNotFound) {
    PackageRegistry registry;
    DriverAssignmentManager manager(registry);

    std::string errorMessage;

    EXPECT_FALSE(manager.assignPackage("PKG999", errorMessage));
    EXPECT_EQ(errorMessage, "package not found.");
    EXPECT_TRUE(manager.getAssignments().empty());
}

// If two drivers depart at the same time, select the first registered driver.
TEST(DriverAssignmentTest, SameDepartureTime) {
    Location destination("123 Main St", "", "Boston", "MA", "02118");

    auto intakeTime = makeTestTime(15, 8, 0);

    Package package("John Smith", destination, "PKG008",
                    {10, 8, 6}, 5.0, destination,
                    false, false, intakeTime);

    package.updateStatus(Status("Ready to be Delivered", "Test time"));

    PackageRegistry registry;
    std::string errorMessage;
    ASSERT_TRUE(registry.registerPackage(package, errorMessage));

    DriverAssignmentManager manager(registry);

    // Both drivers serve Boston and depart at 11:00 AM.
    manager.addDriver(Driver("D001", "Boston", "MA", 11, 0));
    manager.addDriver(Driver("D002", "Boston", "MA", 11, 0));

    auto testTime = makeTestTime(15, 9, 0);

    EXPECT_TRUE(manager.assignPackage("PKG008", errorMessage, testTime));

    ASSERT_EQ(manager.getAssignments().size(), 1u);
    EXPECT_EQ(manager.getAssignments()[0].driverID, "D001");
    EXPECT_EQ(manager.getAssignments()[0].departureTime,
              makeTestTime(15, 11, 0));
}