#ifndef ERL_HUB_GROUND_CONVOY_H
#define ERL_HUB_GROUND_CONVOY_H

#include "ReliefVehicle.h"
#include <string>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Represents heavy all-terrain trucks and relief supply transport vehicles.
 * Demonstrates Unit III Inheritance and Polymorphism.
 */
class GroundConvoy : public ReliefVehicle {
protected:
    int axleCount;
    std::string driveType; // "6x6 Heavy All-Terrain", "4x4 Tactical Transport"
    double groundClearanceMm;
    bool winchEquipped;

public:
    GroundConvoy();
    GroundConvoy(const std::string& id, 
                 const std::string& name, 
                 double maxPayloadKg, 
                 double fuel = 100.0, 
                 const std::string& status = "Operational",
                 const std::string& location = "Central Hub Dehradun",
                 int axles = 6,
                 const std::string& drive = "6x6 All-Wheel-Drive Heavy Truck",
                 double clearance = 450.0,
                 bool winch = true);

    virtual ~GroundConvoy();

    int getAxleCount() const;
    std::string getDriveType() const;
    double getGroundClearanceMm() const;
    bool isWinchEquipped() const;

    // Unit III Polymorphic implementations
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

#endif // ERL_HUB_GROUND_CONVOY_H
