// ==========================================
// 原子模型头文件: FlightMissionControl
// ==========================================
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
// [TODO: 请在此处补充 X 语言底层的其他 #include 和 namespace]

class FlightMissionControl : public Atomic<double> {
    enum STATE {  };
private:
    shared_ptr<double> _$evstart;
    double evstart;
    shared_ptr<double> _$evInstruct;
    double evInstruct;
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
    FlightMissionControl(const string & _variable_name = "FlightMissionControl") : Atomic<double>(_variable_name), _$evstart(make_shared<double>()), evstart(), _$evInstruct(make_shared<double>()), evInstruct() {
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
                _$evstart = static_pointer_cast<double>(it_x_bag.value);
                evstart = * _$evstart;
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
