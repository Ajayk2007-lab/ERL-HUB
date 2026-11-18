#ifndef ERL_HUB_PRIORITY_DISPATCH_QUEUE_H
#define ERL_HUB_PRIORITY_DISPATCH_QUEUE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include "Manifest.h"

namespace ReliefOps {
namespace Logistics {

/**
 * @brief Represents an emergency dispatch request submitted for vehicle assignment.
 */
class DispatchRequest {
private:
    std::string requestId;
    std::string sourceDepotId;
    std::string destinationName;
    std::string corridorName;
    std::string assignedVehicleId;
    int priorityLevel; // 1 (Routine) to 5 (Critical)
    double estimatedWeightKg;
    bool medicalEmergency;
    std::string requestTimestamp;

public:
    DispatchRequest()
        : requestId("REQ-000"), sourceDepotId(""), destinationName(""), corridorName(""),
          assignedVehicleId(""), priorityLevel(1), estimatedWeightKg(0.0), 
          medicalEmergency(false), requestTimestamp("00:00:00") {}

    DispatchRequest(const std::string& reqId, 
                    const std::string& srcDepot, 
                    const std::string& dest, 
                    const std::string& corridor, 
                    const std::string& vehicleId, 
                    int priority, 
                    double weightKg, 
                    bool isMedical, 
                    const std::string& timestamp)
        : requestId(reqId), sourceDepotId(srcDepot), destinationName(dest), 
          corridorName(corridor), assignedVehicleId(vehicleId), priorityLevel(priority), 
          estimatedWeightKg(weightKg), medicalEmergency(isMedical), requestTimestamp(timestamp) {}

    // Getters
    std::string getRequestId() const { return requestId; }
    std::string getSourceDepotId() const { return sourceDepotId; }
    std::string getDestinationName() const { return destinationName; }
    std::string getCorridorName() const { return corridorName; }
    std::string getAssignedVehicleId() const { return assignedVehicleId; }
    int getPriorityLevel() const { return priorityLevel; }
    double getEstimatedWeightKg() const { return estimatedWeightKg; }
    bool isMedicalEmergency() const { return medicalEmergency; }
    std::string getRequestTimestamp() const { return requestTimestamp; }

    void setAssignedVehicleId(const std::string& vId) { assignedVehicleId = vId; }

    // Natural ordering for std::less: Higher priority should be "greater" so top has highest priority
    bool operator<(const DispatchRequest& other) const {
        if (priorityLevel != other.priorityLevel) {
            return priorityLevel < other.priorityLevel; // lower priority considered smaller
        }
        if (medicalEmergency != other.medicalEmergency) {
            return !medicalEmergency && other.medicalEmergency;
        }
        return estimatedWeightKg < other.estimatedWeightKg;
    }

    bool operator>(const DispatchRequest& other) const {
        return other < *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const DispatchRequest& req) {
        os << "[" << req.requestId << "] Priority: " << req.priorityLevel 
           << (req.medicalEmergency ? " [CRITICAL MEDICAL]" : " [GENERAL]")
           << " | Src: " << req.sourceDepotId << " -> Dest: " << req.destinationName
           << " | Weight: " << req.estimatedWeightKg << " kg"
           << " | Assigned: " << (req.assignedVehicleId.empty() ? "None" : req.assignedVehicleId);
        return os;
    }
};

/**
 * @brief Generic Template Priority Dispatch Queue.
 * Demonstrates Unit IV 9.7 Template Classes and generic programming.
 * Implements push, pop, top, empty, size, and display methods.
 */
template <typename T, typename Comparator = std::less<T>>
class PriorityDispatchQueue {
private:
    std::vector<T> elements;
    Comparator comp;

    void bubbleUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            // In max-heap with std::less, if parent < current, swap
            if (comp(elements[parent], elements[index])) {
                std::swap(elements[parent], elements[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void bubbleDown(size_t index) {
        size_t n = elements.size();
        while (true) {
            size_t largest = index;
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;

            if (left < n && comp(elements[largest], elements[left])) {
                largest = left;
            }
            if (right < n && comp(elements[largest], elements[right])) {
                largest = right;
            }
            if (largest != index) {
                std::swap(elements[index], elements[largest]);
                index = largest;
            } else {
                break;
            }
        }
    }

public:
    PriorityDispatchQueue() : comp(Comparator()) {}

    void push(const T& item) {
        elements.push_back(item);
        bubbleUp(elements.size() - 1);
    }

    void pop() {
        if (elements.empty()) {
            throw std::out_of_range("PriorityDispatchQueue::pop() called on empty queue");
        }
        elements[0] = elements.back();
        elements.pop_back();
        if (!elements.empty()) {
            bubbleDown(0);
        }
    }

    const T& top() const {
        if (elements.empty()) {
            throw std::out_of_range("PriorityDispatchQueue::top() called on empty queue");
        }
        return elements[0];
    }

    bool empty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }

    void clear() {
        elements.clear();
    }

    void display() const {
        if (elements.empty()) {
            std::cout << "  [Empty Priority Dispatch Queue]\n";
            return;
        }
        // Display in sorted priority order without mutating original queue
        std::vector<T> copy = elements;
        std::sort(copy.begin(), copy.end(), [this](const T& a, const T& b) {
            return !comp(a, b); // descending order
        });
        for (size_t i = 0; i < copy.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << copy[i] << "\n";
        }
    }

    std::vector<T> toVector() const {
        return elements;
    }
};

} // namespace Logistics
} // namespace ReliefOps

#endif // ERL_HUB_PRIORITY_DISPATCH_QUEUE_H
