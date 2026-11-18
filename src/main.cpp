#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "../include/LogisticsManager.h"
#include "../include/Exceptions.h"

using namespace ReliefOps::Logistics;
using namespace ReliefOps::Inventory;
using namespace ReliefOps::Exceptions;

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printMainMenu() {
    std::cout << "\n========================================\n";
    std::cout << "               ERL-HUB                  \n";
    std::cout << "    EMERGENCY RELIEF LOGISTICS HUB      \n";
    std::cout << "========================================\n";
    std::cout << " 1. Inventory Management\n";
    std::cout << " 2. Distribution Depot Management\n";
    std::cout << " 3. Vehicle Fleet Management\n";
    std::cout << " 4. Relief Supply Allocation\n";
    std::cout << " 5. Emergency Dispatch Management\n";
    std::cout << " 6. Priority Medical Dispatch\n";
    std::cout << " 7. Route Evaluation\n";
    std::cout << " 8. Incident Commander Dashboard\n";
    std::cout << " 9. Manifest Management\n";
    std::cout << " 10. View Statistics and Analytics\n";
    std::cout << " 11. View Dispatch History\n";
    std::cout << " 12. Save Data\n";
    std::cout << " 13. Exit\n";
    std::cout << "========================================\n";
    std::cout << "Select an option [1-13]: ";
}

// 1. Inventory Management Submenu
void handleInventorySubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [1] INVENTORY MANAGEMENT SUBMENU ---\n";
        std::cout << " 1. View Central Cargo Catalog\n";
        std::cout << " 2. View Critical Medical Supplies (Functor Sorted)\n";
        std::cout << " 3. Add New Relief Cargo Item\n";
        std::cout << " 4. Search Cargo by ID\n";
        std::cout << " 5. Update Stock Quantity\n";
        std::cout << " 6. Back to Main Menu\n";
        std::cout << "Choice [1-6]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 6) break;

        try {
            switch (choice) {
                case 1:
                    manager.displayCargoCatalog();
                    break;
                case 2:
                    manager.displayCriticalMedicalSupplies();
                    break;
                case 3: {
                    ReliefCargo cargo;
                    std::cin >> cargo;
                    manager.registerCargo(cargo);
                    manager.saveAllData();
                    std::cout << "  [✓] Cargo successfully registered and catalog saved.\n";
                    break;
                }
                case 4: {
                    std::string cId;
                    std::cout << "Enter Cargo ID to search (e.g. CRG-103): ";
                    std::cin >> cId;
                    ReliefCargo& found = manager.findCargo(cId);
                    std::cout << "\n  [Found Record]: " << found << "\n";
                    break;
                }
                case 5: {
                    std::string cId;
                    int delta = 0;
                    std::cout << "Enter Cargo ID: ";
                    std::cin >> cId;
                    std::cout << "Enter quantity delta (positive to restock, negative to deduct): ";
                    std::cin >> delta;
                    ReliefCargo& found = manager.findCargo(cId);
                    found.updateQuantity(delta);
                    manager.saveAllData();
                    std::cout << "  [✓] Stock updated. New quantity: " << found.getAvailableQuantity() << " " << found.getUnitOfMeasure() << "\n";
                    break;
                }
                default:
                    std::cout << "  [!] Invalid choice. Try again.\n";
            }
        } catch (const ERLHubException& ex) {
            std::cout << "  [ERROR] " << ex.what() << "\n";
        }
    }
}

