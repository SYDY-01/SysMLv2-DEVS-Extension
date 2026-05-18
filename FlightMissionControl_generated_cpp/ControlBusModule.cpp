// ==========================================
// 原子模型头文件: ControlBusModule
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// [TODO: 请在此处补充 X 语言底层的其他 #include 和 namespace]

class ControlBusModule : public Atomic<double> {
    enum STATE {  };
private:
    shared_ptr<double> _$evInstruct;
    double evInstruct;
    shared_ptr<bool> _$evSensorPrep;
    bool evSensorPrep;
    shared_ptr<bool> _$evActuatorEnable;
    bool evActuatorEnable;
    shared_ptr<bool> _$evPayloadActivate;
    bool evPayloadActivate;
    shared_ptr<bool> _$evPowerModev1;
    bool evPowerModev1;
    shared_ptr<bool> _$evHighPowerOn;
    bool evHighPowerOn;
    shared_ptr<bool> _$evPowerModev2;
    bool evPowerModev2;
    shared_ptr<bool> _$evPowerModev3;
    bool evPowerModev3;
    STATE state = undefined; // 默认初始状态
    map<STATE, std::string> stateToString = {
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    ControlBusModule(const string & _variable_name = "ControlBusModule") : Atomic<double>(_variable_name), _$evInstruct(make_shared<double>()), evInstruct(), _$evSensorPrep(make_shared<bool>()), evSensorPrep(), _$evActuatorEnable(make_shared<bool>()), evActuatorEnable(), _$evPayloadActivate(make_shared<bool>()), evPayloadActivate(), _$evPowerModev1(make_shared<bool>()), evPowerModev1(), _$evHighPowerOn(make_shared<bool>()), evHighPowerOn(), _$evPowerModev2(make_shared<bool>()), evPowerModev2(), _$evPowerModev3(make_shared<bool>()), evPowerModev3() {
        this->inPort.push_back("evInstruct");
        this->outPort.push_back("evSensorPrep");
        this->outPort.push_back("evActuatorEnable");
        this->outPort.push_back("evPayloadActivate");
        this->outPort.push_back("evPowerModev1");
        this->outPort.push_back("evHighPowerOn");
        this->outPort.push_back("evPowerModev2");
        this->outPort.push_back("evPowerModev3");
    }
    ~ControlBusModule() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        port_map["evInstruct"] = false;
        for (auto it_x_bag:x_bag) {
            if (it_x_bag.port == "evInstruct") {
                port_map["evInstruct"] = true;
                _$evInstruct = static_pointer_cast<double>(it_x_bag.value);
                evInstruct = * _$evInstruct;
            }
        }
        NowState = stateToString[state];
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
        default:break; 
        }
        NextState = stateToString[state];
    }

    void init(double t, ...)override {
    }

    void output_func(Bag<PortValue> &y_bag){
        switch (state) {
        default:break; 
        }
    }

    double ta() {
        switch (state) {
        default:break; 
        }
        return state_time;
    }
};

// [TODO: 请在此处补充原子模型的尾部代码 (如闭合 namespace)]
// ==========================================
