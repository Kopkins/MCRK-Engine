#include <cmath>
#include "Matrix4.h"

namespace Math
{

  Matrix4::Matrix4 () :
      m_00 (0), m_01 (0), m_02 (0), m_03 (0), m_10 (0), m_11 (0), m_12 (0), m_13 (
	  0), m_20 (0), m_21 (0), m_22 (0), m_23 (0), m_30 (0), m_31 (0), m_32 (
	  0), m_33 (0)
  {
  }

  Matrix4::Matrix4 (bool makeIdentity) :
      m_00 (1), m_01 (0), m_02 (0), m_03 (0), m_10 (0), m_11 (1), m_12 (0), m_13 (
	  0), m_20 (0), m_21 (0), m_22 (1), m_23 (0), m_30 (0), m_31 (0), m_32 (
	  0), m_33 (1)
  {
  }

  Matrix4::Matrix4 (const float arrayOf16[16]) :
      m_00 (arrayOf16[0]), m_01 (arrayOf16[1]), m_02 (arrayOf16[2]), m_03 (
	  arrayOf16[3]), m_10 (arrayOf16[4]), m_11 (arrayOf16[5]), m_12 (
	  arrayOf16[6]), m_13 (arrayOf16[7]), m_20 (arrayOf16[8]), m_21 (
	  arrayOf16[9]), m_22 (arrayOf16[10]), m_23 (arrayOf16[11]), m_30 (
	  arrayOf16[12]), m_31 (arrayOf16[13]), m_32 (arrayOf16[14]), m_33 (
	  arrayOf16[15])
  {
  }

  Matrix4::Matrix4 (const Matrix3& m) :
      m_00 (m.getRight ().x), m_01 (m.getRight ().y), m_02 (m.getRight ().z), m_03 (
	  0), m_10 (m.getUp ().x), m_11 (m.getUp ().y), m_12 (m.getUp ().z), m_13 (
	  0), m_20 (m.getBack ().x), m_21 (m.getBack ().y), m_22 (
	  m.getBack ().z), m_23 (0), m_30 (0), m_31 (0), m_32 (0), m_33 (1)
  {
  }

  void
  Matrix4::setFromMatrix (const Matrix3& m)
  {
    m_00 = m.getRight ().x;
    m_01 = m.getRight ().y;
    m_02 = m.getRight ().y;
    m_03 = 0;
    m_10 = m.getUp ().x;
    m_11 = m.getUp ().y;
    m_12 = m.getUp ().z;
    m_13 = 0;
    m_20 = m.getBack ().x;
    m_21 = m.getBack ().y;
    m_22 = m.getBack ().z;
    m_23 = 0;
    m_30 = 0;
    m_31 = 0;
    m_32 = 0;
    m_33 = 1;
  }

  void
  Matrix4::setToIdentity ()
  {
    setToZero ();
    m_00 = 1;
    m_11 = 1;
    m_22 = 1;
    m_33 = 1;
  }

  void
  Matrix4::setToZero ()
  {
    m_00 = 0;
    m_01 = 0;
    m_02 = 0;
    m_03 = 0;
    m_10 = 0;
    m_11 = 0;
    m_12 = 0;
    m_13 = 0;
    m_20 = 0;
    m_21 = 0;
    m_22 = 0;
    m_23 = 0;
    m_30 = 0;
    m_31 = 0;
    m_32 = 0;
    m_33 = 1;
  }

  void
  Matrix4::setToTranslation (const Vector3& translation)
  {
    setToZero ();
    m_30 = translation.x;
    m_31 = translation.y;
    m_32 = translation.z;
    m_33 = 1;
  }

  void
  Matrix4::setToScale (float scale)
  {
    setToZero ();
    m_00 = scale;
    m_11 = scale;
    m_22 = scale;
    m_33 = 1;
  }

