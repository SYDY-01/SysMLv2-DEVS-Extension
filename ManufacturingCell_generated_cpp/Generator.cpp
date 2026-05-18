// ==========================================
// 原子模型头文件: Generator
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// [TODO: 请在此处补充 X 语言底层的其他 #include 和 namespace]

class Generator : public Atomic<double> {
    enum STATE { generating };
private:
    shared_ptr<double> _$part_out;
    double part_out;
    STATE state = generating; // 默认初始状态
    map<STATE, std::string> stateToString = {
        { generating, "generating" }
    };
    string NowState;
    string NextState;
    string TranCon;
    string StateAll;
    double state_time = 0;
    vector<bool> event;
    bool first_save = true;

public:
    Generator(const string & _variable_name = "Generator") : Atomic<double>(_variable_name), _$part_out(make_shared<double>()), part_out() {
        this->outPort.push_back("part_out");
    }
    ~Generator() {}

    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{
        std::map<std::string,bool> port_map{};
        for (auto it_x_bag:x_bag) {
        }
        NowState = stateToString[state];
        if(state == generating){
        }
        NextState = stateToString[state];
        out_variable_val(time_last + e,first_save);      if (first_save) {
            first_save = false;
        }
    }

    void delta_int() override{
        NowState = stateToString[state];
        switch (state) {
            case generating:{
                state = generating;
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
        case generating:{
        shared_ptr<double> $val_temp1 = make_shared<double>(1);
        shared_ptr<void> $result1 = static_pointer_cast<void>($val_temp1);
        y_bag.insert(PortValue("part_out",$result1));
        break;
        }
        default:break; 
        }
    }

    double ta() {
        switch (state) {
        case generating:{
            state_time = 10;
            break;
        }
        default:break; 
        }
        return state_time;
    }
};

// [TODO: 请在此处补充原子模型的尾部代码 (如闭合 namespace)]
// ==========================================
