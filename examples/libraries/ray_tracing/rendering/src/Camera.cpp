

#include "Camera.h"
#include "BuilderUtility.h"

#include "glm/trigonometric.hpp"

using namespace BuilderUtility;

Camera::Builder& Camera::Builder::ParseCamera(const std::string& data)
{
   std::string cut = data.substr(2, data.length() - 4);

   for (const auto attribute : ParseParams(cut))
   {
      if (attribute.find(POS) == 0)
      {
         m_Pos = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(FOV) == 0)
      {
         m_FOV = ParseUint(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(F) == 0)
      {
         m_Focal = ParseUint(attribute.substr(OFFSET_1CHAR));
      }
      else if (attribute.find(A) == 0)
      {
         m_AspectRatio = ParseDouble(attribute.substr(OFFSET_1CHAR));
      }
   }

   return *this;
}

void Camera::GetImageDimensions(int* out_width, int* out_height) const
{
   *out_height = 2*static_cast<int>(m_Focal * std::tan(glm::radians(m_FOV / 2.0)));
   *out_width = static_cast<int>(*out_height * m_AspectRatio);
}