  void
  Matrix4::setToScale (float scaleX, float scaleY, float scaleZ)
  {
    setToZero ();
    m_00 = scaleX;
    m_11 = scaleY;
    m_22 = scaleZ;
    m_33 = 1;
  }

  void
  Matrix4::setToRotationX (float angleDegrees)
  {
    setToZero ();
    m_00 = 1;
    m_11 = cos (angleDegrees);
    m_12 = sin (angleDegrees);
    m_21 = -1 * sin (angleDegrees);
    m_22 = cos (angleDegrees);
    m_33 = 1;
  }

  void
  Matrix4::setToRotationY (float angleDegrees)
  {
    setToZero ();
    m_00 = cos (angleDegrees);
    m_02 = -1 * sin (angleDegrees);
    m_11 = 1;
    m_20 = sin (angleDegrees);
    m_22 = cos (angleDegrees);
    m_33 = 1;
  }

  void
  Matrix4::setToRotationZ (float angleDegrees)
  {
    setToZero ();
    m_00 = cos (angleDegrees);
    m_01 = sin (angleDegrees);
    m_10 = -1 * sin (angleDegrees);
    m_11 = cos (angleDegrees);
    m_22 = 1;
    m_33 = 1;
  }

  void
  Matrix4::setFrontAngleAxis (float angleDegrees, const Vector3& unitAxis)
  {
    setToZero ();
    m_00 = cos (angleDegrees)
	+ unitAxis.x * unitAxis.x * (1 - cos (angleDegrees));
    m_01 = unitAxis.y * unitAxis.x * (1 - cos (angleDegrees))
	+ unitAxis.z * sin (angleDegrees);
    m_02 = unitAxis.z * unitAxis.x * (1 - cos (angleDegrees))
	- unitAxis.y * sin (angleDegrees);
    m_10 = unitAxis.y * unitAxis.x * (1 - cos (angleDegrees))
	- unitAxis.z * sin (angleDegrees);
    m_11 = cos (angleDegrees)
	+ unitAxis.y * unitAxis.y * (1 - cos (angleDegrees));
    m_12 = unitAxis.z * unitAxis.y * (1 - cos (angleDegrees))
	+ unitAxis.x * sin (angleDegrees);
    m_20 = unitAxis.x * unitAxis.z * (1 - cos (angleDegrees))
	+ unitAxis.y * sin (angleDegrees);
    m_21 = unitAxis.y * unitAxis.z * (1 - cos (angleDegrees))
	- unitAxis.x * sin (angleDegrees);
    m_22 = cos (angleDegrees)
	+ unitAxis.z * unitAxis.z * (1 - cos (angleDegrees));
  }

  void
  Matrix4::getAsArray (float arrayOf16[16]) const
  {
    arrayOf16[0] = m_00;
    arrayOf16[1] = m_01;
    arrayOf16[2] = m_02;
    arrayOf16[3] = m_03;
    arrayOf16[4] = m_10;
    arrayOf16[5] = m_11;
    arrayOf16[6] = m_12;
    arrayOf16[7] = m_13;
    arrayOf16[8] = m_20;
    arrayOf16[9] = m_21;
    arrayOf16[10] = m_22;
    arrayOf16[11] = m_23;
    arrayOf16[12] = m_30;
    arrayOf16[13] = m_31;
    arrayOf16[14] = m_32;
    arrayOf16[15] = m_33;
  }

  const float*
  Matrix4::data () const
  {
    return &m_matrix[0][0];
  }

  void
  Matrix4::transpose ()
  {
    float temp = m_01;
    m_01 = m_10;
    m_10 = temp;
    temp = m_02;
    m_02 = m_20;
    m_20 = temp;
    temp = m_12;
    m_12 = m_21;
    m_21 = temp;
    temp = m_03;
    m_03 = m_30;
    m_30 = temp;
    temp = m_13;
    m_13 = m_31;
    m_31 = temp;
    temp = m_23;
    m_23 = m_32;
    m_32 = temp;
  }

