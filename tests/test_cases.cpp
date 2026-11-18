#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

#include "../include/ReliefCargo.h"
#include "../include/DistributionDepot.h"
#include "../include/TransportVehicle.h"
#include "../include/ReliefVehicle.h"
#include "../include/AerialTransport.h"
#include "../include/GroundConvoy.h"
#include "../include/AmphibiousCraft.h"
#include "../include/HybridRescueVehicle.h"
#include "../include/GPSBeacon.h"
#include "../include/EmergencyLifeSupportUnit.h"
#include "../include/Manifest.h"
#include "../include/IncidentCommander.h"
#include "../include/PriorityDispatchQueue.h"
#include "../include/Exceptions.h"
#include "../include/FileManager.h"
#include "../include/LogisticsManager.h"

using namespace ReliefOps::Inventory;
using namespace ReliefOps::Logistics;
using namespace ReliefOps::Exceptions;

int testsPassed = 0;
int testsTotal = 0;

void reportTest(const std::string& testId, const std::string& description, bool condition) {
    ++testsTotal;
    std::cout << "[" << testId << "] " << description << ": ";
    if (condition) {
        ++testsPassed;
        std::cout << ">>> PASS <<<\n";
    } else {
        std::cout << ">>> FAIL <<<\n";
    }
}

// 1. Cargo Registration Test
void test01_CargoRegistration() {
    ReliefCargo cargo("CRG-T01", "High-Energy Biscuits", "Food/Ration Kits", 500, "packs", 0.5, 3, false);
    reportTest("TC-01", "Cargo Registration & Field Validation",
               cargo.getCargoId() == "CRG-T01" && 
               cargo.getAvailableQuantity() == 500 &&
               std::abs(cargo.getTotalWeightKg() - 250.0) < 0.001);
}

// 2. Duplicate Cargo ID Test
void test02_DuplicateCargoId() {
    LogisticsManager mgr("data_test");
    mgr.registerCargo(ReliefCargo("CRG-DUP", "Item A", "General", 10, "units", 1.0, 1, false));
    bool caught = false;
    try {
        mgr.registerCargo(ReliefCargo("CRG-DUP", "Item B", "General", 20, "units", 1.0, 1, false));
    } catch (const DuplicateIdException&) {
        caught = true;
    }
    reportTest("TC-02", "Duplicate Cargo ID Exception Handling", caught);
}

// 3. Depot Registration Test
void test03_DepotRegistration() {
    DistributionDepot depot("DPT-T01", "Test Forward Depot", "Sector 7", 30000.0);
    reportTest("TC-03", "Depot Registration & Properties",
               depot.getDepotId() == "DPT-T01" && 
               depot.getMaxStorageCapacityKg() == 30000.0 &&
               depot.getCargoCount() == 0);
}

// 4. Inventory Restocking Test
void test04_InventoryRestocking() {
    DistributionDepot depot("DPT-T02", "Test Depot 2", "Base", 20000.0);
    ReliefCargo c1("CRG-T02", "Water Bottles", "Clean Water", 100, "bottles", 1.0, 4, false);
    depot.addOrUpdateCargo(c1);
    depot.restockSupplies("CRG-T02", 50);
    reportTest("TC-04", "Inventory Restocking & Weight Update",
               depot.findCargoById("CRG-T02").getAvailableQuantity() == 150 &&
               std::abs(depot.getCurrentStoredWeightKg() - 150.0) < 0.001);
}

// 5. Inventory Deduction Test
void test05_InventoryDeduction() {
    DistributionDepot depot("DPT-T03", "Test Depot 3", "Base", 20000.0);
    ReliefCargo c1("CRG-T03", "Bandages", "Trauma Triage Medicine", 200, "boxes", 0.5, 4, true);
    depot.addOrUpdateCargo(c1);
    depot.deductSupplies("CRG-T03", 80);
    reportTest("TC-05", "Inventory Deduction & Stock Verification",
               depot.findCargoById("CRG-T03").getAvailableQuantity() == 120 &&
               std::abs(depot.getCurrentStoredWeightKg() - 60.0) < 0.001);
}

