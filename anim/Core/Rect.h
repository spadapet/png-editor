#pragma once

namespace anim
{
	template<typename T>
	class RectType
	{
	public:
		RectType();
		RectType(T left, T top, T right, T bottom);
		RectType(const RectType<T> &rhs);
		RectType(const PointType<T> &rhs1, const PointType<T> &rhs2);
		RectType(const PointType<T> &rhs);
		RectType(const RECT &rhs);

		static RectType<T> Zeros();

		T Width() const;
		T Height() const;
		T Area() const;
		PointType<T> Size() const;
		PointType<T> Center() const;
		PointType<T> TopLeft() const;
		PointType<T> TopRight() const;
		PointType<T> BottomLeft() const;
		PointType<T> BottomRight() const;

		bool IsEmpty() const;
		bool IsNull() const;
		bool PointInRect(const PointType<T> &point) const;

		void SetRect(T left, T top, T right, T bottom);
		void SetRect(const PointType<T> &topLeft, const PointType<T> &bottomRight);
		void SetRect(T right, T bottom);
		void SetRect(const PointType<T> &bottomRight);

		void Normalize();
		void EnsurePositiveSize();
		void EnsureMinimumSize(const PointType<T> &point);
		void EnsureMinimumSize(T width, T height);

		bool Intersect(const RectType<T> &rhs); // intersect self with rhs
		bool Intersect(const RectType<T> &rhs1, const RectType<T> &rhs2);
		bool DoesTouch(const RectType<T> &rhs) const; // returns true if (*this) touches rhs at all
		bool IsInside(const RectType<T> &rhs) const; // returns true if (*this) is totally inside of rhs
		bool IsOutside(const RectType<T> &rhs) const; // returns true if (*this) is totally outside of rhs
		void Bound(const RectType<T> &rhs); // make myself larger to also cover rhs
		void Bound(const RectType<T> &rhs1, const RectType<T> &rhs2); // make myself cover rhs1 and rhs2
		void Bound(const PointType<T> &point);
		void Bound(const PointType<T> &pt1, const PointType<T> &pt2);

		void CenterWithin(const RectType<T> &rhs); // center (*this) within rhs, if bigger than rhs, center on top of rhs
		void CenterOn(const PointType<T> &point);// center (*this) on top of point
		void ScaleToFit(const RectType<T> &rhs); // moves bottom-right so that (*this) would fit within rhs, but keeps aspect ratio
		void MoveInside(const RectType<T> &rhs); // move this rect so that it is inside of rhs (impossible to do if rhs is smaller than *this)
		void MoveTopLeft(const PointType<T> &point); // keeps the same size, but moves the rect to point
		void MoveTopLeft(T tx, T ty); // same as above
		void Crop(const RectType<T> &rhs); // cut off my edges so that I'm inside of rhs
		void Interpolate(const RectType<T> &rhs1, const RectType<T> &rhs2, double value); // 0 is rhs1, 1 is rhs2
		void FloorToIntegers();

		void Offset(T tx, T ty);
		void Offset(const PointType<T> &point);
		void OffsetSize(T tx, T ty);
		void OffsetSize(const PointType<T> &point);
		void Deflate(T tx, T ty);
		void Deflate(T tx, T ty, T tx2, T ty2);
		void Deflate(const PointType<T> &point);
		void Deflate(const RectType<T> &rhs);

		// operators

		RectType<T> &operator=(const RectType<T> &rhs);
		bool operator==(const RectType<T> &rhs) const;
		bool operator!=(const RectType<T> &rhs) const;

		void operator+=(const PointType<T> &point);
		void operator-=(const PointType<T> &point);

		const RectType<T> operator+(const PointType<T> &point) const;
		const RectType<T> operator-(const PointType<T> &point) const;

		const RectType<T> operator*(const RectType<T> &rhs) const;
		const RectType<T> operator*(const PointType<T> &rhs) const;
		const RectType<T> operator/(const RectType<T> &rhs) const;
		const RectType<T> operator/(const PointType<T> &rhs) const;

		RectType<T> &operator=(const RECT &rhs);
		RECT ToRECT() const;
		RectType<int> ToInt() const;
		RectType<short> ToShort() const;
		RectType<float> ToFloat() const;
		RectType<double> ToDouble() const;
		RectType<size_t> ToSize() const;

