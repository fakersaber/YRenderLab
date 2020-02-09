#ifndef _YRENDER_YGM_TRANSFORM_H
#define _YRENDER_YGM_TRANSFORM_H

#include <Public/YGM/Vector3.hpp>
#include <Public/YGM/Vector4.hpp>
#include <Public/YGM/Matrix4.hpp>



namespace YGM {
	//右手系，y轴朝上
	class Transform {
	public:
		Transform(float d) :Matrix(d) {}//InvMatrix(1.f/d){};.
		Transform() :Transform(1.f) { }
		Transform(const Mat4f& matrix) : Matrix(matrix) {} //InvMatrix(matrix.Inverse()) {}
		Transform(const Mat4f& matrix, const Mat4f& invMatrix) :Matrix(matrix)/*, InvMatrix(invMatrix)*/ {};

		Vector3 TrasformVec(const Vector3& LocalVec) const;
		Vector3 InverseTrasformVec(const Vector3& WorldVec) const;
		
		Vector3 TrasformPoint(const Vector3& LocalPos) const;
		Vector3 InverseTrasformPoint(const Vector3& WorldPos) const;
		

	public:
		static const Transform LookAt(const Vector3& pos, const Vector3& target, const Vector3& up = Vector3(0, 1, 0));
		static const Transform Perspective(const float fovy, const float aspect, const float zNear, const float zFar);
		static const Transform Orthographic(const float width, const float height, const float zNear, const float zFar);
	public:
		const Mat4f& GetMatrix() const {
			return Matrix;
		};

		//const Mat4f& GetInvMatrix() const {
		//	return InvMatrix;
		//}

	private:
		Mat4f Matrix;
		//Mat4f InvMatrix;
	};
}



#endif

