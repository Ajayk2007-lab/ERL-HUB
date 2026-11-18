#ifndef ERL_HUB_MANIFEST_H
#define ERL_HUB_MANIFEST_H

#include <string>
#include <iostream>
#include <fstream>
#include "ReliefCargo.h"

namespace ReliefOps {
namespace Inventory {

/**
 * @brief Manages dispatch cargo records using dynamically allocated memory.
 * Demonstrates Unit II Dynamic Memory Allocation, Dynamic Constructors, and the Rule of Three.
 */
class Manifest {
private:
    std::string manifestId;
    std::string destinationDepot;
    std::string assignedVehicleId;
    int capacity;
    int count;
    ReliefCargo* cargoRecords; // Dynamically allocated array of ReliefCargo

    // Internal helper to resize dynamic array when capacity is reached
    void resize(int newCapacity);

public:
    // Dynamic Parameterized Constructor
    Manifest(const std::string& id = "MNF-DEFAULT", 
             const std::string& destination = "Unassigned", 
             const std::string& vehicleId = "Unassigned", 
             int initialCapacity = 5);

    // Deep Copy Constructor (Rule of Three #1)
    Manifest(const Manifest& other);

    // Copy Assignment Operator (Rule of Three #2)
    Manifest& operator=(const Manifest& other);

    // Explicit Destructor (Rule of Three #3)
    ~Manifest();

    // Operations
    void addCargo(const ReliefCargo& cargo);
    bool removeCargo(const std::string& cargoId);
    void clear();

    // Getters (const-correctness)
    std::string getManifestId() const;
    std::string getDestinationDepot() const;
    std::string getAssignedVehicleId() const;
    int getCount() const;
    int getCapacity() const;
    double getTotalWeightKg() const;
    const ReliefCargo* getCargoRecords() const;
    const ReliefCargo& getCargoAt(int index) const;

    // Setters
    void setDestinationDepot(const std::string& destination);
    void setAssignedVehicleId(const std::string& vehicleId);

    // Unit II Operator Overloading
    bool operator==(const Manifest& other) const;
    bool operator!=(const Manifest& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Manifest& manifest);
    friend std::istream& operator>>(std::istream& is, Manifest& manifest);

    // Binary serialization
    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);
};

} // namespace Inventory
} // namespace ReliefOps

#endif // ERL_HUB_MANIFEST_H
