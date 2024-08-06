package org.quanta.ir;

public class ArrayIndex extends Expression {
    private final Expression array;
    private final Expression index;

    public ArrayIndex(Expression array, Expression index) {
        this.array = array;
        this.index = index;
    }

    public Expression getArray() {
        return array;
    }

    public Expression getIndex() {
        return index;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
