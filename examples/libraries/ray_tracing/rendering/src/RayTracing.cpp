#include "Scene.h"

#include <iostream>

int main()
{
   std::cout << "Welcome to Ray Tracing Effect!" << std::endl;

   // ---------------------------------------------------------------------------------------------
   std::cout << "Loading Scene....";
   Scene scene("assets/scene2.txt");
   std::cout << "  COMPLETE!" << std::endl;

   return 0;
}
