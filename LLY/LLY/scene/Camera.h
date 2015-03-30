#ifndef CAMERA_H_
#define CAMERA_H_

#include "GameObject.h"

namespace lly {

	class Camera : public GameObject
	{
	public:
		enum Type
		{
			PERSPECTIVE  = 1,
			ORTHOGRAPHIC = 2
		};

		virtual ~Camera();

		static Camera* createPerspective(float fiedlOfView, float aspectRatio, float near, float far);
		static Camera* createPerspective(float zoomX, float zoomY, float aspectRatio, float near, float far);

		Camera::Type getType();
		float getFieldOfView();
		float getZoomX();
		float getZoomY();
		float getAspectRatio();
		float getNear();
		float getFar();

	protected:
	private:
		Camera(float fiedlOfView, float aspectRatio, float near, float far);
		Camera(float zoomX, float zoomY, float aspectRatio, float near, float far);

		Camera::Type _type;

		float _fieldOfView;
		float _zoomX;
		float _zoomY;
		float _aspectRatio;
		float _near;
		float _far;
	};
}

#endif