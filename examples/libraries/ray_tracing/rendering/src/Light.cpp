#include "Light.h"
#include "BuilderUtility.h"

using namespace BuilderUtility;

const Light::Builder& Light::Builder::ParseLight(const std::string& data)
{
   std::string cut = data.substr(2, data.length() - 4);

   for (const auto attribute : ParseParams(cut))
   {
      if (attribute.find(POS) == 0)
      {
         m_Pos = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
      else if (attribute.find(COL) == 0)
      {
         m_Col = ParseVec3(attribute.substr(OFFSET_3CHAR));
      }
   }

   return *this;
}
