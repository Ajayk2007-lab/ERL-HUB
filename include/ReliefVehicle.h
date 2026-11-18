#ifndef ERL_HUB_RELIEF_VEHICLE_H
#define ERL_HUB_RELIEF_VEHICLE_H

#include "TransportVehicle.h"
#include "GPSBeacon.h"
#include "EmergencyLifeSupportUnit.h"
#include <string>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Abstract polymorphic base class for specialized relief vehicles.
 * Demonstrates Unit III Inheritance, Containership, and Virtual Destructors.
 */
class ReliefVehicle : public TransportVehicle {
protected:
    // Unit III Containership (Composition): embedded subsystem objects
    GPSBeacon gpsBeacon;
    EmergencyLifeSupportUnit lifeSupport;

public:
    ReliefVehicle();
    ReliefVehicle(const std::string& id, 
                  const std::string& name, 
                  double maxPayloadKg, 
                  double fuel = 100.0, 
                  const std::string& status = "Operational",
                  const std::string& location = "Central Hub Dehradun",
                  const std::string& beaconId = "GPS-01",
                  const std::string& elsuId = "ELSU-01");

    virtual ~ReliefVehicle();

    // Containership accessors
    GPSBeacon& getGPSBeacon();
    const GPSBeacon& getGPSBeacon() const;
    EmergencyLifeSupportUnit& getLifeSupport();
    const EmergencyLifeSupportUnit& getLifeSupport() const;

    // Unit III Runtime Polymorphism: Pure virtual and virtual functions
    virtual bool evaluateRoute(const std::string& corridorName, 
                               double distanceKm, 
                               int terrainSeverity) const = 0;

    virtual bool calculateOperationalFeasibility(double payloadWeightKg, 
                                                double distanceKm) const;

    virtual void displayDetails() const;
    virtual std::string getVehicleType() const = 0;

    // Serialization helper
    virtual void serialize(std::ofstream& out) const;
    virtual void deserialize(std::ifstream& in);
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_RELIEF_VEHICLE_H
