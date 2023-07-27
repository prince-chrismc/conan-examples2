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
   for (int x = 0; x < width; x += 1) // For each vertical line
   {
      results.push_back( std::async(std::launch::async, [this, x, height]{ // Create a task
         std::vector<glm::vec3> retval;
         // Sequentially
         for (int y = 0; y < height; y += 1) // For each pixel in the line
         {
            glm::vec3 rayDirection = CalcRayDirection(x, y); // Get the vector to the camera

            // Does this vector hit any objects
            const auto optionalTarget = FindNearestIntersectingObject(rayDirection);
            if(!optionalTarget.has_value())
            {
               continue;
            }

            const auto target = optionalTarget.value();
            glm::vec3 pixelColor(0.0f);

            for (const Light light : m_Lights)
            {
               // Start with the base glow of the element
               pixelColor += target.m_Element->GetAmbientlight();
               if (!IsLightObstructed(light, target)) // Is the light blocked by other elements
               {
                  // Add the reflected light for the color
                  pixelColor += target.m_Element->CalcLightOuput(rayDirection, target.m_Point, light);
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

std::optional<Scene::IntersectingObject> Scene::FindNearestIntersectingObject(const glm::vec3& ray_dir)
{
   std::optional<IntersectingObject> target;

   for (const auto elem : m_Objects) // For each element in the Scene
   {
       // Is the object seen by the camera
      if (auto intersection = elem->TestIntersection(m_Camera.GetPosition(), ray_dir))
      {
         // If so...
         float distance = std::get<1>(intersection.value());
         glm::vec3 intersectpoint = std::get<0>(intersection.value());

         if (!target.m_Element || // Is it the first one we've seen
            distance < target.m_Distance) // Or, is it closer then the last one
         {
            // Save it to compare against
            target = std::make_optional(IntersectingObject(intersectpoint, distance, elem));
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
