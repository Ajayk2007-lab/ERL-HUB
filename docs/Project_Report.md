# Comprehensive Project Report
## Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)

**Course:** B.Tech Object-Oriented Programming Using C++ (CIC-211)  
**Institution:** Maharaja Agrasen Institute of Technology (MAIT), GGSIPU, Delhi  
**Academic Year:** 2026–2027  

---

## 1. Title Page Information
- **Project Title:** Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)
- **Course Code:** CIC-211 (Object-Oriented Programming Using C++)
- **Department:** Department of Computer Science & Engineering / Information Technology
- **Institution:** Maharaja Agrasen Institute of Technology, PSP Area, Sector 22, Rohini, Delhi-110086
- **Affiliation:** Guru Gobind Singh Indraprastha University (GGSIPU), Dwarka, Delhi
- **Environment:** Visual Studio Code / MinGW GCC (C++11/14/17 standard)

---

## 2. Abstract
Following sudden seismic disasters in mountainous regions, multi-agency humanitarian relief operations face critical logistical challenges: compromised roads, inventory bottlenecks, specialized transport constraints, and medical urgency disparities. **ERL-HUB (Emergency Relief Logistics Hub)** is a robust, modular, production-standard C++ console platform designed to manage and simulate emergency relief operations across disaster zones. 

The software models multi-agency cooperation (NDRF, armed medical teams, civil rescue forces) through rigorous application of Object-Oriented Programming concepts. The architecture spans all four units of the academic curriculum: custom namespaces, call/return by reference, inline utilities, and friend classes (Unit I); encapsulation, static data tracking, dynamic constructors, dynamic array memory management adhering to the Rule of Three, and operator overloading (Unit II); deep inheritance hierarchies featuring virtual base classes to solve the Diamond Problem, containership/composition, and runtime polymorphism via virtual dispatch (Unit III); and custom exception hierarchies, transactional binary file persistence, STL algorithms, custom functors, template classes, and custom stream manipulators (Unit IV).

---

## 3. Introduction
Logistics management in disaster triage differs radically from commercial supply chain systems. When earthquakes devastate infrastructure, transportation corridors suffer severe landslides, vehicle payloads must be strictly balanced against fuel and altitude constraints, and medical life-saving supplies must be expedited over routine sustenance goods. 

ERL-HUB addresses these operational imperatives by providing a centralized coordination engine that monitors stock across regional distribution depots, allocates payloads according to volume and medical triage priority, dynamically evaluates multi-modal transport corridors, records immutable dispatch manifests, and ensures transactional audit logging.

---

## 4. Problem Statement
Disaster response authorities often struggle with:
1. Fragmented multi-agency inventory tracking leading to supply duplication or severe stock-outs.
2. Incompatible vehicle fleet routing (dispatching ground trucks to washed-out bridges or light drones with overweight generators).
3. Inability to dynamically prioritize critical surgical and pediatric medicine over bulky routine supplies during initial triage.
4. Software crashes and state corruption caused by unsafe memory manipulation or non-atomic file persistence during field operation.
5. Inability of field commanders to execute authorized emergency triage overrides when standard inventory quotas are reached.

---

## 5. Objectives
1. Build an end-to-end, functional, modular C++ application simulating post-earthquake logistics.
2. Demonstrate every mandatory concept in the university B.Tech OOPS syllabus (CIC-211) using practical, domain-specific implementations.
3. Guarantee memory safety with custom dynamic memory management strictly adhering to the Rule of Three.
4. Resolve multiple inheritance ambiguities (the Diamond Problem) using virtual base classes.
5. Provide transactional, crash-safe binary file persistence with automatic rollback mechanisms.
6. Provide an intuitive, interactive console interface backed by automated test suites.

---

## 6. Scope
The application encompasses regional post-earthquake logistics in the Uttarakhand Himalayan belt (Dehradun, Rishikesh, Chamoli). The scope includes:
- Managing 5 categories of relief cargo: Food/Ration Kits, Clean Water, Trauma Triage Medicine, Thermal Blankets, and Emergency Medical Equipment.
- Registering regional distribution depots with storage limits and localized stock tracking.
- Modeling four vehicle categories: Ground Convoys, Aerial Transports (Helicopters/Drones), Amphibious Craft, and Hybrid Multi-Modal Rescue Units.
- Real-time route feasibility assessment based on distance, fuel consumption, and terrain damage severity.
- High-priority medical dispatch queue management.
- Persisting state across application restarts using binary file streams.

