// ==========================================
// 原子模型头文件: Radar
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// [TODO: 请在此处补充 X 语言底层的其他 #include 和 namespace]

class Radar : public Atomic<double> {
    enum STATE {  };
private:
    shared_ptr<double> _$evSensorPrep;
    double evSensorPrep;
    shared_ptr<double> _$evHighPowerOn;
    double evHighPowerOn;
    shared_ptr<double> _$voltage;
    double voltage;
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
    Radar(const string & _variable_name = "Radar") : Atomic<double>(_variable_name), _$evSensorPrep(make_shared<double>()), evSensorPrep(), _$evHighPowerOn(make_shared<double>()), evHighPowerOn(), _$voltage(make_shared<double>()), voltage() {
        this->inPort.push_back("evSensorPrep");
        this->inPort.push_back("evHighPowerOn");
        this->inPort.push_back("voltage");
    }
    ~Radar() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        port_map["evSensorPrep"] = false;
        port_map["evHighPowerOn"] = false;
        port_map["voltage"] = false;
        for (auto it_x_bag:x_bag) {
            if (it_x_bag.port == "evSensorPrep") {
                port_map["evSensorPrep"] = true;
                _$evSensorPrep = static_pointer_cast<double>(it_x_bag.value);
                evSensorPrep = * _$evSensorPrep;
            }
            else if (it_x_bag.port == "evHighPowerOn") {
                port_map["evHighPowerOn"] = true;
                _$evHighPowerOn = static_pointer_cast<double>(it_x_bag.value);
                evHighPowerOn = * _$evHighPowerOn;
            }
            else if (it_x_bag.port == "voltage") {
                port_map["voltage"] = true;
                _$voltage = static_pointer_cast<double>(it_x_bag.value);
                voltage = * _$voltage;
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