		union
		{
			struct
			{
				T left;
				T top;
				T right;
				T bottom;
			};

			T arr[4];
		};
	};

	typedef RectType<int> RectInt;
	typedef RectType<short> RectShort;
	typedef RectType<float> RectFloat;
	typedef RectType<double> RectDouble;
	typedef RectType<size_t> RectSize;
}

template<typename T>
anim::RectType<T>::RectType()
{
}

template<typename T>
anim::RectType<T>::RectType(T left, T top, T right, T bottom)
	: left(left)
	, top(top)
	, right(right)
	, bottom(bottom)
{
}

template<typename T>
anim::RectType<T>::RectType(const RectType<T> &rhs)
{
	*this = rhs;
}

template<typename T>
anim::RectType<T>::RectType(const PointType<T> &rhs1, const PointType<T> &rhs2)
	: left(rhs1.x)
	, top(rhs1.y)
	, right(rhs2.x)
	, bottom(rhs2.y)
{
}

template<typename T>
anim::RectType<T>::RectType(const PointType<T> &rhs)
	: left(0)
	, top(0)
	, right(rhs.x)
	, bottom(rhs.y)
{
}

template<typename T>
anim::RectType<T>::RectType(const RECT &rhs)
{
	*this = rhs;
}

// static
template<typename T>
anim::RectType<T> anim::RectType<T>::Zeros()
{
	return RectType<T>(0, 0, 0, 0);
}

template<typename T>
T anim::RectType<T>::Width() const
{
	return this->right - this->left;
}

template<typename T>
T anim::RectType<T>::Height() const
{
	return this->bottom - this->top;
}

template<typename T>
T anim::RectType<T>::Area() const
{
	return this->Width() * this->Height();
}

template<typename T>
anim::PointType<T> anim::RectType<T>::Size() const
{
	return PointType<T>(this->right - this->left, this->bottom - this->top);
}

template<typename T>
anim::PointType<T> anim::RectType<T>::Center() const
{
	return PointType<T>((this->left + this->right) / 2, (this->top + this->bottom) / 2);
}

template<typename T>
bool anim::RectType<T>::IsEmpty() const
{
	return this->bottom == this->top  && this->right == this->left;
}

template<typename T>
bool anim::RectType<T>::IsNull() const
{
	return this->left == 0 && this->top == 0 && this->bottom - this->top == 0 && this->right - this->left == 0;
}

template<typename T>
bool anim::RectType<T>::PointInRect(const PointType<T> &point) const
{
	return point.x >= this->left && point.x < this->right && point.y >= this->top && point.y < this->bottom;
}

template<typename T>
anim::PointType<T> anim::RectType<T>::TopLeft() const
{
	return PointType<T>(this->left, this->top);
}

template<typename T>
anim::PointType<T> anim::RectType<T>::TopRight() const
{
	return PointType<T>(this->right, this->top);
}

template<typename T>
anim::PointType<T> anim::RectType<T>::BottomLeft() const
{
	return PointType<T>(this->left, this->bottom);
}

template<typename T>
anim::PointType<T> anim::RectType<T>::BottomRight() const
{
	return PointType<T>(this->right, this->bottom);
}