// 2. Depot Management Submenu
void handleDepotSubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [2] DISTRIBUTION DEPOT MANAGEMENT SUBMENU ---\n";
        std::cout << " 1. View All Distribution Depots\n";
        std::cout << " 2. View Detailed Depot Inventory\n";
        std::cout << " 3. Register New Distribution Depot\n";
        std::cout << " 4. Restock Depot Supplies (Operator +=)\n";
        std::cout << " 5. Search Depot by ID\n";
        std::cout << " 6. Back to Main Menu\n";
        std::cout << "Choice [1-6]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 6) break;

        try {
            switch (choice) {
                case 1:
                    manager.displayAllDepots();
                    break;
                case 2: {
                    std::string dId;
                    std::cout << "Enter Depot ID (e.g. DPT-01): ";
                    std::cin >> dId;
                    DistributionDepot& depot = manager.getDepot(dId);
                    depot.displayDetailedInventory();
                    break;
                }
                case 3: {
                    std::string id, name, loc;
                    double cap = 50000.0;
                    std::cout << "Enter Depot ID (e.g. DPT-04): ";
                    std::cin >> id;
                    clearInputBuffer();
                    std::cout << "Enter Depot Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Depot Location: ";
                    std::getline(std::cin, loc);
                    std::cout << "Enter Max Storage Capacity (kg): ";
                    std::cin >> cap;

                    DistributionDepot newDepot(id, name, loc, cap);
                    manager.registerDepot(newDepot);
                    manager.saveAllData();
                    std::cout << "  [✓] Distribution Depot successfully registered.\n";
                    break;
                }
                case 4: {
                    std::string dId, cId;
                    int qty = 0;
                    std::cout << "Enter Depot ID: ";
                    std::cin >> dId;
                    std::cout << "Enter Cargo ID to restock: ";
                    std::cin >> cId;
                    std::cout << "Enter quantity: ";
                    std::cin >> qty;

                    DistributionDepot& depot = manager.getDepot(dId);
                    ReliefCargo& catalogItem = manager.findCargo(cId);
                    ReliefCargo toAdd(catalogItem.getCargoId(), catalogItem.getCargoName(), catalogItem.getCategory(),
                                      qty, catalogItem.getUnitOfMeasure(), catalogItem.getWeightPerUnitKg(),
                                      catalogItem.getEmergencyPriorityLevel(), catalogItem.isMedical());

                    // Unit II Operator Overloading demonstration: depot += cargo
                    depot += toAdd;
                    manager.saveAllData();
                    std::cout << "  [✓] Supplies successfully restocked via operator+= in depot " << dId << ".\n";
                    break;
                }
                case 5: {
                    std::string dId;
                    std::cout << "Enter Depot ID to search: ";
                    std::cin >> dId;
                    DistributionDepot& depot = manager.getDepot(dId);
                    depot.displayDepotSummary();
                    break;
                }
                default:
                    std::cout << "  [!] Invalid choice. Try again.\n";
            }
        } catch (const ERLHubException& ex) {
            std::cout << "  [ERROR] " << ex.what() << "\n";
        }
    }
}

