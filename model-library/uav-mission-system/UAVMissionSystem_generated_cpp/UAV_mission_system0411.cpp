// ==========================================
// Auto-generated Coupled Model: UAV_mission_system0411
// ==========================================
#pragma once

#include "FlightMissionControl.cpp"
#include "ControlBusModule.cpp"
#include "Battery.cpp"
#include "Payload.cpp"
#include "Radar.cpp"
#include "AutoPilot.cpp"
#include "oper.cpp"

namespace XLanguage {
    Couple<double> * couple_UAV_mission_system0411_generator() {
        Couple<double> *UAV_mission_system0411 = new Couple<double>("UAV_mission_system0411");

        FlightMissionControl *FC = new FlightMissionControl();
        ControlBusModule *CBM = new ControlBusModule();
        Battery *BAT = new Battery();
        Payload *PL = new Payload();
        Radar *RADAR = new Radar();
        AutoPilot *AP = new AutoPilot();
        oper *OP = new oper();

        // Start connecting components
        UAV_mission_system0411->connect(OP, "evstart", FC, "evstart");
        UAV_mission_system0411->connect(FC, "evInstruct", CBM, "evInstruct");
        UAV_mission_system0411->connect(CBM, "evSensorPrep", RADAR, "evSensorPrep");
        UAV_mission_system0411->connect(CBM, "evActuatorEnable", AP, "evActuatorEnable");
        UAV_mission_system0411->connect(CBM, "evPayloadActivate", PL, "evPayloadActivate");
        UAV_mission_system0411->connect(CBM, "evPowerModev1", AP, "evPowerModev1");
        UAV_mission_system0411->connect(CBM, "evHighPowerOn", RADAR, "evHighPowerOn");
        UAV_mission_system0411->connect(CBM, "evPowerModev2", AP, "evPowerModev2");
        UAV_mission_system0411->connect(CBM, "evPowerModev3", AP, "evPowerModev3");
        UAV_mission_system0411->connect(BAT, "voltage", AP, "voltage");
        UAV_mission_system0411->connect(BAT, "voltage", PL, "voltage");
        UAV_mission_system0411->connect(BAT, "voltage", RADAR, "voltage");
        UAV_mission_system0411->connect(OP, "evstart", BAT, "evstart");

        return UAV_mission_system0411;
    }
} // end of namespace XLanguage
// ==========================================
