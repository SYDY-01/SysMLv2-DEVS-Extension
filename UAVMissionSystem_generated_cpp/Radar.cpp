// ==========================================
// 自动生成原子模型: Radar
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class Radar : public Atomic<double> {
    enum STATE { off, yure, gaoyajietong };
private:
    shared_ptr<bool> _$evSensorPrep;
    bool evSensorPrep;
    shared_ptr<bool> _$evHighPowerOn;
    bool evHighPowerOn;
    shared_ptr<double> _$voltage;
    double voltage;
    shared_ptr<double> _$current;
    double current;
    STATE state = off;
    map<STATE, std::string> stateToString = {
        { off, "off" },
        { yure, "yure" },
        { gaoyajietong, "gaoyajietong" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    Radar(const string & _variable_name = "Radar") : Atomic<double>(_variable_name), _$evSensorPrep(make_shared<bool>()), evSensorPrep(), _$evHighPowerOn(make_shared<bool>()), evHighPowerOn(), _$voltage(make_shared<double>()), voltage(), _$current(make_shared<double>()), current() {
        this->inPort.push_back("evSensorPrep");
        this->inPort.push_back("evHighPowerOn");
        this->inPort.push_back("voltage");
        this->outPort.push_back("current");
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
                _$evSensorPrep = static_pointer_cast<bool>(it_x_bag.value);
                evSensorPrep = * _$evSensorPrep;
            }
            else if (it_x_bag.port == "evHighPowerOn") {
                port_map["evHighPowerOn"] = true;
                _$evHighPowerOn = static_pointer_cast<bool>(it_x_bag.value);
                evHighPowerOn = * _$evHighPowerOn;
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
            if(port_map["evSensorPrep"]){
                TranCon = "receive(evSensorPrep)";
                current = 20;
                state = yure;
            }
        }
        else if(state == yure){
            if(port_map["evHighPowerOn"]){
                TranCon = "receive(evHighPowerOn)";
                current = 30;
                state = gaoyajietong;
            }
        }
        else if(state == gaoyajietong){
        }
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
        case off:{
            state_time = 2147483647;
            break;
        }
        case yure:{
            state_time = 2147483647;
            break;
        }
        case gaoyajietong:{
            state_time = 2147483647;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// ==========================================
