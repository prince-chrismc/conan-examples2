#pragma once

#include "SceneElement.h"

#include <string>

class Sphere : public SceneElement
{
   public:
      Sphere() = default;
      Sphere(const glm::vec3& pos, const float& rad, const glm::vec3& amb, const glm::vec3& dif, const glm::vec3& spe, const float& shine) :
             m_Pos(pos), m_Radius(rad), m_Amb(amb), m_Dif(dif), m_Spe(spe), m_Shine(shine) {}

      std::optional<SceneElement::Intersection> TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir) const;
      glm::vec3 CalcLightOuput(const glm::vec3& ray_dir, const glm::vec3& intersection_point, const Light& light) const;

      glm::vec3 GetAmbientlight() const { return m_Amb; }

      class Builder
      {
      public:
         Builder() = default;
         Builder(const Builder&) = delete;
         void operator=(const Builder&) = delete;

         const Builder& ParseSphere(const std::string& data);
         Sphere GetSphere() const { return Sphere(m_Pos, m_Radius, m_Amb, m_Dif, m_Spe, m_Shine); }

      private:
         glm::vec3 m_Pos;
         float m_Radius;
         glm::vec3 m_Amb;
         glm::vec3 m_Dif;
         glm::vec3 m_Spe;
         float m_Shine;

         const char* RAD = "rad:";
      };

   private:
      glm::vec3 m_Pos;
      float m_Radius;
      glm::vec3 m_Amb;
      glm::vec3 m_Dif;
      glm::vec3 m_Spe;
      float m_Shine;
};
