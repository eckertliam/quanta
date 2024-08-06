package org.quanta.ir;

public class Expression extends Node {
    public Expression() {
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
