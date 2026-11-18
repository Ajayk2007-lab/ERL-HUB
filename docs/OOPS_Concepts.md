# Object-Oriented Programming (OOPS) Concepts in ERL-HUB

This document provides a comprehensive technical reference for every C++ and Object-Oriented Programming concept implemented in the **Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)**.

---

## Table of Contents
1. [Unit I: Namespaces](#1-namespaces)
2. [Unit I: Function Overloading](#2-function-overloading)
3. [Unit I: Call by Reference](#3-call-by-reference)
4. [Unit I: Return by Reference](#4-return-by-reference)
5. [Unit I: Inline Functions](#5-inline-functions)
6. [Unit I: Friend Class](#6-friend-class)
7. [Unit II: Encapsulation & Const Correctness](#7-encapsulation--const-correctness)
8. [Unit II: Static Data Members & Static Functions](#8-static-data-members--static-functions)
9. [Unit II: Dynamic Constructors & Dynamic Memory](#9-dynamic-constructors--dynamic-memory)
10. [Unit II: The Rule of Three (Deep Copy Constructor, Copy Assignment, Destructor)](#10-the-rule-of-three)
11. [Unit II: Operator Overloading (+=, -=, ==, <<, >>)](#11-operator-overloading)
12. [Unit III: Inheritance (Base Class ReliefVehicle & Derived Classes)](#12-inheritance)
13. [Unit III: Virtual Base Classes & The Diamond Problem](#13-virtual-base-classes--diamond-problem)
14. [Unit III: Containership / Composition](#14-containership--composition)
15. [Unit III: Runtime Polymorphism & Dynamic Binding](#15-runtime-polymorphism--dynamic-binding)
16. [Unit III: Virtual Destructors](#16-virtual-destructors)
17. [Unit IV: Custom Exception Hierarchy](#17-custom-exception-hierarchy)
18. [Unit IV: Binary File Persistence (std::fstream)](#18-binary-file-persistence)
19. [Unit IV: Atomic File Transactions & Rollback](#19-atomic-file-transactions--rollback)
20. [Unit IV: STL Containers & Standard Algorithms](#20-stl-containers--standard-algorithms)
21. [Unit IV: Custom Functors](#21-custom-functors)
22. [Unit IV: Generic Class Templates](#22-generic-class-templates)
23. [Unit IV: User-Defined Stream Manipulators](#23-user-defined-stream-manipulators)

---

## 1. Namespaces

### Definition
A namespace is a declarative region that provides a scope to the identifiers (names of types, functions, variables, etc.) inside it. Namespaces prevent name collisions in large, multi-developer projects.

### Where Implemented
- `ReliefOps::Logistics` in `include/DistributionDepot.h`, `include/ReliefVehicle.h`, `include/LogisticsManager.h`, etc.
- `ReliefOps::Inventory` in `include/ReliefCargo.h` and `include/Manifest.h`.
- `ReliefOps::Exceptions` in `include/Exceptions.h`.

### Code Example
```cpp
namespace ReliefOps {
namespace Inventory {
    class ReliefCargo { /* ... */ };
}
namespace Logistics {
    class DistributionDepot { /* ... */ };
}
}
```

### Why Used in ERL-HUB
Separates inventory domain structures from fleet logistical operations, preventing naming conflicts between depot inventory records and vehicle cargo payloads.

---

## 2. Function Overloading

### Definition
Function overloading is a compile-time polymorphism feature where two or more functions share the same name but have different parameter lists (different types or number of parameters).

### Where Implemented
- `LogisticsManager::allocateSupplies` in `include/LogisticsManager.h` and `src/LogisticsManager.cpp`.

### How It Works
1. **Volume-Based Allocation:** Takes `(DistributionDepot&, ReliefVehicle&, const std::string& cargoId, int quantity)`.
2. **Priority Medical Allocation:** Takes `(DistributionDepot&, ReliefVehicle&, int minPriorityLevel, double maxWeightKg)`.

### Code Example
```cpp
// Overload 1: Quantity/Volume-based
bool allocateSupplies(DistributionDepot& depot, ReliefVehicle& vehicle, 
                      const std::string& cargoId, int quantity);

// Overload 2: Priority-based emergency medical triage
bool allocateSupplies(DistributionDepot& depot, ReliefVehicle& vehicle, 
                      int minPriorityLevel, double maxWeightKg);
```

### Why Used in ERL-HUB
Enables the logistics manager to dispatch general supplies by exact count while allowing emergency field medics to request urgent supplies by priority tier and weight quota.

---

## 3. Call by Reference

### Definition
Call by reference passes the address/alias of an existing argument to a function rather than making a copy. Changes made to the parameter affect the caller's original object.

### Where Implemented
- `LogisticsManager::allocateSupplies(DistributionDepot& depot, ReliefVehicle& vehicle, ...)`
- `IncidentCommander::emergencyTriageOverride(DistributionDepot& depot, ...)`

### Code Example
```cpp
void modifyDepot(DistributionDepot& depot) {
    depot.restockSupplies("CRG-101", 50); // Directly modifies original depot
}
```

### Why Used in ERL-HUB
Ensures that inventory deductions and fuel consumption update the actual depot and vehicle objects in memory without incurring expensive copying overhead.

---

## 4. Return by Reference

### Definition
Return by reference returns an alias to an existing object rather than a temporary copy. This allows the returned reference to be used as an lvalue.

### Where Implemented
- `DistributionDepot::findCargoById(const std::string& cargoId)` in `include/DistributionDepot.h`.
- `LogisticsManager::findCargo()`, `getDepot()`, and `getVehicle()`.

### Code Example
```cpp
ReliefOps::Inventory::ReliefCargo& DistributionDepot::findCargoById(const std::string& cargoId) {
    for (auto& item : inventory) {
        if (item.getCargoId() == cargoId) {
            return item; // Direct reference to element stored in vector
        }
    }
    throw ReliefOps::Exceptions::EntityNotFoundException("Cargo not found");
}

// In client code:
depot.findCargoById("CRG-101").setAvailableQuantity(500); // In-place modification!
```

### Why Used in ERL-HUB
Allows client routines to inspect or modify stored cargo directly in the depot's collection without copying, while safeguarding missing IDs through custom exceptions.

---

## 5. Inline Functions

### Definition
An inline function is a function upon which the compiler is advised to replace calls with the actual function code to eliminate function call overhead (stack frame setup, register spilling).

### Where Implemented
- `calculateDistance` in `include/GPSBeacon.h`.
- `calculateTotalCargoWeight` in `include/ReliefCargo.h`.
- `calculateRemainingCapacity` and `isFuelSufficient` in `include/TransportVehicle.h`.

### Code Example
```cpp
inline double calculateRemainingCapacity(double maxPayloadKg, double currentLoadKg) {
    return (maxPayloadKg > currentLoadKg) ? (maxPayloadKg - currentLoadKg) : 0.0;
}
```

### Why Used in ERL-HUB
Used for frequently evaluated mathematical and validation checks during batch cargo loading and dispatch simulation.

---

## 6. Friend Class

### Definition
A friend class is a class whose member functions are granted direct access to the `private` and `protected` members of another class in which it is declared as a friend.

### Where Implemented
- `class IncidentCommander` declared as friend inside `DistributionDepot` and `ReliefCargo`.

### Code Example
```cpp
class DistributionDepot {
    friend class IncidentCommander;
private:
    std::vector<ReliefCargo> inventory;
    double currentStoredWeightKg;
    // ...
};

// In IncidentCommander:
bool IncidentCommander::emergencyTriageOverride(DistributionDepot& depot, const std::string& cId, int qty) {
    for (auto& item : depot.inventory) { // Direct private member access!
        if (item.cargoId == cId) {
            item.availableQuantity += qty;   // Bypasses public setters!
            return true;
        }
    }
    return false;
}
```

### Why Used in ERL-HUB
Models realistic military/NDRF command hierarchy: senior incident commanders hold executive legal authority to override depot quotas during mass-casualty triage without altering public interface encapsulation.

---

## 7. Encapsulation & Const Correctness

### Definition
Encapsulation bundles data members and the methods that manipulate them into a single unit (class) while restricting direct outside access. Const correctness ensures read-only methods do not modify object state.

### Where Implemented
- All core classes (`ReliefCargo`, `DistributionDepot`, `TransportVehicle`, `Manifest`).

### Code Example
```cpp
class ReliefCargo {
private:
    std::string cargoId;
    int availableQuantity;
public:
    int getAvailableQuantity() const { return availableQuantity; } // Const inspector
    void setAvailableQuantity(int qty) {
        if (qty < 0) throw ERLHubException("Negative quantity invalid");
        availableQuantity = qty; // Controlled mutator
    }
};
```

---

## 8. Static Data Members & Static Functions

### Definition
A static data member is shared by all instances of a class. A static member function can be invoked without an object instance and can only access static members.

### Where Implemented
- `DistributionDepot::totalRegisteredDepots` in `DistributionDepot.h/.cpp`.
- `TransportVehicle::totalOperationalVehicles` in `TransportVehicle.h/.cpp`.
- `LogisticsManager::totalDispatchedMetricTonnage` and `totalDispatchedMissions`.

### Code Example
```cpp
// Header:
class DistributionDepot {
    static int totalRegisteredDepots;
public:
    static int getTotalRegisteredDepots();
};

// Implementation:
int DistributionDepot::totalRegisteredDepots = 0;
int DistributionDepot::getTotalRegisteredDepots() {
    return totalRegisteredDepots;
}
```

### Why Used in ERL-HUB
Maintains global disaster response analytics (cumulative tonnage dispatched, total active depots) across disparate subsystems without resorting to global variables.

---

## 9. Dynamic Constructors & Dynamic Memory

### Definition
A dynamic constructor uses `new` or `new[]` to allocate memory on the free store (heap) during object instantiation.

### Where Implemented
- `Manifest::Manifest(..., int initialCapacity)` in `src/Manifest.cpp`.

### Code Example
```cpp
Manifest::Manifest(const std::string& id, const std::string& dest, const std::string& vId, int initialCapacity)
    : manifestId(id), destinationDepot(dest), assignedVehicleId(vId),
      capacity(initialCapacity > 0 ? initialCapacity : 5), count(0) {
    cargoRecords = new ReliefCargo[capacity]; // Heap allocation
}
```

---

## 10. The Rule of Three

### Definition
The Rule of Three states that if a class requires a user-defined destructor, copy constructor, or copy assignment operator, it almost certainly requires all three to safely manage resources.

### Where Implemented
- Class `Manifest` in `include/Manifest.h` and `src/Manifest.cpp`.

### How It Works
1. **Explicit Destructor:** Releases heap memory via `delete[] cargoRecords`.
2. **Deep Copy Constructor:** Allocates a new heap array and copies each `ReliefCargo` element independently.
3. **Copy Assignment Operator:** Protects against self-assignment (`this != &other`), deallocates old memory, allocates a new buffer, and copies all elements.

### Code Example
```cpp
// 1. Deep Copy Constructor
Manifest::Manifest(const Manifest& other)
    : manifestId(other.manifestId), destinationDepot(other.destinationDepot),
      assignedVehicleId(other.assignedVehicleId), capacity(other.capacity), count(other.count) {
    cargoRecords = new ReliefCargo[capacity];
    for (int i = 0; i < count; ++i) {
        cargoRecords[i] = other.cargoRecords[i]; // Deep value copy
    }
}

// 2. Copy Assignment Operator
Manifest& Manifest::operator=(const Manifest& other) {
    if (this != &other) {
        delete[] cargoRecords; // Free existing resource
        capacity = other.capacity;
        count = other.count;
        manifestId = other.manifestId;
        cargoRecords = new ReliefCargo[capacity]; // Fresh allocation
        for (int i = 0; i < count; ++i) {
            cargoRecords[i] = other.cargoRecords[i];
        }
    }
    return *this;
}

// 3. Destructor
Manifest::~Manifest() {
    delete[] cargoRecords;
    cargoRecords = nullptr;
}
```

### Why Used in ERL-HUB
Prevents double deletions and dangling pointer bugs when manifests are archived or passed through function scopes.

---

## 11. Operator Overloading

### Definition
Operator overloading allows user-defined types to redefine the behavior of C++ operators (`+=`, `-=`, `==`, `<<`, `>>`) for custom classes.

### Where Implemented
- `depot += cargo`: Restocks supplies at a depot.
- `depot -= cargo`: Deducts dispatched supplies from a depot.
- `cargo1 == cargo2` and `manifest1 == manifest2`: Identity comparison.
- `std::cout << cargo`: Stream insertion for formatted display.
- `std::cin >> cargo`: Stream extraction for user console input.

### Code Example
```cpp
// Depot restocking
DistributionDepot& DistributionDepot::operator+=(const ReliefCargo& cargo) {
    addOrUpdateCargo(cargo);
    return *this;
}

// Depot deduction
DistributionDepot& DistributionDepot::operator-=(const ReliefCargo& cargo) {
    deductSupplies(cargo.getCargoId(), cargo.getAvailableQuantity());
    return *this;
}

// Equality comparison
bool ReliefCargo::operator==(const ReliefCargo& other) const {
    return cargoId == other.cargoId;
}
```

---

## 12. Inheritance

### Definition
Inheritance allows a derived class to inherit data members and behaviors from a base class, establishing an "is-a" relationship and promoting code reuse.

### Where Implemented
- `ReliefVehicle` inherits from `TransportVehicle`.
- `AerialTransport`, `GroundConvoy`, and `AmphibiousCraft` inherit from `ReliefVehicle`.

---

## 13. Virtual Base Classes & Diamond Problem

### Definition
The Diamond Problem arises when a class derives from two classes that both inherit from the same base class, resulting in duplicate base-class subobjects and ambiguity. Virtual inheritance (`virtual public Base`) ensures that only one shared instance of the base class subobject exists in the most derived class.

### Hierarchy
```
             ReliefVehicle (Virtual Base Class)
                /       \
     AerialTransport   AmphibiousCraft (Virtual Inheritance)
                \       /
           HybridRescueVehicle (Multiple Inheritance)
```

### Code Example
```cpp
class AerialTransport : virtual public ReliefVehicle { /* ... */ };
class AmphibiousCraft : virtual public ReliefVehicle { /* ... */ };

class HybridRescueVehicle : public AerialTransport, public AmphibiousCraft {
public:
    HybridRescueVehicle(const std::string& id, ...)
        : ReliefVehicle(id, ...),        // Most derived class directly initializes virtual base!
          AerialTransport(id, ...),
          AmphibiousCraft(id, ...) {}
};
```

### Why Used in ERL-HUB
Represents an amphibious-VTOL rescue craft possessing both aeronautic rotorcraft and marine hydrofoil capabilities while sharing a single unified vehicle identifier, payload monitor, and fuel tank.

---

## 14. Containership / Composition

### Definition
Containership (composition) models a "has-a" relationship where an object of one class is embedded as a data member inside another independent class.

### Where Implemented
- `GPSBeacon` and `EmergencyLifeSupportUnit` contained inside `ReliefVehicle`.

### Code Example
```cpp
class ReliefVehicle : public TransportVehicle {
protected:
    GPSBeacon gpsBeacon;                   // Embedded composition object
    EmergencyLifeSupportUnit lifeSupport;   // Embedded composition object
public:
    GPSBeacon& getGPSBeacon() { return gpsBeacon; }
};
```

### Why Used in ERL-HUB
Demonstrates the architectural distinction between inheritance ("is-a") and composition ("has-a"). A vehicle *is a* relief vehicle, but it *has a* GPS beacon.

---

## 15. Runtime Polymorphism & Dynamic Binding

### Definition
Runtime polymorphism allows a function call to be bound to its implementation at runtime rather than compile time. Achieved using base-class pointers or references and `virtual` functions.

### Where Implemented
- Base pointer collection: `std::vector<ReliefVehicle*> fleet`.
- Virtual method: `virtual bool evaluateRoute(...) const = 0;`.

### How It Works
When `v->evaluateRoute()` is invoked on a `ReliefVehicle*`:
- If `v` points to a `GroundConvoy`, it checks road clearance and fails if landslide severity $\ge 6$.
- If `v` points to an `AerialTransport`, it ignores ground rubble and checks high-altitude gale winds.
- If `v` points to an `AmphibiousCraft`, it evaluates flood channels and water current.
- If `v` points to a `HybridRescueVehicle`, it dynamically switches modes between air and water corridors.

### Code Example
```cpp
std::vector<ReliefVehicle*> fleet;
fleet.push_back(new GroundConvoy(...));
fleet.push_back(new AerialTransport(...));

for (ReliefVehicle* v : fleet) {
    v->evaluateRoute("Valley Corridor", 45.0, 7); // Dynamic dispatch via vtable!
}
```

---

## 16. Virtual Destructors

### Definition
A destructor declared `virtual` in a base class ensures that when an object of a derived class is deleted through a base-class pointer, the derived class destructor is invoked first, followed by base class destructors.

### Where Implemented
- `virtual ~TransportVehicle();` and `virtual ~ReliefVehicle();`.

### Code Example
```cpp
ReliefVehicle* v = new HybridRescueVehicle("HYB-01");
delete v; // Correctly invokes ~HybridRescueVehicle, then parents, then ~ReliefVehicle!
```

---

## 17. Custom Exception Hierarchy

### Definition
Custom user-defined exceptions inheriting from `std::exception` allowing granular, domain-specific diagnostic handling.

### Where Implemented
- `include/Exceptions.h`:
  - `ERLHubException : public std::exception`
  - `CargoOverweightException : public ERLHubException`
  - `FuelDepletionException : public ERLHubException`
  - `DamagedCorridorException : public ERLHubException`
  - `InsufficientStockException : public ERLHubException`

### Code Example
```cpp
if (currentLoadKg + weightKg > maxPayloadCapacityKg) {
    throw CargoOverweightException(currentLoadKg + weightKg, maxPayloadCapacityKg);
}
```

---

## 18. Binary File Persistence

### Definition
Reading and writing binary data to persistent storage using `std::fstream` in `std::ios::binary` mode.

### How It Works in ERL-HUB
Avoids raw memory dumps of non-trivial objects (which contain dynamic string pointers). Strings are serialized by writing their length prefix followed by character data.

### Code Example
```cpp
void ReliefCargo::serialize(std::ofstream& out) const {
    uint32_t len = cargoId.size();
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    out.write(cargoId.data(), len);
    out.write(reinterpret_cast<const char*>(&availableQuantity), sizeof(availableQuantity));
    // ...
}
```

---

## 19. Atomic File Transactions & Rollback

### Definition
A two-phase commit file update strategy: writes data to a temporary `.tmp` file, verifies write success, and commits by replacing the target file atomically. If writing is interrupted or fails, the temporary file is deleted and the existing data is preserved.

### Where Implemented
- `FileManager::transactionalFileUpdate()` and `FileManager::atomicCommit()`.

---

## 20. STL Containers & Standard Algorithms

### Definition
Utilizing the Standard Template Library for robust, type-safe data structures (`std::vector`, `std::unordered_map`) and standard algorithms (`std::sort`, `std::for_each`).

### Where Implemented
- `std::vector<ReliefCargo>` and `std::unordered_map<std::string, size_t>`.
- `std::sort` for priority ranking.
- `std::for_each` for formatted catalog display.

---

## 21. Custom Functors

### Definition
A functor (function object) is any class or struct that overloads the function call operator `operator()`. Functors can be passed as arguments to standard algorithms.

### Where Implemented
- `struct MedicalPriorityComparator` in `include/ReliefCargo.h`.

### Code Example
```cpp
struct MedicalPriorityComparator {
    bool operator()(const ReliefCargo& a, const ReliefCargo& b) const {
        if (a.getEmergencyPriorityLevel() != b.getEmergencyPriorityLevel()) {
            return a.getEmergencyPriorityLevel() > b.getEmergencyPriorityLevel(); // Higher first
        }
        if (a.isMedical() != b.isMedical()) {
            return a.isMedical() && !b.isMedical(); // Medical first
        }
        return a.getCargoName() < b.getCargoName();
    }
};

// In client code:
std::sort(medicalList.begin(), medicalList.end(), MedicalPriorityComparator());
```

---

## 22. Generic Class Templates

### Definition
A template is a C++ construct that allows writing generic, type-independent code. Class templates generate family of classes parameterized by types and comparators.

### Where Implemented
- `template <typename T, typename Comparator = std::less<T>> class PriorityDispatchQueue` in `include/PriorityDispatchQueue.h`.

### Code Example
```cpp
template <typename T, typename Comparator = std::less<T>>
class PriorityDispatchQueue {
    std::vector<T> elements;
    Comparator comp;
public:
    void push(const T& item);
    void pop();
    const T& top() const;
    bool empty() const;
    size_t size() const;
};
```

---

## 23. User-Defined Stream Manipulators

### Definition
Functions or classes designed to be used with the stream extraction (`>>`) or insertion (`<<`) operators to format stream output.

### Where Implemented
- `tableHeader(title, width)` and `sectionDivider` in `include/LogisticsManager.h`.

### Code Example
```cpp
std::cout << tableHeader("CENTRAL EMERGENCY CARGO CATALOG", 100) << "\n";
std::cout << sectionDivider;
```
