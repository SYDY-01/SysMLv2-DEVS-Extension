// ==========================================
// 耦合模型头文件: UAV_mission_system
// ==========================================
#pragma once
// [TODO: 请在此处补充耦合模型底层必需的 #include]

#include "FlightMissionControl.cpp"
#include "ControlBusModule.cpp"
#include "Battery.cpp"
#include "Payload.cpp"
#include "Radar.cpp"
#include "AutoPilot.cpp"
#include "oper.cpp"

namespace XLanguage {
    Couple<double> * couple_UAV_mission_system_generator() {
        Couple<double> *UAV_mission_system = new Couple<double>("UAV_mission_system");

        FlightMissionControl *FC = new FlightMissionControl();
        ControlBusModule *CBM = new ControlBusModule();
        Battery *BAT = new Battery();
        Payload *PL = new Payload();
        Radar *RADAR = new Radar();
        AutoPilot *AP = new AutoPilot();
        oper *OP = new oper();

        // 开始互相连接智能体
        UAV_mission_system->connect(OP, "evstart", FC, "evstart");
        UAV_mission_system->connect(FC, "evInstruct", CBM, "evInstruct");
        UAV_mission_system->connect(CBM, "evSensorPrep", RADAR, "evSensorPrep");
        UAV_mission_system->connect(CBM, "evActuatorEnable", AP, "evActuatorEnable");
        UAV_mission_system->connect(CBM, "evPayloadActivate", PL, "evPayloadActivate");
        UAV_mission_system->connect(CBM, "evPowerModev1", AP, "evPowerModev1");
        UAV_mission_system->connect(CBM, "evHighPowerOn", RADAR, "evHighPowerOn");
        UAV_mission_system->connect(CBM, "evPowerModev2", AP, "evPowerModev2");
        UAV_mission_system->connect(CBM, "evPowerModev3", AP, "evPowerModev3");
        UAV_mission_system->connect(BAT, "voltage", AP, "voltage");
        UAV_mission_system->connect(BAT, "voltage", PL, "voltage");
        UAV_mission_system->connect(BAT, "voltage", RADAR, "voltage");
        UAV_mission_system->connect(OP, "evstart", BAT, "evstart");

        return UAV_mission_system;
    }
} // end of namespace XLanguage
// [TODO: 请在此处补充耦合模型的尾部代码]
// ==========================================
