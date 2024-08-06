package org.quanta.ir;

public abstract class IRNode {
    public abstract void accept(IRVisitor visitor);
}
