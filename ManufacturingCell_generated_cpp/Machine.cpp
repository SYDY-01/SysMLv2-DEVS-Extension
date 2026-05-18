// ==========================================
// 原子模型头文件: Machine
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// [TODO: 请在此处补充 X 语言底层的其他 #include 和 namespace]

class Machine : public Atomic<double> {
    enum STATE { idle, processing };
private:
    shared_ptr<double> _$part_in;
    double part_in;
    shared_ptr<double> _$product_out;
    double product_out;
    STATE state = idle; // 默认初始状态
    map<STATE, std::string> stateToString = {
        { idle, "idle" },
        { processing, "processing" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    Machine(const string & _variable_name = "Machine") : Atomic<double>(_variable_name), _$part_in(make_shared<double>()), part_in(), _$product_out(make_shared<double>()), product_out() {
        this->inPort.push_back("part_in");
        this->outPort.push_back("product_out");
    }
    ~Machine() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        port_map["part_in"] = false;
        for (auto it_x_bag:x_bag) {
            if (it_x_bag.port == "part_in") {
                port_map["part_in"] = true;
                _$part_in = static_pointer_cast<double>(it_x_bag.value);
                part_in = * _$part_in;
            }
        }
        NowState = stateToString[state];
        if(state == idle){
            if(port_map["part_in"]){
                TranCon = "receive(part_in)";
                state = processing;
            }
        }
        else if(state == processing){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case processing:{
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
        case processing:{
        shared_ptr<double> $val_temp1 = make_shared<double>(1);
        shared_ptr<void> $result1 = static_pointer_cast<void>($val_temp1);
        y_bag.insert(PortValue("product_out",$result1));
        break;
        }
        default:break; 
        }
    }

    double ta() {
        switch (state) {
        case idle:{
            state_time = 1000;
            break;
        }
        case processing:{
            state_time = 7;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// [TODO: 请在此处补充原子模型的尾部代码 (如闭合 namespace)]
// ==========================================
