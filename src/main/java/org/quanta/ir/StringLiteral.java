package org.quanta.ir;

public class StringLiteral extends Expression {
    private final String value;

    public StringLiteral(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
