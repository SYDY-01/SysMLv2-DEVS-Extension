import type { ValidationAcceptor, ValidationChecks } from 'langium';
// 👇 Completed the import of DevsAtomicDef
import type { SysMLv2ExtendedAstType, DevsTimeAdvance, DevsTransition, DevsFunction, DevsAtomicDef } from './generated/ast.js';
import type { SysMLv2ExtendedServices } from './sys-m-lv-2-extended-module.js';

// Register validation checks
export function registerValidationChecks(services: SysMLv2ExtendedServices) {
    const registry = services.validation.ValidationRegistry;
    const validator = services.validation.SysMLv2ExtendedValidator;
    const checks: ValidationChecks<SysMLv2ExtendedAstType> = {
        DevsTimeAdvance: validator.checkTimeAdvanceSemantics, // Updated method name
        DevsTransition: validator.checkDevsTransitionSemantics
    };
    registry.register(checks, validator);
}

// Concrete validation logic class
export class SysMLv2ExtendedValidator {

    /**
     * Rule 1: Time advance (ta) constraints and security validation.
     * Corresponds to V-Rule 1: Time constraint validation in the paper.
     */
    checkTimeAdvanceSemantics(node: DevsTimeAdvance, accept: ValidationAcceptor): void {
        // Validation A: If it's a constant number, it cannot be negative
        if (typeof node.timeValue === 'number') {
            if (node.timeValue < 0) {
                accept('error', 'DEVS Semantic Error: Time advance (ta) cannot be negative.', { 
                    node: node, 
                    property: 'timeValue' 
                });
            }
        }

        // Validation B: If it's a dynamic variable reference, it MUST be an AttributeDef
        if (node.timeVar && node.timeVar.ref) {
            if (node.timeVar.ref.$type !== 'AttributeDef') {
                accept('error', 'DEVS Semantic Error: The time advance reference must be an attribute (attribute def) variable.', {
                    node: node,
                    property: 'timeVar'
                });
            }
        }
    }

    /**
     * Rule 3: DEVS Internal transition semantics conflict check (INFINITE state has no internal transition).
     * Corresponds to V-Rule 3: Deterministic transition consistency in the paper.
     */
    checkDevsTransitionSemantics(node: DevsTransition, accept: ValidationAcceptor): void {
        const parentFunction = node.$container as DevsFunction;
        
        if (parentFunction && parentFunction.type === 'delta_int') {
            
            // 🚨 Fix: Explicitly cast the implicit $container to the true strongly-typed DevsAtomicDef
            const atomicModel = parentFunction.$container as DevsAtomicDef; 
            
            // 🚨 Fix: Add a safety check to ensure atomicModel exists and has the elements property
            if (atomicModel && atomicModel.elements) {
                
                // 🚨 Fix: Explicitly specify 'any' or a concrete type for the iterated element 'e' to avoid implicit any errors
                const allFunctions = atomicModel.elements.filter((e: any) => e.$type === 'DevsFunction') as DevsFunction[];
                const taFunction = allFunctions.find(f => f.type === 'ta');
                
                if (taFunction && taFunction.elements) {
                    for (const elem of taFunction.elements) {
                        if (elem.$type === 'DevsTimeAdvance') {
                            const taNode = elem as DevsTimeAdvance;
                            
                            // Check if the state defined by ta is INFINITE (using the new timeValue property)
                            if (taNode.state.ref?.name === node.source.ref?.name && taNode.timeValue === 'INFINITE') {
                                accept('error', `DEVS Semantic Error: State '${node.source.ref?.name}' has INFINITE time advance. It can NEVER trigger an internal transition.`, { 
                                    node: node, 
                                    property: 'source' 
                                });
                            }
                        }
                    }
                }
            }
        }
    }
}