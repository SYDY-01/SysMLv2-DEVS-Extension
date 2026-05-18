import type { ValidationAcceptor, ValidationChecks } from 'langium';
// 👇 补全了 DevsAtomicDef 的导入
import type { SysMLv2ExtendedAstType, DevsTimeAdvance, DevsTransition, DevsFunction, DevsAtomicDef } from './generated/ast.js';
import type { SysMLv2ExtendedServices } from './sys-m-lv-2-extended-module.js';

// 注册验证检查器
export function registerValidationChecks(services: SysMLv2ExtendedServices) {
    const registry = services.validation.ValidationRegistry;
    const validator = services.validation.SysMLv2ExtendedValidator;
    const checks: ValidationChecks<SysMLv2ExtendedAstType> = {
        DevsTimeAdvance: validator.checkTimeAdvanceIsPositive,
        DevsTransition: validator.checkDevsTransitionSemantics
    };
    registry.register(checks, validator);
}

// 具体的验证逻辑类
export class SysMLv2ExtendedValidator {

    /**
     * 规则 1：时间推进 (ta) 绝对不能为负数
     * 对应论文理论中的 V-Rule 1: Time constraint validation
     */
    checkTimeAdvanceIsPositive(node: DevsTimeAdvance, accept: ValidationAcceptor): void {
        if (typeof node.time === 'number') {
            if (node.time < 0) {
                accept('error', 'DEVS Semantic Error: Time advance (ta) cannot be negative.', { 
                    node: node, 
                    property: 'time' 
                });
            }
        }
    }

    /**
     * 规则 3：DEVS 内部转移语义冲突检查 (INFINITE 状态无内部转移)
     * 对应论文理论中的 V-Rule 3: Deterministic transition consistency
     */
    checkDevsTransitionSemantics(node: DevsTransition, accept: ValidationAcceptor): void {
        const parentFunction = node.$container as DevsFunction;
        
        if (parentFunction && parentFunction.type === 'delta_int') {
            
            // 🚨 修复点：将隐式的 $container 显式断言为真实的强类型 DevsAtomicDef
            const atomicModel = parentFunction.$container as DevsAtomicDef; 
            
            // 🚨 修复点：增加安全校验，确认 atomicModel 存在且具有 elements 属性
            if (atomicModel && atomicModel.elements) {
                
                // 🚨 修复点：为遍历元素 e 显式指定 any 或具体类型，避免隐式 any 报错
                const allFunctions = atomicModel.elements.filter((e: any) => e.$type === 'DevsFunction') as DevsFunction[];
                const taFunction = allFunctions.find(f => f.type === 'ta');
                
                if (taFunction && taFunction.elements) {
                    for (const elem of taFunction.elements) {
                        if (elem.$type === 'DevsTimeAdvance') {
                            const taNode = elem as DevsTimeAdvance;
                            // 如果 ta 定义的该状态为 INFINITE
                            if (taNode.state.ref?.name === node.source.ref?.name && taNode.time === 'INFINITE') {
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