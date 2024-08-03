/* AnyMeal recipe management software
   Copyright (C) 2020, 2023 Jan Wedekind

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
#include <sstream>
#include "export.hh"


using namespace std;

string recipe_to_mealmaster(Recipe &recipe) {
  ostringstream result;
  // Output recipe header.
  result << "MMMMM----------------Meal-Master recipe exported by AnyMeal-----------------\r\n"
         << "\r\n"
         << "      Title: " << recipe.title() << "\r\n"
         << " Categories: ";
  set<string>::iterator category = recipe.categories().begin();
  while (category != recipe.categories().end()) {
    result << *category++;
    if (category != recipe.categories().end())
      result << ", ";
    else
      break;
  };
  result << "\r\n"
         << "      Yield: " << recipe.servings() << " " << recipe.servings_unit() << "\r\n"
         << "\r\n";
  if (!recipe.ingredients().empty()) {
    // Output ingredients.
    vector<pair<int, string> > sections = recipe.ingredient_sections();
    vector<pair<int, string> >::iterator section = sections.begin();
    for (int i=0; i<recipe.ingredients().size(); i++) {
      while (section != sections.end() && section->first == i) {
        // Output an ingredient section.
        int n = 71 - section->second.length();
        result << "MMMMM" << string(n / 2, '-') << section->second << string((n + 1) / 2, '-') << "\r\n";
        section++;
      };
      Ingredient ingredient = recipe.ingredients()[i];
      string amount;
      if (ingredient.amount_float() > 0) {
        ostringstream s;
        s << ingredient.amount_float();
        amount = s.str();
      } else if (ingredient.amount_integer() > 0) {
        ostringstream s;
        s << ingredient.amount_integer();
        if (ingredient.amount_numerator() > 0) {
          s << " " << ingredient.amount_numerator() << "/" << ingredient.amount_denominator();
        };
        amount = s.str();
      } else if (ingredient.amount_numerator() > 0) {
        ostringstream s;
        s << ingredient.amount_numerator() << "/" << ingredient.amount_denominator();
        amount = s.str();
      };
      amount = amount.substr(0, 7);
      result << string(7 - amount.length(), ' ') << amount;
      result << " ";
      if (ingredient.unit().empty()) {
        result << "  ";
      } else {
        result << ingredient.unit();
      };
      result << " ";
      string txt = ingredient.text();
      // Break up ingredient text using continuation lines if necessary.
      while (!txt.empty()) {
        if (txt.length() <= 28) {
          result << txt << "\r\n";
          txt = "";
        } else {
          int offset;
          // break at space character.
          size_t pos = txt.rfind(" ", 28);
          if (pos == string::npos) {
            // emergency line break if no space character found.
            pos = 28;
            // ensure potential UTF8 sequences are not broken up.
            while (pos > 1 && (txt.at(pos) & 0xC0) == 0x80)
              pos--;
            offset = 0;
          } else
            offset = 1;
          result << txt.substr(0, pos) << "\r\n"
                 << "           -";
          txt = txt.substr(pos + offset, txt.length() - pos - offset);
        };
      };
      if (section != sections.end() && section->first == i + 1) {
        // Newline before next ingredient section.
        result << "\r\n";
      };
    };
    result << "\r\n";
  };
  if (!recipe.instructions().empty()) {
    bool force_newline = false;
    // Output recipe instructions.
    vector<pair<int, string> > sections = recipe.instruction_sections();
    vector<pair<int, string> >::iterator section = sections.begin();
    for (int i=0; i<recipe.instructions().size(); i++) {
      while (section != sections.end() && section->first == i) {
        // Output an instruction section.
        int n = 71 - section->second.length();
        result << "MMMMM" << string(n / 2, '-') << section->second << string((n + 1) / 2, '-') << "\r\n";
        force_newline = false;
        section++;
      };
      string txt = recipe.instructions()[i];
      if (txt.empty()) {
        result << "\r\n";
        force_newline = false;
      } else {
        while (!txt.empty()) {
          int text_width = force_newline ? 74 : 75;
          if (txt.length() <= text_width) {
            result << "  " << (force_newline ? ":" : "") << txt << "\r\n";
            txt = "";
          } else {
            // break at space character.
            int offset;
            size_t pos = txt.rfind(" ", text_width);
            if (pos == string::npos) {
              // emergency line break if no space character found.
              pos = text_width;
              // ensure potential UTF8 sequences are not broken up.
              while (pos > 1 && (txt.at(pos) & 0xC0) == 0x80)
                pos--;
              offset = 0;
            } else
              offset = 1;
            result << "  " << (force_newline ? ":" : "") << txt.substr(0, pos) << "\r\n";
            txt = txt.substr(pos + offset, txt.length() - pos - offset);
          };
          force_newline = false;
        };
        force_newline = true;
      };
      if (section != sections.end() && section->first == i + 1) {
        // Newline before next instruction section.
        result << "\r\n";
      };
    };
    result << "\r\n";
  };
  result << "MMMMM";
  return result.str();
}
