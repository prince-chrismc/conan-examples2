/*
MIT License

Copyright (c) 2017 Chris McArthur, prince.chrismc(at)gmail(dot)com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "glm/vec3.hpp"
#include <string>
#include <vector>

using std::string_view_literals::operator "" sv;

namespace BuilderUtility
{
      std::vector<std::string> ParseParams(std::string cut);
      glm::vec3 ParseVec3(std::string attribute);
      unsigned int ParseUint(std::string attribute);
      double ParseDouble(std::string attribute);
      float ParseFloat(std::string attribute);

      constexpr const auto POS = "pos:"sv;

      constexpr const auto AMB = "amb:"sv;
      constexpr const auto DIF = "dfi:"sv;
      constexpr const auto SPE = "spe:"sv;
      constexpr const auto SHI = "shi:"sv;

      enum NameOffset { OFFSET_3CHAR = 5, OFFSET_2CHAR = 4, OFFSET_1CHAR = 3 };
};
