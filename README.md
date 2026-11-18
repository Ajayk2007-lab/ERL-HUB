# Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)

**Academic Project for B.Tech Object-Oriented Programming Using C++ (CIC-211)**  
**Maharaja Agrasen Institute of Technology (MAIT), GGSIPU, Delhi**  
*C++ Standard: C++11 / C++14 / C++17 Compatible* | *Compiler: GCC / MinGW*

---

## 1. Project Introduction

Following a catastrophic earthquake in the Himalayan foothills (Dehradun, Rishikesh, Chamoli region), multiple civil and paramilitary disaster response agencies (such as NDRF, ITBP, Armed Forces Medical Services, State Disaster Management Authority, and Red Cross) establish temporary relief distribution bases. 

Managing the flow of life-saving medical supplies, food rations, potable water, and heavy rescue equipment across fractured transportation corridors requires a fault-tolerant, high-performance logistics management system.

**ERL-HUB (Emergency Relief Logistics Hub)** is a robust, modular, production-grade C++ console-based software platform engineered to orchestrate multi-agency disaster logistics. It bridges core computer science theory with practical systems engineering, implementing every requirement across all four units of the MAIT CIC-211 Object-Oriented Programming syllabus.

---

## 2. Key Objectives & Features

- **Unit I: Core Foundations & Call Semantics**
  - **Namespaces:** Architectural isolation using `ReliefOps::Logistics` and `ReliefOps::Inventory`.
  - **Function Overloading:** Genuine overloaded dispatch allocation functions (Volume-based vs. Priority-based emergency medical allocation).
  - **Call & Return by Reference:** Direct memory modification of depot inventory objects without copying (`findCargoById()` returning `ReliefCargo&`).
  - **Inline Functions:** Low-overhead utility functions (`calculateDistance`, `calculateRemainingCapacity`, `calculateTotalCargoWeight`).
  - **Friend Class:** Privileged access granted to `IncidentCommander` to bypass inventory quotas and execute emergency medical overrides.

- **Unit II: Encapsulation, Dynamic Memory & Operator Overloading**
  - **Encapsulation:** Strict access control, private data protection, and `const`-correct inspection methods.
  - **Static Members:** Global fleet metrics and tonnage accounting (`totalDispatchedMetricTonnage`, `totalOperationalVehicles`, `totalRegisteredDepots`).
  - **Dynamic Constructors & Rule of Three:** Custom `Manifest` class managing dynamically allocated cargo arrays with deep copy constructor, copy assignment operator, and explicit `delete[]` destructor.
  - **Operator Overloading:** Real-world overloaded operators:
    - `depot += cargo` (Restock depot inventory).
    - `depot -= cargo` (Deduct dispatched supplies).
    - `manifest1 == manifest2` and `cargo1 == cargo2` (Identity comparison & duplicate detection).
    - `os << cargo` and `is >> cargo` (Stream insertion & extraction).

- **Unit III: Inheritance, Virtual Base Classes & Polymorphism**
  - **Virtual Base Classes & Diamond Problem:** Clean resolution of the diamond inheritance hierarchy:
    ```
                 ReliefVehicle (Virtual Base Class)
                    /       \
         AerialTransport   AmphibiousCraft (Virtual Inheritance)
                    \       /
               HybridRescueVehicle (Multiple Inheritance)
    ```
  - **Containership (Composition):** Embedded subsystem objects (`GPSBeacon` for spatial tracking and `EmergencyLifeSupportUnit` for triage medical readiness).
  - **Runtime Polymorphism:** Base-class pointer collection (`std::vector<ReliefVehicle*>`) evaluating dynamic terrain corridors at runtime.
  - **Virtual Destructors:** Safe polymorphic memory deallocation.

