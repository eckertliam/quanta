package org.quanta.ir;

public class FieldAccess extends Expression {
    private final Expression target;
    private final String fieldName;

    public FieldAccess(Expression target, String fieldName) {
        this.target = target;
        this.fieldName = fieldName;
    }

    public Expression getTarget() {
        return target;
    }

    public String getFieldName() {
        return fieldName;
    }

    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
