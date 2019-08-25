#ifndef _YRENDER_YGM_TRANSFORM_H
#define _YRENDER_YGM_TRANSFORM_H

#include <Public/YGM/Vector3.hpp>
#include <Public/YGM/Matrix4.hpp>


namespace YRender {
	namespace YGM {
		//opengl坐标系，相机为右手系，世界模型为左手系
		class Transform {
		public:
			//world to camera 
			static const Transform LookAt(const Vector3& pos, const Vector3& target,const Vector3& up = Vector3(0,1,0));
			

		private:
			Mat4f Matrix;
			Mat4f InvMatrix;
		};
	}
}



#endif

