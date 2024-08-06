package org.quanta.ir;

public interface Visitor {
    void visit(Node node);
    void visit(Statement statement);
    void visit(Expression expression);
    void visit(IntegerLiteral integerLiteral);
    void visit(BoolLiteral boolLiteral);
    void visit(StringLiteral stringLiteral);
    void visit(SymbolLiteral symbolLiteral);
    void visit(ArrayLiteral arrayLiteral);
    void visit(FnCall fnCall);
    void visit(FieldAccess fieldAccess);
    void visit(ArrayIndex arrayIndex);
}
