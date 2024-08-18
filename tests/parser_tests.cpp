#include <gtest/gtest.h>
#include "../src/frontend/parser.h"

TEST(Parser, RecordDecl) {
    std::string src = "record Point { x: Int, y: Int }";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is a record declaration
    auto record_decl = dynamic_cast<RecordDecl*>(program.body[0].get());
    ASSERT_NE(record_decl, nullptr);
    EXPECT_EQ(record_decl->name, "Point");
    EXPECT_EQ(record_decl->fields.size(), 2);
    EXPECT_EQ(record_decl->fields[0].first, "x");
    auto type = dynamic_cast<SymbolTypeExpr*>(record_decl->fields[0].second.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->symbol, "Int");
    EXPECT_EQ(record_decl->fields[1].first, "y");
    type = dynamic_cast<SymbolTypeExpr*>(record_decl->fields[1].second.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->symbol, "Int");
}

TEST(Parser, BasicEnum) {
    std::string src = "enum Color { Red, Green, Blue }";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is an enum declaration
    auto enum_decl = dynamic_cast<EnumDecl*>(program.body[0].get());
    ASSERT_NE(enum_decl, nullptr);
    EXPECT_EQ(enum_decl->name, "Color");
    EXPECT_EQ(enum_decl->variants.size(), 3);
    EXPECT_EQ(enum_decl->variants[0].first, "Red");
    EXPECT_EQ(enum_decl->variants[0].second, nullptr);
    EXPECT_EQ(enum_decl->variants[1].first, "Green");
    EXPECT_EQ(enum_decl->variants[1].second, nullptr);
    EXPECT_EQ(enum_decl->variants[2].first, "Blue");
    EXPECT_EQ(enum_decl->variants[2].second, nullptr);
}

TEST(Parser, Enum) {
    std::string src = "enum Option { Some: a, None }";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is an enum declaration
    auto enum_decl = dynamic_cast<EnumDecl*>(program.body[0].get());
    ASSERT_NE(enum_decl, nullptr);
    EXPECT_EQ(enum_decl->name, "Option");
    EXPECT_EQ(enum_decl->variants.size(), 2);
    EXPECT_EQ(enum_decl->variants[0].first, "Some");
    auto type = dynamic_cast<SymbolTypeExpr*>(enum_decl->variants[0].second.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->symbol, "a");
    EXPECT_EQ(enum_decl->variants[1].first, "None");
    EXPECT_EQ(enum_decl->variants[1].second, nullptr);
}

TEST(Parser, SimpleTypeAlias) {
    std::string src = "type Number = i32;";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is a type alias declaration
    auto type_alias_decl = dynamic_cast<TypeAliasDecl*>(program.body[0].get());
    ASSERT_NE(type_alias_decl, nullptr);
    EXPECT_EQ(type_alias_decl->name, "Number");
    auto type = dynamic_cast<SymbolTypeExpr*>(type_alias_decl->type.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->symbol, "i32");
}

TEST(Parser, SumTypeAlias) {
    std::string src = "type Number = i32 | f32;";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is a type alias declaration
    auto type_alias_decl = dynamic_cast<TypeAliasDecl*>(program.body[0].get());
    ASSERT_NE(type_alias_decl, nullptr);
    EXPECT_EQ(type_alias_decl->name, "Number");
    auto type = dynamic_cast<SumTypeExpr*>(type_alias_decl->type.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->variants.size(), 2);
    auto symbol = dynamic_cast<SymbolTypeExpr*>(type->variants[0].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
    symbol = dynamic_cast<SymbolTypeExpr*>(type->variants[1].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "f32");
}

TEST(Parser, SumTypeAlias2) {
    std::string src = "type Expr = i32 | i64 | f32 | f64;";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is a type alias declaration
    auto type_alias_decl = dynamic_cast<TypeAliasDecl*>(program.body[0].get());
    ASSERT_NE(type_alias_decl, nullptr);
    EXPECT_EQ(type_alias_decl->name, "Expr");
    auto type = dynamic_cast<SumTypeExpr*>(type_alias_decl->type.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->variants.size(), 4);
    auto symbol = dynamic_cast<SymbolTypeExpr*>(type->variants[0].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
    symbol = dynamic_cast<SymbolTypeExpr*>(type->variants[1].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i64");
    symbol = dynamic_cast<SymbolTypeExpr*>(type->variants[2].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "f32");
    symbol = dynamic_cast<SymbolTypeExpr*>(type->variants[3].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "f64");
}

TEST(Parser, TupleTypeAlias) {
    std::string src = "type Point = (i32, i32);";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is a type alias declaration
    auto type_alias_decl = dynamic_cast<TypeAliasDecl*>(program.body[0].get());
    ASSERT_NE(type_alias_decl, nullptr);
    EXPECT_EQ(type_alias_decl->name, "Point");
    auto type = dynamic_cast<TupleTypeExpr*>(type_alias_decl->type.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->fields.size(), 2);
    auto symbol = dynamic_cast<SymbolTypeExpr*>(type->fields[0].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
    symbol = dynamic_cast<SymbolTypeExpr*>(type->fields[1].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
}

TEST(Parser, TupleTypeAlias2) {
    std::string src = "type SquareMatrix = ((i32, i32), (i32, i32));";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is a type alias declaration
    auto type_alias_decl = dynamic_cast<TypeAliasDecl*>(program.body[0].get());
    ASSERT_NE(type_alias_decl, nullptr);
    EXPECT_EQ(type_alias_decl->name, "SquareMatrix");
    auto type = dynamic_cast<TupleTypeExpr*>(type_alias_decl->type.get());
    ASSERT_NE(type, nullptr);
    EXPECT_EQ(type->fields.size(), 2);
    auto inner_type = dynamic_cast<TupleTypeExpr*>(type->fields[0].get());
    ASSERT_NE(inner_type, nullptr);
    EXPECT_EQ(inner_type->fields.size(), 2);
    auto symbol = dynamic_cast<SymbolTypeExpr*>(inner_type->fields[0].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
    symbol = dynamic_cast<SymbolTypeExpr*>(inner_type->fields[1].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
    inner_type = dynamic_cast<TupleTypeExpr*>(type->fields[1].get());
    ASSERT_NE(inner_type, nullptr);
    EXPECT_EQ(inner_type->fields.size(), 2);
    symbol = dynamic_cast<SymbolTypeExpr*>(inner_type->fields[0].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
    symbol = dynamic_cast<SymbolTypeExpr*>(inner_type->fields[1].get());
    ASSERT_NE(symbol, nullptr);
    EXPECT_EQ(symbol->symbol, "i32");
}

TEST(Parser, ArrayTypeAlias) {
    std::string src = "type Numbers = [i32; 5];";
    Parser parser(src);
    auto program = parser.parse_program();
    // we know that the first node in the program is a type alias declaration
    auto type_alias_decl = dynamic_cast<TypeAliasDecl*>(program.body[0].get());
    ASSERT_NE(type_alias_decl, nullptr);
    EXPECT_EQ(type_alias_decl->name, "Numbers");
    auto array_type = dynamic_cast<ArrayTypeExpr*>(type_alias_decl->type.get());
    ASSERT_NE(array_type, nullptr);
    auto symbol_type = dynamic_cast<SymbolTypeExpr*>(array_type->elem.get());
    ASSERT_NE(symbol_type, nullptr);
    EXPECT_EQ(symbol_type->symbol, "i32");
    auto size = dynamic_cast<IntExpr*>(array_type->size.get());
    ASSERT_NE(size, nullptr);
    EXPECT_EQ(size->value, 5);
}