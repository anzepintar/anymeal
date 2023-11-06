/* AnyMeal recipe management software
   Copyright (C) 2021, 2023 Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#include <fstream>
#include <sstream>
#include <gtest/gtest.h>
#include "mealmaster.hh"


using namespace testing;
using namespace std;

TEST(MealMasterTest, RecipeTitle) {
  ifstream f("fixtures/header.mmf");
  Recipe result = parse_mealmaster(f);
  EXPECT_EQ("apple pie", result.title());
}

TEST(MealMasterTest, IgnoreLeadingSpacesInTitle) {
  ifstream f("fixtures/title_spaces.mmf");
  Recipe result = parse_mealmaster(f);
  EXPECT_EQ("apple pie", result.title());
}

TEST(MealMasterTest, Categories) {
  ifstream f("fixtures/header.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.categories().size());
  EXPECT_EQ("pastries", result.categories()[0]);
  EXPECT_EQ("cakes", result.categories()[1]);
}

TEST(MealMasterTest, Servings) {
  ifstream f("fixtures/header2.mmf");
  Recipe result = parse_mealmaster(f);
  EXPECT_EQ(42, result.servings());
  EXPECT_EQ("servings", result.servings_unit());
}

TEST(MealMasterTest, ServingSpaces) {
  ifstream f("fixtures/serving_spaces.mmf");
  Recipe result = parse_mealmaster(f);
  EXPECT_EQ(8, result.servings());
  EXPECT_EQ("", result.servings_unit());
}

TEST(MealMasterTest, IntegerAmount) {
  ifstream f("fixtures/ingredient.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(250, result.ingredients()[0].amount_integer());
}

TEST(MealMasterTest, FractionAmount) {
  ifstream f("fixtures/fraction.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(0, result.ingredients()[0].amount_integer());
  EXPECT_EQ(2, result.ingredients()[0].amount_numerator());
  EXPECT_EQ(3, result.ingredients()[0].amount_denominator());
}

TEST(MealMasterTest, MixedAmount) {
  ifstream f("fixtures/mixed.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(1, result.ingredients()[0].amount_integer());
  EXPECT_EQ(2, result.ingredients()[0].amount_numerator());
  EXPECT_EQ(3, result.ingredients()[0].amount_denominator());
}

TEST(MealMasterTest, NoAmount) {
  ifstream f("fixtures/noamount.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(0, result.ingredients()[0].amount_float());
  EXPECT_EQ(0, result.ingredients()[0].amount_integer());
  EXPECT_EQ(0, result.ingredients()[0].amount_numerator());
  EXPECT_EQ(1, result.ingredients()[0].amount_denominator());
  EXPECT_EQ("  ", result.ingredients()[0].unit());
  EXPECT_EQ("butter", result.ingredients()[0].text());
}

TEST(MealMasterTest, FloatingAmount) {
  ifstream f("fixtures/float.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ(2.5, result.ingredients()[0].amount_float());
}

TEST(MealMasterTest, IngredientUnit) {
  ifstream f("fixtures/ingredient.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("g ", result.ingredients()[0].unit());
}

TEST(MealMasterTest, IngredientText) {
  ifstream f("fixtures/ingredient.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("brown flour", result.ingredients()[0].text());
}

TEST(MealMasterTest, PreparationText) {
  ifstream f("fixtures/preparation.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("brown flour; sifted", result.ingredients()[0].text());
}

TEST(MealMasterTest, IngredientContinuation) {
  ifstream f("fixtures/continuation.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("butter; molten and stirred", result.ingredients()[0].text());
}

TEST(MealMasterTest, IngredientContinuationWhitespace) {
  ifstream f("fixtures/continuation_whitespace.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("butter; molten and stirred", result.ingredients()[0].text());
}

TEST(MealMasterTest, TwoIngredients) {
  ifstream f("fixtures/two_ingredients.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.ingredients().size());
  EXPECT_EQ("pastry", result.ingredients()[0].text());
  EXPECT_EQ("cooking apple", result.ingredients()[1].text());
}

TEST(MealMasterTest, Instructions1) {
  ifstream f("fixtures/instructions1.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Cook all the ingredients.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions2) {
  ifstream f("fixtures/instructions2.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1 cook is used.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions3) {
  ifstream f("fixtures/instructions3.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("     Start cooking.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions4) {
  ifstream f("fixtures/instructions4.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("2.5 apples are used.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions5) {
  ifstream f("fixtures/instructions5.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("-", result.ingredients()[0].text());
}

TEST(MealMasterTest, Instructions6) {
  ifstream f("fixtures/instructions6.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("2 3 apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions7) {
  ifstream f("fixtures/instructions7.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1/x apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions8) {
  ifstream f("fixtures/instructions8.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1/2 apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions9) {
  ifstream f("fixtures/instructions9.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1 2/x apples.", result.instructions()[0]);
}

TEST(MealMasterTest, Instructions10) {
  ifstream f("fixtures/instructions10.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("1 2/3 each.", result.instructions()[0]);
}

TEST(MealMasterTest, TwoInstructions) {
  ifstream f("fixtures/two_instructions.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(3, result.instructions().size());
  EXPECT_EQ("First line.", result.instructions()[0]);
  EXPECT_EQ("", result.instructions()[1]);
  EXPECT_EQ("Third line.", result.instructions()[2]);
}

TEST(MealMasterTest, Ignore0x14) {
  ifstream f("fixtures/ignore_0x14.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("First line.", result.instructions()[0]);
}

TEST(MealMasterTest, Replace0x9) {
  ifstream f("fixtures/replace_0x9.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("First    line.", result.instructions()[0]);
}

TEST(MealMasterTest, UnexpectedEndOfFile) {
  istringstream s("MMMMM---MEAL-MASTER Format---\r\n");
  EXPECT_THROW(parse_mealmaster(s), parse_exception);
}

TEST(MealMasterTest, ExpectingServings) {
  ifstream f("fixtures/no_servings.mmf");
  EXPECT_THROW(parse_mealmaster(f), parse_exception);
}

TEST(MealMasterTest, IngredientSection) {
  ifstream f("fixtures/ingredient_section.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredient_sections().size());
  pair<int, string> section = result.ingredient_sections()[0];
  EXPECT_EQ(1, section.first);
  EXPECT_EQ("meringue", section.second);
}

TEST(MealMasterTest, SectionWithHyphen) {
  ifstream f("fixtures/section_hyphen.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredient_sections().size());
  pair<int, string> section = result.ingredient_sections()[0];
  EXPECT_EQ(1, section.first);
  EXPECT_EQ("section-hyphen", section.second);
}

TEST(MealMasterTest, SectionWithSpaces) {
  ifstream f("fixtures/section_spaces.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredient_sections().size());
  pair<int, string> section = result.ingredient_sections()[0];
  EXPECT_EQ(1, section.first);
  EXPECT_EQ("section spaces", section.second);
}

TEST(MealMasterTest, InstructionsSection) {
  ifstream f("fixtures/instruction_section.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instruction_sections().size());
  pair<int, string> section = result.instruction_sections()[0];
  EXPECT_EQ(1, section.first);
  EXPECT_EQ("meringue", section.second);
  ASSERT_EQ(2, result.instructions().size());
  EXPECT_EQ("First line.", result.instructions()[0]);
  EXPECT_EQ("Second line.", result.instructions()[1]);
}

TEST(MealMasterTest, IgnoreSpecificNewLines) {
  ifstream f("fixtures/ignore_newlines.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(6, result.instructions().size());
  EXPECT_EQ("First line.", result.instructions()[0]);
  EXPECT_EQ("", result.instructions()[1]);
  EXPECT_EQ("Third line.", result.instructions()[2]);
  EXPECT_EQ("Fourth line.", result.instructions()[3]);
  EXPECT_EQ("", result.instructions()[4]);
  EXPECT_EQ("Sixth line.", result.instructions()[5]);
  ASSERT_EQ(1, result.instruction_sections().size());
  pair<int, string> section = result.instruction_sections()[0];
  EXPECT_EQ(3, section.first);
  EXPECT_EQ("section", section.second);
}

TEST(MealMasterTest, AppendToLine) {
  ifstream f("fixtures/append_to_line.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("First part. Second part.", result.instructions()[0]);
}

TEST(MealMasterTest, StrayIngredientLine) {
  ifstream f("fixtures/stray_ingredient.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("First line. 200.0 ml milk", result.instructions()[0]);
}

TEST(MealMasterTest, AcceptIngredientLikeLine) {
  ifstream f("fixtures/ingredient_like.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("First line 1 kg water.", result.instructions()[0]);
}

TEST(MealMasterTest, InstructionHeader) {
  ifstream f("fixtures/instruction_header.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instruction_sections().size());
  pair<int, string> section = result.instruction_sections()[0];
  EXPECT_EQ(0, section.first);
  EXPECT_EQ("meringue", section.second);
}

TEST(MealMasterTest, SkipLeadingSpaces) {
  ifstream f("fixtures/skip_leading_spaces.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("  First part. Second part.", result.instructions()[0]);
}

TEST(MealMasterTest, ForceNewline) {
  ifstream f("fixtures/force_newline.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.instructions().size());
  EXPECT_EQ("First line.", result.instructions()[0]);
  EXPECT_EQ("Second line.", result.instructions()[1]);
}

TEST(MealMasterTest, IgnoreWhitespaces) {
  ifstream f("fixtures/ignore_whitespaces.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.ingredients().size());
}

TEST(MealMasterTest, InstructionNumber) {
  ifstream f("fixtures/instruction_number.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Posted on Apr 27, 1998", result.instructions()[0]);
}

TEST(MealMasterTest, InstructionCRLF) {
  ifstream f("fixtures/float_crlf.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Posted on Apr 27, 1998.", result.instructions()[0]);
}

TEST(MealMasterTest, InstructionEndOfLine) {
  ifstream f("fixtures/instruction_eol.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Posted on Apr 27, 1998.", result.instructions()[0]);
}

TEST(MealMasterTest, InstructionEndOfLine2) {
  ifstream f("fixtures/instruction_eol2.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Posted on Apr 27, 1 2", result.instructions()[0]);
}

TEST(MealMasterTest, InstructionEndOfLine3) {
  ifstream f("fixtures/instruction_eol3.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Posted on Apr 27, 1/", result.instructions()[0]);
}

TEST(MealMasterTest, InstructionEndOfLine4) {
  ifstream f("fixtures/instruction_eol4.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("Posted on Apr 27, 1998.", result.instructions()[0]);
}

TEST(MealMasterTest, NoIngredient) {
  ifstream f("fixtures/no_ingredient.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("20 mg cholesterol", result.instructions()[0]);
}

TEST(MealMasterTest, TwoColumns) {
  ifstream f("fixtures/two_columns.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.ingredients().size());
  EXPECT_EQ("Oil", result.ingredients()[0].text());
  EXPECT_EQ("Baking soda", result.ingredients()[1].text());
}

TEST(MealMasterTest, IsColumnMajor) {
  ifstream f("fixtures/column_major.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(4, result.ingredients().size());
  EXPECT_EQ("ingredient1", result.ingredients()[0].text());
  EXPECT_EQ("ingredient2", result.ingredients()[1].text());
  EXPECT_EQ("ingredient3", result.ingredients()[2].text());
  EXPECT_EQ("ingredient4", result.ingredients()[3].text());
}

TEST(MealMasterTest, ThreeIngredients) {
  ifstream f("fixtures/three_ingredients.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(3, result.ingredients().size());
  EXPECT_EQ("ingredient1", result.ingredients()[0].text());
  EXPECT_EQ("ingredient2", result.ingredients()[1].text());
  EXPECT_EQ("ingredient3", result.ingredients()[2].text());
}

TEST(MealMasterTest, FlushIngredients) {
  ifstream f("fixtures/flush_ingredients.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.ingredients().size());
  EXPECT_EQ("ingredient1", result.ingredients()[0].text());
  EXPECT_EQ("ingredient2", result.ingredients()[1].text());
}

TEST(MealMasterTest, TwoColumnSection) {
  ifstream f("fixtures/two_column_section.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(4, result.ingredients().size());
  EXPECT_EQ("ingredient1", result.ingredients()[0].text());
  EXPECT_EQ("ingredient2", result.ingredients()[1].text());
  EXPECT_EQ("ingredient3", result.ingredients()[2].text());
  EXPECT_EQ("ingredient4", result.ingredients()[3].text());
}

TEST(MealMasterTest, OverlongIngredient) {
  ifstream f("fixtures/overlong_ingredient.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyzabc defghijk", result.ingredients()[0].text());
}

TEST(MealMasterTest, OverlongIngredient2) {
  ifstream f("fixtures/overlong_ingredient2.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyzabcdefghijk", result.ingredients()[0].text());
}

TEST(MealMasterTest, TwoColumnContinuation) {
  ifstream f("fixtures/two_column_continuation.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(3, result.ingredients().size());
  EXPECT_EQ("ingredient1", result.ingredients()[0].text());
  EXPECT_EQ("ingredient2", result.ingredients()[1].text());
  EXPECT_EQ("ingredient3 continued", result.ingredients()[2].text());
}

TEST(MealMasterTest, TwoColumnContinuation2) {
  ifstream f("fixtures/two_column_continuation2.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(3, result.ingredients().size());
  EXPECT_EQ("ingredient1 continued", result.ingredients()[0].text());
  EXPECT_EQ("ingredient2", result.ingredients()[1].text());
  EXPECT_EQ("ingredient3", result.ingredients()[2].text());
}

TEST(MealMasterTest, TwoColumnContinuation3) {
  ifstream f("fixtures/two_column_continuation3.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(3, result.ingredients().size());
  EXPECT_EQ("ingredient1", result.ingredients()[0].text());
  EXPECT_EQ("ingredient2 continued", result.ingredients()[1].text());
  EXPECT_EQ("ingredient3", result.ingredients()[2].text());
}

TEST(MealMasterTest, TwoColumnContinuation4) {
  ifstream f("fixtures/two_column_continuation4.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("ingredient1 cont_a cont_b cont_c", result.ingredients()[0].text());
}

TEST(MealMasterTest, TwoColumnTrailingWhitespace) {
  ifstream f("fixtures/two_column_trailing.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("lemon juice", result.ingredients()[0].text());
  ASSERT_EQ(1, result.instructions().size());
  EXPECT_EQ("In small saucepan with wire whisk beat all ingredients until smooth.", result.instructions()[0]);
}

TEST(MealMasterTest, EmptyIngredientSection) {
  ifstream f("fixtures/empty_ingredient_section.mmf");
  EXPECT_THROW(parse_mealmaster(f), parse_exception);
}

TEST(MealMasterTest, SectionWithTrailingWhitespace) {
  ifstream f("fixtures/section_whitespace.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.instruction_sections().size());
  pair<int, string> section = result.instruction_sections()[0];
  EXPECT_EQ(0, section.first);
  EXPECT_EQ("meringue", section.second);
}

TEST(MealMasterTest, UnexpectedContinuation) {
  ifstream f("fixtures/unexpected_continuation.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(1, result.ingredients().size());
  EXPECT_EQ("-John Doe-", result.ingredients()[0].text());
}

TEST(MealMasterTest, ErroneousContinuation) {
  ifstream f("fixtures/erroneous_continuation.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.ingredients().size());
  EXPECT_EQ("-and stirred", result.ingredients()[1].text());
  EXPECT_THROW(parse_mealmaster(f), parse_exception);
}

TEST(MealMasterTest, OnlyDiscardTwoSpaces) {
  ifstream f("fixtures/discard_two_spaces.mmf");
  Recipe result = parse_mealmaster(f);
  ASSERT_EQ(2, result.instructions().size());
  EXPECT_EQ("Only discard", result.instructions()[0]);
  EXPECT_EQ("         two leading spaces.", result.instructions()[1]);
}
