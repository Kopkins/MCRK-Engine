#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Vector3.h"


Vector3::Vector3() :
  x(), y(), z()
{
  
}

Vector3::Vector3(float xyz) :
  x(xyz), y(xyz), z(xyz)
{
  
}

Vector3::Vector3(float x, float y, float z) :
  x(x), y(y), z(z)
{
  
}

void Vector3::set (float xin, float yin, float zin){
  x=xin;
  y=yin;
  z=zin;
}

void Vector3::negate(){
  x=-1*x;
  y=-1*y;
  z=-1*z;
}

float Vector3::dot(const Vector3& v) const{
  return (x*v.x+y*v.y+z*v.z);
}

float Vector3::angleBetween(const Vector3& v) const{
  return (acos(dot(v)/(length()*v.length())));
}

Vector3 Vector3::cross (const Vector3& v) const {
  return Vector3(y*v.z-v.y*z,x*v.z-v.x*z,x*v.y-v.x*y);
}

float Vector3::length() const{
  return (sqrt(x*x+y*y+z*z));
}

void Vector3::normalize(){
  x=x/length();
  y=y/length();
  z=z/length();
}

Vector3& Vector3::operator+=(const Vector3& v){
  this->x=x+v.x;
  this->y=y+v.y;
  this->z=z+v.z;
  return *this;
}

Vector3& Vector3::operator-=(const Vector3& v){
  this->x=x-v.x;
  this->y=y-v.y;
  this->z=z-v.z;
  return *this;
}

Vector3& Vector3::operator*=(float s){
  this->x=x*s;
  this->y=y*s;
  this->z=z*s;
  return *this;
}

Vector3& Vector3::operator/=(float s){
  this->x=x/s;
  this->y=y/s;
  this->z=z/s;
  return *this;
}

glm::vec3 Vector3::toVec3(){
  return glm::vec3(x, y, z);
}

namespace Math{

  Vector3 operator+ (const Vector3& v1, const Vector3& v2){
    return Vector3(v1.x+v2.x, v1.y+v2.y,v1.z+v2.z);
  }

  Vector3 operator- (const Vector3& v1, const Vector3& v2){
    return Vector3(v1.x-v2.x, v1.y-v2.y,v1.z-v2.z);
  }

  Vector3 operator* (const Vector3& v, float s){
    return Vector3(v.x*s, v.y*s,v.z*s);
  }

  Vector3 operator* (float s, const Vector3& v){
     return Vector3(v.x*s, v.y*s,v.z*s);
   }

  Vector3 operator/ (const Vector3& v, float s){
    return Vector3(v.x/s, v.y/s,v.z/s);
  }

}
