#pragma once

#include "SceneFile.h"
#include "Camera.h"
#include "Light.h"
#include "SceneElement.h"

#define cimg_display 0
#include "CImg.h"

#include <memory>


class Scene : private SceneFile
{
   public:
      Scene(const char* path);

   private:
      Camera m_Camera;
      std::vector<Light> m_Lights;
      std::vector<std::shared_ptr<SceneElement>> m_Objects;

      cimg_library::CImg<float> m_Image;

      void ExtractLights();
      void ExtractSpheres();
      void ExtractTrianlges();
      void ExtractPlanes();

      struct IntersectingObject
      {
         IntersectingObject(const glm::vec3& point, const float& dis, const std::shared_ptr<SceneElement>& elem) : m_Point(point), m_Distance(dis), m_Element(elem) {}
         IntersectingObject() : IntersectingObject(glm::vec3(0.0f), 0.0f, nullptr) {}

         glm::vec3 m_Point;
         float m_Distance;
         std::shared_ptr<SceneElement> m_Element;
      };

      void GenerateScene();
      glm::vec3 CalcRayDirection(const int x_val, const int y_val);
      IntersectingObject FindNearestIntersectingObject(const glm::vec3& ray_dir);
      bool IsLightObstructed(const Light& light, const IntersectingObject& target);
};
