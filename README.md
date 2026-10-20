# CS410-Project4
Our CS410 Course Project

Project goal:
- 
To develop a system that improves the organization and accessibility of package information from the initial intake through final delivery. The system should assist staff in managing packages and tracking their movement throughout the delivery process. This system should allow staff to register packages, manage deliveries, track and review a package's current and past delivery history. 

Repository Structure

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

Object Discovery Table:
-

| Object    | Data/Attributes  | Behavior/Methods    |
| --------- | ---------------- | ------------------- |
| Location  | address, apartment, city, state, zipCode | findDistance(), ensures that each address is valid |
| Status    | statusMessage, date, time | readyToShip() |
| Package   | recipient, destination, packageID, dimensions, weight, requiresSignature, isFragile, locationHistory, statusHistory | getCurrentLocation(), isDelivered() |
| Driver    | employeeName, driverID, truckID, dispatchTime, assignedPackages | findRoute(), getPackage() |
| deliveryAttempt | attemptID, packageID, driverID, attemptTime, attemptDate, isSuccessful, failureReason | recordAttempt(), recordFailure(), isSuccessful() | 

Class Diagrams:
-

| Location |
| -------- |
| - address : string  <br>- apartment : int <br>- city : string <br>- state : string <br>- zipCode : int |
| + findDistance(destination : Location) : double <br>+ displayLocation() : void |

| Status |
| ------ |
| - statusMessage : string <br>- date : vector<int> <br>- time : int |
| + readyToShip() : bool <br>+ setTime(time : int): void  |

| Package |
| ------- |
| - recipient : string <br>- destination : Location <br>- packageID : int <br>- dimensions : vector< int > <br>- weight : int <br>- requiresSignature : bool <br>- isFragile : bool <br>- locationHistory : vector< Location > <br>- statusHistory : vector < Status > |
| + getCurrentLocation() : Location <br>+ getCurrentStatus() : Status <br>+ isDelivered() : bool |

| Driver |
| ------ |
| - employeeName : string <br>- driverID : int <br>- attemptTime : int <br>- attemptDate : vector< int > <br>- isSuccessful : bool <br>- failureReason : string |
| + findRoute() : vector< Package > <br>+ getPackage(packageID : int) : Pacakge |

| DeliveryAttempt |
| --------------- |
| - attemptID : int <br>- packageID : int <br>- driverID : int <br>- attemptTime : int <br>- attemptDate : vector< int > <br>- isSuccessful : bool <br>- failureReason : string |
| + recordAttempt(result : bool, time : int) : void <br>+ recordFailure(reason : string) : void <br>+ isSuccessful() : bool |
