#pragma once

#include "glm/vec3.hpp"

#include <optional>
#include <tuple>

class Light;

class SceneElement
{
   public:
      using Intersection = std::tuple<glm::vec3, float>;
      virtual std::optional<Intersection> TestIntersection(const glm::vec3& cam_pos, const glm::vec3& ray_dir) const = 0;
      virtual glm::vec3 CalcLightOuput(const glm::vec3& ray_dir, const glm::vec3& intersection_point, const Light& light) const = 0;
      virtual glm::vec3 GetAmbientlight() const = 0;
};
