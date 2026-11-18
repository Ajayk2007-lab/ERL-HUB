#ifndef ERL_HUB_HYBRID_RESCUE_VEHICLE_H
#define ERL_HUB_HYBRID_RESCUE_VEHICLE_H

#include "AerialTransport.h"
#include "AmphibiousCraft.h"
#include <string>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Represents an advanced multi-modal amphibious air-rescue unit.
 * Demonstrates Unit III Virtual Base Classes and Resolution of the Diamond Problem:
 * 
 *            ReliefVehicle  (Virtual Base Class)
 *               /       \
 *    AerialTransport   AmphibiousCraft  (Virtual Inheritance)
 *               \       /
 *          HybridRescueVehicle  (Multiple Inheritance)
 */
class HybridRescueVehicle : public AerialTransport, public AmphibiousCraft {
private:
    std::string currentOperationalMode; // "Aerial VTOL", "Water Hydrofoil", "Amphibious Transition"
    double auxiliaryBatteryReserve;     // Percentage (0.0 to 100.0)
    bool rapidDeployTriageUnit;

public:
    HybridRescueVehicle();
    HybridRescueVehicle(const std::string& id, 
                        const std::string& name, 
                        double maxPayloadKg, 
                        double fuel = 100.0, 
                        const std::string& status = "Operational",
                        const std::string& location = "Forward Disaster Base Chamoli",
                        double maxAlt = 5000.0,
                        const std::string& rotor = "Tilt-Rotor Hybrid VTOL",
                        double maxWind = 55.0,
                        const std::string& hull = "Carbon-Kevlar Amphibious Hydrofoil",
                        double draft = 0.50,
                        double speedKnots = 35.0,
                        const std::string& mode = "Amphibious Transition",
                        double battery = 98.0,
                        bool rapidDeploy = true);

    virtual ~HybridRescueVehicle();

    std::string getCurrentOperationalMode() const;
    double getAuxiliaryBatteryReserve() const;
    bool isRapidDeployTriageUnit() const;
    void setOperationalMode(const std::string& mode);

    // Unit III Polymorphic implementations overriding both parents
    virtual bool evaluateRoute(const std::string& corridorName, 
                               double distanceKm, 
                               int terrainSeverity) const override;

    virtual bool calculateOperationalFeasibility(double payloadWeightKg, 
                                                double distanceKm) const override;

    virtual void displayDetails() const override;
    virtual std::string getVehicleType() const override;

    virtual void serialize(std::ofstream& out) const override;
    virtual void deserialize(std::ifstream& in) override;
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_HYBRID_RESCUE_VEHICLE_H
