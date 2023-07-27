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
