#ifndef Vector3_h
#define Vector3_h

class Vector3{
  
public:

  float x,y,z;

  Vector3();
  
  Vector3(float xyz);

  Vector3(float x, float y, float z);
  
  void set(float x, float y, float z);
  
  void negate();
  
  float dot (const Vector3& v) const;
  
  float angleBetween(const Vector3& v) const;
  
  Vector3 cross(const Vector3& v) const;
  
  float length () const;
  
  void normalize();
  
  float* data();

  Vector3& operator+= (const Vector3& v);
  
  Vector3& operator-= (const Vector3& v);
  
  Vector3& operator*= (float s);
  
  Vector3& operator/= (float s);

};

namespace Math{

Vector3 operator+ (const Vector3& v1, const Vector3& v2);

Vector3 operator- (const Vector3& v1, const Vector3& v2);

Vector3 operator* (const Vector3& v, float s);

Vector3 operator* (float s, const Vector3& v);

Vector3 operator/ (const Vector3& v, float s);

}

#endif
