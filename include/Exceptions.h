#ifndef ERL_HUB_EXCEPTIONS_H
#define ERL_HUB_EXCEPTIONS_H

#include <string>
#include <exception>

namespace ReliefOps {
namespace Exceptions {

/**
 * @brief Base exception class for all ERL-HUB operations.
 */
class ERLHubException : public std::exception {
protected:
    std::string message;
public:
    explicit ERLHubException(const std::string& msg) : message(msg) {}
    virtual ~ERLHubException() throw() {}
    virtual const char* what() const throw() {
        return message.c_str();
    }
};

/**
 * @brief Thrown when cargo payload exceeds vehicle maximum capacity.
 */
class CargoOverweightException : public ERLHubException {
private:
    double attemptedWeight;
    double maxCapacity;
public:
    CargoOverweightException(double attempted, double maxCap, const std::string& details = "")
        : ERLHubException("CargoOverweightException: Attempted payload " + std::to_string(attempted) + 
                          " kg exceeds vehicle maximum capacity of " + std::to_string(maxCap) + 
                          " kg. " + details),
          attemptedWeight(attempted), maxCapacity(maxCap) {}
    
    double getAttemptedWeight() const { return attemptedWeight; }
    double getMaxCapacity() const { return maxCapacity; }
};

/**
 * @brief Thrown when vehicle fuel is insufficient for the planned mission route.
 */
class FuelDepletionException : public ERLHubException {
private:
    double currentFuel;
    double requiredFuel;
public:
    FuelDepletionException(double current, double required, const std::string& details = "")
        : ERLHubException("FuelDepletionException: Current fuel (" + std::to_string(current) + 
                          "%) is below required minimum fuel (" + std::to_string(required) + 
                          "%) for mission execution. " + details),
          currentFuel(current), requiredFuel(required) {}

    double getCurrentFuel() const { return currentFuel; }
    double getRequiredFuel() const { return requiredFuel; }
};

/**
 * @brief Thrown when a transportation corridor is blocked/damaged by earthquake debris.
 */
class DamagedCorridorException : public ERLHubException {
private:
    std::string corridorName;
    int damageSeverity; // 1-10 scale
public:
    DamagedCorridorException(const std::string& corridor, int severity, const std::string& details = "")
        : ERLHubException("DamagedCorridorException: Transportation corridor '" + corridor + 
                          "' is impassable due to earthquake damage (Severity Level: " + 
                          std::to_string(severity) + "/10). " + details),
          corridorName(corridor), damageSeverity(severity) {}

    std::string getCorridorName() const { return corridorName; }
    int getDamageSeverity() const { return damageSeverity; }
};

/**
 * @brief Thrown when requested cargo quantity exceeds available stock at a depot.
 */
class InsufficientStockException : public ERLHubException {
private:
    std::string cargoId;
    int requested;
    int available;
public:
    InsufficientStockException(const std::string& id, int req, int avail)
        : ERLHubException("InsufficientStockException: Cargo '" + id + "' has only " + 
                          std::to_string(avail) + " units available, but " + 
                          std::to_string(req) + " units were requested."),
          cargoId(id), requested(req), available(avail) {}
};

/**
 * @brief Thrown when an entity ID already exists in the registry.
 */
class DuplicateIdException : public ERLHubException {
public:
    explicit DuplicateIdException(const std::string& msg)
        : ERLHubException("DuplicateIdException: " + msg) {}
};

/**
 * @brief Thrown when an entity ID cannot be found in the registry.
 */
class EntityNotFoundException : public ERLHubException {
public:
    explicit EntityNotFoundException(const std::string& msg)
        : ERLHubException("EntityNotFoundException: " + msg) {}
};

/**
 * @brief Thrown when depot storage capacity would be exceeded.
 */
class CapacityOverflowException : public ERLHubException {
public:
    explicit CapacityOverflowException(const std::string& msg)
        : ERLHubException("CapacityOverflowException: " + msg) {}
};

} // namespace Exceptions
} // namespace ReliefOps

#endif // ERL_HUB_EXCEPTIONS_H