- **Unit IV: Streams, Persistence, Exception Safety & STL**
  - **Custom Exceptions:** Domain-specific exceptions derived from `std::exception` (`CargoOverweightException`, `FuelDepletionException`, `DamagedCorridorException`, `InsufficientStockException`).
  - **Binary File Persistence:** High-performance binary serialization using `std::fstream` (`inventory.bin`, `depots.bin`, `vehicles.bin`, `dispatch_logs.bin`).
  - **Atomic File Updates:** Transactional two-phase file commit with write verification and automatic rollback if an update is interrupted.
  - **STL Containers & Algorithms:** `std::vector`, `std::unordered_map`, `std::sort`, and `std::for_each`.
  - **Custom Functor:** `MedicalPriorityComparator` for sorting critical medical cargo by urgency.
  - **Generic Template Class:** `PriorityDispatchQueue<T, Comparator>` for managing emergency mission requests.
  - **Custom Stream Manipulators:** Custom table headers and visual dividers.

---

## 3. Project Directory Architecture

```
ERL-HUB/
├── include/
│   ├── Exceptions.h                 # Custom exception hierarchy
│   ├── GPSBeacon.h                  # GPS tracking subsystem (Containership)
│   ├── EmergencyLifeSupportUnit.h   # Life support subsystem (Containership)
│   ├── ReliefCargo.h                # Relief supplies and functor
│   ├── DistributionDepot.h          # Depot management & operators
│   ├── TransportVehicle.h           # Base vehicle specs & static metrics
│   ├── ReliefVehicle.h              # Abstract polymorphic vehicle base
│   ├── AerialTransport.h            # Choppers & UAVs (Virtual inheritance)
│   ├── GroundConvoy.h               # Heavy trucks (Ground corridor evaluation)
│   ├── AmphibiousCraft.h            # Rescue boats (Virtual inheritance)
│   ├── HybridRescueVehicle.h        # Diamond problem resolution
│   ├── Manifest.h                   # Dynamic array & Rule of Three
│   ├── IncidentCommander.h          # Friend class for triage overrides
│   ├── PriorityDispatchQueue.h      # Generic template priority queue
│   ├── FileManager.h                # Binary persistence & atomic transactions
│   └── LogisticsManager.h           # Central system coordinator
│
├── src/
│   ├── GPSBeacon.cpp
│   ├── EmergencyLifeSupportUnit.cpp
│   ├── ReliefCargo.cpp
│   ├── DistributionDepot.cpp
│   ├── TransportVehicle.cpp
│   ├── ReliefVehicle.cpp
│   ├── Manifest.cpp
│   ├── IncidentCommander.cpp
│   ├── FileManager.cpp
│   ├── LogisticsManager.cpp
│   ├── main.cpp                     # Interactive console interface
│   └── vehicles/
│       ├── AerialTransport.cpp
│       ├── GroundConvoy.cpp
│       ├── AmphibiousCraft.cpp
│       └── HybridRescueVehicle.cpp
│
├── data/
│   ├── inventory.bin                # Persisted cargo catalog
│   ├── depots.bin                   # Persisted depot records
│   ├── vehicles.bin                 # Persisted vehicle fleet
│   └── dispatch_logs.bin            # Audit trail logs
│
├── docs/
│   ├── Project_Report.md            # Complete academic project report
│   ├── OOPS_Concepts.md             # Detailed OOPS concept documentation
│   ├── Class_Diagram.md             # Mermaid UML architecture diagrams
│   ├── Test_Cases.md                # 28 comprehensive test case specs
│   └── Traceability_Matrix.md       # Requirement traceability matrix
│
├── tests/
│   └── test_cases.cpp               # Automated unit and integration test suite
│
├── Makefile                         # GNU make build automation
├── README.md                        # Project overview and instructions
└── .gitignore                       # Git ignore configuration
```

---

## 4. Software Requirements & Compilation

### Requirements
- **Operating System:** Windows 10/11, Linux, or macOS.
- **Compiler:** GCC / MinGW with C++11 support (or C++14/C++17).
- **Libraries:** Standard C++ Runtime only (no third-party dependencies).

### Building with Makefile
Open terminal in the project root directory (`ERL-HUB`) and run:
```bash
# Build main application and test suite
make all

# Run test suite
make test

# Clean build artifacts
make clean
```