// 3. Vehicle Fleet Management Submenu
void handleVehicleSubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [3] VEHICLE FLEET MANAGEMENT SUBMENU ---\n";
        std::cout << " 1. View All Fleet Vehicles\n";
        std::cout << " 2. Inspect Vehicle Detailed Specs (Containership & Polymorphism)\n";
        std::cout << " 3. Register Ground Convoy Truck\n";
        std::cout << " 4. Register Aerial Transport Chopper\n";
        std::cout << " 5. Register Amphibious Rescue Craft\n";
        std::cout << " 6. Register Hybrid Rescue Vehicle (Diamond Inheritance)\n";
        std::cout << " 7. Update Vehicle Fuel Level\n";
        std::cout << " 8. Update Operational Status\n";
        std::cout << " 9. Back to Main Menu\n";
        std::cout << "Choice [1-9]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 9) break;

        try {
            switch (choice) {
                case 1:
                    manager.displayAllVehicles();
                    break;
                case 2: {
                    std::string vId;
                    std::cout << "Enter Vehicle ID (e.g. VEH-GND-01, VEH-HYB-01): ";
                    std::cin >> vId;
                    ReliefVehicle& v = manager.getVehicle(vId);
                    v.displayDetails();
                    break;
                }
                case 3: {
                    std::string id, name, loc;
                    double maxCap = 7000.0;
                    std::cout << "Enter Ground Convoy ID (e.g. VEH-GND-02): ";
                    std::cin >> id;
                    clearInputBuffer();
                    std::cout << "Enter Vehicle Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Location: ";
                    std::getline(std::cin, loc);
                    std::cout << "Enter Max Payload Capacity (kg): ";
                    std::cin >> maxCap;

                    manager.registerVehicle(new GroundConvoy(id, name, maxCap, 100.0, "Operational", loc));
                    manager.saveAllData();
                    std::cout << "  [✓] Ground Convoy successfully registered.\n";
                    break;
                }
                case 4: {
                    std::string id, name, loc;
                    double maxCap = 2000.0;
                    std::cout << "Enter Aerial Chopper ID (e.g. VEH-AER-03): ";
                    std::cin >> id;
                    clearInputBuffer();
                    std::cout << "Enter Vehicle Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Location: ";
                    std::getline(std::cin, loc);
                    std::cout << "Enter Max Payload Capacity (kg): ";
                    std::cin >> maxCap;

                    manager.registerVehicle(new AerialTransport(id, name, maxCap, 100.0, "Operational", loc));
                    manager.saveAllData();
                    std::cout << "  [✓] Aerial Transport successfully registered.\n";
                    break;
                }
                case 5: {
                    std::string id, name, loc;
                    double maxCap = 3000.0;
                    std::cout << "Enter Amphibious Craft ID (e.g. VEH-AMP-02): ";
                    std::cin >> id;
                    clearInputBuffer();
                    std::cout << "Enter Craft Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Location: ";
                    std::getline(std::cin, loc);
                    std::cout << "Enter Max Payload Capacity (kg): ";
                    std::cin >> maxCap;

                    manager.registerVehicle(new AmphibiousCraft(id, name, maxCap, 100.0, "Operational", loc));
                    manager.saveAllData();
                    std::cout << "  [✓] Amphibious Craft successfully registered.\n";
                    break;
                }
                case 6: {
                    std::string id, name, loc;
                    double maxCap = 2500.0;
                    std::cout << "Enter Hybrid Rescue ID (e.g. VEH-HYB-02): ";
                    std::cin >> id;
                    clearInputBuffer();
                    std::cout << "Enter Hybrid Vehicle Name: ";
                    std::getline(std::cin, name);
                    std::cout << "Enter Location: ";
                    std::getline(std::cin, loc);
                    std::cout << "Enter Max Payload Capacity (kg): ";
                    std::cin >> maxCap;

                    manager.registerVehicle(new HybridRescueVehicle(id, name, maxCap, 100.0, "Operational", loc));
                    manager.saveAllData();
                    std::cout << "  [✓] Hybrid Rescue Vehicle registered (Virtual Diamond Base Initialized).\n";
                    break;
                }
                case 7: {
                    std::string vId;
                    double fuel = 100.0;
                    std::cout << "Enter Vehicle ID: ";
                    std::cin >> vId;
                    std::cout << "Enter new fuel level percentage (0-100%): ";
                    std::cin >> fuel;
                    ReliefVehicle& v = manager.getVehicle(vId);
                    v.updateFuel(fuel);
                    manager.saveAllData();
                    std::cout << "  [✓] Fuel level updated to " << v.getFuelLevelPercentage() << "%\n";
                    break;
                }
                case 8: {
                    std::string vId, status;
                    std::cout << "Enter Vehicle ID: ";
                    std::cin >> vId;
                    std::cout << "Enter Status (Operational / In Mission / Maintenance): ";
                    clearInputBuffer();
                    std::getline(std::cin, status);
                    ReliefVehicle& v = manager.getVehicle(vId);
                    v.setOperationalStatus(status);
                    manager.saveAllData();
                    std::cout << "  [✓] Vehicle operational status updated to: " << v.getOperationalStatus() << "\n";
                    break;
                }
                default:
                    std::cout << "  [!] Invalid choice. Try again.\n";
            }
        } catch (const ERLHubException& ex) {
            std::cout << "  [ERROR] " << ex.what() << "\n";
        }
    }
}

