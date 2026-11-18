#ifndef ERL_HUB_AERIAL_TRANSPORT_H
#define ERL_HUB_AERIAL_TRANSPORT_H

#include "ReliefVehicle.h"
#include <string>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Represents emergency medical helicopters and search & rescue quadcopters.
 * Demonstrates Unit III Virtual Inheritance (Virtual Base Class).
 */
class AerialTransport : virtual public ReliefVehicle {
protected:
    double maxAltitudeMeters;
    std::string rotorConfiguration;
    double maxWindToleranceKnots;

public:
    AerialTransport();
    AerialTransport(const std::string& id, 
                    const std::string& name, 
                    double maxPayloadKg, 
                    double fuel = 100.0, 
                    const std::string& status = "Operational",
                    const std::string& location = "Helipad Alpha (Dehradun)",
                    double maxAlt = 4500.0,
                    const std::string& rotor = "Twin-Turbine Rescue Chopper",
                    double maxWind = 50.0);

    virtual ~AerialTransport();

    double getMaxAltitudeMeters() const;
    std::string getRotorConfiguration() const;
    double getMaxWindToleranceKnots() const;

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

#endif // ERL_HUB_AERIAL_TRANSPORT_H