template<typename T>
void anim::RectType<T>::SetRect(T left, T top, T right, T bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

template<typename T>
void anim::RectType<T>::SetRect(const PointType<T> &topLeft, const PointType<T> &bottomRight)
{
	this->left = topLeft.x;
	this->top = topLeft.y;
	this->right = bottomRight.x;
	this->bottom = bottomRight.y;
}

template<typename T>
void anim::RectType<T>::SetRect(T right, T bottom)
{
	this->left = 0;
	this->top = 0;
	this->right = right;
	this->bottom = bottom;
}

template<typename T>
void anim::RectType<T>::SetRect(const PointType<T> &bottomRight)
{
	this->left = 0;
	this->top = 0;
	this->right = bottomRight.x;
	this->bottom = bottomRight.y;
}

template<typename T>
void anim::RectType<T>::Offset(T tx, T ty)
{
	this->left += tx;
	this->right += tx;
	this->top += ty;
	this->bottom += ty;
}

template<typename T>
void anim::RectType<T>::Offset(const PointType<T> &point)
{
	this->left += point.x;
	this->right += point.x;
	this->top += point.y;
	this->bottom += point.y;
}

template<typename T>
void anim::RectType<T>::OffsetSize(T tx, T ty)
{
	this->right += tx;
	this->bottom += ty;
}

template<typename T>
void anim::RectType<T>::OffsetSize(const PointType<T> &point)
{
	this->right += point.x;
	this->bottom += point.y;
}

template<typename T>
void anim::RectType<T>::Normalize()
{
	if (this->left > this->right)
	{
		std::swap(this->left, this->right);
	}

	if (this->top > this->bottom)
	{
		std::swap(this->top, this->bottom);
	}
}

template<typename T>
void anim::RectType<T>::EnsurePositiveSize()
{
	if (this->bottom < this->top)
	{
		this->bottom = this->top;
	}

	if (this->right < this->left)
	{
		this->right = this->left;
	}
}

template<typename T>
void anim::RectType<T>::EnsureMinimumSize(const PointType<T> &point)
{
	this->right = std::max(this->left + point.x, this->right);
	this->bottom = std::max(this->top + point.y, this->bottom);
}

template<typename T>
void anim::RectType<T>::EnsureMinimumSize(T width, T height)
{
	this->right = std::max(this->left + width, this->right);
	this->bottom = std::max(this->top + height, this->bottom);
}

template<typename T>
void anim::RectType<T>::Deflate(T tx, T ty)
{
	this->left += tx;
	this->right -= tx;
	this->top += ty;
	this->bottom -= ty;
	this->Normalize();
}

template<typename T>
void anim::RectType<T>::Deflate(T tx, T ty, T tx2, T ty2)
{
	this->left += tx;
	this->right -= tx2;
	this->top += ty;
	this->bottom -= ty2;
	this->Normalize();
}

template<typename T>
void anim::RectType<T>::Deflate(const PointType<T> &point)
{
	this->left += point.x;
	this->right -= point.x;
	this->top += point.y;
	this->bottom -= point.y;
	this->Normalize();
}

template<typename T>
void anim::RectType<T>::Deflate(const RectType<T> &rhs)
{
	this->left += rhs.left;
	this->right -= rhs.right;
	this->top += rhs.top;
	this->bottom -= rhs.bottom;
	this->Normalize();
}

template<typename T>
bool anim::RectType<T>::Intersect(const RectType<T> &rhs)
{
	T nLeft = std::max(this->left, rhs.left);
	T nRight = std::min(this->right, rhs.right);
	T nTop = std::max(this->top, rhs.top);
	T nBottom = std::min(this->bottom, rhs.bottom);

	if (nLeft <= nRight && nTop <= nBottom)
	{
		this->SetRect(nLeft, nTop, nRight, nBottom);
		return true;
	}
	else
	{
		this->SetRect(0, 0, 0, 0);
		return false;
	}
}

template<typename T>
bool anim::RectType<T>::Intersect(const RectType<T> &rhs1, const RectType<T> &rhs2)
{
	*this = rhs1;
	return this->Intersect(rhs2);
}

template<typename T>
bool anim::RectType<T>::DoesTouch(const RectType<T> &rhs) const
{
	return this->right > rhs.left && this->left < rhs.right && this->bottom > rhs.top && this->top < rhs.bottom;
}

template<typename T>
bool anim::RectType<T>::IsInside(const RectType<T> &rhs) const
{
	return this->left >= rhs.left && this->right <= rhs.right && this->top >= rhs.top && this->bottom <= rhs.bottom;
}

template<typename T>
bool anim::RectType<T>::IsOutside(const RectType<T> &rhs) const
{
	return this->left >= rhs.right || this->right <= rhs.left || this->top >= rhs.bottom || this->bottom <= rhs.top;
}

template<typename T>
void anim::RectType<T>::Bound(const RectType<T> &rhs)
{
	this->SetRect(
		std::min(this->left, rhs.left),
		std::min(this->top, rhs.top),
		std::max(this->right, rhs.right),
		std::max(this->bottom, rhs.bottom));
}

template<typename T>
void anim::RectType<T>::Bound(const RectType<T> &rhs1, const RectType<T> &rhs2)
{
	this->SetRect(
		std::min(rhs1.left, rhs2.left),
		std::min(rhs1.top, rhs2.top),
		std::max(rhs1.right, rhs2.right),
		std::max(rhs1.bottom, rhs2.bottom));
}

template<typename T>
void anim::RectType<T>::Bound(const PointType<T> &point)
{
	this->SetRect(
		std::min(this->left, point.x),
		std::min(this->top, point.y),
		std::max(this->right, point.x),
		std::max(this->bottom, point.y));
}

template<typename T>
void anim::RectType<T>::Bound(const PointType<T> &pt1, const PointType<T> &pt2)
{
	this->SetRect(
		std::min(pt1.x, pt2.x),
		std::min(pt1.y, pt2.y),
		std::max(pt1.x, pt2.x),
		std::max(pt1.y, pt2.y));
}

template<typename T>
void anim::RectType<T>::CenterWithin(const RectType<T> &rhs)
{
	T width = this->right - this->left;
	T height = this->bottom - this->top;

	this->left = rhs.left + (rhs.Width() - width) / 2;
	this->top = rhs.top + (rhs.Height() - height) / 2;
	this->right = left + width;
	this->bottom = top + height;
}

template<typename T>
void anim::RectType<T>::CenterOn(const PointType<T> &point)
{
	T width = this->right - this->left;
	T height = this->bottom - this->top;

	this->left = point.x - width / 2;
	this->top = point.y - height / 2;
	this->right = this->left + width;
	this->bottom = this->top + height;
}

template<typename T>
void anim::RectType<T>::MoveInside(const RectType<T> &rhs)
{
	if (this->left < rhs.left)
	{
		T offset = rhs.left - this->left;

		this->left += offset;
		this->right += offset;
	}
	else if (this->right > rhs.right)
	{
		T offset = this->right - rhs.right;

		this->left -= offset;
		this->right -= offset;
	}

	if (this->top < rhs.top)
	{
		T offset = rhs.top - this->top;

		this->top += offset;
		this->bottom += offset;
	}
	else if (this->bottom > rhs.bottom)
	{
		T offset = this->bottom - rhs.bottom;

		this->top -= offset;
		this->bottom -= offset;
	}
}

template<typename T>
void anim::RectType<T>::MoveTopLeft(const PointType<T> &point)
{
	this->Offset(point.x - this->left, point.y - this->top);
}

template<typename T>
void anim::RectType<T>::MoveTopLeft(T tx, T ty)
{
	this->Offset(tx - this->left, ty - this->top);
}

template<typename T>
void anim::RectType<T>::Crop(const RectType<T> &rhs)
{
	if (this->left < rhs.left)
	{
		this->left = rhs.left;
	}

	if (this->right > rhs.right)
	{
		this->right = rhs.right;
	}

	if (this->top < rhs.top)
	{
		this->top = rhs.top;
	}

	if (this->bottom > rhs.bottom)
	{
		this->bottom = rhs.bottom;
	}
}

template<typename T>
void anim::RectType<T>::ScaleToFit(const RectType<T> &rhs)
{
	if (this->left == this->right)
	{
		if (this->top != this->bottom)
		{
			this->bottom = this->top + rhs.Height();
		}
	}
	else if (this->top == this->bottom)
	{
		this->right = this->left + rhs.Width();
	}
	else
	{
		double ratio = (double)(this->right - this->left) / (double)(this->bottom - this->top);

		if ((double)rhs.Width() / ratio > (double)rhs.Height())
		{
			this->right = this->left + (T)((double)rhs.Height() * ratio);
			this->bottom = this->top + rhs.Height();
		}
		else
		{
			this->right = this->left + rhs.Width();
			this->bottom = this->top + (T)((double)rhs.Width() / ratio);
		}
	}
}

template<typename T>
void anim::RectType<T>::Interpolate(const RectType<T> &rhs1, const RectType<T> &rhs2, double value)
{
	RectDouble dr1((double)rhs1.left, (double)rhs1.top, (double)rhs1.right, (double)rhs1.bottom);
	RectDouble dr2((double)rhs2.left, (double)rhs2.top, (double)rhs2.right, (double)rhs2.bottom);

	this->SetRect(
		(T)((dr2.left   - dr1.left)   * value + dr1.left  ),
		(T)((dr2.top    - dr1.top)    * value + dr1.top   ),
		(T)((dr2.right  - dr1.right)  * value + dr1.right ),
		(T)((dr2.bottom - dr1.bottom) * value + dr1.bottom));
}

template<typename T>
void anim::RectType<T>::FloorToIntegers()
{
}

template<>
inline void anim::RectType<float>::FloorToIntegers()
{
	this->left = std::floor(this->left);
	this->top = std::floor(this->top);
	this->right = std::floor(this->right);
	this->bottom = std::floor(this->bottom);
}

template<>
inline void anim::RectType<double>::FloorToIntegers()
{
	this->left = std::floor(this->left);
	this->top = std::floor(this->top);
	this->right = std::floor(this->right);
	this->bottom = std::floor(this->bottom);
}

template<typename T>
anim::RectType<T> &anim::RectType<T>::operator=(const RectType<T> &rhs)
{
	::memcpy(this, &rhs, sizeof(rhs));
	return *this;
}

template<typename T>
bool anim::RectType<T>::operator==(const RectType<T> &rhs) const
{
	return ::memcmp(this, &rhs, sizeof(rhs)) == 0;
}

template<typename T>
bool anim::RectType<T>::operator!=(const RectType<T> &rhs) const
{
	return !(*this == rhs);
}

template<typename T>
void anim::RectType<T>::operator+=(const PointType<T> &point)
{
	this->left += point.x;
	this->top += point.y;
	this->right += point.x;
	this->bottom += point.y;
}

template<typename T>
void anim::RectType<T>::operator-=(const PointType<T> &point)
{
	this->left -= point.x;
	this->top -= point.y;
	this->right -= point.x;
	this->bottom -= point.y;
}

template<typename T>
const anim::RectType<T> anim::RectType<T>::operator+(const PointType<T> &point) const
{
	return RectType<T>(this->left + point.x, this->top + point.y, this->right + point.x, this->bottom + point.y);
}

template<typename T>
const anim::RectType<T> anim::RectType<T>::operator-(const PointType<T> &point) const
{
	return RectType<T>(this->left - point.x, this->top - point.y, this->right - point.x, this->bottom - point.y);
}

template<typename T>
const anim::RectType<T> anim::RectType<T>::operator*(const RectType<T> &rhs) const
{
	return RectType<T>(this->left * rhs.left, this->top * rhs.top, this->right * rhs.right, this->bottom * rhs.bottom);
}

template<typename T>
const anim::RectType<T> anim::RectType<T>::operator*(const PointType<T> &rhs) const
{
	return RectType<T>(this->left * rhs.x, this->top * rhs.y, this->right * rhs.x, this->bottom * rhs.y);
}

template<typename T>
const anim::RectType<T> anim::RectType<T>::operator/(const RectType<T> &rhs) const
{
	return RectType<T>(this->left / rhs.left, this->top / rhs.top, this->right / rhs.right, this->bottom / rhs.bottom);
}

template<typename T>
const anim::RectType<T> anim::RectType<T>::operator/(const PointType<T> &rhs) const
{
	return RectType<T>(this->left / rhs.x, this->top / rhs.y, this->right / rhs.x, this->bottom / rhs.y);
}

template<typename T>
anim::RectType<T> &anim::RectType<T>::operator=(const RECT &rhs)
{
	this->SetRect((T)rhs.left, (T)rhs.top, (T)rhs.right, (T)rhs.bottom);
	return *this;
}

template<typename T>
RECT anim::RectType<T>::ToRECT() const
{
	RECT rect = { (int)this->left, (int)this->top, (int)this->right, (int)this->bottom };
	return rect;
}

template<typename T>
anim::RectType<int> anim::RectType<T>::ToInt() const
{
	return anim::RectType<int>((int)this->left, (int)this->top, (int)this->right, (int)this->bottom);
}

template<typename T>
anim::RectType<short> anim::RectType<T>::ToShort() const
{
	return anim::RectType<short>((short)this->left, (short)this->top, (short)this->right, (short)this->bottom);
}

template<typename T>
anim::RectType<float> anim::RectType<T>::ToFloat() const
{
	return anim::RectType<float>((float)this->left, (float)this->top, (float)this->right, (float)this->bottom);
}

template<typename T>
anim::RectType<double> anim::RectType<T>::ToDouble() const
{
	return anim::RectType<double>((double)this->left, (double)this->top, (double)this->right, (double)this->bottom);
}

template<typename T>
anim::RectType<size_t> anim::RectType<T>::ToSize() const
{
	return anim::RectType<size_t>((size_t)this->left, (size_t)this->top, (size_t)this->right, (size_t)this->bottom);
}
