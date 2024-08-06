package org.quanta.ir;

public class ArrayLiteral extends Expression {
    private final Expression[] elements;

    public ArrayLiteral(Expression[] elements) {
        this.elements = elements;
    }

    public Expression[] getElements() {
        return elements;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
