#pragma once

#include <vector>
#include <string>
#include <iosfwd> // for ifstream

class SceneFile
{
   public:
      SceneFile(const char* path);

      std::string GetAttributes(const char* name);

   protected:
      class ObjDescriptor
      {
         public:
            ObjDescriptor(const char* name, const std::string& attributes) : m_Name(name), m_Attributes(attributes) {}

            bool DoesNameMatch(const char* name) const { return !std::string(name).compare(m_Name); }
            std::string GetAttributes() const { return m_Attributes; }

         private:
            const char* m_Name;
            std::string m_Attributes;
      };

      std::vector<ObjDescriptor> m_Elements;

   private:
      void ExtractCamera(std::ifstream* file);
      void ExtractSphere(std::ifstream* file);
      void ExtractModel(std::ifstream* file);
      void ExtractLight(std::ifstream* file);
      void ExtractTriangle(std::ifstream* file);
      void ExtractPlane(std::ifstream* file);

      static std::string GetNextLine(std::ifstream* file);
};
