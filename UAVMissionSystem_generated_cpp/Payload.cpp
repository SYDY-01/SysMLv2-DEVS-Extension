// ==========================================
// 自动生成原子模型: Payload
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class Payload : public Atomic<double> {
    enum STATE { off, on, over };
private:
    shared_ptr<bool> _$evPayloadActivate;
    bool evPayloadActivate;
    shared_ptr<double> _$voltage;
    double voltage;
    shared_ptr<double> _$current;
    double current;
    STATE state = off;
    map<STATE, std::string> stateToString = {
        { off, "off" },
        { on, "on" },
        { over, "over" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    Payload(const string & _variable_name = "Payload") : Atomic<double>(_variable_name), _$evPayloadActivate(make_shared<bool>()), evPayloadActivate(), _$voltage(make_shared<double>()), voltage(), _$current(make_shared<double>()), current() {
        this->inPort.push_back("evPayloadActivate");
        this->inPort.push_back("voltage");
        this->outPort.push_back("current");
    }
    ~Payload() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        port_map["evPayloadActivate"] = false;
        port_map["voltage"] = false;
        for (auto it_x_bag:x_bag) {
            if (it_x_bag.port == "evPayloadActivate") {
                port_map["evPayloadActivate"] = true;
                _$evPayloadActivate = static_pointer_cast<bool>(it_x_bag.value);
                evPayloadActivate = * _$evPayloadActivate;
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
            if(port_map["evPayloadActivate"]){
                TranCon = "receive(evPayloadActivate)";
                current = 1;
                state = on;
            }
        }
        else if(state == on){
        }
        else if(state == over){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case on:{
                current = 0;
                state = over;
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
            state_time = 15;
            break;
        }
        case over:{
            state_time = 2147483647;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// ==========================================
