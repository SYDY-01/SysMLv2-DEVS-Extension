// ==========================================
// Auto-generated Atomic Model: oper
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class oper : public Atomic<double> {
    enum STATE { idle, off };
private:
    shared_ptr<bool> _$evstart;
    bool evstart;
    STATE state = idle;
    map<STATE, std::string> stateToString = {
        { idle, "idle" },
        { off, "off" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    oper(const string & _variable_name = "oper") : Atomic<double>(_variable_name), _$evstart(make_shared<bool>()), evstart() {
        this->outPort.push_back("evstart");
    }
    ~oper() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        for (auto it_x_bag:x_bag) {
        }
        NowState = stateToString[state];
        if(state == idle){
        }
        else if(state == off){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case idle:{
                state = off;
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
        case idle:{
            shared_ptr<bool> $val_temp0 = make_shared<bool>(true);
            shared_ptr<void> $result0 = static_pointer_cast<void>($val_temp0);
            y_bag.insert(PortValue("evstart",$result0));
            break;
        }
        default:break; 
        }
    }

    double ta() {
        switch (state) {
        case idle:{
            state_time = 0.001;
            break;
        }
        case off:{
            state_time = 2147483647;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// ==========================================
