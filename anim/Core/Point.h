#pragma once

namespace anim
{
	template<typename T>
	class PointType
	{
	public:
		PointType();
		PointType(T x, T y);
		PointType(const PointType<T> &rhs);

		static PointType<T> Zeros();

		// functions

		void SetPoint(T tx, T ty);
		void Offset(T tx, T ty);
		void Offset(const PointType<T> &rhs);
		void FloorToIntegers();
		bool IsNull() const;
		T Length() const;
		T Length2() const;

		// operators

		PointType<T> &operator=(const PointType<T> &rhs);
		PointType<T> &operator=(const SIZE &rhs);
		PointType<T> &operator=(const POINT &rhs);

		bool operator==(const PointType<T> &rhs) const;
		bool operator!=(const PointType<T> &rhs) const;

		PointType<T> operator+(const PointType<T> &rhs) const;
		PointType<T> operator-(const PointType<T> &rhs) const;
		PointType<T> operator-();

		void operator+=(const PointType<T> &rhs);
		void operator-=(const PointType<T> &rhs);

		void operator*=(T scale);
		void operator*=(const PointType<T> &rhs);
		void operator/=(T scale);
		void operator/=(const PointType<T> &rhs);

		PointType<T> operator*(T scale) const;
		PointType<T> operator*(const PointType<T> &rhs) const;
		PointType<T> operator/(T scale) const;
		PointType<T> operator/(const PointType<T> &rhs) const;

		POINT ToPOINT() const;
		PointType<int> ToInt() const;
		PointType<short> ToShort() const;
		PointType<float> ToFloat() const;
		PointType<double> ToDouble() const;
		PointType<size_t> ToSize() const;

		// vars

		union
		{
			struct
			{
				T x;
				T y;
			};

			T arr[2];
		};
	};

	typedef PointType<int> PointInt;
	typedef PointType<short> PointShort;
	typedef PointType<float> PointFloat;
	typedef PointType<double> PointDouble;
	typedef PointType<size_t> PointSize;
}

template<typename T>
anim::PointType<T>::PointType()
{
}

template<typename T>
anim::PointType<T>::PointType(T x, T y)
	: x(x)
	, y(y)
{
}

template<typename T>
anim::PointType<T>::PointType(const PointType<T> &rhs)
	: x(rhs.x)
	, y(rhs.y)
{
}

template<typename T>
anim::PointType<T> anim::PointType<T>::Zeros()
{
	return PointType<T>(0, 0);
}

template<typename T>
anim::PointType<T> &anim::PointType<T>::operator=(const PointType<T> &rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
	return *this;
}

template<typename T>
void anim::PointType<T>::SetPoint(T tx, T ty)
{
	this->x = tx;
	this->y = ty;
}

template<typename T>
void anim::PointType<T>::Offset(T x, T y)
{
	this->x += x;
	this->y += y;
}

template<typename T>
void anim::PointType<T>::Offset(const PointType<T> &pt)
{
	this->x += pt.x;
	this->y += pt.y;
}

template<typename T>
void anim::PointType<T>::FloorToIntegers()
{
	this->x = std::floor(x);
	this->y = std::floor(y);
}

template<typename T>
bool anim::PointType<T>::IsNull() const
{
	return this->x == 0 && this->y == 0;
}

template<typename T>
T anim::PointType<T>::Length() const
{
	return std::sqrt(Length2());
}

template<typename T>
T anim::PointType<T>::Length2() const
{
	return this->x * this->x + this->y * this->y;
}

template<typename T>
bool anim::PointType<T>::operator==(const PointType<T> &rhs) const
{
	return this->x == rhs.x && this->y == rhs.y;
}

template<typename T>
bool anim::PointType<T>::operator!=(const PointType<T> &rhs) const
{
	return this->x != rhs.x || this->y != rhs.y;
}

template<typename T>
anim::PointType<T> anim::PointType<T>::operator+(const PointType<T> &rhs) const
{
	return PointType<T>(this->x + rhs.x, this->y + rhs.y);
}

template<typename T>
anim::PointType<T> anim::PointType<T>::operator-(const PointType<T> &rhs) const
{
	return PointType<T>(this->x - rhs.x, this->y - rhs.y);
}

template<typename T>
anim::PointType<T> anim::PointType<T>::operator-()
{
	return PointType<T>(-this->x, -this->y);
}

template<typename T>
void anim::PointType<T>::operator+=(const PointType<T> &rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
}

template<typename T>
void anim::PointType<T>::operator-=(const PointType<T> &rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
}

template<typename T>
void anim::PointType<T>::operator*=(T scale)
{
	this->x *= scale;
	this->y *= scale;
}

template<typename T>
void anim::PointType<T>::operator*=(const PointType<T> &rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
}

template<typename T>
void anim::PointType<T>::operator/=(T scale)
{
	this->x /= scale;
	this->y /= scale;
}

template<typename T>
void anim::PointType<T>::operator/=(const PointType<T> &rhs)
{
	this->x /= rhs.x;
	this->y /= rhs.y;
}

template<typename T>
anim::PointType<T> anim::PointType<T>::operator*(T scale) const
{
	return PointType<T>(this->x * scale, this->y * scale);
}

template<typename T>
anim::PointType<T> anim::PointType<T>::operator/(T scale) const
{
	return PointType<T>(this->x / scale, this->y / scale);
}

template<typename T>
anim::PointType<T> anim::PointType<T>::operator*(const PointType<T> &rhs) const
{
	return PointType<T>(this->x * rhs.x, this->y * rhs.y);
}

template<typename T>
anim::PointType<T> anim::PointType<T>::operator/(const PointType<T> &rhs) const
{
	return PointType<T>(this->x / rhs.x, this->y / rhs.y);
}

template<typename T>
anim::PointType<T> &anim::PointType<T>::operator=(const SIZE &rhs)
{
	this->x = (T)rhs.cx;
	this->y = (T)rhs.cy;
	return *this;
}

template<typename T>
anim::PointType<T> &anim::PointType<T>::operator=(const POINT &rhs)
{
	this->x = (T)rhs.x;
	this->y = (T)rhs.y;
	return *this;
}

template<typename T>
POINT anim::PointType<T>::ToPOINT() const
{
	POINT pt = { (int)this->x, (int)this->y };
	return pt;
}

template<typename T>
anim::PointType<int> anim::PointType<T>::ToInt() const
{
	return anim::PointType<int>((int)this->x, (int)this->y);
}

template<typename T>
anim::PointType<short> anim::PointType<T>::ToShort() const
{
	return anim::PointType<short>((short)this->x, (short)this->y);
}

template<typename T>
anim::PointType<float> anim::PointType<T>::ToFloat() const
{
	return anim::PointType<float>((float)this->x, (float)this->y);
}

template<typename T>
anim::PointType<double> anim::PointType<T>::ToDouble() const
{
	return anim::PointType<double>((double)this->x, (double)this->y);
}

template<typename T>
anim::PointType<size_t> anim::PointType<T>::ToSize() const
{
	return anim::PointType<size_t>((size_t)this->x, (size_t)this->y);
}