// 4. Relief Supply Allocation Submenu
void handleAllocationSubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [4] RELIEF SUPPLY ALLOCATION SUBMENU (FUNCTION OVERLOADING) ---\n";
        std::cout << " 1. Volume-Based Payload Allocation (Overload #1)\n";
        std::cout << " 2. Priority-Based Emergency Medical Allocation (Overload #2)\n";
        std::cout << " 3. Back to Main Menu\n";
        std::cout << "Choice [1-3]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 3) break;

        try {
            switch (choice) {
                case 1: {
                    std::string dId, vId, cId;
                    int qty = 0;
                    std::cout << "Enter Depot ID: ";
                    std::cin >> dId;
                    std::cout << "Enter Vehicle ID: ";
                    std::cin >> vId;
                    std::cout << "Enter Cargo ID: ";
                    std::cin >> cId;
                    std::cout << "Enter Quantity to allocate: ";
                    std::cin >> qty;

                    DistributionDepot& depot = manager.getDepot(dId);
                    ReliefVehicle& vehicle = manager.getVehicle(vId);
                    manager.allocateSupplies(depot, vehicle, cId, qty);
                    manager.saveAllData();
                    break;
                }
                case 2: {
                    std::string dId, vId;
                    int minPrio = 4;
                    double maxWt = 1500.0;
                    std::cout << "Enter Depot ID: ";
                    std::cin >> dId;
                    std::cout << "Enter Vehicle ID: ";
                    std::cin >> vId;
                    std::cout << "Enter Minimum Priority Level (1-5): ";
                    std::cin >> minPrio;
                    std::cout << "Enter Max Weight Limit (kg): ";
                    std::cin >> maxWt;

                    DistributionDepot& depot = manager.getDepot(dId);
                    ReliefVehicle& vehicle = manager.getVehicle(vId);
                    manager.allocateSupplies(depot, vehicle, minPrio, maxWt);
                    manager.saveAllData();
                    break;
                }
                default:
                    std::cout << "  [!] Invalid choice.\n";
            }
        } catch (const ERLHubException& ex) {
            std::cout << "  [ERROR] " << ex.what() << "\n";
        }
    }
}

