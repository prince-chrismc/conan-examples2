#include "Plane.h"
#include "Light.h"
#include "BuilderUtility.h"

#include "glm/geometric.hpp"

#include <algorithm>

using namespace BuilderUtility;

std::optional<SceneElement::Intersection> Plane::TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir) const
{
   const auto normal = glm::normalize(m_Normal);
   float denom = glm::dot(normal, ray_dir);

   if (abs(denom) > 1e-6)
   {
      glm::vec3 p0l0 = m_Pos - ray_dir;

      const auto distance = glm::dot(p0l0, normal) / denom;
      const auto intersection = cam_pos + ray_dir * (distance);

      return (distance >= 0) ? std::make_optional(std::make_tuple(intersection, distance)) : std::nullopt;
   }

   return std::nullopt;
}

glm::vec3 Plane::CalcLightOuput(const glm::vec3 & ray_dir, const glm::vec3 & intersection_point, const Light & light) const
{
   glm::vec3 normal = glm::normalize(m_Normal);
   glm::vec3 v = -ray_dir;

   glm::vec3 light_direction = glm::normalize(intersection_point - light.GetPosition());
   glm::vec3 reflection = glm::reflect(light_direction, normal);

   float ln = glm::dot(normal, light_direction);
   float rv = glm::dot(reflection, v);

   ln = std::max(ln, 0.0f);
   rv = std::max(rv, 0.0f);

   rv = std::pow(rv, m_Shine);

   return light.GetColor() * (m_Dif*ln + (m_Spe*rv));
}

const Plane::Builder& Plane::Builder::ParsePlane(const std::string& data)
{
   std::string cut = data.substr(2, data.length() - 4);

   for (const auto attribute : ParseParams(cut))
   {
      if (attribute.find(POS) == 0)
      {
         m_Pos = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(NOR) == 0)
      {
         m_Normal = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(AMB) == 0)
      {
         m_Amb = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(DIF) == 0)
      {
         m_Dif = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(SPE) == 0)
      {
         m_Spe = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(SHI) == 0)
      {
         m_Shine = ParseFloat(attribute.substr(OFFSET_3CHAR));
      }
   }

   return *this;
}
