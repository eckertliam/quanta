package org.quanta.ir;

public class Statement {
    public Statement() {
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