---

## 7. System Architecture
ERL-HUB is built as a layered, modular architecture:
```
+-------------------------------------------------------------+
|                     Presentation Layer                      |
|       (Interactive CLI Menu, Custom Stream Manipulators)    |
+-------------------------------------------------------------+
                              |
+-------------------------------------------------------------+
|                     Orchestration Layer                     |
|           LogisticsManager, IncidentCommander               |
+-------------------------------------------------------------+
                              |
+-------------------------------------------------------------+
|                       Domain Model                          |
|  ReliefCargo, DistributionDepot, Manifest, ReliefVehicle    |
|   (GroundConvoy, AerialTransport, AmphibiousCraft, Hybrid)   |
|         Subsystems: GPSBeacon, LifeSupportUnit              |
+-------------------------------------------------------------+
                              |
+-------------------------------------------------------------+
|                      Persistence Layer                      |
|       FileManager (Atomic Commit/Rollback, Binary I/O)      |
+-------------------------------------------------------------+
```

---

## 8. Functional Requirements
- **FR-01 (Cargo Registration):** Register relief supplies with unique alphanumeric ID, category, unit weight, stock quantity, priority level (1-5), and medical classification.
- **FR-02 (Depot Management):** Maintain regional depots with max capacity limits; update stored weight automatically when cargo is added or removed.
- **FR-03 (Inventory Restocking & Deduction):** Restock supplies using `depot += cargo` and deduct supplies using `depot -= cargo`.
- **FR-04 (Supply Allocation):** Overloaded allocation functions supporting volume-based payload allocation and priority-based emergency medical allocation.
- **FR-05 (Fleet Vehicle Management):** Register and inspect diverse vehicle types with individual payload capacities, fuel reserves, GPS beacons, and life support systems.
- **FR-06 (Route Evaluation):** Dynamically assess terrain corridors via runtime polymorphism across ground, air, water, and hybrid vehicles.
- **FR-07 (Incident Commander Override):** Authorized commander can bypass quota limits using privileged friend class access.
- **FR-08 (Priority Dispatch Queue):** Priority queue ordering emergency requests by medical criticality.
- **FR-09 (Manifest Audit Trail):** Generate dynamic cargo manifests; archive using deep copy semantics.
- **FR-10 (Persistence):** Save and restore inventory, depots, fleet vehicles, and dispatch history to/from binary files.

---

## 9. Non-Functional Requirements
- **Performance:** Sub-millisecond lookup times using `std::unordered_map` hash indices.
- **Memory Safety:** Zero memory leaks, zero double deletions, and zero dangling pointers verified via destructor and deep copy testing.
- **Fault Tolerance:** Custom exceptions derived from `std::exception` prevent unexpected termination.
- **Portability:** C++11 compliant code compiling cleanly under GCC MinGW on Windows 10/11 and Linux.
- **Atomicity:** File persistence operates under transactional commit/rollback semantics.

---

## 10. Class Descriptions

### `ReliefCargo` (Namespace `ReliefOps::Inventory`)
Encapsulates relief cargo properties. Data members include `cargoId`, `cargoName`, `category`, `availableQuantity`, `unitOfMeasure`, `weightPerUnitKg`, `emergencyPriorityLevel`, and `medicalFlag`. Implements operators `==`, `!=`, `<<`, and `>>`. Declares `IncidentCommander` as friend class.

### `DistributionDepot` (Namespace `ReliefOps::Logistics`)
Represents regional logistics bases. Maintains `depotId`, `depotName`, `location`, `maxStorageCapacityKg`, `currentStoredWeightKg`, and `std::vector<ReliefCargo>`. Features static member `totalRegisteredDepots`. Implements `operator+=` for restocking, `operator-=` for dispatch deduction, and `findCargoById()` returning `ReliefCargo&` (Return by Reference).

