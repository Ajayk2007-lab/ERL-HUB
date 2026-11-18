# Test Cases and Verification Matrix
## Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)

This document records the automated verification and testing results across all 28 comprehensive test cases executed by `test_suite.exe`.

---

## Comprehensive Test Execution Matrix

| Test ID | Test Category / Objective | Input Parameters / Preconditions | Expected Output | Actual Output | Status |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Cargo Registration & Field Validation | ID: `CRG-T01`, Qty: 500, UnitWt: 0.5 kg | `cargoId == "CRG-T01"`, `availableQuantity == 500`, Total weight = 250.0 kg | Initialized correctly, total weight matches exactly | **PASS** |
| **TC-02** | Duplicate Cargo ID Exception Handling | Register duplicate cargo ID `CRG-DUP` | Throws `DuplicateIdException` | Exception caught with diagnostic message | **PASS** |
| **TC-03** | Depot Registration & Properties | ID: `DPT-T01`, Cap: 30,000.0 kg | Depot registered with 0 initial items and 30,000 kg capacity | Properties verified | **PASS** |
| **TC-04** | Inventory Restocking & Weight Update | Initial: 100 bottles (1.0 kg). Restock: +50 bottles | Qty = 150, Current stored weight = 150.0 kg | Stored weight updated dynamically | **PASS** |
| **TC-05** | Inventory Deduction & Stock Verification | Initial: 200 boxes (0.5 kg). Deduct: 80 boxes | Qty = 120, Current stored weight = 60.0 kg | Stored weight accurately decremented | **PASS** |
| **TC-06** | Function Overloading (Volume vs Priority Medical) | Overload 1: `allocateSupplies(..., 20)`<br>Overload 2: `allocateSupplies(..., 5, 200.0)` | Overload 1 loads 200 kg volume payload; Overload 2 loads 100 kg priority medical payload | Both overloaded functions executed correctly | **PASS** |
| **TC-07** | Call by Reference Actual Object Modification | Lambda accepts `DistributionDepot&` and restocks +30 | Depot object outside lambda modified without copy | Original depot updated directly | **PASS** |
| **TC-08** | Return by Reference Direct In-Place Modification | `findCargoById()` returns `ReliefCargo&`. Call `.setAvailableQuantity(95)` | Change reflected directly in depot's stored vector | Value mutated in-place | **PASS** |
| **TC-09** | IncidentCommander Friend Class Triage Override | Commander overrides restricted item +40 units | Direct private access modifies `item.availableQuantity` and `depot.currentStoredWeightKg` | Quota bypassed and weight recalibrated | **PASS** |
| **TC-10** | Static Members & Static Methods Tracking | Register 2 depots, record 3.5 MT and 2.2 MT dispatch | Depot count increments by 2; Missions = 2; Tonnage = 5.7 MT | Static counters tracked across system | **PASS** |
| **TC-11** | Manifest Rule of Three: Deep Copy Constructor | Instantiate `m1` with 2 items. `m2 = m1`. Add 3rd item to `m1` | `m1.getCount() == 3`, `m2.getCount() == 2`. Separate heap buffers | Dynamic memory is completely independent | **PASS** |
| **TC-12** | Manifest Rule of Three: Copy Assignment Operator | `m1` (1 item), `m2` (1 item). `m2 = m1` | `m2` deallocates old heap array, allocates fresh copy of `m1` | Safe deallocation and deep copy verified | **PASS** |
| **TC-13** | Dynamic Memory Allocation & Destructor Cleanup | Allocate and delete 50 dynamic `Manifest*` objects with `ReliefCargo` | Clean execution of `delete[] cargoRecords` with zero leaks or crashes | Memory reclaimed cleanly | **PASS** |
| **TC-14** | Operator Overloading (`+=`, `-=`, `==` on Cargo & Manifest) | `depot += cargo`, `depot -= cargo`, `cargo1 == cargo2`, `mnf1 == mnf2` | Operators execute real inventory and identity comparison | All 4 operators perform correctly | **PASS** |
| **TC-15** | Virtual Inheritance Subclasses Instantiation | Instantiate `AerialTransport` and `AmphibiousCraft` | Subclasses instantiate cleanly without ambiguity | Clean compilation and runtime execution | **PASS** |
| **TC-16** | Diamond Problem Resolution (`HybridRescueVehicle`) | Instantiate `HybridRescueVehicle`. Load 500 kg payload | Single `ReliefVehicle` base subobject; no ambiguity | Virtual base class resolved single subobject | **PASS** |
| **TC-17** | Containership (Composition) Verification | `GPSBeacon` and `EmergencyLifeSupportUnit` embedded in vehicle | Coordinates update to `(30.45, 78.25)`; oxygen decremented to 175 L | Composition member methods function independently | **PASS** |
| **TC-18** | Runtime Polymorphism Dynamic Method Dispatch | Corridor severity = 7. Invoke `v->evaluateRoute()` on base pointers | `GroundConvoy` rejects (landslide rubble); `AerialTransport` clears (airspace clear) | Dynamic binding correctly invoked | **PASS** |
| **TC-19** | Virtual Destructor & Proper Resource Reclamation | Delete derived object via base pointer `ReliefVehicle*` | `TransportVehicle::totalOperationalVehicles` decrements accurately | Virtual destructor called derived destructor | **PASS** |
| **TC-20** | Custom Exception Classes | Load 1500 kg on 1000 kg truck; consume 10% on 5% fuel; damage severity 8 | Throws `CargoOverweightException`, `FuelDepletionException`, `DamagedCorridorException` | All custom diagnostic messages caught | **PASS** |
| **TC-21** | Binary File Save via `std::fstream` | Save inventory to `data/test_inv.bin` | Transactional binary write succeeds | File written with length-prefixed strings | **PASS** |
| **TC-22** | Binary File Restore & Deserialization | Load inventory from `data/test_inv.bin` | Cargo restored with identical ID, name, quantity | Deserialization matches original data exactly | **PASS** |
| **TC-23** | Atomic File Transaction Rollback Behavior | Intentional writer failure during transactional update | Returns `false`, temporary file removed, destination preserved | Atomic rollback verified | **PASS** |
| **TC-24** | STL Containers (`std::vector` & `std::unordered_map`) | Store item in vector and hash map | Lookup `map.find("CRG-MAP1")` succeeds in $O(1)$ time | Container operations verified | **PASS** |
| **TC-25** | Custom Functor (`MedicalPriorityComparator`) | Sort list with mixed priorities and medical flags | Medical priority 5 items appear before non-medical priority 5 and priority 2 | Exact priority ordering confirmed | **PASS** |
| **TC-26** | Template Class `PriorityDispatchQueue<T>` | Push 3 requests (Priority 2, 5, 3) | `top()` returns Priority 5; `pop()` reveals Priority 3 | Generic max-heap priority queue confirmed | **PASS** |
| **TC-27** | User-Defined Stream Manipulators Formatting | Stream `tableHeader("TEST", 40)` and `sectionDivider` | Formats bordered banner and horizontal rule | Custom stream manipulators formatted cleanly | **PASS** |
| **TC-28** | End-to-End Emergency Relief Dispatch Workflow | Full mission execution: stock check, capacity check, fuel check, corridor evaluation, deduction, manifest archive | Dispatches 40 kg, increments missions by 1, updates global tonnage | Complete workflow succeeded | **PASS** |

---

## Summary
- **Total Test Cases:** 28
- **Passed:** 28
- **Failed:** 0
- **Success Rate:** 100.0%
