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

#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Plane.h"

#include "glm/geometric.hpp"

#include <string>
#include <future>
#include <tuple>

#ifdef _WIN32
#include <corecrt_math_defines.h>
#endif

using cimg_library::CImg;

static const float LIGHT_BIAS = 1e-3f;

Scene::Scene(const char* path) : SceneFile(path)
{
   if(m_Elements.size() > 0)
   {
      m_Camera = Camera::Builder().ParseCamera(GetAttributes("camera")).GetCamera();
      ExtractLights();
      ExtractSpheres();
      ExtractTrianlges();
      ExtractPlanes();

      GenerateScene();
   }
}

void Scene::ExtractLights()
{
   std::string light_attr = "";
   while ((light_attr = GetAttributes("light")) != "")
   {
      m_Lights.push_back(Light::Builder().ParseLight(light_attr).GetLight());
   }
}
void Scene::ExtractSpheres()
{
   std::string sphere_attr = "";
   while ((sphere_attr = GetAttributes("sphere")) != "")
   {
      m_Objects.push_back(std::make_shared<Sphere>(Sphere::Builder().ParseSphere(sphere_attr).GetSphere()));
   }
}
void Scene::ExtractTrianlges()
{
   std::string triangle_attr = "";
   while ((triangle_attr = GetAttributes("triangle")) != "")
   {
      m_Objects.push_back(std::make_shared<Triangle>(Triangle::Builder().ParseTriangle(triangle_attr).GetTriangle()));
   }
}
void Scene::ExtractPlanes()
{
   std::string plane_attr = "";
   while ((plane_attr = GetAttributes("plane")) != "")
   {
      m_Objects.push_back(std::make_shared<Plane>(Plane::Builder().ParsePlane(plane_attr).GetPlane()));
   }
}

void Scene::GenerateScene()
{
   int width = 0, height = 0;
   m_Camera.GetImageDimensions(&width, &height);
   m_Image = CImg<float>(width, height, 1, 3, 0);

   std::vector<std::future<std::vector<glm::vec3>>> results;
   for (int x = 0; x < width; x += 1)
   {
      results.push_back( std::async(std::launch::async, [this, x, height]{
         std::vector<glm::vec3> retval;
         for (int y = 0; y < height; y += 1)
         {
            glm::vec3 rayDirection = CalcRayDirection(x, y);

            IntersectingObject target = FindNearestIntersectingObject(rayDirection);
            glm::vec3 pixelColor(0.0f);

            if (target.m_Element)
            {
               for (const Light light : m_Lights)
               {
                  pixelColor += target.m_Element->GetAmbientlight();
                  if (!IsLightObstructed(light, target))
                  {
                     pixelColor += target.m_Element->CalcLightOuput(rayDirection, target.m_Point, light);
                  }
               }
            }
            retval.push_back(pixelColor);
         }
         return retval;
      }));
   }

   int x = 0, y = 0;
   for (auto itor = results.begin(); itor != results.end(); itor++)
   {
      for(auto pixelColor : itor->get())
      {
         float color[3] = { pixelColor.r, pixelColor.g, pixelColor.b };
         m_Image.draw_point(x, y++, color);

         if (y >= height) { y = 0; x++; }
      }
   }

   m_Image.normalize(0, 255);
   m_Image.blur(0.5f, 0.5f, 0.5f);
   m_Image.save_png("render2.png", true);
}

glm::vec3 Scene::CalcRayDirection(const int x_val, const int y_val)
{
   int width = 0, height = 0;
   m_Camera.GetImageDimensions(&width, &height);

   double PCx = (2.0 * ((x_val + 0.5) / width) - 1.0) * tan(m_Camera.GetFeildOfView() / 2.0 * M_PI / 180.0) * m_Camera.GetAspectRatio();
   double PCy = (1.0 - 2.0 * ((y_val + 0.5) / height)) * tan(m_Camera.GetFeildOfView() / 2.0 * M_PI / 180.0);

   return glm::normalize(glm::vec3(PCx, PCy, -1) - m_Camera.GetPosition());
}

Scene::IntersectingObject Scene::FindNearestIntersectingObject(const glm::vec3& ray_dir)
{
   IntersectingObject target;

   for (const auto elem : m_Objects)
   {

      if (auto intersection = elem->TestIntersection(m_Camera.GetPosition(), ray_dir))
      {
         float distance = std::get<1>(intersection.value());
         glm::vec3 intersectpoint = std::get<0>(intersection.value());
         if (!target.m_Element || distance < target.m_Distance)
         {
            target = IntersectingObject(intersectpoint, distance, elem);
         }
      }
   }

   return target;
}

bool Scene::IsLightObstructed(const Light& light, const IntersectingObject& target)
{
   glm::vec3 lightRay = glm::normalize(light.GetPosition() - target.m_Point);
   glm::vec3 lightRayWithBias = (LIGHT_BIAS * lightRay) + target.m_Point;

   for (const auto elem : m_Objects)
   {
      if (elem->TestIntersection(lightRayWithBias, lightRay).has_value()) return true;
   }

   return false;
}
