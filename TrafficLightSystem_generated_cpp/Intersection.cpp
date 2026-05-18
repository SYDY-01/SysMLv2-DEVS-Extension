// ==========================================
// 耦合模型头文件: Intersection
// ==========================================
#pragma once
// [TODO: 请在此处补充耦合模型底层必需的 #include]

#include "PedestrianGen.cpp"
#include "TrafficLight.cpp"

namespace XLanguage {
    Couple<double> * couple_Intersection_generator() {
        Couple<double> *Intersection = new Couple<double>("Intersection");

        PedestrianGen *pGen = new PedestrianGen();
        TrafficLight *myLight = new TrafficLight();

        // 开始互相连接智能体
        Intersection->connect(pGen, "change_req", myLight, "ped_request");
6
        return Intersection;
    }
} // end of namespace XLanguage
// [TODO: 请在此处补充耦合模型的尾部代码]
// ==========================================
