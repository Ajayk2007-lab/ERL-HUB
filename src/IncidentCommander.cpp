#include "../include/IncidentCommander.h"
#include "../include/ReliefCargo.h"
#include "../include/DistributionDepot.h"
#include "../include/ReliefVehicle.h"
#include "../include/Exceptions.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

IncidentCommander::IncidentCommander(const std::string& id, 
                                     const std::string& name, 
                                     const std::string& rank)
    : commanderId(id), officerName(name), authorizationRank(rank), overrideCount(0) {}

std::string IncidentCommander::getCommanderId() const {
    return commanderId;
}

std::string IncidentCommander::getOfficerName() const {
    return officerName;
}

std::string IncidentCommander::getAuthorizationRank() const {
    return authorizationRank;
}

int IncidentCommander::getOverrideCount() const {
    return overrideCount;
}

// Unit I 6.6 Friend Class Demonstration:
// Directly accesses private members of DistributionDepot (depot.inventory, depot.currentStoredWeightKg)
// and ReliefCargo (item.availableQuantity, item.weightPerUnitKg) to execute executive triage override.
bool IncidentCommander::emergencyTriageOverride(DistributionDepot& depot, 
                                                const std::string& cargoId, 
                                                int overrideQuantity) {
    std::cout << "\n[!] INCIDENT COMMANDER EMERGENCY TRIAGE OVERRIDE INITIATED\n";
    std::cout << "    Commander: " << officerName << " (" << authorizationRank << ")\n";
    std::cout << "    Target Depot: " << depot.depotName << " [" << depot.depotId << "]\n";
    std::cout << "    Authorized Executive Triage Override for Cargo ID: " << cargoId << "\n";

    // Direct access to private member depot.inventory (granted via friend class)
    for (auto& item : depot.inventory) {
        // Direct access to private member item.cargoId
        if (item.cargoId == cargoId) {
            std::cout << "    -> Bypassing standard quota restrictions...\n";
            std::cout << "    -> Previous Quantity: " << item.availableQuantity << "\n";
            // Direct modification of private member item.availableQuantity
            item.availableQuantity += overrideQuantity;
            std::cout << "    -> Overridden Quantity: " << item.availableQuantity << "\n";

            // Direct modification of private member depot.currentStoredWeightKg
            depot.currentStoredWeightKg += (overrideQuantity * item.weightPerUnitKg);
            std::cout << "    -> Depot stored weight directly recalibrated to " 
                      << depot.currentStoredWeightKg << " kg.\n";
            
            ++overrideCount;
            std::cout << "    [✓] Executive Triage Override Successfully Executed.\n\n";
            return true;
        }
    }

    std::cout << "    [!] Override Failed: Cargo ID '" << cargoId << "' not found in depot records.\n\n";
    return false;
}

// Unit I 6.6 Friend Class Demonstration:
// Directly accesses private member cargo.emergencyPriorityLevel
void IncidentCommander::forceElevateCargoPriority(ReliefOps::Inventory::ReliefCargo& cargo, int newPriority) {
    if (newPriority < 1 || newPriority > 5) {
        throw ReliefOps::Exceptions::ERLHubException("Invalid priority elevation level.");
    }
    std::cout << "  [Commander Authorization] Elevating priority of " << cargo.cargoName 
              << " from " << cargo.emergencyPriorityLevel << " -> " << newPriority << "\n";
    
    // Direct access to private member cargo.emergencyPriorityLevel
    cargo.emergencyPriorityLevel = newPriority;
    ++overrideCount;
}

void IncidentCommander::authorizeHazardousDispatch(TransportVehicle& vehicle, const std::string& missionCode) {
    std::cout << "  [Commander Override] Executive waiver granted for hazardous mission: " << missionCode
              << " on vehicle: " << vehicle.getVehicleName() << "\n";
    ++overrideCount;
}

void IncidentCommander::displayCommanderDashboard(const std::vector<DistributionDepot>& depots, 
                                                   const std::vector<ReliefVehicle*>& fleet) const {
    std::cout << "\n================================================================================\n";
    std::cout << "           INCIDENT COMMANDER EMERGENCY LOGISTICS DASHBOARD\n";
    std::cout << " Commanding Officer: " << officerName << " | Rank: " << authorizationRank << "\n";
    std::cout << " Executive Triage Overrides Logged: " << overrideCount << "\n";
    std::cout << "================================================================================\n";

    // Fleet operational readiness
    int activeCount = 0;
    for (const auto* v : fleet) {
        if (v && v->isOperational()) ++activeCount;
    }
    std::cout << " [1] FLEET READINESS: " << activeCount << "/" << fleet.size() << " vehicles ready for dispatch.\n";

    // Critical medical supplies across all depots
    std::cout << " [2] CRITICAL MEDICAL SUPPLIES ACROSS DEPOTS:\n";
    int criticalCount = 0;
    for (const auto& d : depots) {
        for (const auto& item : d.getInventory()) {
            if (item.isUrgentMedicalSupply()) {
                std::cout << "     * Depot [" << d.getDepotId() << "] " << item.getCargoName() 
                          << " | Stock: " << item.getAvailableQuantity() << " " << item.getUnitOfMeasure()
                          << " | Priority: " << item.getEmergencyPriorityLevel() << "/5\n";
                ++criticalCount;
            }
        }
    }
    if (criticalCount == 0) {
        std::cout << "     * No critical medical supplies registered currently.\n";
    }

    std::cout << " [3] DEPOT STATUS OVERVIEW:\n";
    for (const auto& d : depots) {
        std::cout << "     * [" << d.getDepotId() << "] " << d.getDepotName() << ": " 
                  << std::fixed << std::setprecision(1) << d.getCurrentStoredWeightKg() 
                  << "/" << d.getMaxStorageCapacityKg() << " kg (" 
                  << d.getCargoCount() << " items)\n";
    }
    std::cout << "================================================================================\n\n";
}

} // namespace Logistics
} // namespace ReliefOps
