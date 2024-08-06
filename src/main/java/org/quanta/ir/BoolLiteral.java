package org.quanta.ir;

public class BoolLiteral extends Expression {
    private final boolean value;

    public BoolLiteral(boolean value) {
        this.value = value;
    }

    public boolean getValue() {
        return value;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