// 6. Function Overloading Test
void test06_FunctionOverloading() {
    LogisticsManager mgr("data_test");
    DistributionDepot depot("DPT-OVL", "Overload Depot", "Base", 50000.0);
    ReliefCargo c1("CRG-V01", "Rice Sacks", "Food/Ration Kits", 100, "bags", 10.0, 3, false);
    ReliefCargo c2("CRG-M01", "Morphine & Triage Kits", "Trauma Triage Medicine", 50, "kits", 2.0, 5, true);
    depot.addOrUpdateCargo(c1);
    depot.addOrUpdateCargo(c2);

    GroundConvoy v1("VEH-OV1", "Truck 1", 5000.0, 100.0, "Operational", "Base");
    GroundConvoy v2("VEH-OV2", "Truck 2", 5000.0, 100.0, "Operational", "Base");

    // Overload 1: Volume-based
    bool ok1 = mgr.allocateSupplies(depot, v1, "CRG-V01", 20); // 20 * 10 = 200 kg
    // Overload 2: Priority-based medical
    bool ok2 = mgr.allocateSupplies(depot, v2, 5, 200.0); // 50 * 2kg = 100 kg

    reportTest("TC-06", "Function Overloading (Volume vs Priority Medical)",
               ok1 && ok2 && 
               std::abs(v1.getCurrentLoadKg() - 200.0) < 0.001 &&
               std::abs(v2.getCurrentLoadKg() - 100.0) < 0.001);
}

// 7. Call by Reference Test
void test07_CallByReference() {
    DistributionDepot depot("DPT-REF", "Ref Depot", "Base", 20000.0);
    ReliefCargo c("CRG-REF", "Blankets", "Thermal Blankets", 50, "units", 2.0, 3, false);
    depot.addOrUpdateCargo(c);

    // Lambda taking depot by reference
    auto modifyByRef = [](DistributionDepot& d) {
        d.restockSupplies("CRG-REF", 30);
    };

    modifyByRef(depot);
    reportTest("TC-07", "Call by Reference Actual Object Modification",
               depot.findCargoById("CRG-REF").getAvailableQuantity() == 80);
}

// 8. Return by Reference Test
void test08_ReturnByReference() {
    DistributionDepot depot("DPT-RET", "Return Depot", "Base", 20000.0);
    ReliefCargo c("CRG-RET", "Tents", "Shelter", 40, "units", 5.0, 3, false);
    depot.addOrUpdateCargo(c);

    // Modify directly through returned non-const reference
    ReliefCargo& cargoRef = depot.findCargoById("CRG-RET");
    cargoRef.setAvailableQuantity(95);

    // Verify change is reflected in depot's stored object
    reportTest("TC-08", "Return by Reference Direct In-Place Modification",
               depot.findCargoById("CRG-RET").getAvailableQuantity() == 95);
}

// 9. Friend Class Access Test
void test09_FriendClassAccess() {
    DistributionDepot depot("DPT-FRD", "Friend Depot", "Base", 30000.0);
    ReliefCargo c("CRG-FRD", "Restricted Emergency Serum", "Trauma Triage Medicine", 10, "vials", 0.5, 3, true);
    depot.addOrUpdateCargo(c);

    IncidentCommander commander("CMD-01", "Commander Test", "NDRF");
    // Direct private member access by friend class
    bool overrideOk = commander.emergencyTriageOverride(depot, "CRG-FRD", 40);

    reportTest("TC-09", "IncidentCommander Friend Class Access & Triage Override",
               overrideOk && depot.findCargoById("CRG-FRD").getAvailableQuantity() == 50);
}

