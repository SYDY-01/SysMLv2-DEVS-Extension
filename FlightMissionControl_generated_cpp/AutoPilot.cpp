// ==========================================
// 原子模型头文件: AutoPilot
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// [TODO: 请在此处补充 X 语言底层的其他 #include 和 namespace]

class AutoPilot : public Atomic<double> {
    enum STATE { off, on, tran };
private:
    shared_ptr<bool> _$evActuatorEnable;
    bool evActuatorEnable;
    shared_ptr<bool> _$evPowerModev1;
    bool evPowerModev1;
    shared_ptr<bool> _$evPowerModev2;
    bool evPowerModev2;
    shared_ptr<bool> _$evPowerModev3;
    bool evPowerModev3;
    shared_ptr<double> _$voltage;
    double voltage;
    shared_ptr<double> _$current;
    double current;
    STATE state = off; // 默认初始状态
    map<STATE, std::string> stateToString = {
        { off, "off" },
        { on, "on" },
        { tran, "tran" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    AutoPilot(const string & _variable_name = "AutoPilot") : Atomic<double>(_variable_name), _$evActuatorEnable(make_shared<bool>()), evActuatorEnable(), _$evPowerModev1(make_shared<bool>()), evPowerModev1(), _$evPowerModev2(make_shared<bool>()), evPowerModev2(), _$evPowerModev3(make_shared<bool>()), evPowerModev3(), _$voltage(make_shared<double>()), voltage(), _$current(make_shared<double>()), current() {
        this->inPort.push_back("evActuatorEnable");
        this->inPort.push_back("evPowerModev1");
        this->inPort.push_back("evPowerModev2");
        this->inPort.push_back("evPowerModev3");
        this->inPort.push_back("voltage");
        this->outPort.push_back("current");
    }
    ~AutoPilot() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        port_map["evActuatorEnable"] = false;
        port_map["evPowerModev1"] = false;
        port_map["evPowerModev2"] = false;
        port_map["evPowerModev3"] = false;
        port_map["voltage"] = false;
        for (auto it_x_bag:x_bag) {
            if (it_x_bag.port == "evActuatorEnable") {
                port_map["evActuatorEnable"] = true;
                _$evActuatorEnable = static_pointer_cast<bool>(it_x_bag.value);
                evActuatorEnable = * _$evActuatorEnable;
            }
            else if (it_x_bag.port == "evPowerModev1") {
                port_map["evPowerModev1"] = true;
                _$evPowerModev1 = static_pointer_cast<bool>(it_x_bag.value);
                evPowerModev1 = * _$evPowerModev1;
            }
            else if (it_x_bag.port == "evPowerModev2") {
                port_map["evPowerModev2"] = true;
                _$evPowerModev2 = static_pointer_cast<bool>(it_x_bag.value);
                evPowerModev2 = * _$evPowerModev2;
            }
            else if (it_x_bag.port == "evPowerModev3") {
                port_map["evPowerModev3"] = true;
                _$evPowerModev3 = static_pointer_cast<bool>(it_x_bag.value);
                evPowerModev3 = * _$evPowerModev3;
            }
            else if (it_x_bag.port == "voltage") {
                port_map["voltage"] = true;
                _$voltage = static_pointer_cast<double>(it_x_bag.value);
                voltage = * _$voltage;
            }
        }
        NowState = stateToString[state];
        if(state == off){
            if(port_map["voltage"]){
                TranCon = "receive(voltage)";
                state = off;
            }
            if(port_map["evActuatorEnable"]){
                TranCon = "receive(evActuatorEnable)";
                current = 30;
                state = on;
            }
        }
        else if(state == on){
            if(port_map["evPowerModev1"]){
                TranCon = "receive(evPowerModev1)";
                current = 18;
                state = tran;
            }
            if(port_map["evPowerModev2"]){
                TranCon = "receive(evPowerModev2)";
                current = 30;
                state = tran;
            }
            if(port_map["evPowerModev3"]){
                TranCon = "receive(evPowerModev3)";
                current = 28;
                state = tran;
            }
        }
        else if(state == tran){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case tran:{
                state = on;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
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
        case off:{
            state_time = 2147483647;
            break;
        }
        case on:{
            state_time = 2147483647;
            break;
        }
        case tran:{
            state_time = 0.001;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// [TODO: 请在此处补充原子模型的尾部代码 (如闭合 namespace)]
// ==========================================