### `Manifest` (Namespace `ReliefOps::Inventory`)
Implements dynamic array memory management for dispatch cargo records. Manages dynamically allocated `ReliefCargo* cargoRecords` buffer with `capacity` and `count`. Strictly adheres to the **Rule of Three** by implementing a parameterized dynamic constructor, deep copy constructor, copy assignment operator, and explicit `delete[]` destructor.

### `TransportVehicle` (Namespace `ReliefOps::Logistics`)
Encapsulates baseline vehicular properties: `vehicleId`, `vehicleName`, `maxPayloadCapacityKg`, `currentLoadKg`, `fuelLevelPercentage`, `operationalStatus`, and `currentLocation`. Tracks static fleet count `totalOperationalVehicles`.

### `ReliefVehicle` (Namespace `ReliefOps::Logistics`)
Abstract polymorphic base class deriving from `TransportVehicle`. Contains composition subsystems: `GPSBeacon` and `EmergencyLifeSupportUnit`. Declares pure virtual method `evaluateRoute()` and virtual method `calculateOperationalFeasibility()`. Features an explicit virtual destructor.

### `AerialTransport`, `GroundConvoy`, `AmphibiousCraft`
Specialized derived classes. `AerialTransport` and `AmphibiousCraft` virtually inherit `ReliefVehicle` (`virtual public ReliefVehicle`) to participate in the diamond inheritance hierarchy. Each implements specialized `evaluateRoute()` logic.

### `HybridRescueVehicle`
Represents an advanced amphibious-VTOL rescue platform. Derives via multiple inheritance from both `AerialTransport` and `AmphibiousCraft`, cleanly resolving the Diamond Problem through virtual inheritance. Directly initializes the `ReliefVehicle` virtual base subobject.

### `GPSBeacon` & `EmergencyLifeSupportUnit`
Containership components demonstrating object composition inside `ReliefVehicle`. `GPSBeacon` tracks geographical coordinates and signal status; `EmergencyLifeSupportUnit` monitors oxygen reserves and medical readiness.

### `IncidentCommander`
Executive triage commander class. Declared as a `friend class` inside `DistributionDepot` and `ReliefCargo`, allowing direct manipulation of private members during disaster overrides.

### `PriorityDispatchQueue<T, Comparator>`
Generic C++ template class providing priority queue capabilities (`push`, `pop`, `top`, `empty`, `size`, `display`) backed by heap/sorting algorithms.

### `FileManager`
Binary file persistence manager executing safe serialization of strings and objects. Features `transactionalFileUpdate()` executing two-phase atomic file replacement with rollback guarantees.

### `LogisticsManager`
Central facade coordinator maintaining system state, coordinating mission execution, updating static metrics (`totalDispatchedMetricTonnage`, `totalDispatchedMissions`), and driving the user menu.

---

## 11. Module Descriptions
1. **Module A: Inventory Management:** Central catalog registry, stock updates, validation checks, and functor-based medical sorting.
2. **Module B: Depot Management:** Multi-depot tracking, storage quota enforcement, restocking via `+=`, deduction via `-=`.
3. **Module C: Vehicle Fleet Management:** Polymorphic vehicle registration, embedded GPS/Life-Support monitoring, capacity checking, and fuel updating.
4. **Module D: Supply Allocation:** Overloaded volume-based and priority medical allocation algorithms.
5. **Module E: Emergency Dispatch Execution:** Mission validation, route evaluation, manifest creation, dynamic tonnage logging, and persistent audit trail recording.
6. **Module F: Incident Commander Dashboard:** Fleet readiness overview and privileged friend class triage overrides.

---

