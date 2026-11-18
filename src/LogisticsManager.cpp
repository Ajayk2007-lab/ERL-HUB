#include "../include/LogisticsManager.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>

namespace ReliefOps {
namespace Logistics {

// Unit II Static Data Member Initialization
double LogisticsManager::totalDispatchedMetricTonnage = 0.0;
int LogisticsManager::totalDispatchedMissions = 0;

LogisticsManager::LogisticsManager(const std::string& dataDirectory)
    : incidentCommander("CMD-NDRF-01", "Col. Rajesh Sharma", "Senior Disaster Logistics Commander"),
      fileManager(dataDirectory) {}

LogisticsManager::~LogisticsManager() {
    // Clean up dynamically allocated vehicle objects in fleet
    for (auto* vehicle : fleet) {
        delete vehicle;
    }
    fleet.clear();
    fleetMap.clear();
}

double LogisticsManager::getTotalDispatchedMetricTonnage() {
    return totalDispatchedMetricTonnage;
}

int LogisticsManager::getTotalDispatchedMissions() {
    return totalDispatchedMissions;
}

void LogisticsManager::recordDispatchTonnage(double tonnageMetricTons) {
    if (tonnageMetricTons > 0.0) {
        totalDispatchedMetricTonnage += tonnageMetricTons;
        ++totalDispatchedMissions;
    }
}

void LogisticsManager::resetDispatchStatistics() {
    totalDispatchedMetricTonnage = 0.0;
    totalDispatchedMissions = 0;
}

void LogisticsManager::initializeSampleData() {
    // 1. Initialize Central Cargo Catalog
    centralCatalog.clear();
    cargoIndexMap.clear();

    registerCargo(ReliefOps::Inventory::ReliefCargo("CRG-101", "High-Energy Ration Packs", "Food/Ration Kits", 800, "kits", 2.5, 3, false));
    registerCargo(ReliefOps::Inventory::ReliefCargo("CRG-102", "Emergency Potable Water", "Clean Water", 1200, "packs", 5.0, 4, false));
    registerCargo(ReliefOps::Inventory::ReliefCargo("CRG-103", "Trauma & Surgical Kits", "Trauma Triage Medicine", 250, "boxes", 3.0, 5, true));
    registerCargo(ReliefOps::Inventory::ReliefCargo("CRG-104", "Thermal Fleece Blankets", "Thermal Blankets", 700, "units", 1.2, 3, false));
    registerCargo(ReliefOps::Inventory::ReliefCargo("CRG-105", "Defibrillators & Monitors", "Emergency Medical Equipment", 50, "units", 8.0, 5, true));
    registerCargo(ReliefOps::Inventory::ReliefCargo("CRG-106", "Pediatric Antibiotic Packs", "Trauma Triage Medicine", 300, "kits", 1.5, 5, true));

    // 2. Initialize Distribution Depots
    DistributionDepot::resetDepotCounter();
    depots.clear();
    depotIndexMap.clear();

    DistributionDepot d1("DPT-01", "Central Logistics Hub Dehradun", "Dehradun Sector 1", 60000.0);
    d1 += findCargo("CRG-101");
    d1 += findCargo("CRG-102");
    d1 += findCargo("CRG-103");
    d1 += findCargo("CRG-104");
    registerDepot(d1);

    DistributionDepot d2("DPT-02", "Valley Forward Depot Rishikesh", "Rishikesh Riverfront", 40000.0);
    d2 += findCargo("CRG-102");
    d2 += findCargo("CRG-103");
    d2 += findCargo("CRG-105");
    d2 += findCargo("CRG-106");
    registerDepot(d2);

    DistributionDepot d3("DPT-03", "High-Altitude Base Chamoli", "Chamoli Forward Helipad", 25000.0);
    d3 += findCargo("CRG-101");
    d3 += findCargo("CRG-103");
    d3 += findCargo("CRG-104");
    d3 += findCargo("CRG-105");
    registerDepot(d3);

    // 3. Initialize Polymorphic Fleet
    for (auto* v : fleet) delete v;
    fleet.clear();
    fleetMap.clear();
    TransportVehicle::resetOperationalVehicleCount();

    registerVehicle(new GroundConvoy("VEH-GND-01", "Heavy Relief Truck (Ashok Leyland 6x6)", 8500.0, 95.0, "Operational", "Dehradun Base", 6, "6x6 All-Wheel Drive", 450.0, true));
    registerVehicle(new AerialTransport("VEH-AER-01", "Emergency Medical Chopper (MI-17 V5)", 2400.0, 90.0, "Operational", "Helipad Alpha (Dehradun)", 4800.0, "Twin-Turbine Heavy Rotor", 50.0));
    registerVehicle(new AerialTransport("VEH-AER-02", "Search & Rescue Heavy Quadcopter", 350.0, 85.0, "Operational", "Chamoli Helipad", 3800.0, "Carbon Coaxial Quad-Rotor", 38.0));
    registerVehicle(new AmphibiousCraft("VEH-AMP-01", "Amphibious Riverine Rescue Boat", 3200.0, 92.0, "Operational", "Rishikesh Ganga Ghat", "Deep-V Reinforced Marine", 0.70, 32.0, true));
    registerVehicle(new HybridRescueVehicle("VEH-HYB-01", "Hybrid Amphibious-Air Triage Unit", 2800.0, 98.0, "Operational", "Forward Disaster Base Chamoli", 5200.0, "Tilt-Rotor VTOL Turbines", 55.0, "Carbon Hydrofoil", 0.45, 36.0, "Amphibious Transition", 98.0, true));
}

bool LogisticsManager::loadAllData() {
    bool invOk = fileManager.loadInventory(centralCatalog);
    if (invOk) {
        cargoIndexMap.clear();
        for (size_t i = 0; i < centralCatalog.size(); ++i) {
            cargoIndexMap[centralCatalog[i].getCargoId()] = i;
        }
    }

    bool dptOk = fileManager.loadDepots(depots);
    if (dptOk) {
        depotIndexMap.clear();
        for (size_t i = 0; i < depots.size(); ++i) {
            depotIndexMap[depots[i].getDepotId()] = i;
        }
    }

    bool vehOk = fileManager.loadVehicles(fleet);
    if (vehOk) {
        fleetMap.clear();
        for (auto* v : fleet) {
            if (v) fleetMap[v->getVehicleId()] = v;
        }
    }

    fileManager.readDispatchLogs(dispatchHistoryLogs);

    if (!invOk || !dptOk || !vehOk) {
        // If files were missing or empty, initialize realistic sample data
        initializeSampleData();
        saveAllData();
        return false;
    }
    return true;
}

bool LogisticsManager::saveAllData() {
    bool s1 = fileManager.saveInventory(centralCatalog);
    bool s2 = fileManager.saveDepots(depots);
    bool s3 = fileManager.saveVehicles(fleet);
    return s1 && s2 && s3;
}

// Module A
void LogisticsManager::registerCargo(const ReliefOps::Inventory::ReliefCargo& cargo) {
    if (cargoIndexMap.find(cargo.getCargoId()) != cargoIndexMap.end()) {
        throw ReliefOps::Exceptions::DuplicateIdException("Cargo with ID '" + cargo.getCargoId() + "' already exists in catalog.");
    }
    cargoIndexMap[cargo.getCargoId()] = centralCatalog.size();
    centralCatalog.push_back(cargo);
}

void LogisticsManager::displayCargoCatalog() const {
    std::cout << "\n" << tableHeader("CENTRAL EMERGENCY RELIEF CARGO CATALOG", 110) << "\n";
    if (centralCatalog.empty()) {
        std::cout << "  [No cargo records registered in catalog]\n";
    } else {
        // STL Algorithm: std::for_each
        int idx = 1;
        std::for_each(centralCatalog.begin(), centralCatalog.end(), [&idx](const ReliefOps::Inventory::ReliefCargo& item) {
            std::cout << "  " << std::setw(2) << idx++ << ". " << item << "\n";
        });
    }
    std::cout << std::string(110, '-') << "\n";
}

void LogisticsManager::displayCriticalMedicalSupplies() {
    std::cout << "\n" << tableHeader("CRITICAL EMERGENCY MEDICAL SUPPLIES (SORTED VIA FUNCTOR)", 110) << "\n";
    std::vector<ReliefOps::Inventory::ReliefCargo> medicalList;
    for (const auto& item : centralCatalog) {
        if (item.isMedical()) {
            medicalList.push_back(item);
        }
    }

    // Unit IV 9.6: Custom Functor sorting using MedicalPriorityComparator
    std::sort(medicalList.begin(), medicalList.end(), ReliefOps::Inventory::MedicalPriorityComparator());

    if (medicalList.empty()) {
        std::cout << "  [No emergency medical supplies found]\n";
    } else {
        int rank = 1;
        for (const auto& med : medicalList) {
            std::cout << "  Rank #" << rank++ << " [Priority " << med.getEmergencyPriorityLevel() << "/5] "
                      << med << "\n";
        }
    }
    std::cout << std::string(110, '-') << "\n";
}

// Unit I 6.4: Return by Reference
ReliefOps::Inventory::ReliefCargo& LogisticsManager::findCargo(const std::string& cargoId) {
    auto it = cargoIndexMap.find(cargoId);
    if (it == cargoIndexMap.end()) {
        throw ReliefOps::Exceptions::EntityNotFoundException("Cargo ID '" + cargoId + "' not found in catalog.");
    }
    return centralCatalog[it->second];
}

bool LogisticsManager::hasCargo(const std::string& cargoId) const {
    return cargoIndexMap.find(cargoId) != cargoIndexMap.end();
}

// Module B
void LogisticsManager::registerDepot(const DistributionDepot& depot) {
    if (depotIndexMap.find(depot.getDepotId()) != depotIndexMap.end()) {
        throw ReliefOps::Exceptions::DuplicateIdException("Depot with ID '" + depot.getDepotId() + "' already exists.");
    }
    depotIndexMap[depot.getDepotId()] = depots.size();
    depots.push_back(depot);
}

void LogisticsManager::displayAllDepots() const {
    std::cout << "\n" << tableHeader("REGISTERED DISTRIBUTION DEPOTS & STRATEGIC BASES", 100) << "\n";
    for (size_t i = 0; i < depots.size(); ++i) {
        std::cout << "  " << (i + 1) << ". ";
        depots[i].displayDepotSummary();
    }
    std::cout << std::string(100, '-') << "\n";
}

DistributionDepot& LogisticsManager::getDepot(const std::string& depotId) {
    auto it = depotIndexMap.find(depotId);
    if (it == depotIndexMap.end()) {
        throw ReliefOps::Exceptions::EntityNotFoundException("Depot ID '" + depotId + "' not registered.");
    }
    return depots[it->second];
}

const DistributionDepot& LogisticsManager::getDepot(const std::string& depotId) const {
    auto it = depotIndexMap.find(depotId);
    if (it == depotIndexMap.end()) {
        throw ReliefOps::Exceptions::EntityNotFoundException("Depot ID '" + depotId + "' not registered.");
    }
    return depots[it->second];
}

bool LogisticsManager::hasDepot(const std::string& depotId) const {
    return depotIndexMap.find(depotId) != depotIndexMap.end();
}

// Module C
void LogisticsManager::registerVehicle(ReliefVehicle* vehicle) {
    if (!vehicle) return;
    if (fleetMap.find(vehicle->getVehicleId()) != fleetMap.end()) {
        throw ReliefOps::Exceptions::DuplicateIdException("Vehicle with ID '" + vehicle->getVehicleId() + "' already exists.");
    }
    fleetMap[vehicle->getVehicleId()] = vehicle;
    fleet.push_back(vehicle);
}

void LogisticsManager::displayAllVehicles() const {
    std::cout << "\n" << tableHeader("EMERGENCY RELIEF VEHICLE FLEET", 110) << "\n";
    for (size_t i = 0; i < fleet.size(); ++i) {
        if (fleet[i]) {
            std::cout << "  " << (i + 1) << ". [" << fleet[i]->getVehicleType() << "] " 
                      << *fleet[i] << "\n";
        }
    }
    std::cout << std::string(110, '-') << "\n";
}

ReliefVehicle& LogisticsManager::getVehicle(const std::string& vehicleId) {
    auto it = fleetMap.find(vehicleId);
    if (it == fleetMap.end() || !it->second) {
        throw ReliefOps::Exceptions::EntityNotFoundException("Vehicle ID '" + vehicleId + "' not found in fleet.");
    }
    return *(it->second);
}

const ReliefVehicle& LogisticsManager::getVehicle(const std::string& vehicleId) const {
    auto it = fleetMap.find(vehicleId);
    if (it == fleetMap.end() || !it->second) {
        throw ReliefOps::Exceptions::EntityNotFoundException("Vehicle ID '" + vehicleId + "' not found in fleet.");
    }
    return *(it->second);
}

bool LogisticsManager::hasVehicle(const std::string& vehicleId) const {
    return fleetMap.find(vehicleId) != fleetMap.end();
}

const std::vector<ReliefVehicle*>& LogisticsManager::getFleet() const {
    return fleet;
}

// Unit I 6.2: Function Overloading
// Overload 1: Volume/Quantity-based payload allocation
bool LogisticsManager::allocateSupplies(DistributionDepot& depot, 
                                       ReliefVehicle& vehicle, 
                                       const std::string& cargoId, 
                                       int quantity) {
    std::cout << "\n[Supply Allocation - Volume-Based Mode]\n";
    std::cout << "  Depot: " << depot.getDepotName() << " | Vehicle: " << vehicle.getVehicleName() << "\n";
    std::cout << "  Requested Cargo ID: " << cargoId << " | Quantity: " << quantity << "\n";

    if (!depot.hasStock(cargoId, quantity)) {
        throw ReliefOps::Exceptions::InsufficientStockException(
            cargoId, quantity, depot.findCargoById(cargoId).getAvailableQuantity());
    }

    ReliefOps::Inventory::ReliefCargo& stockItem = depot.findCargoById(cargoId);
    double requestedWeight = quantity * stockItem.getWeightPerUnitKg();

    if (!vehicle.checkCapacity(requestedWeight)) {
        throw ReliefOps::Exceptions::CargoOverweightException(
            vehicle.getCurrentLoadKg() + requestedWeight, vehicle.getMaxPayloadCapacityKg(),
            "Cannot allocate cargo: vehicle capacity exceeded.");
    }

    // Execute allocation: load payload and deduct from depot
    vehicle.loadPayload(requestedWeight);
    stockItem.updateQuantity(-quantity);
    depot.recalculateStoredWeight();

    std::cout << "  [✓] Volume Allocation Successful: " << quantity << " " << stockItem.getUnitOfMeasure() 
              << " (" << requestedWeight << " kg) loaded onto " << vehicle.getVehicleId() << ".\n";
    return true;
}

// Overload 2: Priority-based emergency medical allocation
bool LogisticsManager::allocateSupplies(DistributionDepot& depot, 
                                       ReliefVehicle& vehicle, 
                                       int minPriorityLevel, 
                                       double maxWeightKg) {
    std::cout << "\n[Supply Allocation - Priority Medical Mode]\n";
    std::cout << "  Depot: " << depot.getDepotName() << " | Vehicle: " << vehicle.getVehicleName() << "\n";
    std::cout << "  Minimum Priority Level: " << minPriorityLevel << " | Weight Cap: " << maxWeightKg << " kg\n";

    double allocatedWeight = 0.0;
    int itemsAllocated = 0;

    // Filter available medical cargo from depot
    std::vector<ReliefOps::Inventory::ReliefCargo> candidates;
    for (const auto& item : depot.getInventory()) {
        if (item.isMedical() && item.getEmergencyPriorityLevel() >= minPriorityLevel && item.getAvailableQuantity() > 0) {
            candidates.push_back(item);
        }
    }

    // Sort by medical priority using custom functor
    std::sort(candidates.begin(), candidates.end(), ReliefOps::Inventory::MedicalPriorityComparator());

    for (const auto& cand : candidates) {
        ReliefOps::Inventory::ReliefCargo& depotItem = depot.findCargoById(cand.getCargoId());
        int possibleUnits = static_cast<int>((maxWeightKg - allocatedWeight) / depotItem.getWeightPerUnitKg());
        if (possibleUnits > depotItem.getAvailableQuantity()) {
            possibleUnits = depotItem.getAvailableQuantity();
        }

        if (possibleUnits > 0) {
            double wt = possibleUnits * depotItem.getWeightPerUnitKg();
            if (vehicle.checkCapacity(wt)) {
                vehicle.loadPayload(wt);
                depotItem.updateQuantity(-possibleUnits);
                allocatedWeight += wt;
                ++itemsAllocated;
                std::cout << "  -> Allocated " << possibleUnits << " units of " << depotItem.getCargoName() 
                          << " (Weight: " << wt << " kg, Priority " << depotItem.getEmergencyPriorityLevel() << ")\n";
            }
        }
    }

    depot.recalculateStoredWeight();
    std::cout << "  [✓] Priority Allocation Complete: " << itemsAllocated << " medical line items (" 
              << allocatedWeight << " kg) loaded.\n";
    return itemsAllocated > 0;
}

// Module D: Emergency Dispatch Execution
bool LogisticsManager::executeDispatch(const std::string& depotId, 
                                       const std::string& vehicleId, 
                                       const std::string& destination, 
                                       const std::string& corridor, 
                                       const std::vector<std::pair<std::string, int>>& cargoList,
                                       int terrainSeverity,
                                       double distanceKm) {
    std::cout << "\n" << tableHeader("INITIATING EMERGENCY RELIEF DISPATCH MISSION", 80) << "\n";
    DistributionDepot& depot = getDepot(depotId);
    ReliefVehicle& vehicle = getVehicle(vehicleId);

    std::cout << " Source Depot : " << depot.getDepotName() << " [" << depotId << "]\n";
    std::cout << " Target Dest  : " << destination << "\n";
    std::cout << " Corridor     : " << corridor << " (" << distanceKm << " km, Damage Severity: " << terrainSeverity << "/10)\n";
    std::cout << " Vehicle      : " << vehicle.getVehicleName() << " [" << vehicleId << "] (" << vehicle.getVehicleType() << ")\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    // Step 1: Pre-validation of Stock
    double totalWeightToDispatch = 0.0;
    for (const auto& pair : cargoList) {
        const std::string& cId = pair.first;
        int qty = pair.second;
        if (!depot.hasStock(cId, qty)) {
            throw ReliefOps::Exceptions::InsufficientStockException(
                cId, qty, depot.findCargoById(cId).getAvailableQuantity());
        }
        totalWeightToDispatch += (qty * depot.findCargoById(cId).getWeightPerUnitKg());
    }

    // Step 2: Vehicle Payload Capacity Validation (Unit IV 9.1 Exception)
    if (!vehicle.checkCapacity(totalWeightToDispatch)) {
        throw ReliefOps::Exceptions::CargoOverweightException(
            vehicle.getCurrentLoadKg() + totalWeightToDispatch, vehicle.getMaxPayloadCapacityKg(),
            "Mission dispatch aborted: Total payload exceeds vehicle limits.");
    }

    // Step 3: Vehicle Fuel Validation (Unit IV 9.1 Exception)
    double requiredFuel = distanceKm * 0.25;
    if (!vehicle.checkFuel(requiredFuel)) {
        throw ReliefOps::Exceptions::FuelDepletionException(
            vehicle.getFuelLevelPercentage(), requiredFuel,
            "Mission dispatch aborted: Insufficient fuel for planned corridor.");
    }

    // Step 4: Route Corridor Feasibility Evaluation (Unit III Runtime Polymorphism)
    std::cout << " Evaluating Corridor via Runtime Polymorphism:\n";
    bool routeFeasible = vehicle.evaluateRoute(corridor, distanceKm, terrainSeverity);
    if (!routeFeasible) {
        throw ReliefOps::Exceptions::DamagedCorridorException(
            corridor, terrainSeverity,
            "Vehicle evaluation determined corridor is impassable under current conditions.");
    }

    // Step 5: Manifest Generation using Dynamic Memory Allocation (Unit II Rule of Three)
    std::string mId = "MNF-" + std::to_string(totalDispatchedMissions + 101);
    ReliefOps::Inventory::Manifest manifest(mId, destination, vehicleId, cargoList.size() + 2);

    for (const auto& pair : cargoList) {
        ReliefOps::Inventory::ReliefCargo& depotCargo = depot.findCargoById(pair.first);
        ReliefOps::Inventory::ReliefCargo dispatchedItem(
            depotCargo.getCargoId(), depotCargo.getCargoName(), depotCargo.getCategory(),
            pair.second, depotCargo.getUnitOfMeasure(), depotCargo.getWeightPerUnitKg(),
            depotCargo.getEmergencyPriorityLevel(), depotCargo.isMedical());
        
        manifest.addCargo(dispatchedItem);

        // Deduct from depot inventory using Operator Overloading: depot -= cargo
        depot -= dispatchedItem;
    }

    // Load payload onto vehicle and update status
    vehicle.loadPayload(totalWeightToDispatch);
    vehicle.consumeFuel(requiredFuel);
    vehicle.setOperationalStatus("In Mission");
    vehicle.setCurrentLocation(corridor + " -> " + destination);
    vehicle.getGPSBeacon().updateCoordinates(30.5000, 78.5000, "En-Route: " + corridor);

    // Archive Manifest (Demonstrates Deep Copy Constructor)
    archiveManifest(manifest);

    // Update Global Statistics (Unit II Static Members)
    double metricTons = totalWeightToDispatch / 1000.0;
    recordDispatchTonnage(metricTons);

    // Append Audit Log
    std::time_t now = std::time(nullptr);
    char timeBuf[64];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::ostringstream oss;
    oss << "[" << timeBuf << "] MISSION #" << totalDispatchedMissions 
        << " | Manifest: " << mId << " | Vehicle: " << vehicleId 
        << " | Src: " << depotId << " -> Dest: " << destination 
        << " | Weight: " << totalWeightToDispatch << " kg (" << metricTons << " MT)"
        << " | Fuel Consumed: " << requiredFuel << "%";
    
    std::string logLine = oss.str();
    dispatchHistoryLogs.push_back(logLine);
    fileManager.appendDispatchLog(logLine);

    // Persist all data changes atomically
    saveAllData();

    std::cout << "\n" << manifest << "\n";
    std::cout << " [✓] DISPATCH MISSION COMPLETED & AUDIT RECORDED SUCCESSFULLY.\n";
    std::cout << "     Dispatched Metric Tons : " << metricTons << " MT\n";
    std::cout << "     Cumulative Total MT    : " << totalDispatchedMetricTonnage << " MT\n";
    std::cout << "================================================================================\n";
    return true;
}

void LogisticsManager::submitEmergencyRequest(const DispatchRequest& req) {
    emergencyQueue.push(req);
    std::cout << "  [✓] Priority Medical Dispatch Request queued: " << req << "\n";
}

bool LogisticsManager::processNextEmergencyDispatch() {
    if (emergencyQueue.empty()) {
        std::cout << "  [!] Emergency priority dispatch queue is currently empty.\n";
        return false;
    }

    DispatchRequest topReq = emergencyQueue.top();
    emergencyQueue.pop();

    std::cout << "\n>>> PROCESSING HIGHEST PRIORITY EMERGENCY REQUEST <<<\n";
    std::cout << "    Request: " << topReq << "\n";

    // Auto-match an available operational vehicle capable of handling the request
    ReliefVehicle* selectedVehicle = nullptr;
    for (auto* v : fleet) {
        if (v && v->isOperational() && v->checkCapacity(topReq.getEstimatedWeightKg())) {
            selectedVehicle = v;
            break;
        }
    }

    if (!selectedVehicle) {
        std::cout << "  [!] No operational vehicle with sufficient capacity available right now.\n";
        std::cout << "  -> Re-queuing request to priority queue.\n";
        emergencyQueue.push(topReq);
        return false;
    }

    topReq.setAssignedVehicleId(selectedVehicle->getVehicleId());
    std::cout << "  -> Assigned Vehicle: " << selectedVehicle->getVehicleName() 
              << " [" << selectedVehicle->getVehicleId() << "]\n";

    // Prepare cargo list from depot
    DistributionDepot& depot = getDepot(topReq.getSourceDepotId());
    std::vector<std::pair<std::string, int>> cargoList;
    for (const auto& item : depot.getInventory()) {
        if (topReq.isMedicalEmergency() && item.isMedical() && item.getAvailableQuantity() > 0) {
            int qtyToTake = std::min(item.getAvailableQuantity(), 20);
            cargoList.push_back({item.getCargoId(), qtyToTake});
            break;
        }
    }

    if (cargoList.empty()) {
        // Fallback to first available item
        if (!depot.getInventory().empty() && depot.getInventory()[0].getAvailableQuantity() > 0) {
            cargoList.push_back({depot.getInventory()[0].getCargoId(), 5});
        }
    }

    return executeDispatch(topReq.getSourceDepotId(), 
                           selectedVehicle->getVehicleId(), 
                           topReq.getDestinationName(), 
                           topReq.getCorridorName(), 
                           cargoList, 
                           4, 35.0);
}

void LogisticsManager::displayEmergencyQueue() const {
    std::cout << "\n" << tableHeader("PRIORITY EMERGENCY DISPATCH QUEUE (TEMPLATE PRIORITY QUEUE)", 90) << "\n";
    emergencyQueue.display();
    std::cout << std::string(90, '-') << "\n";
}

IncidentCommander& LogisticsManager::getIncidentCommander() {
    return incidentCommander;
}

void LogisticsManager::displayIncidentCommanderDashboard() const {
    incidentCommander.displayCommanderDashboard(depots, fleet);
}

bool LogisticsManager::executeCommanderOverride(const std::string& depotId, 
                                                const std::string& cargoId, 
                                                int quantity) {
    DistributionDepot& depot = getDepot(depotId);
    bool ok = incidentCommander.emergencyTriageOverride(depot, cargoId, quantity);
    if (ok) {
        saveAllData();
    }
    return ok;
}

// Unit III Polymorphism demonstration: polymorphic fleet evaluation
void LogisticsManager::evaluateCorridorForFleet(const std::string& corridorName, 
                                              double distanceKm, 
                                              int terrainSeverity) const {
    std::cout << "\n================================================================================\n";
    std::cout << " MULTI-VEHICLE ROUTE CORRIDOR EVALUATION (RUNTIME POLYMORPHISM AUDIT)\n";
    std::cout << " Corridor Name: " << corridorName << " | Distance: " << distanceKm 
              << " km | Earthquake Damage: " << terrainSeverity << "/10\n";
    std::cout << "--------------------------------------------------------------------------------\n";

    for (const auto* v : fleet) {
        if (v) {
            // Virtual function call through base pointer: dynamic binding at runtime!
            bool pass = v->evaluateRoute(corridorName, distanceKm, terrainSeverity);
            std::cout << "  Result for " << v->getVehicleId() << " (" << v->getVehicleType() << "): "
                      << (pass ? "[CLEARED / FEASIBLE]" : "[REJECTED / UNFEASIBLE]") << "\n\n";
        }
    }
    std::cout << "================================================================================\n";
}

void LogisticsManager::displayStatistics() const {
    std::cout << "\n" << tableHeader("ERL-HUB DISASTER LOGISTICS ANALYTICS & FLEET METRICS", 80) << "\n";
    std::cout << " Total Registered Distribution Depots : " << DistributionDepot::getTotalRegisteredDepots() << "\n";
    std::cout << " Total Operational Fleet Vehicles     : " << TransportVehicle::getTotalOperationalVehicles() << "\n";
    std::cout << " Total Dispatched Missions Executed   : " << totalDispatchedMissions << "\n";
    std::cout << " Total Dispatched Metric Tonnage      : " << std::fixed << std::setprecision(3) 
              << totalDispatchedMetricTonnage << " MT (" << (totalDispatchedMetricTonnage * 1000.0) << " kg)\n";
    std::cout << " Central Catalog Registered Items     : " << centralCatalog.size() << "\n";
    std::cout << " Archived Cargo Manifests             : " << archivedManifests.size() << "\n";
    std::cout << " Pending Priority Queue Requests      : " << emergencyQueue.size() << "\n";
    std::cout << " Executive Commander Overrides Logged : " << incidentCommander.getOverrideCount() << "\n";
    std::cout << std::string(80, '=') << "\n";
}

void LogisticsManager::displayDispatchHistory() const {
    std::cout << "\n" << tableHeader("PERSISTENT DISPATCH AUDIT LOGS (RESTORED FROM BINARY FSTREAM)", 100) << "\n";
    if (dispatchHistoryLogs.empty()) {
        std::cout << "  [No dispatch audit history recorded yet]\n";
    } else {
        for (size_t i = 0; i < dispatchHistoryLogs.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << dispatchHistoryLogs[i] << "\n";
        }
    }
    std::cout << std::string(100, '-') << "\n";
}

void LogisticsManager::archiveManifest(const ReliefOps::Inventory::Manifest& manifest) {
    // Demonstrates Unit II Deep Copy: invokes Manifest copy constructor
    ReliefOps::Inventory::Manifest copyManifest = manifest;
    archivedManifests.push_back(copyManifest);
}

const std::vector<ReliefOps::Inventory::Manifest>& LogisticsManager::getArchivedManifests() const {
    return archivedManifests;
}

} // namespace Logistics
} // namespace ReliefOps
