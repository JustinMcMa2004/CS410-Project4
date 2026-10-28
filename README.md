# CS410-Project4
Our CS410 Course Project


Project goal:
- 
To develop a system that improves the organization and accessibility of package information from the initial intake through final delivery. The system should assist staff in managing packages and tracking their movement throughout the delivery process. This system should allow staff to register packages, manage deliveries, track and review a package's current and past delivery history. 


Stakeholders: 
-
- Customers (Person who sent the package)
- Delivery drivers
- Recipients (Person who receives the package)
- Customer Service
- Warehouse staff
- Dispatcher
- System Administrator 


Open questions:
-
1. Can customers use the system, and if so, what can they see?
2. What package information should each type of user be allowed to see?
3. What user roles should the system have, and what permissions should each role have?
4. What information should be required for each package when registering?
5. What information should be used to identify a package?
6. What information should be updated and stored in a package’s delivery history from intake to delivery?
7. How should users be able to search for packages?
8. How should packages enter the system? Are they registered at a company facility, received from external sellers such Amazon, or submitted through another process?
9. Who should be notified when package information or status is updated?
10. How should packages be assigned to delivery drivers? Should assignments be automatic or manual, and should they be based on factors such as the package’s destination area?
11. What delivery statuses should the system support, and how can the status change?
12. Are there different delivery service levels, such as express and standard delivery?
13. How should the system handle failed delivery attempts, potentially many times?
14. How should the system handle delayed, lost, or refunded packages?
15. Which features should be prioritized for the first version of the system? Registration? Search? Or status updates?
16. What additional features should be considered after the core requirements are completed?


Assumptions and Design questions:
-
1. Is the delivery order entered manually or calculated automatically?
2. Which delivery statuses and transitions are allowed?
3. What date and time formats, time zone, and measurement units will the system use?
4. Should finished package deliveries forever stay in the system? Or is there a point where they should be deleted? 
5. Is the Package class doing too much? 
6. Is there too much coupling among any of the classes? 


Object Discovery Table (Current Build):
-

| Object | Data/Attributes | Behavior/Methods |
| ------ | --------------- | ---------------- |
| Location | address, apartment, city, state, zipCode | isValid() |
| Status | statusMessage, date, time | readyToShip(), setTime(), getStatusMessage() |
| Package | recipient, destination, packageID, dimensions, weight, requiresSignature, isFragile, locationHistory, statusHistory, intakeTime | isValid(), getPackageID(), getCurrentStatus() |
| PackageRegistry | packages | registerPackage(), isRegistered(), getPackages() |
| Product | quantity | hasEnough() |

Object Discovery Table (Future):
-

| Object | Data/Attributes | Behavior/Methods |
| ------ | --------------- | ---------------- |
| Location | — | findDistance() |
| Package | deliveryAttempts | getCurrentLocation(), isDelivered(), updateStatus(), getStatusHistory(), addDeliveryAttempt(), getDeliveryAttempts() |
| Driver | employeeName, driverID, truckID, dispatchTime, assignedPackages | findRoute(), assignPackage(), getAssignedPackages() |
| DeliveryAttempt | attemptID, packageID, driverID, attemptTime, attemptDate, successful, failureReason | recordAttempt(), recordFailure(), isSuccessful() |


Class Diagrams:
-

| Location |
| -------- |
| - address : string  <br>- apartment : int <br>- city : string <br>- state : string <br>- zipCode : int |
| + findDistance(destination : const Location&) : double <br>+ displayLocation() : void |

| Status |
| ------ |
| - statusMessage : string <br>- date : vector<int> <br>- time : int |
| + readyToShip() : bool <br>+ setTime(time : int): void  |