// 10. Static Member Test
void test10_StaticMembers() {
    int initialDepots = DistributionDepot::getTotalRegisteredDepots();
    DistributionDepot d1("DPT-S1", "Depot S1", "Base", 10000.0);
    DistributionDepot d2("DPT-S2", "Depot S2", "Base", 10000.0);

    LogisticsManager::resetDispatchStatistics();
    LogisticsManager::recordDispatchTonnage(3.5);
    LogisticsManager::recordDispatchTonnage(2.2);

    reportTest("TC-10", "Static Members & Static Methods Tracking",
               DistributionDepot::getTotalRegisteredDepots() == initialDepots + 2 &&
               LogisticsManager::getTotalDispatchedMissions() == 2 &&
               std::abs(LogisticsManager::getTotalDispatchedMetricTonnage() - 5.7) < 0.001);
}

// 11. Deep Copy Constructor Test
void test11_DeepCopyConstructor() {
    Manifest m1("MNF-001", "Base Alpha", "VEH-01", 3);
    m1.addCargo(ReliefCargo("CRG-D1", "Item 1", "General", 10, "units", 2.0, 1, false));
    m1.addCargo(ReliefCargo("CRG-D2", "Item 2", "General", 20, "units", 3.0, 2, false));

    // Deep copy
    Manifest m2 = m1;

    // Mutate m1
    m1.addCargo(ReliefCargo("CRG-D3", "Item 3", "General", 30, "units", 1.0, 3, false));

    reportTest("TC-11", "Manifest Rule of Three: Deep Copy Constructor Independence",
               m1.getCount() == 3 && m2.getCount() == 2 &&
               m1.getTotalWeightKg() != m2.getTotalWeightKg());
}

// 12. Copy Assignment Operator Test
void test12_CopyAssignmentOperator() {
    Manifest m1("MNF-A1", "Base Alpha", "VEH-01", 2);
    m1.addCargo(ReliefCargo("CRG-A1", "Item A1", "General", 15, "units", 2.0, 1, false));

    Manifest m2("MNF-B2", "Base Beta", "VEH-02", 2);
    m2.addCargo(ReliefCargo("CRG-B2", "Item B2", "General", 50, "units", 1.0, 1, false));

    m2 = m1; // Copy assignment

    reportTest("TC-12", "Manifest Rule of Three: Copy Assignment Operator",
               m2.getManifestId() == "MNF-A1" && 
               m2.getCount() == 1 &&
               m2.getTotalWeightKg() == m1.getTotalWeightKg());
}

// 13. Destructor and Memory Management Test
void test13_DestructorAndMemory() {
    bool survived = true;
    try {
        for (int i = 0; i < 50; ++i) {
            Manifest* dynamicManifest = new Manifest("MNF-DYN", "Dest", "VEH", 10);
            dynamicManifest->addCargo(ReliefCargo("CRG-X", "Temp", "General", 5, "u", 1.0, 1, false));
            delete dynamicManifest; // Exercises explicit destructor delete[]
        }
    } catch (...) {
        survived = false;
    }
    reportTest("TC-13", "Dynamic Memory Allocation & Destructor Cleanup", survived);
}

// 14. Operator Overloading Test
void test14_OperatorOverloading() {
    DistributionDepot depot("DPT-OP", "Operator Depot", "Base", 20000.0);
    ReliefCargo c1("CRG-OP1", "Ration Packs", "Food", 50, "kits", 2.0, 3, false);
    ReliefCargo c2("CRG-OP1", "Ration Packs", "Food", 20, "kits", 2.0, 3, false);

    // Operator +=
    depot += c1;
    bool addOk = (depot.findCargoById("CRG-OP1").getAvailableQuantity() == 50);

    // Operator -=
    depot -= c2;
    bool subOk = (depot.findCargoById("CRG-OP1").getAvailableQuantity() == 30);

    // Operator == on ReliefCargo
    ReliefCargo cDup("CRG-OP1", "Other", "Other", 1, "u", 1.0, 1, false);
    bool eqCargo = (c1 == cDup);

    // Operator == on Manifest
    Manifest mf1("MNF-EQ", "Dest", "V1", 2);
    Manifest mf2("MNF-EQ", "Dest", "V1", 2);
    bool eqManifest = (mf1 == mf2);

    reportTest("TC-14", "Operator Overloading (+=, -=, == on Cargo & Manifest)",
               addOk && subOk && eqCargo && eqManifest);
}

