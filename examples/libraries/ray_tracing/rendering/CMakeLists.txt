cmake_minimum_required(VERSION 3.15)
project(ray_tracing)

find_package(glm CONFIG REQUIRED)
find_package(cimg CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(
  ray_tracing
  src/BuilderUtility.cpp
  src/BuilderUtility.h
  src/Camera.cpp
  src/Camera.h
  src/Light.cpp
  src/Plane.cpp
  src/RayTracing.cpp
  src/Scene.h
  src/SceneFile.cpp
  src/Sphere.cpp
  src/Triangle.cpp
  src/Light.h
  src/Plane.h
  src/Scene.cpp
  src/SceneElement.h
  src/SceneFile.h
  src/Sphere.h
  src/Triangle.h)
target_compile_features(ray_tracing PRIVATE cxx_std_17)
target_link_libraries(ray_tracing PRIVATE glm::glm cimg::cimg Threads::Threads)
