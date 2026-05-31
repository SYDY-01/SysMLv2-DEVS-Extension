grammar SysMLv2Extended

// --- Abstract Interface Definition (MA: Mapping to M2 layer of MOF) ---
interface SysMLElement {
    name?: string
}

// --- Root Node Parsing Entry ---
entry SysMLModel:
    (elements+=Element)*;

// [Architecture Core]: Highly inclusive polymorphic Element reservation slot \Phi
Element returns SysMLElement:
    Package | PartDef | PartUsage | AttributeDef | 
    EnumDef | PortDef | ActionDef | StateDef | ConnectDef | TransitionDef | FlowDef | RefDef |
    DomainPartDef | DevsAtomicDef | DevsCoupledDef | DevsFunction; 

// ==========================================
// 1. Native SysML v2 High-level Syntax (Native SysML v2 SP)
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
// 2. Case: General Domain Extension Syntax Pattern (Domain SP)
// ==========================================
DomainPartDef:
    'domain' 'part' 'def' name=ID '{' (elements+=Element)* '}';

// ==========================================
// 3. Formal Research: DEVS Extension Syntax Pattern (DEVS SP)
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

// 【MODIFICATION 1】: Dynamic Time Advance
// Now it accepts either a constant (NUMBER/'INFINITE') OR a variable reference (timeVar)
DevsTimeAdvance:
    state=[StateDef:ID] ':' (timeValue=(NUMBER | 'INFINITE') | timeVar=[SysMLElement:ID]) ';';

DevsTransition:
    source=[StateDef:ID] '->' target=[StateDef:ID] 
    ('when' trigger=[PortDef:ID])? 
    ('do' '{' (actions+=DevsStatement)* '}')? ';';

// ==========================================
// 4. Action Language for Control Flow
// Core Highlight: Supports Turing-complete nested if-else and arithmetic/boolean expressions
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

// 【MODIFICATION 2】: Math Operations and the Implicit 'e' Variable
Expression:
    Comparison;

Comparison:
    left=Addition (operator=('==' | '!=' | '>' | '<' | '>=' | '<=') right=Addition)?;

// Added standard Addition/Subtraction for "sigma - e"
Addition:
    left=Multiplication (operator+=('+' | '-') right+=Multiplication)*;

// Added standard Multiplication/Division for completeness
Multiplication:
    left=PrimaryExpression (operator+=('*' | '/') right+=PrimaryExpression)*;

PrimaryExpression:
    NumberLiteral | BooleanLiteral | ElapsedTimeLiteral | VariableRef | '(' Expression ')';

NumberLiteral:
    value=NUMBER;

BooleanLiteral:
    value=('true' | 'false');

// Added the implicit DEVS elapsed time variable 'e'
ElapsedTimeLiteral:
    isE?='e';

VariableRef:
    ref=[SysMLElement:ID];

// ==========================================
// Core Lexical Parsing (Lexer Rules)
// ==========================================
hidden terminal WS: /\s+/;
terminal ID: /[_a-zA-Z][\w_]*/;
terminal NUMBER returns number: /[0-9]+(\.[0-9]+)?/;
terminal STRING: /"(\\.|[^"\\])*"|'(\\.|[^'\\])*'/;

hidden terminal ML_COMMENT: /\/\*[\s\S]*?\*\//;
hidden terminal SL_COMMENT: /\/\/[^\n\r]*/;