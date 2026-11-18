# Requirement Traceability Matrix (RTM)
## Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)

**Course:** B.Tech Object-Oriented Programming Using C++ (CIC-211)  
**Institution:** Maharaja Agrasen Institute of Technology (MAIT), GGSIPU, Delhi  

This matrix maps every syllabus and assignment requirement to its concrete implementation in the codebase and its verification in the automated test suite.

---

| Assignment Requirement | Unit | Implemented Class | Function / Operator | Source File | Verification Test Case | Implementation Status |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Namespaces (`ReliefOps::Logistics`, `ReliefOps::Inventory`)** | Unit I | All Classes | Scope declarations | `include/ReliefCargo.h`, `include/DistributionDepot.h` | TC-01, TC-03 | **COMPLETE** |
| **Function Overloading** | Unit I | `LogisticsManager` | `allocateSupplies(...)` (Volume vs Priority Medical) | `include/LogisticsManager.h`, `src/LogisticsManager.cpp` | TC-06 | **COMPLETE** |
| **Call by Reference** | Unit I | `LogisticsManager`, `IncidentCommander` | `allocateSupplies(Depot&, Vehicle&, ...)`, `emergencyTriageOverride(Depot&, ...)` | `src/LogisticsManager.cpp`, `src/IncidentCommander.cpp` | TC-07 | **COMPLETE** |
| **Return by Reference** | Unit I | `DistributionDepot`, `LogisticsManager` | `findCargoById(id) -> ReliefCargo&`, `getDepot(id) -> Depot&` | `include/DistributionDepot.h`, `src/DistributionDepot.cpp` | TC-08 | **COMPLETE** |
| **Inline Functions** | Unit I | Utility Functions | `calculateDistance()`, `calculateRemainingCapacity()`, `calculateTotalCargoWeight()` | `include/GPSBeacon.h`, `include/ReliefCargo.h`, `include/TransportVehicle.h` | TC-01, TC-17 | **COMPLETE** |
| **Friend Class** | Unit I | `IncidentCommander` | Friend of `DistributionDepot` & `ReliefCargo`, `emergencyTriageOverride()` | `include/IncidentCommander.h`, `src/IncidentCommander.cpp` | TC-09 | **COMPLETE** |
| **Encapsulation & Const Correctness** | Unit II | `ReliefCargo`, `DistributionDepot`, `TransportVehicle` | Private data members, public getters with `const`, input validation | `src/ReliefCargo.cpp`, `src/TransportVehicle.cpp` | TC-01, TC-03 | **COMPLETE** |
| **Static Data Members & Static Functions** | Unit II | `DistributionDepot`, `TransportVehicle`, `LogisticsManager` | `totalRegisteredDepots`, `totalOperationalVehicles`, `totalDispatchedMetricTonnage` | `src/DistributionDepot.cpp`, `src/TransportVehicle.cpp`, `src/LogisticsManager.cpp` | TC-10 | **COMPLETE** |
| **Dynamic Constructors** | Unit II | `Manifest` | `Manifest(..., int initialCapacity)` | `include/Manifest.h`, `src/Manifest.cpp` | TC-11, TC-13 | **COMPLETE** |
| **Rule of Three: Deep Copy Constructor** | Unit II | `Manifest` | `Manifest(const Manifest& other)` | `src/Manifest.cpp` | TC-11 | **COMPLETE** |
| **Rule of Three: Copy Assignment Operator** | Unit II | `Manifest` | `Manifest& operator=(const Manifest& other)` | `src/Manifest.cpp` | TC-12 | **COMPLETE** |
| **Rule of Three: Explicit Destructor** | Unit II | `Manifest` | `~Manifest()` (deallocates `delete[] cargoRecords`) | `src/Manifest.cpp` | TC-13 | **COMPLETE** |
| **Operator Overloading: `+=`** | Unit II | `DistributionDepot` | `DistributionDepot& operator+=(const ReliefCargo&)` | `include/DistributionDepot.h`, `src/DistributionDepot.cpp` | TC-14 | **COMPLETE** |
| **Operator Overloading: `-=`** | Unit II | `DistributionDepot` | `DistributionDepot& operator-=(const ReliefCargo&)` | `include/DistributionDepot.h`, `src/DistributionDepot.cpp` | TC-14 | **COMPLETE** |
| **Operator Overloading: `==` and `!=`** | Unit II | `ReliefCargo`, `Manifest` | `bool operator==(const ReliefCargo&) const`, `bool operator==(const Manifest&) const` | `src/ReliefCargo.cpp`, `src/Manifest.cpp` | TC-14 | **COMPLETE** |
| **Operator Overloading: `<<` and `>>`** | Unit II | `ReliefCargo`, `Manifest`, `TransportVehicle` | `operator<<(ostream&, ...)`, `operator>>(istream&, ...)` | `src/ReliefCargo.cpp`, `src/Manifest.cpp` | TC-14, TC-27 | **COMPLETE** |
| **Base Class `ReliefVehicle`** | Unit III | `ReliefVehicle` | Abstract base class deriving from `TransportVehicle` | `include/ReliefVehicle.h`, `src/ReliefVehicle.cpp` | TC-15, TC-18 | **COMPLETE** |
| **Inheritance Hierarchy** | Unit III | `AerialTransport`, `GroundConvoy`, `AmphibiousCraft` | Public inheritance from `ReliefVehicle` | `include/AerialTransport.h`, `include/GroundConvoy.h`, `include/AmphibiousCraft.h` | TC-15, TC-18 | **COMPLETE** |
| **Virtual Base Classes & Diamond Problem** | Unit III | `AerialTransport`, `AmphibiousCraft`, `HybridRescueVehicle` | `virtual public ReliefVehicle` in parents; resolved in `HybridRescueVehicle` | `include/HybridRescueVehicle.h`, `src/vehicles/HybridRescueVehicle.cpp` | TC-16 | **COMPLETE** |
| **Containership (Composition)** | Unit III | `GPSBeacon`, `EmergencyLifeSupportUnit` | Embedded subsystem objects in `ReliefVehicle` | `include/GPSBeacon.h`, `include/EmergencyLifeSupportUnit.h`, `src/ReliefVehicle.cpp` | TC-17 | **COMPLETE** |
| **Runtime Polymorphism & Virtual Functions** | Unit III | `ReliefVehicle` hierarchy | `virtual bool evaluateRoute(...) const = 0` called via base pointers `vector<ReliefVehicle*>` | `src/vehicles/*.cpp`, `src/LogisticsManager.cpp` | TC-18 | **COMPLETE** |
| **Virtual Destructors** | Unit III | `ReliefVehicle`, `TransportVehicle` | `virtual ~ReliefVehicle()`, `virtual ~TransportVehicle()` | `include/ReliefVehicle.h`, `src/ReliefVehicle.cpp` | TC-19 | **COMPLETE** |
| **Custom Exception Hierarchy** | Unit IV | `ERLHubException`, `CargoOverweightException`, `FuelDepletionException`, `DamagedCorridorException` | Custom `what()` diagnostic messages | `include/Exceptions.h` | TC-02, TC-20 | **COMPLETE** |
| **Binary File Handling (`std::fstream`)** | Unit IV | `FileManager` | Binary mode read/write for inventory, depots, vehicles, logs | `include/FileManager.h`, `src/FileManager.cpp` | TC-21, TC-22 | **COMPLETE** |
| **Atomic File Transactions & Rollback** | Unit IV | `FileManager` | `transactionalFileUpdate()`, `atomicCommit()` | `src/FileManager.cpp` | TC-23 | **COMPLETE** |
| **STL Containers (`vector`, `unordered_map`)** | Unit IV | `LogisticsManager`, `DistributionDepot` | Dynamic arrays and hash-map lookups | `include/LogisticsManager.h`, `include/DistributionDepot.h` | TC-24 | **COMPLETE** |
| **Standard Algorithms (`std::sort`, `std::for_each`)** | Unit IV | `LogisticsManager` | Catalog iteration and priority sorting | `src/LogisticsManager.cpp` | TC-25 | **COMPLETE** |
| **Custom Functors** | Unit IV | `MedicalPriorityComparator` | Struct with overloaded `operator()` for sorting medical urgency | `include/ReliefCargo.h` | TC-25 | **COMPLETE** |
| **Class Templates (`PriorityDispatchQueue<T>`)** | Unit IV | `PriorityDispatchQueue<T, Comparator>` | Generic priority queue template (`push`, `pop`, `top`, `empty`, `size`) | `include/PriorityDispatchQueue.h` | TC-26 | **COMPLETE** |
| **User-Defined Stream Manipulators** | Unit IV | `tableHeader`, `sectionDivider` | Stream manipulator classes and helper functions | `include/LogisticsManager.h` | TC-27 | **COMPLETE** |
| **Interactive Console Interface** | UI | `main.cpp` | 13-option professional hierarchical menu and submenus | `src/main.cpp` | Interactive / CLI | **COMPLETE** |
| **Automated Test Harness** | Testing | `test_cases.cpp` | 28 automated test cases covering Units I-IV | `tests/test_cases.cpp` | All 28 Tests Pass | **COMPLETE** |