// 15. Virtual Inheritance Test
void test15_VirtualInheritance() {
    AerialTransport chopper;
    AmphibiousCraft boat;
    reportTest("TC-15", "Virtual Inheritance Subclasses Instantiation",
               chopper.getVehicleType() == "Aerial Transport" &&
               boat.getVehicleType() == "Amphibious Craft");
}

// 16. Diamond Problem Resolution Test
void test16_DiamondProblemResolution() {
    HybridRescueVehicle hybrid("HYB-TEST", "Hybrid Unit 1", 3000.0, 95.0, "Operational", "Forward Base");
    // Verify single ReliefVehicle subobject: setting property affects both views unambiguously
    hybrid.loadPayload(500.0);

    reportTest("TC-16", "Diamond Problem Resolution (HybridRescueVehicle)",
               hybrid.getCurrentLoadKg() == 500.0 &&
               hybrid.getVehicleId() == "HYB-TEST" &&
               hybrid.getVehicleType() == "Hybrid Rescue Vehicle");
}

// 17. Containership Test
void test17_Containership() {
    AerialTransport chopper("AER-CNT", "Chopper Containership", 2000.0, 100.0, "Operational", "Base", 4000.0, "Twin-Rotor", 45.0);
    chopper.getGPSBeacon().updateCoordinates(30.4500, 78.2500, "Ridge Sector Alpha");
    chopper.getLifeSupport().consumeOxygen(25.0);

    reportTest("TC-17", "Containership (GPSBeacon & EmergencyLifeSupportUnit Composition)",
               chopper.getGPSBeacon().getCurrentLocationName() == "Ridge Sector Alpha" &&
               std::abs(chopper.getLifeSupport().getOxygenSupplyLiters() - 175.0) < 0.001);
}

// 18. Runtime Polymorphism Test
void test18_RuntimePolymorphism() {
    std::vector<ReliefVehicle*> fleet;
    fleet.push_back(new GroundConvoy("V-GND", "Truck", 6000.0, 90.0));
    fleet.push_back(new AerialTransport("V-AER", "Helicopter", 2000.0, 90.0));
    fleet.push_back(new AmphibiousCraft("V-AMP", "Boat", 3000.0, 90.0));
    fleet.push_back(new HybridRescueVehicle("V-HYB", "Hybrid", 2500.0, 90.0));

    // High ground severity (7) blocks GroundConvoy but AerialTransport passes
    bool gndPass = fleet[0]->evaluateRoute("Valley Pass", 30.0, 7);
    bool aerPass = fleet[1]->evaluateRoute("Valley Pass", 30.0, 7);

    for (auto* v : fleet) delete v;

    reportTest("TC-18", "Runtime Polymorphism Dynamic Method Dispatch",
               !gndPass && aerPass);
}

// 19. Virtual Destructor Test
void test19_VirtualDestructor() {
    int initialCount = TransportVehicle::getTotalOperationalVehicles();
    ReliefVehicle* polyVehicle = new HybridRescueVehicle("HYB-DEL", "Temp Hybrid", 2500.0, 100.0, "Operational", "Base");
    int countWithVehicle = TransportVehicle::getTotalOperationalVehicles();
    delete polyVehicle; // Virtual destructor ensures proper derived and base destruction
    int countAfterDelete = TransportVehicle::getTotalOperationalVehicles();

    reportTest("TC-19", "Virtual Destructor & Proper Resource Reclamation",
               countWithVehicle == initialCount + 1 && countAfterDelete == initialCount);
}