### Direct Compilation with g++
If `make` is not installed on Windows:
```powershell
# 1. Compile Main Application
g++ -std=c++11 -Wall -Wextra -Iinclude -o erl_hub.exe src/GPSBeacon.cpp src/EmergencyLifeSupportUnit.cpp src/ReliefCargo.cpp src/DistributionDepot.cpp src/TransportVehicle.cpp src/ReliefVehicle.cpp src/Manifest.cpp src/IncidentCommander.cpp src/FileManager.cpp src/LogisticsManager.cpp src/vehicles/AerialTransport.cpp src/vehicles/GroundConvoy.cpp src/vehicles/AmphibiousCraft.cpp src/vehicles/HybridRescueVehicle.cpp src/main.cpp

# 2. Compile Test Suite
g++ -std=c++11 -Wall -Wextra -Iinclude -o test_suite.exe src/GPSBeacon.cpp src/EmergencyLifeSupportUnit.cpp src/ReliefCargo.cpp src/DistributionDepot.cpp src/TransportVehicle.cpp src/ReliefVehicle.cpp src/Manifest.cpp src/IncidentCommander.cpp src/FileManager.cpp src/LogisticsManager.cpp src/vehicles/AerialTransport.cpp src/vehicles/GroundConvoy.cpp src/vehicles/AmphibiousCraft.cpp src/vehicles/HybridRescueVehicle.cpp tests/test_cases.cpp
```

---

## 5. Execution Instructions

### Running the Test Suite
```powershell
.\test_suite.exe
```
Output verifies all 28 syllabus test cases, producing 100% test success across Units I-IV.

### Running the Interactive Main Application
```powershell
.\erl_hub.exe
```

The system presents the main management dashboard:
```
========================================
               ERL-HUB                  
    EMERGENCY RELIEF LOGISTICS HUB      
========================================
 1. Inventory Management
 2. Distribution Depot Management
 3. Vehicle Fleet Management
 4. Relief Supply Allocation
 5. Emergency Dispatch Management
 6. Priority Medical Dispatch
 7. Route Evaluation
 8. Incident Commander Dashboard
 9. Manifest Management
 10. View Statistics and Analytics
 11. View Dispatch History
 12. Save Data
 13. Exit
========================================
```

---

## 6. Sample Operational Workflow

1. **Catalog & Depots:** The system automatically seeds 3 strategic bases (Dehradun Central Hub, Rishikesh Forward Depot, Chamoli Triage Base) and 6 emergency cargo types.
2. **Restock Supplies:** Use Option 2 -> 4 to restock supplies at `DPT-01` using `depot += cargo`.
3. **Medical Functor:** Use Option 1 -> 2 to view critical medical supplies automatically sorted by urgency using `MedicalPriorityComparator`.
4. **Supply Allocation:** Use Option 4 to allocate volume-based or priority medical supplies onto vehicles.
5. **Emergency Dispatch:** Use Option 5 to launch a mission. The system validates stock, checks payload limits (`CargoOverweightException`), verifies fuel reserves (`FuelDepletionException`), evaluates corridor viability (`DamagedCorridorException`), and creates an audited `Manifest`.
6. **Polymorphic Corridor Audit:** Use Option 7 to evaluate multi-modal routes across Ground Convoys, Choppers, Boats, and Hybrid units simultaneously.
7. **Incident Commander Override:** Use Option 8 to activate executive friend class overrides, bypassing standard inventory restrictions during mass-casualty triage.
8. **Rule of Three Demonstration:** Use Option 9 -> 2 & 3 to verify deep copy construction and copy assignment on dynamic `Manifest` memory.
9. **Persistence & Restart:** Use Option 12 to save all records. Restart `erl_hub.exe` to see that previous missions, inventory deductions, and fleet status are restored from binary storage.

---

## 7. Known Limitations & Operating System Notes
- **Atomic File Replacement:** On Windows operating systems, `std::rename` requires destination files to be replaced via a two-phase backup/rename mechanism. `FileManager::atomicCommit` incorporates this pattern.
- **Academic Simulation:** Vehicle routes, distances, and corridor conditions are simulated models designed for academic rigor and demonstration.
