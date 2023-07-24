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

#include "Camera.h"
#include "glm/trigonometric.hpp"

#include "BuilderUtility.h"
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
