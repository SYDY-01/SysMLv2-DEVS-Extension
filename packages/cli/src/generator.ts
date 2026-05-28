import type { SysMLModel } from 'sys-m-lv-2-extended-language';
import { 
    isDevsAtomicDef, isPortDef, isStateDef, isDevsFunction, 
    isDevsTransition, isDevsTimeAdvance, 
    isPackage, isDevsCoupledDef, isPartUsage, isConnectDef 
} from 'sys-m-lv-2-extended-language';
import * as fs from 'fs';
import * as path from 'path';

// ==========================================
// Core 1: Expression Parsing Engine
// ==========================================
/**
 * Parses an expression AST node and generates the corresponding C++ expression string
 * @param expr Expression AST node to parse
 * @returns Generated C++ expression string
 */
function generateExpr(expr: any): string {
    if (!expr) return "";
    let left = generatePrim(expr.left);
    if (expr.operator && expr.right) {
        let right = generatePrim(expr.right);
        return `${left} ${expr.operator} ${right}`;
    }
    return left;
}

// ==========================================
// Core 2: Primitive Value Conversion Engine
// ==========================================
/**
 * Converts primitive value AST nodes to their string representation
 * Handles numbers, booleans, and variable references
 * @param prim Primitive value AST node
 * @returns String representation of the primitive value
 */
function generatePrim(prim: any): string {
    if (!prim) return "";
    if (prim.$type === 'NumberLiteral') return prim.value.toString();
    if (prim.$type === 'BooleanLiteral') return prim.value; // Directly returns 'true' or 'false'
    if (prim.$type === 'VariableRef') return prim.ref?.ref?.name || "";
    return "";
}

// ==========================================
// Core 3: Recursive Control Flow Generation Engine (Key Breakthrough: if-else Nesting)
// ==========================================
/**
 * Recursively generates C++ statements from AST nodes
 * Supports assignments and nested if-else statements with different generation modes
 * @param stmts Array of statement AST nodes
 * @param mode Generation mode: 'ext' (external transition), 'int' (internal transition), 'lambda' (output function)
 * @param portTypes Map of port names to their data types
 * @param tempRef Reference object for temporary variable counter
 * @param indent Current indentation level string
 * @returns Generated C++ statements block
 */
function generateStatements(stmts: any[], mode: 'ext' | 'lambda'| 'int', portTypes: Record<string, string>, tempRef: {counter: number}, indent: string): string {
    let code = "";
    for (const stmt of stmts) {
        if (stmt.$type === 'DevsAssignment') {
            const target = stmt.target?.ref?.name;
            const val = generateExpr(stmt.value);
            
            if (mode === 'ext' || mode === 'int') {
                // Regular variable assignment in internal/external transitions
                code += `${indent}${target} = ${val};\n`;
            } else if (mode === 'lambda') {
                // Complex memory allocation and port delivery in Lambda output functions
                const pType = portTypes[target] || "double"; // Smart port type inference
                const c = tempRef.counter++;
                code += `${indent}shared_ptr<${pType}> $val_temp${c} = make_shared<${pType}>(${val});\n`;
                code += `${indent}shared_ptr<void> $result${c} = static_pointer_cast<void>($val_temp${c});\n`;
                code += `${indent}y_bag.insert(PortValue("${target}",$result${c}));\n`;
            }
        } 
        else if (stmt.$type === 'DevsIfStatement') {
            // Generate if condition
            const cond = generateExpr(stmt.condition);
            code += `${indent}if (${cond}) {\n`;
            code += generateStatements(stmt.thenBlock || [], mode, portTypes, tempRef, indent + "    ");
            code += `${indent}}\n`;
            
            // Recursively generate else if or else blocks
            if (stmt.elseIfNode) {
                code += `${indent}else ${generateStatements([stmt.elseIfNode], mode, portTypes, tempRef, indent).trimStart()}`;
            } else if (stmt.elseBlock && stmt.elseBlock.length > 0) {
                code += `${indent}else {\n`;
                code += generateStatements(stmt.elseBlock, mode, portTypes, tempRef, indent + "    ");
                code += `${indent}}\n`;
            }
        }
    }
    return code;
}

