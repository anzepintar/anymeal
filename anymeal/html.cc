/* AnyMeal recipe management software
   Copyright (C) 2021 Jan Wedekind

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
#include <cassert>
#include <sstream>
#include "html.hh"


using namespace std;

#define QT_TRANSLATE_NOOP(scope, x) x

string html_encode(const string &data) {
  // https://stackoverflow.com/questions/5665231/most-efficient-way-to-escape-xml-html-in-c-string
  string result;
  result.reserve(data.size());
  for(size_t pos=0; pos!=data.size(); pos++) {
    switch(data[pos]) {
      case '&':  result.append("&amp;");       break;
      case '\"': result.append("&quot;");      break;
      case '\'': result.append("&apos;");      break;
      case '<':  result.append("&lt;");        break;
      case '>':  result.append("&gt;");        break;
      default:   result.append(&data[pos], 1); break;
    };
  };
  return result;
}

string html_amount(Ingredient &ingredient) {
  ostringstream stream;
  if (ingredient.amount_float() > 0) {
    stream << ingredient.amount_float();
  } else {
    if (ingredient.amount_integer() > 0)
      stream << ingredient.amount_integer();
    if (ingredient.amount_numerator() > 0) {
      if (ingredient.amount_integer() > 0)
        stream << " ";
      stream << ingredient.amount_numerator() << "/" << ingredient.amount_denominator();
    };
  };
  return stream.str();
}

string html_unit(const string &unit, string (*translate)(const char *, const char *)) {
  const char *result = "";
  if (unit == "x ") result = QT_TRANSLATE_NOOP("units", "per serving");
  if (unit == "sm") result = QT_TRANSLATE_NOOP("units", "small");
  if (unit == "md") result = QT_TRANSLATE_NOOP("units", "medium");
  if (unit == "lg") result = QT_TRANSLATE_NOOP("units", "large");
  if (unit == "cn") result = QT_TRANSLATE_NOOP("units", "can");
  if (unit == "pk") result = QT_TRANSLATE_NOOP("units", "package");
  if (unit == "pn") result = QT_TRANSLATE_NOOP("units", "pinch");
  if (unit == "dr") result = QT_TRANSLATE_NOOP("units", "drop");
  if (unit == "ds") result = QT_TRANSLATE_NOOP("units", "dash");
  if (unit == "ct") result = QT_TRANSLATE_NOOP("units", "carton");
  if (unit == "bn") result = QT_TRANSLATE_NOOP("units", "bunch");
  if (unit == "sl") result = QT_TRANSLATE_NOOP("units", "slice");
  if (unit == "ea") result = QT_TRANSLATE_NOOP("units", "each");
  if (unit == "t ") result = QT_TRANSLATE_NOOP("units", "teaspoon");
  if (unit == "ts") result = QT_TRANSLATE_NOOP("units", "teaspoon");
  if (unit == "T ") result = QT_TRANSLATE_NOOP("units", "tablespoon");
  if (unit == "tb") result = QT_TRANSLATE_NOOP("units", "tablespoon");
  if (unit == "fl") result = QT_TRANSLATE_NOOP("units", "fluid ounce");
  if (unit == "c ") result = QT_TRANSLATE_NOOP("units", "cup");
  if (unit == "pt") result = QT_TRANSLATE_NOOP("units", "pint");
  if (unit == "qt") result = QT_TRANSLATE_NOOP("units", "quart");
  if (unit == "ga") result = QT_TRANSLATE_NOOP("units", "gallon");
  if (unit == "oz") result = QT_TRANSLATE_NOOP("units", "ounce");
  if (unit == "lb") result = QT_TRANSLATE_NOOP("units", "pound");
  if (unit == "ml") result = QT_TRANSLATE_NOOP("units", "milliliter");
  if (unit == "cb") result = QT_TRANSLATE_NOOP("units", "cubic cm");
  if (unit == "cl") result = QT_TRANSLATE_NOOP("units", "centiliter");
  if (unit == "dl") result = QT_TRANSLATE_NOOP("units", "deciliter");
  if (unit == "l ") result = QT_TRANSLATE_NOOP("units", "liter");
  if (unit == "mg") result = QT_TRANSLATE_NOOP("units", "milligram");
  if (unit == "cg") result = QT_TRANSLATE_NOOP("units", "centigram");
  if (unit == "dg") result = QT_TRANSLATE_NOOP("units", "decigram");
  if (unit == "g ") result = QT_TRANSLATE_NOOP("units", "gram");
  if (unit == "kg") result = QT_TRANSLATE_NOOP("units", "kilogram");
  return (*translate)("units", result);
}

int index_of_unit(const std::string &unit) {
  int result = 32;
  if (unit == "x ") result =  0;
  if (unit == "sm") result =  1;
  if (unit == "md") result =  2;
  if (unit == "lg") result =  3;
  if (unit == "cn") result =  4;
  if (unit == "pk") result =  5;
  if (unit == "pn") result =  6;
  if (unit == "dr") result =  7;
  if (unit == "ds") result =  8;
  if (unit == "ct") result =  9;
  if (unit == "bn") result = 10;
  if (unit == "sl") result = 11;
  if (unit == "ea") result = 12;
  if (unit == "t ") result = 13;
  if (unit == "ts") result = 13;
  if (unit == "T ") result = 14;
  if (unit == "tb") result = 14;
  if (unit == "fl") result = 15;
  if (unit == "c ") result = 16;
  if (unit == "pt") result = 17;
  if (unit == "qt") result = 18;
  if (unit == "ga") result = 19;
  if (unit == "oz") result = 20;
  if (unit == "lb") result = 21;
  if (unit == "ml") result = 22;
  if (unit == "cb") result = 23;
  if (unit == "cl") result = 24;
  if (unit == "dl") result = 25;
  if (unit == "l ") result = 26;
  if (unit == "mg") result = 27;
  if (unit == "cg") result = 28;
  if (unit == "dg") result = 29;
  if (unit == "g ") result = 30;
  if (unit == "kg") result = 31;
  return result;
}

const char *unit_from_index(int index) {
  switch (index) {
    case  0: return "x ";
    case  1: return "sm";
    case  2: return "md";
    case  3: return "lg";
    case  4: return "cn";
    case  5: return "pk";
    case  6: return "pn";
    case  7: return "dr";
    case  8: return "ds";
    case  9: return "ct";
    case 10: return "bn";
    case 11: return "sl";
    case 12: return "ea";
    case 13: return "ts";
    case 14: return "tb";
    case 15: return "fl";
    case 16: return "c ";
    case 17: return "pt";
    case 18: return "qt";
    case 19: return "ga";
    case 20: return "oz";
    case 21: return "lb";
    case 22: return "ml";
    case 23: return "cb";
    case 24: return "cl";
    case 25: return "dl";
    case 26: return "l ";
    case 27: return "mg";
    case 28: return "cg";
    case 29: return "dg";
    case 30: return "g ";
    case 31: return "kg";
  };
  return "  ";
}

string notrans(const char *context, const char *text) {
  return text;
}

static void stream_recipe(ostringstream &stream, Recipe &recipe, string (*translate)(const char *, const char *)) {
  if (!recipe.title().empty()) {
    stream << "    <h2>" << html_encode(recipe.title()) << "</h2>\n";
  };
  if (!recipe.categories().empty()) {
    stream << "    <p><b>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Categories")) << ":</b> ";
    for (vector<string>::iterator category=recipe.categories().begin(); category!=recipe.categories().end(); category++) {
      stream << html_encode(*category);
      if (category + 1 != recipe.categories().end())
        stream << ", ";
    };
    stream << "</p>\n";
  };
  if (recipe.servings() > 0) {
    stream << "    <p><b>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Yield")) << ":</b> " << recipe.servings()
           << " " << html_encode(recipe.servings_unit()) << "</p>\n";
  };
  if (!recipe.ingredients().empty()) {
    stream << "    <h3>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Ingredients")) << "</h3>\n"
           << "    <table>\n"
           << "      <tr style=\"white-space:nowrap;\">\n"
           << "        <th style=\"text-align:left\">" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "amount")) << "&nbsp;</th>\n"
           << "        <th style=\"text-align:left\">" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "unit")) << "&nbsp;</th>\n"
           << "        <th style=\"text-align:left\">" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "ingredient")) << "</th>\n"
           << "      </tr>\n";
    vector<pair<int, string> >::iterator section = recipe.ingredient_sections().begin();
    for (int i=0; i<recipe.ingredients().size(); i++) {
      while (section != recipe.ingredient_sections().end() && section->first == i) {
        stream << "      <tr>\n"
               << "        <td colspan=\"3\"><em>" << html_encode(section->second) << "</em></td>\n"
               << "      </tr>\n";
        section++;
      };
      Ingredient ingredient = recipe.ingredients()[i];
      stream << "      <tr>\n"
             << "        <td style=\"white-space:nowrap;\">" << html_amount(ingredient) << "&nbsp;</td>\n"
             << "        <td style=\"white-space:nowrap;\">" << html_unit(ingredient.unit(), translate) << "&nbsp;</td>\n"
             << "        <td>" << ingredient.text() << "</td>\n"
             << "      </tr>\n";
    };
    stream << "    </table>\n";
  };
  if (!recipe.instructions().empty()) {
    stream << "    <h3>" << (*translate)("recipe", QT_TRANSLATE_NOOP("recipe", "Instructions")) << "</h3>\n";
    vector<pair<int, string> >::iterator section = recipe.instruction_sections().begin();
    bool paragraph_open = false;
    for (int i=0; i<recipe.instructions().size(); i++) {
      while (section != recipe.instruction_sections().end() && section->first == i) {
        if (paragraph_open) {
          stream << "</p>\n";
          paragraph_open = false;
        };
        stream << "    <h4>" << html_encode(section->second) << "</h4>\n";
        section++;
      };
      if (!paragraph_open) {
        stream << "    <p>";
        paragraph_open = true;
      } else
        stream << "<br/>";
      stream << html_encode(recipe.instructions()[i]);
    };
    stream << "</p>\n";
  };
}

string recipe_to_html(Recipe &recipe, string (*translate)(const char *, const char *)) {
  ostringstream stream;
  stream << "<html>\n"
         << "  <head>\n";
  if (!recipe.title().empty()) {
    stream << "    <title>" << html_encode(recipe.title()) << "</title>\n";
  };
  stream << "  </head>\n"
         << "  <body>\n";
  stream_recipe(stream, recipe, translate);
  stream << "  </body>\n"
         << "</html>";
  return stream.str();
}

string recipes_to_html(vector<Recipe> &recipes, string (*translate)(const char *, const char *)) {
  ostringstream stream;
  stream << "<html>\n"
         << "  <head>\n"
         << "    <title>AnyMeal Recipe Export</title>\n"
         << "  </head>\n"
         << "  <body>\n";
  for (vector<Recipe>::iterator recipe=recipes.begin(); recipe!=recipes.end(); recipe++) {
    if (recipe!=recipes.begin())
      stream << "    <div style=\"page-break-before:always\"></div>\n";
    stream_recipe(stream, *recipe, translate);
  };
  stream << "  </body>\n"
         << "</html>";
  return stream.str();
}