| Package |
| ------- |
| - recipient : string <br>- destination : Location <br>- packageID : int <br>- dimensions : vector< double > <br>- weight : double <br>- requiresSignature : bool <br>- isFragile : bool <br>- locationHistory : vector< Location > <br>- statusHistory : vector < Status > <br>- intakeTime : string <br>- deliveryAttempts : vector< DeliveryAttempt > |
| + getCurrentLocation() : Location <br>+ getCurrentStatus() : Status <br>+ isDelivered() : bool <br>+ updateStatus(newStatus : const Status&) : bool <br>+	getStatusHistory() : vector< Status > <br>+ addDeliveryAttempt(attempt : const DeliveryAttempt&) : bool <br>+ getDeliveryAttempts() : vector< DeliveryAttempt > |

| Driver |
| ------ |
| - employeeName : string <br>- driverID : int <br>- truckID : int <br>- dispatchTime: int <br>- assignedPackages : vector< Package > |
| + findRoute() : vector< Package > <br>+ getPackage(packageID : int) : Package <br>+ assignPackage(package : Package&) : bool <br>+ getAssignedPackages() : vector< Package > |

| DeliveryAttempt |
| --------------- |
| - attemptID : int <br>- packageID : int <br>- driverID : int <br>- attemptTime : int <br>- attemptDate : vector< int > <br>- successful : bool <br>- failureReason : string |
| + recordAttempt(result : bool, time : int) : void <br>+ recordFailure(reason : string) : void <br>+ isSuccessful() : bool |


Design Decisions:
-
- **A Package has Locations.**
  - A Package contains its destination and location history. These values belong to the package and are modeled using composition.

- **A Package has Statuses.**
  - A Package owns its status history. Status changes are made through `updateStatus()` so that transition validation and history updates remain together.

- **A Package has DeliveryAttempts.**
  - A Package owns its delivery attempt records. `DeliveryAttempt` is a separate record rather than a subclass of `Status` because an attempt result and a delivery status represent different concepts.

- **A Driver has Packages.**
  - A Driver is associated with assigned packages. Driver does not own the packages, and removing a driver must not delete package records. The representation of package references remains to be determined.


Repository Structure:
-

| Directory / File | Description |
|---|---|
| `.github/workflows/` | GitHub Actions continuous integration workflow. |
| `data/` | Placeholder for project data. |
| `docs/Requirements/` | Project goal, stakeholders, and interview questions. |
| `include/` | C++ headers for the project classes and shared helpers. |
| `src/` | C++ implementations and the application entry point. |
| `tests/` | GoogleTest unit tests. |
| `CMakeLists.txt` | CMake configuration for building the application and tests. |
| `README.md` | Project overview, requirements context, and build/test instructions. |

```text
CS410-Project4/
├── .github/
│   └── workflows/
│       └── ci.yml
├── data/
│   └── .gitkeep
├── docs/
│   └── Requirements/
│       ├── interview-questions.md
│       ├── project_goal.md
│       └── stakeholders.md
├── include/
│   ├── Location.h
│   ├── Package.h
│   ├── PackageRegistry.h
│   ├── Product.h
│   ├── Status.h
│   └── isBlank.h
├── src/
│   ├── Location.cpp
│   ├── Package.cpp
│   ├── PackageRegistry.cpp
│   ├── Product.cpp
│   ├── Status.cpp
│   ├── main.cpp
│   └── .gitkeep
├── tests/
│   ├── test_package_registration.cpp
│   ├── test_product.cpp
│   └── .gitkeep
├── .gitignore
├── CMakeLists.txt
├── CONTRIBUTIONS.md
├── LICENSE
└── README.md
```

The `MyProject` executable is built in the `build/` directory, which is generated locally and is not tracked in the repository.


## Build and Test Instructions

### Prerequisites

- C++ compiler with C++17 support
- CMake 3.16 or later
- A compatible build tool (such as Ninja)

### Configure the Project

From the project root directory:

```bash
cmake -S . -B build
```

### Build the Project

```bash
cmake --build build
```

### Run the Application

After building, run the executable from the repository root:

```bash
./build/MyProject
```

### Run Automated Tests

```bash
ctest --test-dir build --output-on-failure
```

The project uses GoogleTest for automated testing. These commands have been verified on Windows using CMake and the GNU C++ compiler.