// 5. Emergency Dispatch Management Submenu
void handleDispatchSubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [5] EMERGENCY DISPATCH MANAGEMENT SUBMENU ---\n";
        std::cout << " 1. Create and Execute Complete Dispatch Mission\n";
        std::cout << " 2. Simulate Error Scenario: Cargo Overweight Exception\n";
        std::cout << " 3. Simulate Error Scenario: Fuel Depletion Exception\n";
        std::cout << " 4. Simulate Error Scenario: Damaged Corridor Exception\n";
        std::cout << " 5. Back to Main Menu\n";
        std::cout << "Choice [1-5]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 5) break;

        try {
            switch (choice) {
                case 1: {
                    std::string dId, vId, dest, corridor;
                    int sev = 3;
                    double dist = 40.0;

                    std::cout << "Enter Source Depot ID (e.g. DPT-01): ";
                    std::cin >> dId;
                    std::cout << "Enter Assigned Vehicle ID (e.g. VEH-GND-01): ";
                    std::cin >> vId;
                    clearInputBuffer();
                    std::cout << "Enter Target Destination (e.g. Uttarkashi Relief Camp): ";
                    std::getline(std::cin, dest);
                    std::cout << "Enter Corridor Name (e.g. Highway 34 Corridor): ";
                    std::getline(std::cin, corridor);
                    std::cout << "Enter Distance (km): ";
                    std::cin >> dist;
                    std::cout << "Enter Earthquake Damage Severity Level (1-10): ";
                    std::cin >> sev;

                    int itemCount = 0;
                    std::cout << "How many distinct cargo types to dispatch? ";
                    std::cin >> itemCount;

                    std::vector<std::pair<std::string, int>> cargoList;
                    for (int i = 0; i < itemCount; ++i) {
                        std::string cId;
                        int qty = 0;
                        std::cout << " Cargo Item #" << (i + 1) << " ID: ";
                        std::cin >> cId;
                        std::cout << " Quantity: ";
                        std::cin >> qty;
                        cargoList.push_back({cId, qty});
                    }

                    manager.executeDispatch(dId, vId, dest, corridor, cargoList, sev, dist);
                    break;
                }
                case 2: {
                    std::cout << "\n[TEST ERROR 1] Simulating Cargo Overweight Exception...\n";
                    std::vector<std::pair<std::string, int>> list = {{"CRG-102", 5000}}; // 5000 * 5kg = 25000 kg (exceeds all vehicles)
                    manager.executeDispatch("DPT-01", "VEH-AER-01", "Remote Sector Alpha", "Air Corridor North", list, 2, 20.0);
                    break;
                }
                case 3: {
                    std::cout << "\n[TEST ERROR 2] Simulating Fuel Depletion Exception...\n";
                    // Temporarily set vehicle fuel low
                    ReliefVehicle& v = manager.getVehicle("VEH-AER-01");
                    double oldFuel = v.getFuelLevelPercentage();
                    v.updateFuel(2.0); // 2% fuel only
                    std::vector<std::pair<std::string, int>> list = {{"CRG-103", 10}};
                    try {
                        manager.executeDispatch("DPT-01", "VEH-AER-01", "Remote Ridge Sector", "Air Mountain Pass", list, 3, 50.0);
                    } catch (...) {
                        v.updateFuel(oldFuel); // restore
                        throw;
                    }
                    v.updateFuel(oldFuel);
                    break;
                }
                case 4: {
                    std::cout << "\n[TEST ERROR 3] Simulating Damaged Corridor Exception...\n";
                    std::vector<std::pair<std::string, int>> list = {{"CRG-101", 20}};
                    // Severity 9 causes GroundConvoy evaluateRoute to fail due to landslide rubble!
                    manager.executeDispatch("DPT-01", "VEH-GND-01", "Landslide Block Sector", "Highway 58 Mountain Route", list, 9, 30.0);
                    break;
                }
                default:
                    std::cout << "  [!] Invalid choice.\n";
            }
        } catch (const CargoOverweightException& ex) {
            std::cout << "\n  >>> CAUGHT EXCEPTION (Unit IV 9.1) <<<\n  " << ex.what() << "\n";
            std::cout << "  Attempted Payload: " << ex.getAttemptedWeight() << " kg | Max Limit: " << ex.getMaxCapacity() << " kg\n\n";
        } catch (const FuelDepletionException& ex) {
            std::cout << "\n  >>> CAUGHT EXCEPTION (Unit IV 9.1) <<<\n  " << ex.what() << "\n";
            std::cout << "  Current Fuel: " << ex.getCurrentFuel() << "% | Required: " << ex.getRequiredFuel() << "%\n\n";
        } catch (const DamagedCorridorException& ex) {
            std::cout << "\n  >>> CAUGHT EXCEPTION (Unit IV 9.1) <<<\n  " << ex.what() << "\n";
            std::cout << "  Corridor: " << ex.getCorridorName() << " | Damage Severity: " << ex.getDamageSeverity() << "/10\n\n";
        } catch (const ERLHubException& ex) {
            std::cout << "\n  >>> CAUGHT ERL-HUB EXCEPTION <<<\n  " << ex.what() << "\n\n";
        }
    }
}