// 20. Custom Exception Test
void test20_CustomExceptions() {
    bool caughtOverweight = false;
    bool caughtFuel = false;
    bool caughtDamaged = false;

    GroundConvoy truck("TRK-EX", "Test Truck", 1000.0, 5.0); // max 1000kg, 5% fuel

    try {
        truck.loadPayload(1500.0); // Exceeds 1000kg
    } catch (const CargoOverweightException&) {
        caughtOverweight = true;
    }

    try {
        truck.consumeFuel(10.0); // Only 5% fuel
    } catch (const FuelDepletionException&) {
        caughtFuel = true;
    }

    try {
        throw DamagedCorridorException("Bridge 4", 8, "Collapsed span");
    } catch (const DamagedCorridorException&) {
        caughtDamaged = true;
    }

    reportTest("TC-20", "Custom Exception Classes (Overweight, Fuel, Corridor)",
               caughtOverweight && caughtFuel && caughtDamaged);
}

// 21. Binary File Save Test
void test21_BinaryFileSave() {
    FileManager fm("data");
    std::vector<ReliefCargo> inv;
    inv.push_back(ReliefCargo("CRG-BIN1", "Binary Test Item", "Test", 100, "units", 2.5, 3, false));
    bool saveOk = fm.saveInventory(inv, "test_inv.bin");
    reportTest("TC-21", "Binary File Handling (Save Inventory via fstream)", saveOk);
}

// 22. Binary File Restore Test
void test22_BinaryFileRestore() {
    FileManager fm("data");
    std::vector<ReliefCargo> loaded;
    bool loadOk = fm.loadInventory(loaded, "test_inv.bin");
    reportTest("TC-22", "Binary File Handling (Restore & Data Deserialization)",
               loadOk && !loaded.empty() && 
               loaded[0].getCargoId() == "CRG-BIN1" &&
               loaded[0].getAvailableQuantity() == 100);
}

// 23. File Update Rollback Test
void test23_FileUpdateRollback() {
    // Writer that intentionally returns false to trigger atomic rollback
    bool result = FileManager::transactionalFileUpdate("data/non_existent.bin", [](std::ofstream&) -> bool {
        return false; // Force abort
    });
    reportTest("TC-23", "Atomic File Transaction Rollback Behavior", !result);
}

// 24. STL Container Test
void test24_STLContainers() {
    std::unordered_map<std::string, ReliefCargo> map;
    ReliefCargo c1("CRG-MAP1", "Item Map", "Test", 50, "units", 1.0, 1, false);
    map[c1.getCargoId()] = c1;

    std::vector<ReliefCargo> vec;
    vec.push_back(c1);

    reportTest("TC-24", "STL Containers (std::vector & std::unordered_map)",
               map.find("CRG-MAP1") != map.end() && vec.size() == 1);
}

// 25. Custom Functor Test
void test25_CustomFunctor() {
    std::vector<ReliefCargo> list;
    list.push_back(ReliefCargo("C1", "Low Med", "Med", 10, "u", 1.0, 2, true));
    list.push_back(ReliefCargo("C2", "High Non-Med", "General", 10, "u", 1.0, 5, false));
    list.push_back(ReliefCargo("C3", "Critical Med", "Med", 10, "u", 1.0, 5, true));

    std::sort(list.begin(), list.end(), MedicalPriorityComparator());

    // C3 (Prio 5, Med) must be first, then C2 (Prio 5, non-Med), then C1 (Prio 2, Med)
    reportTest("TC-25", "Custom Functor (MedicalPriorityComparator Sorting)",
               list[0].getCargoId() == "C3" && 
               list[1].getCargoId() == "C2" && 
               list[2].getCargoId() == "C1");
}

