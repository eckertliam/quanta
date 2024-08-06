package org.quanta.ir;

public abstract class Node {
    public abstract void accept(Visitor visitor);
}