// ==========================================
// Core 4: Main Code Generator
// ==========================================
/**
 * Main entry point for DEVS model to C++ code generation
 * Processes both atomic and coupled DEVS models and generates corresponding C++ files
 * @param model Root DEVS model AST
 * @param filePath Path to the source model file
 * @param destination Output directory for generated C++ files (defaults to same directory as source)
 * @returns Status message with number of generated files and output path
 */
export function generateJavaScript(model: SysMLModel, filePath: string, destination: string | undefined): string {
    const sourceFileName = path.parse(filePath).name;
    const dest = destination ?? path.join(path.dirname(filePath), `${sourceFileName}_generated_cpp`);
    
    if (!fs.existsSync(dest)) {
        fs.mkdirSync(dest, { recursive: true });
    }
    const FILE_EXT = ".cpp"; 

    const allElements: any[] = [];
    // Flatten package elements into a single list
    for (const el of model.elements) {
        if (isPackage(el)) {
            allElements.push(...el.elements);
        } else {
            allElements.push(el);
        }
    }

    // Separate atomic and coupled models
    const devsModels = allElements.filter(isDevsAtomicDef);
    const coupledModels = allElements.filter(isDevsCoupledDef);

    // Header template for atomic model files
    const getAtomicHeader = (name: string) => `// ==========================================\n// Auto-generated Atomic Model: ${name}\n// ==========================================\n#pragma once\n#include <iostream>\n#include <string>\n#include <map>\n#include <vector>\n#include <memory>\n\n`;
    // Footer template for atomic model files
    const getAtomicFooter = (name: string) => `\n// ==========================================\n`;
    
    // Header template for coupled model files
    const getCoupledHeader = (name: string, parts: any[]) => {
        let header = `// ==========================================\n// Auto-generated Coupled Model: ${name}\n// ==========================================\n#pragma once\n\n`;
        for (const part of parts) {
            const typeName = part.isDef?.ref?.name;
            if (typeName) header += `#include "${typeName}${FILE_EXT}"\n`;
        }
        header += `\nnamespace XLanguage {\n`;
        return header;
    };
    // Footer template for coupled model files
    const getCoupledFooter = (name: string) => `} // end of namespace XLanguage\n// ==========================================\n`;

    // ----------------------------------------------------
    // Part 1: Generate all DEVS Atomic Models
    // ----------------------------------------------------
    for (const devs of devsModels) {
        let cppCode = getAtomicHeader(devs.name);

        // Extract ports, states, and functions from the atomic model
        const inPorts = devs.elements.filter(isPortDef).filter(p => p.direction === 'in');
        const outPorts = devs.elements.filter(isPortDef).filter(p => p.direction === 'out');
        const allPorts = [...inPorts, ...outPorts];
        const allStates = devs.elements.filter(isStateDef).map(s => s.name);
        const functions = devs.elements.filter(isDevsFunction);

        // Build port type mapping dictionary
        const portTypes: Record<string, string> = {};
        for (const port of allPorts) {
            portTypes[port.name] = port.type?.ref?.name || "double";
        }

        // Generate class definition
        cppCode += `class ${devs.name} : public Atomic<double> {\n`;
        cppCode += `    enum STATE { ${allStates.join(', ')} };\nprivate:\n`;
        
        // Declare port variables and shared pointers
        for (const port of allPorts) {
            const pType = portTypes[port.name];
            cppCode += `    shared_ptr<${pType}> _$${port.name};\n    ${pType} ${port.name};\n`;
        }
        
        // Declare state variables and helper members
        cppCode += `    STATE state = ${allStates[0]};\n    map<STATE, std::string> stateToString = {\n`;
        for (let i = 0; i < allStates.length; i++) {
            cppCode += `        { ${allStates[i]}, "${allStates[i]}" }${i < allStates.length - 1 ? ',' : ''}\n`;
        }
        cppCode += `    };\n`;
        cppCode += `    string NowState;\n    string NextState;\n    string TranCon;\n    string StateAll;\n    double state_time = 0;\n    vector<bool> event;\n    bool first_save = true;\n\npublic:\n`;
        
        // Generate constructor with initializer list
        let initList = `Atomic<double>(_variable_name)`;
        for (const port of allPorts) {
            const pType = portTypes[port.name];
            initList += `, _$${port.name}(make_shared<${pType}>()), ${port.name}()`;
        }
        
        cppCode += `    ${devs.name}(const string & _variable_name = "${devs.name}") : ${initList} {\n`;
        // Register input and output ports
        for (const port of inPorts) cppCode += `        this->inPort.push_back("${port.name}");\n`;
        for (const port of outPorts) cppCode += `        this->outPort.push_back("${port.name}");\n`;
        cppCode += `    }\n    ~${devs.name}() {}\n\n`;
        
        // --- 1. External Transition Function (delta_ext) ---
        cppCode += `    void delta_ext(const double e, const Bag<PortValue> &x_bag) override{\n`;
        cppCode += `        std::map<std::string,bool> port_map{};\n`;
        // Initialize port trigger flags
        for (const port of inPorts) cppCode += `        port_map["${port.name}"] = false;\n`;
        
        // Process incoming port values
        cppCode += `        for (auto it_x_bag:x_bag) {\n`;
        for (let i = 0; i < inPorts.length; i++) {
            const port = inPorts[i];
            const pType = portTypes[port.name];
            const ifKeyword = i === 0 ? "if" : "else if";
            cppCode += `            ${ifKeyword} (it_x_bag.port == "${port.name}") {\n`;
            cppCode += `                port_map["${port.name}"] = true;\n`;
            cppCode += `                _$${port.name} = static_pointer_cast<${pType}>(it_x_bag.value);\n`;
            cppCode += `                ${port.name} = * _$${port.name};\n            }\n`;
        }
        cppCode += `        }\n        NowState = stateToString[state];\n`;

        // Process external transitions for each state
        const extFunc = functions.find(f => f.type === 'delta_ext');
        const extTransitions = extFunc ? extFunc.elements.filter(isDevsTransition) : [];
        const tempRef = { counter: 0 };

        for (let i = 0; i < allStates.length; i++) {
            const stateName = allStates[i];
            const ifKeyword = i === 0 ? "if" : "else if";
            cppCode += `        ${ifKeyword}(state == ${stateName}){\n`;
            const transForState = extTransitions.filter(t => t.source?.ref?.name === stateName);
            for (const trans of transForState) {
                const triggerPort = trans.trigger?.ref?.name;
                const targetState = trans.target?.ref?.name;
                if (triggerPort) {
                    cppCode += `            if(port_map["${triggerPort}"]){\n`;
                    cppCode += `                TranCon = "receive(${triggerPort})";\n`;
                    // Generate transition action statements
                    if (trans.actions && trans.actions.length > 0) {
                        cppCode += generateStatements(trans.actions, 'ext', portTypes, tempRef, "                ");
                    }
                    cppCode += `                state = ${targetState};\n            }\n`;
                }
            }
            cppCode += `        }\n`;
        }
        cppCode += `        NextState = stateToString[state];\n`;
        cppCode += `        out_variable_val(time_last + e,first_save);      if (first_save) {\n            first_save = false;\n        }\n    }\n\n`;

        // --- 2. Internal Transition Function (delta_int) ---
        cppCode += `    void delta_int() override{\n        NowState = stateToString[state];\n        switch (state) {\n`;
        const intFunc = functions.find(f => f.type === 'delta_int');
        const intTransitions = intFunc ? intFunc.elements.filter(isDevsTransition) : [];
        // Process internal transitions for each state
        for (const trans of intTransitions) {
            const sourceState = trans.source?.ref?.name;
            const targetState = trans.target?.ref?.name;
            cppCode += `            case ${sourceState}:{\n`;
            // Generate transition action statements
            if (trans.actions && trans.actions.length > 0) {
                cppCode += generateStatements(trans.actions, 'int', portTypes, tempRef, "                ");
            }
            cppCode += `                state = ${targetState};\n                TranCon += "timeover:"+to_string(state_time);\n                break;\n            }\n`;
        }
        cppCode += `        default:break; \n        }\n        NextState = stateToString[state];\n        out_variable_val(time_last + state_time,first_save);\n        if (first_save) {\n            first_save = false;\n        }\n    }\n\n`;

        // --- 3. Initialization Function ---
        cppCode += `    void init(double t, ...)override {\n    }\n\n`;

        // --- 4. Output Function (lambda) ---
        cppCode += `    void output_func(Bag<PortValue> &y_bag){\n        switch (state) {\n`;
        const lambdaFunc = functions.find(f => f.type === 'lambda');
        const lambdaTransitions = lambdaFunc ? lambdaFunc.elements.filter(isDevsTransition) : [];
        // Process output actions for each state
        for (const trans of lambdaTransitions) {
            const sourceState = trans.source?.ref?.name;
            if (trans.actions && trans.actions.length > 0) {
                cppCode += `        case ${sourceState}:{\n`;
                // Call control flow engine to generate if-else and make_shared code
                cppCode += generateStatements(trans.actions, 'lambda', portTypes, tempRef, "            ");
                cppCode += `            break;\n        }\n`;
            }
        }
        cppCode += `        default:break; \n        }\n    }\n\n`;

        // --- 5. Time Advance Function (ta) ---
        cppCode += `    double ta() {\n        switch (state) {\n`;
        const taFunc = functions.find(f => f.type === 'ta');
        const taAdvances = taFunc ? taFunc.elements.filter(isDevsTimeAdvance) : [];
        // Set time advance values for each state
        for (const advance of taAdvances) {
            const stateName = advance.state?.ref?.name;
            // Map INFINITE to maximum int value for C++ compatibility
            const timeValue = advance.time === 'INFINITE' ? '2147483647' : advance.time;
            cppCode += `        case ${stateName}:{\n            state_time = ${timeValue};\n            break;\n        }\n`;
        }
        cppCode += `        default:break; \n        }\n        return state_time;\n    }\n};\n`;
        
        cppCode += getAtomicFooter(devs.name);
        // Write atomic model to file
        fs.writeFileSync(path.join(dest, `${devs.name}${FILE_EXT}`), cppCode);
    }

    // ----------------------------------------------------
    // Part 2: Generate all DEVS Coupled Models
    // ----------------------------------------------------
    for (const coupled of coupledModels) {
        const parts = coupled.elements.filter(isPartUsage);
        let cppCode = getCoupledHeader(coupled.name, parts);
        // Generate coupled model generator function
        cppCode += `    Couple<double> * couple_${coupled.name}_generator() {\n`;
        cppCode += `        Couple<double> *${coupled.name} = new Couple<double>("${coupled.name}");\n\n`;
        // Instantiate all subcomponents
        for (const part of parts) {
            const typeName = part.isDef?.ref?.name;
            if (typeName) cppCode += `        ${typeName} *${part.name} = new ${typeName}();\n`;
        }
        cppCode += `\n        // Start connecting components\n`;
        // Generate all connection statements
        const connects = coupled.elements.filter(isConnectDef);
        for (const conn of connects) {
            const srcPart = conn.source.step[0];
            const srcPort = conn.source.step[1];
            const tgtPart = conn.target.step[0];
            const tgtPort = conn.target.step[1];
            cppCode += `        ${coupled.name}->connect(${srcPart}, "${srcPort}", ${tgtPart}, "${tgtPort}");\n`;
        }
        cppCode += `\n        return ${coupled.name};\n    }\n`;
        cppCode += getCoupledFooter(coupled.name);
        // Write coupled model to file
        fs.writeFileSync(path.join(dest, `${coupled.name}${FILE_EXT}`), cppCode);
    }
    return `Generated ${devsModels.length + coupledModels.length} C++ files in directory: ${dest}`;
}