  void
  Matrix4::negate ()
  {
    for (int i = 0; i < 4; i++)
      {
	for (int j = 0; j < 4; j++)
	  {
	    m_matrix[i][j] = -1 * m_matrix[i][j];
	  }
      }
  }

  void
  Matrix4::setToLookAt(const Vector3& eye, const Vector3& at, const Vector3& up){
    Vector3 back = -1*at;
    Vector3 upn=up;
    upn.normalize();
    back.normalize();
    Vector3 right=upn.cross(back);

    setToZero();
    m_00=right.x;
    m_01=upn.x;
    m_02=back.x;
    m_10=right.y;
    m_11=upn.y;
    m_12=back.y;
    m_20=right.z;
    m_21=upn.z;
    m_22=back.z;
    m_30=m_00*eye.x*-1+m_10*eye.y*-1+m_20*eye.z*-1;
    m_31=m_01*eye.x*-1+m_11*eye.y*-1+m_21*eye.z*-1;
    m_32=m_02*eye.x*-1+m_12*eye.y*-1+m_22*eye.z*-1;
    m_33=1;
  }

  void
  Matrix4::setToPerspectiveProjection(double fovYDegrees,double aspectRatio,double nearPlaneZ,double farPlaneZ){
    setToZero();
    m_00=1/(aspectRatio*tan(fovYDegrees/2));
    m_11=1/tan(fovYDegrees/2);
    m_22=(nearPlaneZ+farPlaneZ)/(nearPlaneZ-farPlaneZ);
    m_32=(2*nearPlaneZ*farPlaneZ)/(nearPlaneZ-farPlaneZ);
    m_23=-1;
  }

  void
  Matrix4::setToPerspectiveProjection(double left,double right,double bottom,double top,double nearPlaneZ,double farPlaneZ){
    setToZero();
    m_00=2*(nearPlaneZ)/(right-left);
    m_11=2*(nearPlaneZ)/(top-bottom);
    m_20=(right+left)/(right-left);
    m_21=(top+bottom)/(top-bottom);
    m_22=-1*(farPlaneZ+nearPlaneZ)/(farPlaneZ-nearPlaneZ);
    m_32=-2*farPlaneZ*nearPlaneZ/(farPlaneZ-nearPlaneZ);
    m_23=-1;
  }



  void Matrix4::setToOrthographicProjection(double left, double right, double bottom, double top, double nearPlaneZ, double farPlaneZ){
    setToZero();
    m_00=2/(right-left);
    m_11=2/(top-bottom);
    m_22=-2/(farPlaneZ-nearPlaneZ);
    m_30=-1*(right+left)/(right-left);
    m_31=-1*(top+bottom)/(top-bottom);
    m_32=-1*(farPlaneZ+nearPlaneZ)/(farPlaneZ-nearPlaneZ);
    m_33=1;
  }

  Matrix4& Matrix4::operator+=(const Matrix4& m){
    *this = *this + m;
    return *this;
  }

  Matrix4& Matrix4::operator -=(const Matrix4& m){
    *this = *this - m;
    return *this;
  }

  Matrix4& Matrix4::operator *=(float s){
    for (int i=0;i<4;i++){
	for (int j=0;j<4;j++){
	this->m_matrix[i][j]=this->m_matrix[i][j]*s;
	}
    }
    return *this;
  }

  Matrix4& Matrix4::operator *=(const Matrix4& m){
    *this = *this * m;
    return *this;
  }

  Matrix4
  operator+ (const Matrix4& m1, const Matrix4& m2){
    float output[16];
    float mat1[16];
    float mat2[16];
    m1.getAsArray(mat1);
    m2.getAsArray(mat2);
    for (int i=0;i<16;i++){
	output[i]=mat1[i]+mat2[i];
    }
    return Matrix4(output);
  }

