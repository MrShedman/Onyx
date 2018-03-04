#pragma once

#include "..\math\Vector.hpp"
#include "..\math\Matrix.hpp"
#include "..\math\Angle.hpp"
#include "..\math\Quaternion.hpp"

class Transform
{
public:

	Transform();

	virtual ~Transform();

	void setPosition(float x, float y, float z = 0.0f);

	void setPosition(const Vector3f& position);

	void setRotation(Angle angle);

	void setRotation(const Vector3f& axis, Angle angle);

	void setRotation(const Vector3f& pivot, const Vector3f& axis, Angle angle);

	void setRotation(const Quaternion& rotation);

	void setScale(float factor);

	void setScale(float factorX, float factorY, float factorZ = 1.0f);

	void setScale(const Vector3f& factors);

	void setOrigin(float x, float y, float z = 0.0f);

	void setOrigin(const Vector3f& origin);

	const Vector3f& getPosition() const;

	Angle get2DRotation() const;

	const Quaternion& getRotation() const;

	const Vector3f& getScale() const;

	const Vector3f& getOrigin() const;

	void move(float offsetX, float offsetY, float offsetZ = 0.0f);

	void move(const Vector3f& offset);

	void rotate(Angle angle);

	void rotate(const Vector3f& axis, Angle angle);

	void rotate(const Vector3f& pivot, const Vector3f& axis, Angle angle);

	void rotate(const Quaternion& rotation);

	void scale(float factorX, float factorY, float factorZ = 1.0f);

	void scale(const Vector3f& factor);

	const Matrix4f& getTransform() const;

	const Matrix4f& getInverseTransform() const;

	bool transformNeedsUpdate() const;

	void reset();

private:

	void update() const;

	Vector3f          m_origin;                     ///< Origin of translation/rotation/scaling of the object
	Vector3f          m_position;                   ///< Position of the object in the 3D world
	Quaternion		  m_rotation;					///< Orientation of the object
	Vector3f          m_scale;                      ///< Scale of the object
	mutable Matrix4f  m_transform;                  ///< Combined transformation of the object
	mutable bool      m_transformNeedUpdate;        ///< Does the transform need to be recomputed?
	mutable Matrix4f  m_inverseTransform;           ///< Combined transformation of the object
	mutable bool      m_inverseTransformNeedUpdate; ///< Does the transform need to be recomputed?
};