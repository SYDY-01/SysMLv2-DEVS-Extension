// ==========================================
// 原子模型头文件: TrafficLight
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// [TODO: 请在此处补充 X 语言底层的其他 #include 和 namespace]

class TrafficLight : public Atomic<double> {
    enum STATE { Green, Yellow, Red };
private:
    shared_ptr<double> _$ped_request;
    double ped_request;
    shared_ptr<double> _$light_status;
    double light_status;
    STATE state = Green; // 默认初始状态
    map<STATE, std::string> stateToString = {
        { Green, "Green" },
        { Yellow, "Yellow" },
        { Red, "Red" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    TrafficLight(const string & _variable_name = "TrafficLight") : Atomic<double>(_variable_name), _$ped_request(make_shared<double>()), ped_request(), _$light_status(make_shared<double>()), light_status() {
        this->inPort.push_back("ped_request");
        this->outPort.push_back("light_status");
    }
    ~TrafficLight() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        port_map["ped_request"] = false;
        for (auto it_x_bag:x_bag) {
            if (it_x_bag.port == "ped_request") {
                port_map["ped_request"] = true;
                _$ped_request = static_pointer_cast<double>(it_x_bag.value);
                ped_request = * _$ped_request;
            }
        }
        NowState = stateToString[state];
        if(state == Green){
            if(port_map["ped_request"]){
                TranCon = "receive(ped_request)";
                state = Yellow;
            }
        }
        else if(state == Yellow){
        }
        else if(state == Red){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case Green:{
                state = Yellow;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case Yellow:{
                state = Red;
                TranCon += "timeover:"+to_string(state_time);
                break;
            }
            case Red:{
                state = Green;
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
        case Green:{
        shared_ptr<double> $val_temp1 = make_shared<double>(2);
        shared_ptr<void> $result1 = static_pointer_cast<void>($val_temp1);
        y_bag.insert(PortValue("light_status",$result1));
        break;
        }
        case Yellow:{
        shared_ptr<double> $val_temp2 = make_shared<double>(3);
        shared_ptr<void> $result2 = static_pointer_cast<void>($val_temp2);
        y_bag.insert(PortValue("light_status",$result2));
        break;
        }
        case Red:{
        shared_ptr<double> $val_temp3 = make_shared<double>(1);
        shared_ptr<void> $result3 = static_pointer_cast<void>($val_temp3);
        y_bag.insert(PortValue("light_status",$result3));
        break;
        }
        default:break; 
        }
    }

    double ta() {
        switch (state) {
        case Green:{
            state_time = 60;
            break;
        }
        case Yellow:{
            state_time = 5;
            break;
        }
        case Red:{
            state_time = 30;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// [TODO: 请在此处补充原子模型的尾部代码 (如闭合 namespace)]
// ==========================================