  Matrix4
  operator- (const Matrix4& m1, const Matrix4& m2){
    float output[16];
    float mat1[16];
    float mat2[16];
    m1.getAsArray(mat1);
    m2.getAsArray(mat2);
    for (int i=0;i<16;i++){
	output[i]=mat1[i]-mat2[i];
    }
    return Matrix4(output);
  }

  // Unary negation
  Matrix4
  operator- (const Matrix4& m){
    float mat1[16];
    m.getAsArray(mat1);
    for (int i=0;i<16;i++){
	mat1[i]=mat1[i]*-1;
    }
    return Matrix4(mat1);
  }

  Matrix4
  operator* (const Matrix4& m1, const Matrix4& m2){
    float output[16];
    float mat1[16];
    float mat2[16];
    m1.getAsArray(mat1);
    m2.getAsArray(mat2);
    output[0]=mat1[0]*mat2[0]+mat1[4]*mat2[1]+mat1[8]*mat2[2]+mat1[12]*mat2[3];
    output[1]=mat1[1]*mat2[0]+mat1[5]*mat2[1]+mat1[9]*mat2[2]+mat1[13]*mat2[3];
    output[2]=mat1[2]*mat2[0]+mat1[6]*mat2[1]+mat1[10]*mat2[2]+mat1[14]*mat2[3];
    output[3]=mat1[3]*mat2[0]+mat1[7]*mat2[1]+mat1[11]*mat2[2]+mat1[15]*mat2[3];
    output[4]=mat1[0]*mat2[4]+mat1[4]*mat2[5]+mat1[8]*mat2[6]+mat1[12]*mat2[7];
    output[5]=mat1[1]*mat2[4]+mat1[5]*mat2[5]+mat1[9]*mat2[6]+mat1[13]*mat2[7];
    output[6]=mat1[2]*mat2[4]+mat1[6]*mat2[5]+mat1[10]*mat2[6]+mat1[14]*mat2[7];
    output[7]=mat1[3]*mat2[4]+mat1[7]*mat2[5]+mat1[11]*mat2[6]+mat1[15]*mat2[7];
    output[8]=mat1[0]*mat2[8]+mat1[4]*mat2[9]+mat1[8]*mat2[10]+mat1[12]*mat2[11];
    output[9]=mat1[1]*mat2[8]+mat1[5]*mat2[9]+mat1[9]*mat2[10]+mat1[13]*mat2[11];
    output[10]=mat1[2]*mat2[8]+mat1[6]*mat2[9]+mat1[10]*mat2[10]+mat1[14]*mat2[11];
    output[11]=mat1[3]*mat2[8]+mat1[7]*mat2[9]+mat1[11]*mat2[10]+mat1[15]*mat2[11];
    output[12]=mat1[0]*mat2[12]+mat1[4]*mat2[13]+mat1[8]*mat2[14]+mat1[12]*mat2[15];
    output[13]=mat1[1]*mat2[12]+mat1[5]*mat2[13]+mat1[9]*mat2[14]+mat1[13]*mat2[15];
    output[14]=mat1[2]*mat2[12]+mat1[6]*mat2[13]+mat1[10]*mat2[14]+mat1[14]*mat2[15];
    output[15]=mat1[3]*mat2[12]+mat1[7]*mat2[13]+mat1[11]*mat2[14]+mat1[15]*mat2[15];
    return Matrix4(output);
  }

  std::ostream&
  operator<< (std::ostream& outStream, const Matrix4& m){
    float array[16];
    m.getAsArray(array);
    outStream << array[0]<<"    "<<array[1]<<"    "<<array[2]<<"    "<<array[3]<<"\n"<< array[4]<<"    "<<array[5]<<"    "<<array[6]<<"    "<<array[7]<<"\n"<< array[8]<<"    "<<array[9]<<"    "<<array[10]<<"    "<<array[11]<<"n"<< array[12]<<"    "<<array[13]<<"    "<<array[14]<<"    "<<array[15]<<"\n";
    return outStream;
  }



}

