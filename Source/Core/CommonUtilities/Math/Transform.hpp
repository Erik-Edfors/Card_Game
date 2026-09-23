#pragma once
#include "Matrix4x4.hpp"
#include "Quaternion.hpp"

namespace CommonUtilities
{
	template<typename _Ty>
	class Transform
	{
	public:
		Transform();
		Transform(Vector3<_Ty> aScale, Vector3<_Ty> aTranslation, Vector3<_Ty> aEulerRotation);
		Transform(Vector3<_Ty> aScale, Vector3<_Ty> aTranslation, Quaternion<_Ty> aRotation);
		~Transform() = default;

		void SetScale(Vector3<_Ty> aScale);
		void SetRotation(Quaternion<_Ty> aQuaternion);
		void SetRotation(const Vector3<_Ty>& aEulerAngles);
		void SetTranslation(Vector3<_Ty> aTranslation);

		const Vector3<_Ty>& GetScale() const;
		const Quaternion<_Ty>& GetRotation() const;
		const Vector3<_Ty>& GetTranslation() const;

		void SetMatrix(const Matrix4x4<_Ty>& aMatrix);
		const Matrix4x4<_Ty>& GetMatrix() const;
		Matrix4x4<_Ty> GetInverseMatrix() const;

		Vector3<_Ty> GetForward() const;
		Vector3<_Ty> GetUp() const;
		Vector3<_Ty> GetRight() const;

	private:
		void Clean() const;

		mutable bool myIsDirty;
		mutable Matrix4x4<_Ty> myMatrix;

		Vector3<_Ty> myScale;
		Quaternion<_Ty> myRotation;
		Vector3<_Ty> myTranslation;

	};

	using Transformf = Transform<float>;
	using Transformd = Transform<double>;


	template<typename _Ty>
	inline Transform<_Ty>::Transform() :
		myIsDirty(false),
		myScale(Vector3<_Ty>(static_cast<float>(1), static_cast<float>(1), static_cast<float>(1)))
	{}
	template<typename _Ty>
	inline Transform<_Ty>::Transform(Vector3<_Ty> aScale, Vector3<_Ty> aTranslation, Vector3<_Ty> aEulerRotation) :
		myIsDirty(true),
		myScale(aScale),
		myTranslation(aTranslation),
		myRotation(Quaternion<_Ty>::FromEulerZYX(aEulerRotation))
	{}
	template<typename _Ty>
	inline Transform<_Ty>::Transform(Vector3<_Ty> aScale, Vector3<_Ty> aTranslation, Quaternion<_Ty> aRotation) :
		myIsDirty(true),
		myScale(aScale),
		myTranslation(aTranslation),
		myRotation(aRotation)
	{}
	template<typename _Ty>
	inline void Transform<_Ty>::SetScale(Vector3<_Ty> aScale)
	{
		myScale = aScale;
		myIsDirty = true;
	}
	template<typename _Ty>
	inline void Transform<_Ty>::SetRotation(Quaternion<_Ty> aQuaternion)
	{
		myRotation = aQuaternion;
		myIsDirty = true;
	}
	template<typename _Ty>
	inline void Transform<_Ty>::SetRotation(const Vector3<_Ty>& aEulerAngles)
	{
		myRotation = Quaternion<_Ty>::FromEulerZYX(aEulerAngles);
		myIsDirty = true;
	}
	template<typename _Ty>
	inline void Transform<_Ty>::SetTranslation(Vector3<_Ty> aTranslation)
	{
		myTranslation = aTranslation;
		myIsDirty = true;
	}
	template<typename _Ty>
	inline const Vector3<_Ty>& Transform<_Ty>::GetScale() const
	{
		return myScale;
	}
	template<typename _Ty>
	inline const Quaternion<_Ty>& Transform<_Ty>::GetRotation() const
	{
		return myRotation;
	}
	template<typename _Ty>
	inline const Vector3<_Ty>& Transform<_Ty>::GetTranslation() const
	{
		return myTranslation;
	}
	template<typename _Ty>
	inline void Transform<_Ty>::SetMatrix(const Matrix4x4<_Ty>& aMatrix)
	{
		aMatrix.Decompose(&myTranslation, &myRotation, &myScale);
		myMatrix = aMatrix;
		myIsDirty = false;	// Everything is set.
	}
	template<typename _Ty>
	inline const Matrix4x4<_Ty>& Transform<_Ty>::GetMatrix() const
	{
		Clean();
		return myMatrix;
	}
	template<typename _Ty>
	inline Matrix4x4<_Ty> Transform<_Ty>::GetInverseMatrix() const
	{
		Clean();

		Matrix4x4<float> inv = Matrix4x4<float>::CreateRotation(-myRotation);

		Vector4<float> invTranslation = Vector4<float>(-myTranslation, 1.f) * inv;

		inv.Row(1) /= myScale.x;
		inv.Row(2) /= myScale.y;
		inv.Row(3) /= myScale.z;

		inv(4, 1) = invTranslation.x;
		inv(4, 2) = invTranslation.y;
		inv(4, 3) = invTranslation.z;
		inv(4, 4) = 1.f;

		return inv;
	}
	template<typename _Ty>
	inline Vector3<_Ty> Transform<_Ty>::GetForward() const
	{
		return GetMatrix().Row(3).xyz();
	}
	template<typename _Ty>
	inline Vector3<_Ty> Transform<_Ty>::GetUp() const
	{
		return GetMatrix().Row(2).xyz();
	}
	template<typename _Ty>
	inline Vector3<_Ty> Transform<_Ty>::GetRight() const
	{
		return GetMatrix().Row(1).xyz();
	}
	template<typename _Ty>
	inline void Transform<_Ty>::Clean() const
	{
		if (myIsDirty)
		{
			myMatrix.Compose(myTranslation, myRotation, myScale);

			myIsDirty = false;
		}
	}
}