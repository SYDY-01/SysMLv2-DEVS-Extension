// ==========================================
// 自动生成原子模型: FlightMissionControl
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

class FlightMissionControl : public Atomic<double> {
    enum STATE { idle, takeoff_climb0, takeoff_climb, fadongjizhunbeidianhuo, motor_ignition, cruise_turn, descent, pingfei2, ercijianggao, pingfei3, landing_approach, finish };
private:
    shared_ptr<bool> _$evstart;
    bool evstart;
    shared_ptr<double> _$evInstruct;
    double evInstruct;
    STATE state = idle;
    map<STATE, std::string> stateToString = {
        { idle, "idle" },
        { takeoff_climb0, "takeoff_climb0" },
        { takeoff_climb, "takeoff_climb" },
        { fadongjizhunbeidianhuo, "fadongjizhunbeidianhuo" },
        { motor_ignition, "motor_ignition" },
        { cruise_turn, "cruise_turn" },
        { descent, "descent" },
        { pingfei2, "pingfei2" },
        { ercijianggao, "ercijianggao" },
        { pingfei3, "pingfei3" },
        { landing_approach, "landing_approach" },
        { finish, "finish" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    FlightMissionControl(const string & _variable_name = "FlightMissionControl") : Atomic<double>(_variable_name), _$evstart(make_shared<bool>()), evstart(), _$evInstruct(make_shared<double>()), evInstruct() {
        this->inPort.push_back("evstart");
        this->outPort.push_back("evInstruct");
    }
    ~FlightMissionControl() {}

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
                state = takeoff_climb0;
            }
        }
        else if(state == takeoff_climb0){
        }
        else if(state == takeoff_climb){
        }
        else if(state == fadongjizhunbeidianhuo){
        }
        else if(state == motor_ignition){
        }
        else if(state == cruise_turn){
        }
        else if(state == descent){
        }
        else if(state == pingfei2){
        }
        else if(state == ercijianggao){
        }
        else if(state == pingfei3){
        }
        else if(state == landing_approach){
        }
        else if(state == finish){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case takeoff_climb0:{
                state = takeoff_climb;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case takeoff_climb:{
                state = fadongjizhunbeidianhuo;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case fadongjizhunbeidianhuo:{
                state = motor_ignition;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case motor_ignition:{
                state = cruise_turn;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case cruise_turn:{
                state = descent;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case descent:{
                state = pingfei2;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case pingfei2:{
                state = ercijianggao;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case ercijianggao:{
                state = pingfei3;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case pingfei3:{
                state = landing_approach;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case landing_approach:{
                state = finish;
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
        case takeoff_climb0:{
            shared_ptr<double> $val_temp0 = make_shared<double>(10000);
            shared_ptr<void> $result0 = static_pointer_cast<void>($val_temp0);
            y_bag.insert(PortValue("evInstruct",$result0));
            break;
        }
        case takeoff_climb:{
            shared_ptr<double> $val_temp1 = make_shared<double>(11000);
            shared_ptr<void> $result1 = static_pointer_cast<void>($val_temp1);
            y_bag.insert(PortValue("evInstruct",$result1));
            break;
        }
        case descent:{
            shared_ptr<double> $val_temp2 = make_shared<double>(10101);
            shared_ptr<void> $result2 = static_pointer_cast<void>($val_temp2);
            y_bag.insert(PortValue("evInstruct",$result2));
            break;
        }
        case pingfei2:{
            shared_ptr<double> $val_temp3 = make_shared<double>(10010);
            shared_ptr<void> $result3 = static_pointer_cast<void>($val_temp3);
            y_bag.insert(PortValue("evInstruct",$result3));
            break;
        }
        case ercijianggao:{
            shared_ptr<double> $val_temp4 = make_shared<double>(10001);
            shared_ptr<void> $result4 = static_pointer_cast<void>($val_temp4);
            y_bag.insert(PortValue("evInstruct",$result4));
            break;
        }
        case pingfei3:{
            shared_ptr<double> $val_temp5 = make_shared<double>(10011);
            shared_ptr<void> $result5 = static_pointer_cast<void>($val_temp5);
            y_bag.insert(PortValue("evInstruct",$result5));
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
        case takeoff_climb0:{
            state_time = 0.001;
            break;
        }
        case takeoff_climb:{
            state_time = 100;
            break;
        }
        case fadongjizhunbeidianhuo:{
            state_time = 50;
            break;
        }
        case motor_ignition:{
            state_time = 250;
            break;
        }
        case cruise_turn:{
            state_time = 6950;
            break;
        }
        case descent:{
            state_time = 250;
            break;
        }
        case pingfei2:{
            state_time = 7000;
            break;
        }
        case ercijianggao:{
            state_time = 100;
            break;
        }
        case pingfei3:{
            state_time = 8300;
            break;
        }
        case landing_approach:{
            state_time = 5.5;
            break;
        }
        case finish:{
            state_time = 2147483647;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// ==========================================
