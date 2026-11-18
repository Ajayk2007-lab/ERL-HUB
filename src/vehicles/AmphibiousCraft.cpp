#include "../../include/AmphibiousCraft.h"
#include "../../include/FileManager.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

AmphibiousCraft::AmphibiousCraft()
    : ReliefVehicle("AMP-001", "Amphibious Rescue Boat Alpha", 3000.0, 100.0, "Operational", "Rishikesh Launch Base", "GPS-AMP-01", "ELSU-AMP-01"),
      hullType("Reinforced Deep-V Marine Aluminum"), draftMeters(0.75), maxWaterSpeedKnots(28.0), waterjetPropulsion(true) {}

AmphibiousCraft::AmphibiousCraft(const std::string& id, 
                                 const std::string& name, 
                                 double maxPayloadKg, 
                                 double fuel, 
                                 const std::string& status,
                                 const std::string& location,
                                 const std::string& hull,
                                 double draft,
                                 double speedKnots,
                                 bool waterjet)
    : ReliefVehicle(id, name, maxPayloadKg, fuel, status, location, "GPS-" + id, "ELSU-" + id),
      hullType(hull), draftMeters(draft), maxWaterSpeedKnots(speedKnots), waterjetPropulsion(waterjet) {}

AmphibiousCraft::~AmphibiousCraft() {}

std::string AmphibiousCraft::getHullType() const {
    return hullType;
}

double AmphibiousCraft::getDraftMeters() const {
    return draftMeters;
}

double AmphibiousCraft::getMaxWaterSpeedKnots() const {
    return maxWaterSpeedKnots;
}

bool AmphibiousCraft::isWaterjetPropulsion() const {
    return waterjetPropulsion;
}

// Unit III Polymorphic implementation: evaluates water routes and flood channels
bool AmphibiousCraft::evaluateRoute(const std::string& corridorName, 
                                    double distanceKm, 
                                    int terrainSeverity) const {
    std::cout << "  [Waterway Route Evaluation] Vehicle: " << vehicleName << " (" << vehicleId << ")\n";
    std::cout << "  -> Aquatic Corridor: " << corridorName << " | Distance: " << distanceKm 
              << " km | Flood / Debris Severity: " << terrainSeverity << "/10\n";

    // Amphibious crafts navigate river channels, canals, and flooded valley plains
    if (terrainSeverity >= 7) {
        std::cout << "  [!] Water Hazard Alert: Turbulent flash flood currents and submerged debris in '" 
                  << corridorName << "' exceed safe hull limits.\n";
        return false;
    }

    double fuelNeeded = distanceKm * 0.25; // 0.25% fuel per km in water transit
    if (!checkFuel(fuelNeeded)) {
        std::cout << "  [!] Range Warning: Water navigation requires " << fuelNeeded 
                  << "% fuel, but current reserve is " << fuelLevelPercentage << "%.\n";
        return false;
    }

    std::cout << "  [✓] Water Corridor FEASIBLE: Draft " << draftMeters << "m clear for " << hullType << " navigation.\n";
    return true;
}

bool AmphibiousCraft::calculateOperationalFeasibility(double payloadWeightKg, double distanceKm) const {
    if (!isOperational()) return false;
    if (!checkCapacity(payloadWeightKg)) return false;
    double fuelNeeded = distanceKm * 0.25;
    return checkFuel(fuelNeeded);
}

void AmphibiousCraft::displayDetails() const {
    ReliefVehicle::displayDetails();
    std::cout << "   [Amphibious Craft Specs] Hull: " << hullType 
              << " | Draft: " << draftMeters << " m"
              << " | Speed: " << maxWaterSpeedKnots << " knots"
              << " | Waterjet: " << (waterjetPropulsion ? "YES" : "NO") << "\n";
}

std::string AmphibiousCraft::getVehicleType() const {
    return "Amphibious Craft";
}

void AmphibiousCraft::serialize(std::ofstream& out) const {
    ReliefVehicle::serialize(out);
    FileManager::writeString(out, hullType);
    out.write(reinterpret_cast<const char*>(&draftMeters), sizeof(draftMeters));
    out.write(reinterpret_cast<const char*>(&maxWaterSpeedKnots), sizeof(maxWaterSpeedKnots));
    out.write(reinterpret_cast<const char*>(&waterjetPropulsion), sizeof(waterjetPropulsion));
}

void AmphibiousCraft::deserialize(std::ifstream& in) {
    ReliefVehicle::deserialize(in);
    hullType = FileManager::readString(in);
    in.read(reinterpret_cast<char*>(&draftMeters), sizeof(draftMeters));
    in.read(reinterpret_cast<char*>(&maxWaterSpeedKnots), sizeof(maxWaterSpeedKnots));
    in.read(reinterpret_cast<char*>(&waterjetPropulsion), sizeof(waterjetPropulsion));
}

} // namespace Logistics
} // namespace ReliefOps
