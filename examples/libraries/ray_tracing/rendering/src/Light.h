#pragma once

#include "glm/vec3.hpp"

#include <string>

class Light
{
   public:
      Light(const glm::vec3& pos, const glm::vec3& col) : m_Pos(pos), m_Col(col) {}
      Light() : Light(glm::vec3(), glm::vec3()) {}

       glm::vec3 GetPosition() const { return m_Pos; }
       glm::vec3 GetColor() const { return m_Col; }

      class Builder
      {
         public:
            Builder() = default;
            Builder(const Builder&) = delete;
            void operator=(const Builder&) = delete;

            const Builder& ParseLight(const std::string& data);
            Light GetLight() const { return Light(m_Pos, m_Col); }

         private:
            glm::vec3 m_Pos;
            glm::vec3 m_Col;

            const char* COL = "col:";
      };

   private:
      glm::vec3 m_Pos;
      glm::vec3 m_Col;
};
