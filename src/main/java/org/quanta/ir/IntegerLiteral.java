package org.quanta.ir;

public class IntegerLiteral extends Expression {
    private final int value;

    public IntegerLiteral(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
