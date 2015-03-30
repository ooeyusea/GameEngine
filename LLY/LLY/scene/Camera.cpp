#include "Camera.h"

namespace lly {

	Camera* Camera::createPerspective(float fiedlOfView, float aspectRatio, float near, float far)
	{
		return new Camera(fiedlOfView, aspectRatio, near, far);
	}

	Camera* Camera::createPerspective(float zoomX, float zoomY, float aspectRatio, float near, float far)
	{
		return new Camera(zoomX, zoomY, aspectRatio, near, far);
	}

	Camera::Camera(float fiedlOfView, float aspectRatio, float near, float far)
		: _type(PERSPECTIVE),
		_fieldOfView(fiedlOfView),
		_zoomX(0.0f),
		_zoomY(0.0f),
		_aspectRatio(aspectRatio),
		_near(near),
		_far(far)
	{

	}

	Camera::Camera(float zoomX, float zoomY, float aspectRatio, float near, float far)
		: _type(PERSPECTIVE),
		_fieldOfView(0.0f),
		_zoomX(zoomX),
		_zoomY(zoomY),
		_aspectRatio(aspectRatio),
		_near(near),
		_far(far)
	{
	}

	Camera::~Camera()
	{
	}

	Camera::Type Camera::getType()
	{
		return _type;
	}

	float Camera::getFieldOfView()
	{
		return _fieldOfView;
	}

	float Camera::getZoomX()
	{
		return _zoomX;
	}

	float Camera::getZoomY()
	{
		return _zoomY;
	}

	float Camera::getAspectRatio()
	{
		return _aspectRatio;
	}

	float Camera::getNear()
	{
		return _near;
	}

	float Camera::getFar()
	{
		return _far;
	}
}