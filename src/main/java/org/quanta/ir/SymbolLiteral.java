package org.quanta.ir;

public class SymbolLiteral extends Expression {
    private final String value;

    public SymbolLiteral(String value) {
        this.value = value;
    }

    public String getValue() {
        return value;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
