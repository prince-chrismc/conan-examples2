#pragma once

#include "glm/vec3.hpp"

#include <string>

class Camera
{
   public:
      Camera(const glm::vec3& pos, const unsigned int& FOV, const unsigned int& focal, const double& angle) :
             m_Pos(pos), m_FOV(FOV), m_Focal(focal), m_AspectRatio(angle) {}
      Camera() : Camera(glm::vec3(), 0, 0, 0.0) {}

      void GetImageDimensions(int* out_width, int* out_height) const;
      glm::vec3 GetPosition() const { return m_Pos; }
      double GetAspectRatio() const { return m_AspectRatio; }
      unsigned int GetFeildOfView() const { return m_FOV; }

      class Builder
      {
         public:
            Builder() = default;
            Builder(const Builder&) = delete;
            void operator=(const Builder&) = delete;

            Builder& ParseCamera(const std::string& data);
            Camera GetCamera() const { return Camera(m_Pos, m_FOV, m_Focal, m_AspectRatio); }

         private:
            glm::vec3 m_Pos;
            unsigned int m_FOV;
            unsigned int m_Focal;
            double m_AspectRatio;

            const char* FOV = "fov:";
            const char* F = "f:";
            const char* A = "a:";
      };

   private:
      glm::vec3 m_Pos;
      unsigned int m_FOV;
      unsigned int m_Focal;
      double m_AspectRatio;
};
