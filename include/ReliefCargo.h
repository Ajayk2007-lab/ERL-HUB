#ifndef ERL_HUB_RELIEF_CARGO_H
#define ERL_HUB_RELIEF_CARGO_H

#include <string>
#include <iostream>
#include <fstream>

namespace ReliefOps {

// Forward declaration of friend class in Logistics namespace
namespace Logistics {
    class IncidentCommander;
}

namespace Inventory {

/**
 * @brief Inline utility function for calculating total cargo weight.
 * Demonstrates Unit I Inline Functions.
 */
inline double calculateTotalCargoWeight(int quantity, double weightPerUnit) {
    return (quantity > 0 && weightPerUnit > 0.0) ? (quantity * weightPerUnit) : 0.0;
}

/**
 * @brief Represents emergency relief supplies in the ERL-HUB system.
 * Demonstrates Unit I Namespaces, Encapsulation, Operator Overloading, and Friend Class.
 */
class ReliefCargo {
    // IncidentCommander declared as friend class for privileged emergency override
    friend class ReliefOps::Logistics::IncidentCommander;

private:
    std::string cargoId;
    std::string cargoName;
    std::string category;
    int availableQuantity;
    std::string unitOfMeasure;
    double weightPerUnitKg;
    int emergencyPriorityLevel; // 1 (Routine) to 5 (Critical Emergency Life-Saving)
    bool medicalFlag;

public:
    // Constructors
    ReliefCargo();
    ReliefCargo(const std::string& id, 
                const std::string& name, 
                const std::string& cat,
                int quantity, 
                const std::string& unit, 
                double weightPerUnit,
                int priority, 
                bool isMed);

    // Getters (const-correctness)
    std::string getCargoId() const;
    std::string getCargoName() const;
    std::string getCategory() const;
    int getAvailableQuantity() const;
    std::string getUnitOfMeasure() const;
    double getWeightPerUnitKg() const;
    int getEmergencyPriorityLevel() const;
    bool isMedical() const;
    double getTotalWeightKg() const;

    // Setters with validation
    void setAvailableQuantity(int qty);
    void updateQuantity(int delta); // delta can be positive (restock) or negative (deduct)
    void setEmergencyPriorityLevel(int priority);
    void setWeightPerUnitKg(double weight);

    // Query helper methods
    bool isUrgentMedicalSupply() const;
    bool hasStock(int requestedQty) const;

    // Binary serialization / deserialization
    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);

    // Operator Overloading
    // Unit II Operator Overloading: operator== to compare identities and detect duplicates
    bool operator==(const ReliefCargo& other) const;
    bool operator!=(const ReliefCargo& other) const;

    // Unit II Operator Overloading: stream insertion & extraction
    friend std::ostream& operator<<(std::ostream& os, const ReliefCargo& cargo);
    friend std::istream& operator>>(std::istream& is, ReliefCargo& cargo);
};

/**
 * @brief Functor for sorting medical supplies by urgency.
 * Demonstrates Unit IV Custom Functors.
 * Higher priority (5 down to 1) appears first.
 * If priorities match, medical supplies precede non-medical supplies.
 */
struct MedicalPriorityComparator {
    bool operator()(const ReliefCargo& a, const ReliefCargo& b) const {
        if (a.getEmergencyPriorityLevel() != b.getEmergencyPriorityLevel()) {
            return a.getEmergencyPriorityLevel() > b.getEmergencyPriorityLevel(); // Higher first
        }
        if (a.isMedical() != b.isMedical()) {
            return a.isMedical() && !b.isMedical(); // Medical first
        }
        return a.getCargoName() < b.getCargoName();
    }
};

} // namespace Inventory
} // namespace ReliefOps

#endif // ERL_HUB_RELIEF_CARGO_H