## 12. OOPS Concepts Implemented
- **Namespaces:** `ReliefOps::Logistics` and `ReliefOps::Inventory`.
- **Function Overloading:** Overloaded `allocateSupplies()` functions with distinct parameter signatures.
- **Call by Reference:** Used across all depot, vehicle, and cargo mutators (`&`).
- **Return by Reference:** `findCargoById()` returning non-const reference to existing cargo.
- **Inline Functions:** `calculateDistance`, `calculateRemainingCapacity`, `calculateTotalCargoWeight`, `isFuelSufficient`.
- **Friend Class:** `IncidentCommander` accessing private depot inventory and cargo priority.
- **Encapsulation:** Private data members, public accessors, `const` correctness.
- **Static Members & Functions:** `totalRegisteredDepots`, `totalOperationalVehicles`, `totalDispatchedMetricTonnage`, `totalDispatchedMissions`.
- **Dynamic Constructors & Rule of Three:** `Manifest` parameterized constructor, deep copy constructor, copy assignment operator, explicit destructor.
- **Operator Overloading:** `+=`, `-=`, `==`, `!=`, `<<`, `>>`.
- **Virtual Base Classes (Diamond Problem):** `HybridRescueVehicle` inheriting from `AerialTransport` and `AmphibiousCraft` with `virtual public ReliefVehicle`.
- **Containership (Composition):** `ReliefVehicle` containing `GPSBeacon` and `EmergencyLifeSupportUnit`.
- **Runtime Polymorphism:** Base-class pointer collection `vector<ReliefVehicle*>` calling virtual `evaluateRoute()`.
- **Virtual Destructors:** `virtual ~ReliefVehicle()` guaranteeing clean polymorphic deallocation.
- **Custom Exceptions:** Derived from `std::exception` with diagnostic `what()` messages.
- **Templates:** `PriorityDispatchQueue<T, Comparator>`.
- **STL Containers & Algorithms:** `std::vector`, `std::unordered_map`, `std::sort`, `std::for_each`.
- **Custom Functor:** `MedicalPriorityComparator`.
- **Custom I/O Manipulators:** `tableHeader` and `sectionDivider`.

---

## 13. Algorithm Explanations
1. **Priority Medical Allocation Algorithm:** Iterates through depot inventory, filters candidate items where `isMedical == true` and `priority >= minPriority`, sorts candidates using `MedicalPriorityComparator`, and iteratively packs items onto the vehicle until payload capacity is reached.
2. **Polymorphic Route Evaluation Algorithm:** Dispatches route parameters (`corridorName`, `distanceKm`, `terrainSeverity`) through base pointer `ReliefVehicle*`. Ground convoys fail if ground damage severity $\ge 6$ (landslides); choppers fail if wind/altitude severity $\ge 8$; boats fail if waterway currents $\ge 7$; hybrid units adaptively switch modes between air and water.
3. **Atomic File Persistence Algorithm:** Writes binary records to a temporary file (`.tmp`). Flushes and verifies file integrity. If valid, replaces the target file via two-phase rename. If writing fails, deletes temporary file and leaves existing valid records unchanged.

---

## 14. File Handling Implementation
All persistence is executed using `std::fstream` in binary mode (`std::ios::binary`). Because objects contain dynamic strings (`std::string`), raw byte-dumping is strictly avoided. Strings are serialized by prefixing a 32-bit length (`uint32_t len`), followed by exact character bytes. Containers write count prefixes followed by element-by-element serialization.

---

## 15. Exception Handling
Domain-specific exceptions derived from `ERLHubException` (`std::exception`):
- `CargoOverweightException`: Thrown when attempted payload exceeds max vehicle capacity.
- `FuelDepletionException`: Thrown when vehicle fuel is insufficient for the mission distance.
- `DamagedCorridorException`: Thrown when earthquake rubble blocks the transport corridor.
- `InsufficientStockException`: Thrown when depot supplies are insufficient.
- `DuplicateIdException`: Thrown on registration of non-unique identifiers.
- `EntityNotFoundException`: Thrown when searching for non-existent IDs.

All exceptions are caught in interactive menus and test suites, displaying clear diagnostics without application crashes.

---

