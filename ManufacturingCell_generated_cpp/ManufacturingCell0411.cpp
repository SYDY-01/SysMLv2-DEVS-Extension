// ==========================================
// 耦合模型头文件: ManufacturingCell0411
// ==========================================
#pragma once
// [TODO: 请在此处补充耦合模型底层必需的 #include]

#include "Generator.cpp"
#include "Machine.cpp"

namespace XLanguage {
    Couple<double> * couple_ManufacturingCell0411_generator() {
        Couple<double> *ManufacturingCell0411 = new Couple<double>("ManufacturingCell0411");

        Generator *gen = new Generator();
        Machine *mach = new Machine();

        // 开始互相连接智能体
        ManufacturingCell0411->connect(gen, "part_out", mach, "part_in");

        return ManufacturingCell0411;
    }
} // end of namespace XLanguage
// [TODO: 请在此处补充耦合模型的尾部代码]
// ==========================================
