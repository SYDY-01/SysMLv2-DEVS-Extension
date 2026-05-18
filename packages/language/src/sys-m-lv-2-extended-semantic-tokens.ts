import { AstNode } from 'langium';
import { AbstractSemanticTokenProvider, SemanticTokenAcceptor } from 'langium/lsp';
import { isDevsAssignment, isDevsTimeAdvance, isDevsTransition } from './generated/ast.js';

export class SysMLv2ExtendedSemanticTokenProvider extends AbstractSemanticTokenProvider {
    
    protected override highlightElement(node: AstNode, acceptor: SemanticTokenAcceptor): void {
        // (已删除了非法的 super 调用)

        // 1. 为时间前进法则 (ta) 中的浮点数赋予 'number' 颜色
        if (isDevsTimeAdvance(node)) {
            if (typeof node.time === 'number') {
                acceptor({ node, property: 'time', type: 'number' });
            }
        } 
        
        // 2. 为动作赋值 (do) 中的浮点数赋予 'number' 颜色
        else if (isDevsAssignment(node)) {
            acceptor({ node, property: 'value', type: 'number' });
        }

        // 3. 强制让状态转移法则中的 '->' 箭头高亮
        else if (isDevsTransition(node)) {
            acceptor({ node, keyword: '->', type: 'keyword' });
        }
    }
}