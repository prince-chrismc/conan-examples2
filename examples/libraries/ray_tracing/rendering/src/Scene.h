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

#include "SceneFile.h"
#include "Camera.h"
#include "Light.h"
#include "SceneElement.h"
#include <memory>

#define cimg_display 0
#include "CImg.h"

class Scene : private SceneFile
{
   public:
      Scene(const char* path);

      void Display();

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
