#ifndef ERL_HUB_INCIDENT_COMMANDER_H
#define ERL_HUB_INCIDENT_COMMANDER_H

#include <string>
#include <vector>
#include <iostream>

// Forward declarations
namespace ReliefOps {
namespace Inventory {
    class ReliefCargo;
}
namespace Logistics {
    class DistributionDepot;
    class ReliefVehicle;
    class TransportVehicle;

/**
 * @brief Represents the senior Incident Commander with privileged triage override authority.
 * Demonstrates Unit I Friend Class by directly accessing private members of DistributionDepot and ReliefCargo.
 */
class IncidentCommander {
private:
    std::string commanderId;
    std::string officerName;
    std::string authorizationRank; // e.g. "NDRF Deputy Inspector General", "Chief Medical Officer"
    int overrideCount;

public:
    IncidentCommander(const std::string& id = "CMD-001", 
                      const std::string& name = "Col. Rajesh Sharma", 
                      const std::string& rank = "NDRF Disaster Relief Commander");

    std::string getCommanderId() const;
    std::string getOfficerName() const;
    std::string getAuthorizationRank() const;
    int getOverrideCount() const;

    // Unit I 6.6 Friend Class Operations:
    // Directly accesses private members of DistributionDepot (e.g. depot.inventory, depot.currentStoredWeightKg)
    // and private members of ReliefCargo (e.g. cargo.availableQuantity)
    bool emergencyTriageOverride(DistributionDepot& depot, 
                                const std::string& cargoId, 
                                int overrideQuantity);

    // Directly accesses private members of ReliefCargo (cargo.emergencyPriorityLevel)
    void forceElevateCargoPriority(ReliefOps::Inventory::ReliefCargo& cargo, int newPriority);

    // Authorizes hazardous mission dispatch
    void authorizeHazardousDispatch(TransportVehicle& vehicle, const std::string& missionCode);

    void displayCommanderDashboard(const std::vector<DistributionDepot>& depots, 
                                   const std::vector<ReliefVehicle*>& fleet) const;
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_INCIDENT_COMMANDER_H
