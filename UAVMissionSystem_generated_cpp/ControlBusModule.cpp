// ==========================================
// 自动生成原子模型: ControlBusModule
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class ControlBusModule : public Atomic<double> {
    enum STATE { idle, tran };
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
    STATE state = idle;
    map<STATE, std::string> stateToString = {
        { idle, "idle" },
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
        if(state == idle){
            if(port_map["evInstruct"]){
                TranCon = "receive(evInstruct)";
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
                state = idle;
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
        case tran:{
            if (evInstruct == 10000) {
                shared_ptr<bool> $val_temp0 = make_shared<bool>(true);
                shared_ptr<void> $result0 = static_pointer_cast<void>($val_temp0);
                y_bag.insert(PortValue("evSensorPrep",$result0));
                shared_ptr<bool> $val_temp1 = make_shared<bool>(true);
                shared_ptr<void> $result1 = static_pointer_cast<void>($val_temp1);
                y_bag.insert(PortValue("evActuatorEnable",$result1));
            }
            else if (evInstruct == 11000) {
                shared_ptr<bool> $val_temp2 = make_shared<bool>(true);
                shared_ptr<void> $result2 = static_pointer_cast<void>($val_temp2);
                y_bag.insert(PortValue("evPayloadActivate",$result2));
            }
            else if (evInstruct == 10101) {
                shared_ptr<bool> $val_temp3 = make_shared<bool>(true);
                shared_ptr<void> $result3 = static_pointer_cast<void>($val_temp3);
                y_bag.insert(PortValue("evPowerModev1",$result3));
                shared_ptr<bool> $val_temp4 = make_shared<bool>(true);
                shared_ptr<void> $result4 = static_pointer_cast<void>($val_temp4);
                y_bag.insert(PortValue("evHighPowerOn",$result4));
            }
            else if (evInstruct == 10010) {
                shared_ptr<bool> $val_temp5 = make_shared<bool>(true);
                shared_ptr<void> $result5 = static_pointer_cast<void>($val_temp5);
                y_bag.insert(PortValue("evPowerModev2",$result5));
            }
            else if (evInstruct == 10001) {
                shared_ptr<bool> $val_temp6 = make_shared<bool>(true);
                shared_ptr<void> $result6 = static_pointer_cast<void>($val_temp6);
                y_bag.insert(PortValue("evPowerModev1",$result6));
            }
            else {
                shared_ptr<bool> $val_temp7 = make_shared<bool>(true);
                shared_ptr<void> $result7 = static_pointer_cast<void>($val_temp7);
                y_bag.insert(PortValue("evPowerModev3",$result7));
            }
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
        case tran:{
            state_time = 0.001;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// ==========================================
