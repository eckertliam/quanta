package org.quanta.frontend;


import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CharStream;
import org.quanta.antlr.QuantaLexer;
import org.quanta.antlr.QuantaParser;


public class Frontend {
    public ParseTree parse(String src) {
        CharStream input = CharStreams.fromString(src);
        Lexer lexer = new QuantaLexer(input);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        QuantaParser parser = new QuantaParser(tokens);
        return parser.program();
    }
}
