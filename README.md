# SysMLv2-DEVS: A Metamodel Extension Framework for Dynamic System Simulation

This repository contains the official implementation and model library for the paper: 
*"[A Formalized Metamodel Extension Method for Modeling Language]"*.

## 🌟 Overview
This project presents a lightweight, non-destructive metamodel extension framework that brings Discrete Event System Specification (DEVS) semantics into SysML v2. It includes a customized textual workbench, a formal semantic validator, and a semantics-preserving C++ code generator.

##  Repository Structure
* `/packages/language`: Textual syntax definition (`.langium`) and contextual validator.
* `/packages/cli`: C++ code generator (M2T transformation engine).
* `/model-library`: A collection of benchmarking SysML v2 models and the comprehensive UAV Mission System case study.

##  Quick Start

### Prerequisites
* [Node.js](https://nodejs.org/) (v18 or higher recommended)
* [Visual Studio Code](https://code.visualstudio.com/)

### 1. Installation & Compilation
Clone this repository and install dependencies:
\`\`\`bash
npm install
npm run langium:generate
npm run build
\`\`\`

### 2. Run the Textual Workbench (Language Server)
To evaluate the customized syntax highlighting and real-time semantics validation:
1. Open this entire folder in VS Code.
2. Press **`F5`** on your keyboard to launch a new VS Code Extension Development Host (the Evaluation Sandbox).
3. In the sandbox, open any model file in `/model-library/uav-mission-system/` to observe the frontend parsing capabilities.

### 3. C++ Code Generation via CLI (M2T Transformation)
To execute the semantics-preserving code generation without entering the sandbox, run the CLI tool directly from the root workspace terminal. 

For example, to compile a test model into executable C++ code, run:
\`\`\`bash
node ./packages/cli/bin/cli.js generate ./model-library/your_model_folder/your_model.sysmlx
node ./packages/cli/bin/cli.js generate ./model-library/uav-mission-system/UAVMissionSystem.sysmlx
\`\`\`
*(Note: The generated C++ files will be automatically output to a "generated" folder located in the exact same directory as the input model file).*

##  Evaluation (Subset Compatibility)
As stated in Section 4 of the paper, this framework extracts a *Simulation-Oriented Core Subset* ($SP_{core}$) of SysML v2. 
We provided 50 benchmarking models under `/model-library/benchmark-subset/`. All profiles achieve a **100%** parsing success rate under our extended compiler environment without any modifications.

---

##  Troubleshooting & Tips

To ensure a smooth reproduction experience, please note the following IDE-specific behaviors:

1. **Use CMD or Git Bash instead of PowerShell**: 
   Windows PowerShell occasionally blocks script execution due to strict default `ExecutionPolicy`. If you encounter errors like `npm is not recognized...` or script execution blocks, please use **Command Prompt (CMD)** or **Git Bash** in the VS Code terminal to execute the installation and generation commands.
2. **The "Markdown Debugger" F5 Trap**: 
   If you press `F5` while having this `README.md` file active in the editor, VS Code might prompt you to search for a "Markdown debugger". To avoid this, please open the **Run and Debug** view (`Ctrl+Shift+D`), select **`Launch Extension`** from the dropdown menu, and click the green Play button.

---

##  Acknowledgments
This project is built upon the excellent [Langium](https://langium.org/) language engineering framework. We sincerely thank the Eclipse Foundation and the Langium community for providing the robust parsing and language server infrastructure that made this metamodel extension possible.
