# SysMLv2-DEVS: A Metamodel Extension Framework for Dynamic System Simulation

This repository contains the official implementation and model library for the paper: 
*"[A Formalized Metamodel Extension Method for  Modeling Language]"*.

##  Overview
This project presents a lightweight, non-destructive metamodel extension framework that brings Discrete Event System Specification (DEVS) semantics into SysML v2. It includes a customized textual workbench, a formal semantic validator, and a semantics-preserving C++ code generator.

##  Repository Structure
* `/packages/language`: Textual syntax definition (`.langium`) and contextual validator.
* `/packages/cli`: C++ code generator (M2T transformation engine).
* `/model-library`: A collection of benchmarking SysML v2 models and the comprehensive UAV Mission System case study.

##  Quick Start (For Reviewers)

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

### 2. Run the Textual Workbench
1. Open this entire folder in VS Code.
2. Press **`F5`** on your keyboard to launch a new VS Code Extension Development Host (the Evaluation Sandbox).
3. In the sandbox, open any model file in `/model-library/uav-mission-system/` to check the customized syntax highlighting, real-time semantics validation, and code generation.

##  Evaluation (Subset Compatibility)
As stated in Section 4 of the paper, this framework extracts a *Simulation-Oriented Core Subset* ($SP_{core}$) of SysML v2. 
We provided 50 benchmarking models under `/model-library/benchmark-subset/`. All profiles achieve a **100%** parsing success rate under our extended compiler environment without any modifications.
---
##  Acknowledgments
This project is built upon the excellent [Langium](https://langium.org/) language engineering framework. We sincerely thank the Eclipse Foundation and the Langium community for providing the robust parsing and language server infrastructure that made this metamodel extension possible.