// ==========================================
// Auto-generated Atomic Model: Battery
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class Battery : public Atomic<double> {
    enum STATE { idle, discharge0, discharge };
private:
    shared_ptr<bool> _$evstart;
    bool evstart;
    shared_ptr<double> _$voltage;
    double voltage;
    STATE state = idle;
    map<STATE, std::string> stateToString = {
        { idle, "idle" },
        { discharge0, "discharge0" },
        { discharge, "discharge" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    Battery(const string & _variable_name = "Battery") : Atomic<double>(_variable_name), _$evstart(make_shared<bool>()), evstart(), _$voltage(make_shared<double>()), voltage() {
        this->inPort.push_back("evstart");
        this->outPort.push_back("voltage");
    }
    ~Battery() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        port_map["evstart"] = false;
        for (auto it_x_bag:x_bag) {
            if (it_x_bag.port == "evstart") {
                port_map["evstart"] = true;
                _$evstart = static_pointer_cast<bool>(it_x_bag.value);
                evstart = * _$evstart;
            }
        }
        NowState = stateToString[state];
        if(state == idle){
            if(port_map["evstart"]){
                TranCon = "receive(evstart)";
                state = discharge0;
            }
        }
        else if(state == discharge0){
        }
        else if(state == discharge){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case discharge0:{
                state = discharge;
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
        case discharge0:{
            shared_ptr<double> $val_temp0 = make_shared<double>(28.5);
            shared_ptr<void> $result0 = static_pointer_cast<void>($val_temp0);
            y_bag.insert(PortValue("voltage",$result0));
            break;
        }
        default:break; 
        }
    }

    double ta() {
        switch (state) {
        case idle:{
            state_time = 2147483647;
            break;
        }
        case discharge0:{
            state_time = 0;
            break;
        }
        case discharge:{
            state_time = 2147483647;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// ==========================================