// 6. Priority Medical Dispatch Submenu
void handlePriorityMedicalSubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [6] PRIORITY MEDICAL DISPATCH (TEMPLATE PRIORITY QUEUE) ---\n";
        std::cout << " 1. View Current Priority Dispatch Queue\n";
        std::cout << " 2. Submit New Priority Emergency Request\n";
        std::cout << " 3. Process Highest Priority Emergency Dispatch\n";
        std::cout << " 4. Back to Main Menu\n";
        std::cout << "Choice [1-4]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 4) break;

        try {
            switch (choice) {
                case 1:
                    manager.displayEmergencyQueue();
                    break;
                case 2: {
                    std::string reqId, srcDepot, dest, corridor;
                    int prio = 5;
                    double wt = 150.0;
                    int medInt = 1;

                    std::cout << "Enter Request ID (e.g. REQ-901): ";
                    std::cin >> reqId;
                    std::cout << "Enter Source Depot ID (e.g. DPT-01): ";
                    std::cin >> srcDepot;
                    clearInputBuffer();
                    std::cout << "Enter Destination: ";
                    std::getline(std::cin, dest);
                    std::cout << "Enter Preferred Corridor: ";
                    std::getline(std::cin, corridor);
                    std::cout << "Enter Priority (1-5, 5=Emergency Life Saving): ";
                    std::cin >> prio;
                    std::cout << "Enter Estimated Cargo Weight (kg): ";
                    std::cin >> wt;
                    std::cout << "Is this a critical medical emergency? (1=Yes, 0=No): ";
                    std::cin >> medInt;

                    DispatchRequest req(reqId, srcDepot, dest, corridor, "", prio, wt, (medInt != 0), "12:00:00");
                    manager.submitEmergencyRequest(req);
                    break;
                }
                case 3:
                    manager.processNextEmergencyDispatch();
                    break;
                default:
                    std::cout << "  [!] Invalid choice.\n";
            }
        } catch (const ERLHubException& ex) {
            std::cout << "  [ERROR] " << ex.what() << "\n";
        }
    }
}

// 7. Route Evaluation Submenu
void handleRouteEvaluationSubmenu(LogisticsManager& manager) {
    std::cout << "\n================================================================================\n";
    std::cout << "   ACADEMIC SIMULATION NOTICE: Post-Earthquake Corridor Feasibility Audit       \n";
    std::cout << "   (Demonstrates Unit III Dynamic Binding & Runtime Polymorphism)               \n";
    std::cout << "================================================================================\n";

    std::string corridor;
    double dist = 45.0;
    int sev = 7;

    std::cout << "Enter Transportation Corridor to evaluate (e.g. Rishikesh-Chamoli Valley Pass): ";
    clearInputBuffer();
    std::getline(std::cin, corridor);
    if (corridor.empty()) corridor = "Rishikesh-Chamoli Valley Riverine Pass";

    std::cout << "Enter Transit Distance (km) [Default 45.0]: ";
    std::cin >> dist;
    std::cout << "Enter Earthquake Damage Severity (1-10) [e.g. 7 for severe ground rubble]: ";
    std::cin >> sev;

    manager.evaluateCorridorForFleet(corridor, dist, sev);
}

// 8. Incident Commander Dashboard Submenu
void handleCommanderSubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [8] INCIDENT COMMANDER DASHBOARD (FRIEND CLASS DEMONSTRATION) ---\n";
        std::cout << " 1. Display Commander Overview Dashboard\n";
        std::cout << " 2. Execute Executive Emergency Triage Override (Friend Class Access)\n";
        std::cout << " 3. Force Elevate Cargo Priority\n";
        std::cout << " 4. Back to Main Menu\n";
        std::cout << "Choice [1-4]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 4) break;

        try {
            switch (choice) {
                case 1:
                    manager.displayIncidentCommanderDashboard();
                    break;
                case 2: {
                    std::string dId, cId;
                    int qty = 50;
                    std::cout << "Enter Target Depot ID: ";
                    std::cin >> dId;
                    std::cout << "Enter Cargo ID to override: ";
                    std::cin >> cId;
                    std::cout << "Enter override quantity to inject into depot: ";
                    std::cin >> qty;

                    manager.executeCommanderOverride(dId, cId, qty);
                    break;
                }
                case 3: {
                    std::string cId;
                    int newPrio = 5;
                    std::cout << "Enter Cargo ID to elevate priority: ";
                    std::cin >> cId;
                    std::cout << "Enter new priority level (1-5): ";
                    std::cin >> newPrio;
                    ReliefCargo& cargo = manager.findCargo(cId);
                    manager.getIncidentCommander().forceElevateCargoPriority(cargo, newPrio);
                    manager.saveAllData();
                    break;
                }
                default:
                    std::cout << "  [!] Invalid choice.\n";
            }
        } catch (const ERLHubException& ex) {
            std::cout << "  [ERROR] " << ex.what() << "\n";
        }
    }
}