// 26. Template Priority Queue Test
void test26_TemplateQueue() {
    PriorityDispatchQueue<DispatchRequest> queue;
    queue.push(DispatchRequest("REQ-1", "D1", "Dest1", "Corridor1", "V1", 2, 100.0, false, "10:00"));
    queue.push(DispatchRequest("REQ-2", "D1", "Dest2", "Corridor2", "V2", 5, 200.0, true, "10:05"));
    queue.push(DispatchRequest("REQ-3", "D1", "Dest3", "Corridor3", "V3", 3, 150.0, false, "10:10"));

    bool okSize = (queue.size() == 3);
    DispatchRequest top = queue.top();
    bool okTop = (top.getRequestId() == "REQ-2" && top.getPriorityLevel() == 5);
    queue.pop();
    bool okPop = (queue.size() == 2 && queue.top().getPriorityLevel() == 3);

    reportTest("TC-26", "Template Class PriorityDispatchQueue<T> Operations",
               okSize && okTop && okPop);
}

// 27. Custom Stream Manipulator Test
void test27_CustomStreamManipulators() {
    std::ostringstream oss;
    oss << tableHeader("TEST HEADER", 40) << "\n";
    oss << sectionDivider;
    std::string out = oss.str();
    reportTest("TC-27", "User-Defined Stream Manipulators Formatting",
               out.find("TEST HEADER") != std::string::npos && 
               out.find("--------------------------------") != std::string::npos);
}

// 28. End-to-End Dispatch Workflow Test
void test28_EndToEndDispatchWorkflow() {
    LogisticsManager mgr("data");
    mgr.initializeSampleData();

    double initialTonnage = LogisticsManager::getTotalDispatchedMetricTonnage();
    int initialMissions = LogisticsManager::getTotalDispatchedMissions();

    std::vector<std::pair<std::string, int>> cargoToDispatch = {
        {"CRG-101", 10}, // 10 * 2.5 = 25 kg
        {"CRG-103", 5}   // 5 * 3.0 = 15 kg -> Total 40 kg = 0.04 MT
    };

    bool dispatchOk = mgr.executeDispatch("DPT-01", "VEH-GND-01", "Sector 4 Relief Camp", 
                                          "Valley Truck Corridor", cargoToDispatch, 2, 20.0);

    reportTest("TC-28", "End-to-End Emergency Relief Dispatch Workflow",
               dispatchOk && 
               LogisticsManager::getTotalDispatchedMissions() == initialMissions + 1 &&
               LogisticsManager::getTotalDispatchedMetricTonnage() > initialTonnage);
}

int main() {
    std::cout << "================================================================================\n";
    std::cout << "        ERL-HUB COMPREHENSIVE OOPS VERIFICATION & TEST HARNESS                  \n";
    std::cout << "        Course: CIC-211 Object-Oriented Programming Using C++                  \n";
    std::cout << "        Maharaja Agrasen Institute of Technology (MAIT), GGSIPU                \n";
    std::cout << "================================================================================\n\n";

    test01_CargoRegistration();
    test02_DuplicateCargoId();
    test03_DepotRegistration();
    test04_InventoryRestocking();
    test05_InventoryDeduction();
    test06_FunctionOverloading();
    test07_CallByReference();
    test08_ReturnByReference();
    test09_FriendClassAccess();
    test10_StaticMembers();
    test11_DeepCopyConstructor();
    test12_CopyAssignmentOperator();
    test13_DestructorAndMemory();
    test14_OperatorOverloading();
    test15_VirtualInheritance();
    test16_DiamondProblemResolution();
    test17_Containership();
    test18_RuntimePolymorphism();
    test19_VirtualDestructor();
    test20_CustomExceptions();
    test21_BinaryFileSave();
    test22_BinaryFileRestore();
    test23_FileUpdateRollback();
    test24_STLContainers();
    test25_CustomFunctor();
    test26_TemplateQueue();
    test27_CustomStreamManipulators();
    test28_EndToEndDispatchWorkflow();

    std::cout << "\n================================================================================\n";
    std::cout << " TEST SUMMARY: " << testsPassed << " / " << testsTotal << " Tests Passed (" 
              << (testsPassed * 100.0 / testsTotal) << "% Success Rate)\n";
    std::cout << "================================================================================\n";

    return (testsPassed == testsTotal) ? 0 : 1;
}
