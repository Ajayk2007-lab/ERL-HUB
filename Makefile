# Makefile for ERL-HUB (Emergency Relief Logistics Hub)
# Academic Project for B.Tech CIC-211 (MAIT, GGSIPU)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude
SRC_DIR = src
VEHICLE_DIR = src/vehicles
TEST_DIR = tests
DATA_DIR = data

SRCS = $(SRC_DIR)/GPSBeacon.cpp \
       $(SRC_DIR)/EmergencyLifeSupportUnit.cpp \
       $(SRC_DIR)/ReliefCargo.cpp \
       $(SRC_DIR)/DistributionDepot.cpp \
       $(SRC_DIR)/TransportVehicle.cpp \
       $(SRC_DIR)/ReliefVehicle.cpp \
       $(SRC_DIR)/Manifest.cpp \
       $(SRC_DIR)/IncidentCommander.cpp \
       $(SRC_DIR)/FileManager.cpp \
       $(SRC_DIR)/LogisticsManager.cpp \
       $(VEHICLE_DIR)/AerialTransport.cpp \
       $(VEHICLE_DIR)/GroundConvoy.cpp \
       $(VEHICLE_DIR)/AmphibiousCraft.cpp \
       $(VEHICLE_DIR)/HybridRescueVehicle.cpp

MAIN_SRC = $(SRC_DIR)/main.cpp
TEST_SRC = $(TEST_DIR)/test_cases.cpp

TARGET = erl_hub.exe
TEST_TARGET = test_suite.exe

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(SRCS) $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(MAIN_SRC)

$(TEST_TARGET): $(SRCS) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(SRCS) $(TEST_SRC)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) *.o
