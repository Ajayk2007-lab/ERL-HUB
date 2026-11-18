#include "../../include/HybridRescueVehicle.h"
#include "../../include/FileManager.h"
#include <iomanip>

namespace ReliefOps {
namespace Logistics {

// Default constructor resolves diamond by directly initializing virtual base ReliefVehicle
HybridRescueVehicle::HybridRescueVehicle()
    : ReliefVehicle("HYB-001", "Hybrid Mobile Rescue Unit Alpha", 2500.0, 100.0, "Operational", "Forward Disaster Base Chamoli", "GPS-HYB-01", "ELSU-HYB-01"),
      AerialTransport("HYB-001", "Hybrid Mobile Rescue Unit Alpha", 2500.0, 100.0, "Operational", "Forward Disaster Base Chamoli", 5000.0, "Tilt-Rotor Hybrid VTOL", 55.0),
      AmphibiousCraft("HYB-001", "Hybrid Mobile Rescue Unit Alpha", 2500.0, 100.0, "Operational", "Forward Disaster Base Chamoli", "Carbon-Kevlar Hydrofoil Hull", 0.50, 35.0, true),
      currentOperationalMode("Amphibious Transition"), auxiliaryBatteryReserve(98.0), rapidDeployTriageUnit(true) {}

// Parameterized constructor explicitly initializing the virtual base class subobject
HybridRescueVehicle::HybridRescueVehicle(const std::string& id, 
                                         const std::string& name, 
                                         double maxPayloadKg, 
                                         double fuel, 
                                         const std::string& status,
                                         const std::string& location,
                                         double maxAlt,
                                         const std::string& rotor,
                                         double maxWind,
                                         const std::string& hull,
                                         double draft,
                                         double speedKnots,
                                         const std::string& mode,
                                         double battery,
                                         bool rapidDeploy)
    : ReliefVehicle(id, name, maxPayloadKg, fuel, status, location, "GPS-" + id, "ELSU-" + id),
      AerialTransport(id, name, maxPayloadKg, fuel, status, location, maxAlt, rotor, maxWind),
      AmphibiousCraft(id, name, maxPayloadKg, fuel, status, location, hull, draft, speedKnots, true),
      currentOperationalMode(mode), auxiliaryBatteryReserve(battery), rapidDeployTriageUnit(rapidDeploy) {}

HybridRescueVehicle::~HybridRescueVehicle() {}

std::string HybridRescueVehicle::getCurrentOperationalMode() const {
    return currentOperationalMode;
}

double HybridRescueVehicle::getAuxiliaryBatteryReserve() const {
    return auxiliaryBatteryReserve;
}

bool HybridRescueVehicle::isRapidDeployTriageUnit() const {
    return rapidDeployTriageUnit;
}

void HybridRescueVehicle::setOperationalMode(const std::string& mode) {
    currentOperationalMode = mode;
}

// Unit III Polymorphic implementation: Multi-Modal Route Evaluation
bool HybridRescueVehicle::evaluateRoute(const std::string& corridorName, 
                                        double distanceKm, 
                                        int terrainSeverity) const {
    std::cout << "  [Multi-Modal Route Evaluation] Vehicle: " << vehicleName << " (" << vehicleId << ")\n";
    std::cout << "  -> Hybrid Corridor: " << corridorName << " | Distance: " << distanceKm 
              << " km | Hazard Severity: " << terrainSeverity << "/10\n";
    std::cout << "  -> Active Mode: " << currentOperationalMode << " | Battery Reserve: " << auxiliaryBatteryReserve << "%\n";

    // Extreme catastrophe check (severity == 10)
    if (terrainSeverity >= 10) {
        std::cout << "  [!] Extreme Catastrophe: Both airspace and water channels compromised by seismic volcanic debris.\n";
        return false;
    }

    // Adaptive capability: can dynamically switch between air and water modes
    if (terrainSeverity >= 7) {
        std::cout << "  -> Water debris high: Switching dynamically to Tilt-Rotor Aerial VTOL flight corridor.\n";
    } else {
        std::cout << "  -> Water channels clear: Engaging high-speed Amphibious Hydrofoil cruising mode.\n";
    }

    // Hybrid efficiency: 0.20% fuel per km assisted by auxiliary battery
    double fuelNeeded = distanceKm * 0.20;
    if (!checkFuel(fuelNeeded)) {
        if (auxiliaryBatteryReserve > 30.0) {
            std::cout << "  -> Fuel low, but auxiliary electric emergency drive engaged. Route feasible.\n";
        } else {
            std::cout << "  [!] Insufficient combined energy for hybrid transit.\n";
            return false;
        }
    }

    std::cout << "  [✓] Hybrid Multi-Modal Corridor FEASIBLE: Autonomous mode adaptation confirmed.\n";
    return true;
}

bool HybridRescueVehicle::calculateOperationalFeasibility(double payloadWeightKg, double distanceKm) const {
    if (!isOperational()) return false;
    if (!checkCapacity(payloadWeightKg)) return false;
    double fuelNeeded = distanceKm * 0.20;
    return checkFuel(fuelNeeded) || (auxiliaryBatteryReserve >= 40.0);
}

void HybridRescueVehicle::displayDetails() const {
    // Virtual inheritance ensures a single unique call to ReliefVehicle::displayDetails
    ReliefVehicle::displayDetails();
    std::cout << "   [Hybrid Rescue Vehicle Specs - Diamond Inheritance Resolved]\n";
    std::cout << "   - Aerial Subsystem: Max Altitude " << maxAltitudeMeters 
              << " m | " << rotorConfiguration << " (Wind Tol: " << maxWindToleranceKnots << " kt)\n";
    std::cout << "   - Marine Subsystem: Hull " << hullType 
              << " | Draft: " << draftMeters << " m | Speed: " << maxWaterSpeedKnots << " kt\n";
    std::cout << "   - Hybrid Subsystem: Active Mode: " << currentOperationalMode 
              << " | Battery Reserve: " << auxiliaryBatteryReserve << "%"
              << " | Rapid Triage: " << (rapidDeployTriageUnit ? "ENABLED" : "DISABLED") << "\n";
}

std::string HybridRescueVehicle::getVehicleType() const {
    return "Hybrid Rescue Vehicle";
}

void HybridRescueVehicle::serialize(std::ofstream& out) const {
    ReliefVehicle::serialize(out);
    out.write(reinterpret_cast<const char*>(&maxAltitudeMeters), sizeof(maxAltitudeMeters));
    FileManager::writeString(out, rotorConfiguration);
    out.write(reinterpret_cast<const char*>(&maxWindToleranceKnots), sizeof(maxWindToleranceKnots));
    FileManager::writeString(out, hullType);
    out.write(reinterpret_cast<const char*>(&draftMeters), sizeof(draftMeters));
    out.write(reinterpret_cast<const char*>(&maxWaterSpeedKnots), sizeof(maxWaterSpeedKnots));
    out.write(reinterpret_cast<const char*>(&waterjetPropulsion), sizeof(waterjetPropulsion));
    FileManager::writeString(out, currentOperationalMode);
    out.write(reinterpret_cast<const char*>(&auxiliaryBatteryReserve), sizeof(auxiliaryBatteryReserve));
    out.write(reinterpret_cast<const char*>(&rapidDeployTriageUnit), sizeof(rapidDeployTriageUnit));
}

void HybridRescueVehicle::deserialize(std::ifstream& in) {
    ReliefVehicle::deserialize(in);
    in.read(reinterpret_cast<char*>(&maxAltitudeMeters), sizeof(maxAltitudeMeters));
    rotorConfiguration = FileManager::readString(in);
    in.read(reinterpret_cast<char*>(&maxWindToleranceKnots), sizeof(maxWindToleranceKnots));
    hullType = FileManager::readString(in);
    in.read(reinterpret_cast<char*>(&draftMeters), sizeof(draftMeters));
    in.read(reinterpret_cast<char*>(&maxWaterSpeedKnots), sizeof(maxWaterSpeedKnots));
    in.read(reinterpret_cast<char*>(&waterjetPropulsion), sizeof(waterjetPropulsion));
    currentOperationalMode = FileManager::readString(in);
    in.read(reinterpret_cast<char*>(&auxiliaryBatteryReserve), sizeof(auxiliaryBatteryReserve));
    in.read(reinterpret_cast<char*>(&rapidDeployTriageUnit), sizeof(rapidDeployTriageUnit));
}

} // namespace Logistics
} // namespace ReliefOps
