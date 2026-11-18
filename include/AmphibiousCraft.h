#ifndef ERL_HUB_AMPHIBIOUS_CRAFT_H
#define ERL_HUB_AMPHIBIOUS_CRAFT_H

#include "ReliefVehicle.h"
#include <string>

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Represents amphibious rescue boats and water transportation craft.
 * Demonstrates Unit III Virtual Inheritance (Virtual Base Class).
 */
class AmphibiousCraft : virtual public ReliefVehicle {
protected:
    std::string hullType;
    double draftMeters;
    double maxWaterSpeedKnots;
    bool waterjetPropulsion;

public:
    AmphibiousCraft();
    AmphibiousCraft(const std::string& id, 
                    const std::string& name, 
                    double maxPayloadKg, 
                    double fuel = 100.0, 
                    const std::string& status = "Operational",
                    const std::string& location = "Ganga River Launch Point (Rishikesh)",
                    const std::string& hull = "Reinforced Deep-V Marine Grade",
                    double draft = 0.75,
                    double speedKnots = 28.0,
                    bool waterjet = true);

    virtual ~AmphibiousCraft();

    std::string getHullType() const;
    double getDraftMeters() const;
    double getMaxWaterSpeedKnots() const;
    bool isWaterjetPropulsion() const;

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

#endif // ERL_HUB_AMPHIBIOUS_CRAFT_H
