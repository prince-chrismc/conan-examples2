#include "Sphere.h"
#include "Light.h"
#include "BuilderUtility.h"

#include "glm/geometric.hpp"   //normalize

#include <algorithm>

using namespace BuilderUtility;

std::optional<SceneElement::Intersection> Sphere::TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir) const
{
   // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

   float intersection_zero, intersection_one;
   glm::vec3 length = m_Pos - cam_pos;
   float tca = glm::dot(length, ray_dir);

   float rad_dir_squared = glm::dot(length, length) - tca * tca;
   if (rad_dir_squared > (m_Radius * m_Radius)) return std::nullopt; // doesn't pass through

   float thc = std::sqrt((m_Radius * m_Radius) - rad_dir_squared);
   intersection_zero = tca - thc;
   intersection_one = tca + thc;

   if (intersection_zero > intersection_one) std::swap(intersection_zero, intersection_one);

   if (intersection_zero < 0.0f)
   {
      intersection_zero = intersection_one;
      if (intersection_zero < 0.0f) return std::nullopt; // both intersection are negative
   }

   const auto distance = intersection_zero;
   const auto intersection = cam_pos + ray_dir * intersection_zero;

   return std::make_optional(std::make_tuple(intersection, distance));
}

glm::vec3 Sphere::CalcLightOuput(const glm::vec3& ray_dir, const glm::vec3 & intersection_point, const Light & light) const
{
   glm::vec3 normal = glm::normalize(m_Pos - intersection_point);
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

const Sphere::Builder& Sphere::Builder::ParseSphere(const std::string& data)
{
   std::string cut = data.substr(2, data.length() - 4);

   for (const auto attribute : ParseParams(cut))
   {
      if (attribute.find(POS) == 0)
      {
         m_Pos = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(RAD) == 0)
      {
         m_Radius = ParseFloat(attribute.substr(OFFSET_3CHAR));
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
