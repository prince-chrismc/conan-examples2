#pragma once

#include "SceneElement.h"

#include <string>

class Triangle : public SceneElement
{
   public:
      Triangle() = default;
      Triangle(const glm::vec3& vert1, const glm::vec3& vert2, const glm::vec3& vert3, const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe, const float& shine) :
         m_Vert1(vert1), m_Vert2(vert2), m_Vert3(vert3), m_Amb(amb), m_Dif(dif), m_Spe(spe), m_Shine(shine) {}

      std::optional<SceneElement::Intersection> TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir) const;
      glm::vec3 CalcLightOuput(const glm::vec3& ray_dir, const glm::vec3& intersection_point, const Light& light) const;

      glm::vec3 GetAmbientlight() const { return m_Amb; }

      class Builder
      {
      public:
         Builder() = default;
         Builder(const Builder&) = delete;
         void operator=(const Builder&) = delete;

         const Builder& ParseTriangle(const std::string& data);
         Triangle GetTriangle() const { return Triangle(m_Vert1, m_Vert2, m_Vert3, m_Amb, m_Dif, m_Spe, m_Shine); }

      private:
         glm::vec3 m_Vert1;
         glm::vec3 m_Vert2;
         glm::vec3 m_Vert3;
         glm::vec3 m_Amb;
         glm::vec3 m_Dif;
         glm::vec3 m_Spe;
         float m_Shine;

         const char* V1 = "v1:";
         const char* V2 = "v2:";
         const char* V3 = "v3:";
      };

private:
   glm::vec3 m_Vert1;
   glm::vec3 m_Vert2;
   glm::vec3 m_Vert3;
   glm::vec3 m_Amb;
   glm::vec3 m_Dif;
   glm::vec3 m_Spe;
   float m_Shine;
};
