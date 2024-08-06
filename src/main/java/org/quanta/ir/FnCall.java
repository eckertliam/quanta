package org.quanta.ir;

public class FnCall extends Expression {
    private final String name;
    private final Expression[] arguments;

    public FnCall(String name, Expression[] arguments) {
        this.name = name;
        this.arguments = arguments;
    }

    public String getName() {
        return name;
    }

    public Expression[] getArguments() {
        return arguments;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
