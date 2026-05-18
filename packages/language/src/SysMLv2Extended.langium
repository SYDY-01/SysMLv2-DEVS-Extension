grammar SysMLv2Extended

// --- 抽象接口定义 (MA: 映射到 MOF 的 M2 层) ---
interface SysMLElement {
    name?: string
}

// --- 根节点解析入口 ---
entry SysMLModel:
    (elements+=Element)*;

// 【架构核心】：极其包容的多态 Element 预留槽 \Phi
Element returns SysMLElement:
    Package | PartDef | PartUsage | AttributeDef | 
    EnumDef | PortDef | ActionDef | StateDef | ConnectDef | TransitionDef | FlowDef | RefDef |
    DomainPartDef | DevsAtomicDef | DevsCoupledDef | DevsFunction; 

// ==========================================
// 1. SysML v2 原生高级语法 (Native SysML v2 SP)
// ==========================================

Package:
    'package' name=ID '{' (elements+=Element)* '}';

EnumDef:
    'enum' 'def' name=ID '{' (literals+=EnumLiteral)* '}';

EnumLiteral:
    name=ID ';';

PortDef:
    (direction=('in' | 'out' | 'inout'))? 'port' ('def')? name=ID (':' type=[SysMLElement:ID])? ';';

AttributeDef:
    (direction=('in' | 'out' | 'inout'))? 'attribute' ('def')? name=ID (':' type=[SysMLElement:ID])? ('=' value=ValueRef)? ';';

RefDef:
    'ref' name=ID (':' type=[SysMLElement:ID])? ';';

PartDef:
    'part' 'def' name=ID '{' (elements+=Element)* '}';

PartUsage:
    (direction=('in' | 'out' | 'inout'))? 'part' name=ID (':' isDef=[SysMLElement:ID])? 
    ('{' (assignments+=Assignment)* '}' | ';');

Assignment:
    name=ID '=' value=ValueRef ';';

ActionDef:
    'action' ('def')? name=ID ('{' (elements+=Element)* '}' | ';');

FlowDef:
    'flow' source=[SysMLElement:ID] 'to' target=[SysMLElement:ID] ';';

StateDef:
    ('initial')? 'state' ('def')? name=ID ('{' (elements+=Element)* '}' | ';');

TransitionDef:
    'transition' name=ID '{'
        'first' source=[SysMLElement:ID] ';'
        'then' target=[SysMLElement:ID] ';'
    '}';

ConnectDef:
    'connect' source=DotPath 'to' target=DotPath ';';

DotPath:
    step+=ID ('.' step+=ID)*;

ValueRef:
    numValue=NUMBER | strValue=STRING | enumRef=QualifiedName;

QualifiedName:
    step+=ID ('::' step+=ID)*;

// ==========================================
// 2. 案例：通用领域扩展语法模式 (Domain SP)
// ==========================================
DomainPartDef:
    'domain' 'part' 'def' name=ID '{' (elements+=Element)* '}';

// ==========================================
// 3. 正式研究：DEVS 扩展语法模式 (DEVS SP)
// ==========================================

DevsAtomicDef:
    'devs' 'atomic' 'def' name=ID '{' (elements+=Element)* '}';

DevsCoupledDef:
    'devs' 'coupled' 'def' name=ID '{' (elements+=Element)* '}';

DevsFunction:
    type=('delta_int' | 'delta_ext' | 'lambda' | 'ta') 'def' name=ID '{'
        (elements+=DevsBehaviorElement)*
    '}';

DevsBehaviorElement:
    DevsTimeAdvance | DevsTransition;

DevsTimeAdvance:
    state=[StateDef:ID] ':' time=(NUMBER | 'INFINITE') ';';

DevsTransition:
    source=[StateDef:ID] '->' target=[StateDef:ID] 
    ('when' trigger=[PortDef:ID])? 
    ('do' '{' (actions+=DevsStatement)* '}')? ';';

// ==========================================
// 4. 动作语言扩展 (Action Language for Control Flow)
// 核心亮点：支持图灵完备的 if-else 嵌套与布尔表达式
// ==========================================

DevsStatement:
    DevsIfStatement | DevsAssignment;

DevsIfStatement:
    'if' '(' condition=Expression ')' '{'
        (thenBlock+=DevsStatement)*
    '}'
    ('else' (elseIfNode=DevsIfStatement | '{' (elseBlock+=DevsStatement)* '}'))?;

DevsAssignment:
    target=[SysMLElement:ID] '=' value=Expression ';';

Expression:
    Comparison;

Comparison:
    left=PrimaryExpression (operator=('==' | '!=' | '>' | '<' | '>=' | '<=') right=PrimaryExpression)?;

PrimaryExpression:
    NumberLiteral | BooleanLiteral | VariableRef;

NumberLiteral:
    value=NUMBER;

BooleanLiteral:
    value=('true' | 'false');

VariableRef:
    ref=[SysMLElement:ID];

// ==========================================
// 核心词法解析 (Lexer Rules)
// ==========================================
hidden terminal WS: /\s+/;
terminal ID: /[_a-zA-Z][\w_]*/;
terminal NUMBER returns number: /[0-9]+(\.[0-9]+)?/;
terminal STRING: /"(\\.|[^"\\])*"|'(\\.|[^'\\])*'/;

hidden terminal ML_COMMENT: /\/\*[\s\S]*?\*\//;
hidden terminal SL_COMMENT: /\/\/[^\n\r]*/;