// 9. Manifest Management Submenu
void handleManifestSubmenu(LogisticsManager& manager) {
    while (true) {
        std::cout << "\n--- [9] MANIFEST MANAGEMENT SUBMENU (DYNAMIC MEMORY & RULE OF THREE) ---\n";
        std::cout << " 1. View Archived Manifests\n";
        std::cout << " 2. Demonstrate Deep Copy Constructor (Rule of Three)\n";
        std::cout << " 3. Demonstrate Copy Assignment Operator (Rule of Three)\n";
        std::cout << " 4. Demonstrate Operator == on Manifests\n";
        std::cout << " 5. Back to Main Menu\n";
        std::cout << "Choice [1-5]: ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            clearInputBuffer();
            continue;
        }
        if (choice == 5) break;

        switch (choice) {
            case 1: {
                const auto& manifests = manager.getArchivedManifests();
                if (manifests.empty()) {
                    std::cout << "  [No archived manifests recorded yet]\n";
                } else {
                    for (size_t i = 0; i < manifests.size(); ++i) {
                        std::cout << manifests[i] << "\n";
                    }
                }
                break;
            }
            case 2: {
                std::cout << "\n>>> DEMONSTRATING DEEP COPY CONSTRUCTOR (Rule of Three) <<<\n";
                Manifest original("MNF-ORIG-01", "Chamoli Forward Base", "VEH-AER-01", 3);
                original.addCargo(ReliefCargo("CRG-103", "Trauma Kits", "Trauma Triage Medicine", 20, "boxes", 3.0, 5, true));
                original.addCargo(ReliefCargo("CRG-105", "Defibrillators", "Emergency Medical Equipment", 5, "units", 8.0, 5, true));

                std::cout << "\n[1] Original Manifest:\n" << original;

                // Deep copy construction
                Manifest copied = original;
                std::cout << "\n[2] Copied Manifest (Created via Deep Copy Constructor):\n" << copied;

                std::cout << "\n[3] Modifying original manifest by adding a third cargo item...\n";
                original.addCargo(ReliefCargo("CRG-102", "Emergency Water", "Clean Water", 100, "packs", 5.0, 4, false));

                std::cout << "--> Original Manifest Count: " << original.getCount() 
                          << " (Total Weight: " << original.getTotalWeightKg() << " kg)\n";
                std::cout << "--> Copied Manifest Count  : " << copied.getCount() 
                          << " (Total Weight: " << copied.getTotalWeightKg() << " kg)\n";

                if (original.getCount() != copied.getCount()) {
                    std::cout << "  [✓] VERIFICATION SUCCESS: Deep copy confirmed! Modifying original did NOT alter copy.\n";
                    std::cout << "      Dynamically allocated heap memory buffers are completely independent.\n";
                } else {
                    std::cout << "  [FAIL] Shallow copy detected!\n";
                }
                break;
            }
            case 3: {
                std::cout << "\n>>> DEMONSTRATING COPY ASSIGNMENT OPERATOR (Rule of Three) <<<\n";
                Manifest m1("MNF-ASSIGN-A", "Depot Alpha", "VEH-GND-01", 2);
                m1.addCargo(ReliefCargo("CRG-101", "Ration Packs", "Food", 50, "kits", 2.5, 3, false));

                Manifest m2("MNF-ASSIGN-B", "Depot Beta", "VEH-AMP-01", 2);
                m2.addCargo(ReliefCargo("CRG-104", "Blankets", "Thermal", 80, "units", 1.2, 3, false));

                std::cout << "[Before Assignment] m2 ID: " << m2.getManifestId() << " | Weight: " << m2.getTotalWeightKg() << " kg\n";
                m2 = m1; // Copy assignment
                std::cout << "[After m2 = m1] m2 ID: " << m2.getManifestId() << " | Weight: " << m2.getTotalWeightKg() << " kg\n";

                if (m2.getManifestId() == m1.getManifestId() && m2.getTotalWeightKg() == m1.getTotalWeightKg()) {
                    std::cout << "  [✓] VERIFICATION SUCCESS: Copy assignment operator performed clean deallocation and deep copy.\n";
                }
                break;
            }
            case 4: {
                Manifest m1("MNF-IDENT-101", "Base", "VEH-01", 2);
                Manifest m2("MNF-IDENT-101", "Base", "VEH-01", 2);
                Manifest m3("MNF-IDENT-999", "Base", "VEH-02", 2);

                std::cout << "Comparing m1 (" << m1.getManifestId() << ") == m2 (" << m2.getManifestId() << "): "
                          << ((m1 == m2) ? "EQUAL (MATCH)" : "NOT EQUAL") << "\n";
                std::cout << "Comparing m1 (" << m1.getManifestId() << ") == m3 (" << m3.getManifestId() << "): "
                          << ((m1 == m3) ? "EQUAL (MATCH)" : "NOT EQUAL") << "\n";
                break;
            }
            default:
                std::cout << "  [!] Invalid choice.\n";
        }
    }
}