## 16. Sample Output
```
================================================================================
        ERL-HUB COMPREHENSIVE OOPS VERIFICATION & TEST HARNESS                  
        Course: CIC-211 Object-Oriented Programming Using C++                  
        Maharaja Agrasen Institute of Technology (MAIT), GGSIPU                
================================================================================

[TC-01] Cargo Registration & Field Validation: >>> PASS <<<
[TC-02] Duplicate Cargo ID Exception Handling: >>> PASS <<<
[TC-03] Depot Registration & Properties: >>> PASS <<<
[TC-04] Inventory Restocking & Weight Update: >>> PASS <<<
[TC-05] Inventory Deduction & Stock Verification: >>> PASS <<<
[TC-06] Function Overloading (Volume vs Priority Medical): >>> PASS <<<
[TC-07] Call by Reference Actual Object Modification: >>> PASS <<<
[TC-08] Return by Reference Direct In-Place Modification: >>> PASS <<<
[TC-09] IncidentCommander Friend Class Access & Triage Override: >>> PASS <<<
[TC-10] Static Members & Static Methods Tracking: >>> PASS <<<
[TC-11] Manifest Rule of Three: Deep Copy Constructor Independence: >>> PASS <<<
[TC-12] Manifest Rule of Three: Copy Assignment Operator: >>> PASS <<<
[TC-13] Dynamic Memory Allocation & Destructor Cleanup: >>> PASS <<<
[TC-14] Operator Overloading (+=, -=, == on Cargo & Manifest): >>> PASS <<<
[TC-15] Virtual Inheritance Subclasses Instantiation: >>> PASS <<<
[TC-16] Diamond Problem Resolution (HybridRescueVehicle): >>> PASS <<<
[TC-17] Containership (GPSBeacon & EmergencyLifeSupportUnit Composition): >>> PASS <<<
[TC-18] Runtime Polymorphism Dynamic Method Dispatch: >>> PASS <<<
[TC-19] Virtual Destructor & Proper Resource Reclamation: >>> PASS <<<
[TC-20] Custom Exception Classes (Overweight, Fuel, Corridor): >>> PASS <<<
[TC-21] Binary File Handling (Save Inventory via fstream): >>> PASS <<<
[TC-22] Binary File Handling (Restore & Data Deserialization): >>> PASS <<<
[TC-23] Atomic File Transaction Rollback Behavior: >>> PASS <<<
[TC-24] STL Containers (std::vector & std::unordered_map): >>> PASS <<<
[TC-25] Custom Functor (MedicalPriorityComparator Sorting): >>> PASS <<<
[TC-26] Template Class PriorityDispatchQueue<T> Operations: >>> PASS <<<
[TC-27] User-Defined Stream Manipulators Formatting: >>> PASS <<<
[TC-28] End-to-End Emergency Relief Dispatch Workflow: >>> PASS <<<

================================================================================
 TEST SUMMARY: 28 / 28 Tests Passed (100.0% Success Rate)
================================================================================
```

---

## 17. Testing Results
All 28 automated test cases were executed and validated. The test suite verifies memory independence, dynamic polymorphism, operator correctness, atomic rollback, and complete end-to-end dispatch execution. Zero assertions failed and zero memory errors were encountered.

---

## 18. Limitations
1. Route simulation uses mathematical models rather than real-time satellite GIS telemetry.
2. Console-based user interface rather than a graphical desktop/web GIS mapping interface.
3. Single-threaded execution model; multi-threaded concurrent vehicle simulation is reserved for future revisions.

---

## 19. Future Scope
1. **Networked Multi-Depot Mesh:** Integrating socket programming for inter-depot peer-to-peer synchronization across remote field laptops.
2. **GIS Map Integration:** Exporting generated dispatch manifests and vehicle trajectories directly to KML/GeoJSON for display in Google Earth / QGIS.
3. **Multi-Threaded Real-Time Fleet Tracking:** Utilizing C++11 `<thread>` and `<mutex>` to simulate concurrent vehicle movement along corridors.

---

## 20. Conclusion
The **ERL-HUB (Emergency Relief Logistics Hub)** project provides an academic and technical demonstration of Object-Oriented Programming principles using C++. By modeling a realistic post-earthquake multi-agency relief logistics hub, the system moves beyond isolated toy examples and implements an integrated, fault-tolerant software system. Every requirement of the MAIT CIC-211 syllabus—spanning namespaces, reference semantics, friend classes, dynamic memory and the Rule of Three, operator overloading, virtual base classes solving the Diamond Problem, containership, runtime polymorphism, custom exceptions, binary persistence, templates, and STL algorithms—has been implemented in working code, verified by 28 automated tests, and documented.