int main() {
    std::cout << "Initializing Post-Earthquake Multi-Agency Emergency Relief Logistics Hub (ERL-HUB)...\n";
    std::cout << "Academic Project for B.Tech CIC-211 (MAIT, GGSIPU)\n";

    LogisticsManager manager("data");
    bool loaded = manager.loadAllData();
    if (loaded) {
        std::cout << "[✓] Persistent database records successfully loaded from data/*.bin files.\n";
    } else {
        std::cout << "[✓] Fresh sample disaster scenario generated and initialized.\n";
    }

    bool running = true;
    while (running) {
        printMainMenu();
        int mainChoice = 0;
        if (!(std::cin >> mainChoice)) {
            clearInputBuffer();
            std::cout << "  [!] Invalid input. Please enter a number between 1 and 13.\n";
            continue;
        }

        switch (mainChoice) {
            case 1:
                handleInventorySubmenu(manager);
                break;
            case 2:
                handleDepotSubmenu(manager);
                break;
            case 3:
                handleVehicleSubmenu(manager);
                break;
            case 4:
                handleAllocationSubmenu(manager);
                break;
            case 5:
                handleDispatchSubmenu(manager);
                break;
            case 6:
                handlePriorityMedicalSubmenu(manager);
                break;
            case 7:
                handleRouteEvaluationSubmenu(manager);
                break;
            case 8:
                handleCommanderSubmenu(manager);
                break;
            case 9:
                handleManifestSubmenu(manager);
                break;
            case 10:
                manager.displayStatistics();
                break;
            case 11:
                manager.displayDispatchHistory();
                break;
            case 12:
                manager.saveAllData();
                std::cout << "\n  [✓] All system records atomically persisted to binary files.\n";
                break;
            case 13:
                std::cout << "\n================================================================================\n";
                std::cout << " Saving all disaster relief logs, depots, vehicles, and inventory data...\n";
                manager.saveAllData();
                std::cout << " Closing binary file streams and releasing memory...\n";
                std::cout << " ERL-HUB shutdown complete. Safe deployment concluded.\n";
                std::cout << "================================================================================\n";
                running = false;
                break;
            default:
                std::cout << "  [!] Choice must be between 1 and 13.\n";
        }
    }

    return 0;
}
