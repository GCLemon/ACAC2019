#pragma once
#define __ALTSEED_HEADER__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <functional>

#include <memory>
#include <chrono>
#include <thread>

#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#elif ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )

#else
#include <unistd.h>
#endif

#if ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
#define _CONSOLE_GAME 1
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	文字コード
	MSVC		sizeof(wchar_t)==2
	gcc(cygwin)	sizeof(wchar_t)==2
	gcc(linux)	sizeof(wchar_t)==4
*/
namespace asd
{
#ifdef _WIN32

#ifdef _CHAR16T
	typedef wchar_t achar;
	typedef std::wstring astring;
#else
	typedef char16_t achar;
	typedef std::basic_string<char16_t> astring;
#endif
#else 
typedef char16_t achar;
typedef std::basic_string<char16_t> astring;
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ASD_STDCALL __stdcall
# else
#   define ASD_STDCALL
# endif 

# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define ASD_DLLEXPORT __declspec(dllexport)
# else
#   define ASD_DLLEXPORT
# endif 



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct Vector2DF;
struct Vector2DI;
struct Vector3DF;
struct Vector4DF;
struct Matrix43;
struct Matrix44;
struct RectF;
struct RectI;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	最大値取得
*/
template <typename T,typename U>
T Max( T t, U u )
{
	if( t > (T)u )
	{
		return t;
	}
	return u;
}

/**
	@brief	最小値取得
*/
template <typename T,typename U>
T Min( T t, U u )
{
	if( t < (T)u )
	{
		return t;
	}
	return u;
}

/**
	@brief	範囲内値取得
*/
template <typename T,typename U,typename V>
T Clamp( T t, U max_, V min_ )
{
	if( t > (T)max_ )
	{
		t = (T)max_;
	}

	if( t < (T)min_ )
	{
		t = (T)min_;
	}

	return t;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline float NormalizeAngle(float angle)
{
	int32_t ofs = (*(int32_t*)&angle & 0x80000000) | 0x3F000000; 
	return (angle - ((int)(angle * 0.159154943f + *(float*)&ofs) * 6.283185307f)); 
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline void SinCos(float x, float& s, float& c)
{
	x = NormalizeAngle(x);
	float x2 = x * x;
	float x4 = x2 * x2;
	float x6 = x2 * x4;
	float x8 = x4 * x4;
	float x10 = x2 * x8;
	s = x * (1.0f - x2 / 6.0f + x4 / 120.0f - x6 / 5040.0f + x8 / 362880.0f - x10 / 39916800.0f);
	c = 1.0f - x2 / 2.0f + x4 / 24.0f - x6 / 720.0f + x8 / 40320.0f - x10 / 3628800.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
@brief	文字コードを変換する。(UTF16 -> UTF8)
@param	dst	[out]	出力vector
@param	src			[in]	入力配列の先頭ポインタ
@return	文字数
*/
int32_t Utf16ToUtf8(std::vector<int8_t> &dst, const int16_t* src);

/**
	@brief	文字コードを変換する。(UTF8 -> UTF16)
	@param	dst			[out]	出力vector
	@param	src			[in]	入力配列の先頭ポインタ
	@return	文字数
*/
int32_t Utf8ToUtf16(std::vector<int16_t> &dst, const int8_t* src);

std::wstring ToWide(const char* pText);

astring ToAString(const char16_t* src);

astring ToAString(const wchar_t* src);

astring ToAString(const char* src);

std::string ToUtf8String(const achar* src);

astring ReplaceAll(const astring text, const achar* from, const achar* to);

astring CombinePath(const achar* rootPath, const achar* path);

#if !_WIN32 && !SWIG
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static astring ReplaceAll(const astring text, const wchar_t* from, const wchar_t* to)
{
	return ReplaceAll(text, ToAString(from).c_str(), ToAString(to).c_str());
}
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#if !SWIG
void ShowMessageBox(const achar* title, const achar* text);

#if _WIN32
#define SPRINTF sprintf_s
#else
#define SPRINTF snprintf
#endif

#define ACE_ASSERT(condition, message) { \
if (!(condition)) { \
	char lbuf[100]; \
	SPRINTF(lbuf, 100, "%d", __LINE__); \
	auto m = ::asd::ToAString(message); \
	auto f = ::asd::ToAString(__FILE__); \
	auto l = ::asd::ToAString(lbuf); \
	auto state = f + ::asd::ToAString("(") + l + ::asd::ToAString(")"); \
	auto m_ = state + ::asd::ToAString("\n") + m; \
	::asd::ShowMessageBox(::asd::ToAString("Assert").c_str(), m_.c_str()); \
	(*((volatile int*)0x0) = 0x0);  } \
}

#define ACE_ASSERT_A(condition, message) { \
if (!(condition)) { \
	char lbuf[100]; \
	SPRINTF(lbuf, 100, "%d", __LINE__); \
	auto m = message; \
	auto f = ::asd::ToAString(__FILE__); \
	auto l = ::asd::ToAString(lbuf); \
	auto state = f + ::asd::ToAString("(") + l + ::asd::ToAString(")"); \
	auto m_ = state + ::asd::ToAString("\n") + m; \
	::asd::ShowMessageBox(::asd::ToAString("Assert").c_str(), m_.c_str()); \
	(*((volatile int*)0x0) = 0x0);  } \
}

#endif

/**
	@brief	static_castとdynamic_castの値が違う場合に警告するassert
*/
#define ASSERT_STATIC_CAST(type,name) assert( static_cast<type>(name) == dynamic_cast<type>(name) )

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline void Sleep( int32_t ms )
{
	if (ms <= 0) return;
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeAddRef(T& t)
{
	if (t != NULL)
	{
		t->AddRef();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeRelease(T& t)
{
	if (t != NULL)
	{
		t->Release();
		t = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <class T>
void SafeSubstitute(T& target, T& value)
{
	SafeAddRef(value);
	SafeRelease(target);
	target = value;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <typename T>
inline void SafeDelete(T*& p)
{
	if (p != NULL)
	{
		delete (p);
		(p) = NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
template <typename T>
inline void SafeDeleteArray(T*& p)
{
	if (p != NULL)
	{
		delete[] (p);
		(p) = NULL;
	}
}


const float PI = 3.14159265358979f;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static float DegreeToRadian(float degree)
{
	return degree / 180.0f * PI;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static float RadianToDegree(float radian)
{
	return radian / PI * 180.0f;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

template<typename T> static  std::u16string ToU16String(T const &arg) {
	std::string const src = std::to_string(arg);
	auto tmp = std::vector<int16_t>(src.length());
	Utf8ToUtf16(tmp, (int8_t const*)(src.c_str()));
	tmp.pop_back(); // drop null char
	return std::u16string(tmp.begin(), tmp.end());
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}




//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
		@brief	全ての参照カウンタの値の合計を取得する。
		@return	参照カウンタ合計値
	*/
	int32_t GetGlobalReferenceCount();

	/**
		@brief	全ての参照カウンタの値を取得し、0ならDLLを解放する。
	*/
	void CheckDLL();

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
		@brief	2次元ベクトル(整数)
		*/
	struct Vector2DI
	{
	public:
		/**
			@brief	X
			*/
		int32_t	X;

		/**
			@brief	Y
			*/
		int32_t	Y;

		/**
			@brief	コンストラクタ
			*/
		Vector2DI();

		/**
			@brief	コンストラクタ
			*/
		Vector2DI(int32_t x, int32_t y);

		bool operator == (const Vector2DI& o);
		bool operator != (const Vector2DI& o);

		Vector2DI operator-();

		Vector2DI operator+(const Vector2DI& right);

		Vector2DI operator-(const Vector2DI& right);

		Vector2DI operator*(const Vector2DI& right);

		Vector2DI operator/(const Vector2DI& right);

		Vector2DI operator*(int32_t right);

		Vector2DI operator/(int32_t right);

		Vector2DI& operator+=(const Vector2DI& right);

		Vector2DI& operator-=(const Vector2DI& right);

		Vector2DI& operator*=(const Vector2DI& right);

		Vector2DI& operator/=(const Vector2DI& right);

		Vector2DI& operator*=(int32_t right);

		Vector2DI& operator/=(int32_t right);

		/**
		@brief	Vector2DF型に変換する。
		@return	Vector2DF型
		*/
		Vector2DF To2DF() const;

		/**
			@brief	スカラーで除算する。
			@param	v1	値1
			@param	v2	値2
			@return	v1/v2
		*/
		static Vector2DI DivideByScalar(const Vector2DI& v1, float v2);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
		@brief	2次元ベクトル
		*/
	struct Vector2DF
	{
	public:
		/**
			@brief	X成分
			*/
		float	X;

		/**
			@brief	Y成分
			*/
		float	Y;

		/**
			@brief	コンストラクタ
			*/
		Vector2DF();

		/**
			@brief	コンストラクタ
			@param	x	X成分
			@param	y	Y成分
		*/
		Vector2DF(float x, float y);

		/**
			@brief	このベクトルの長さを取得する。
		*/
		float GetLength() const
		{
			return sqrt(GetSquaredLength());
		}

		/**
		@brief	このベクトルの長さの二乗を取得する。
		*/
		float GetSquaredLength() const
		{
			return X * X + Y * Y;
		}

		/**
			@brief	このベクトルの長さを設定する。
		*/
		void SetLength(float value)
		{
			float length = GetLength();
			(*this) *= (value / length);
		}

		/**
		@brief	このベクトルの単位ベクトルを取得する。
		*/
		Vector2DF GetNormal()
		{
			float length = GetLength();
			return Vector2DF(X / length, Y / length);
		}

		/**
		@brief	このベクトルの単位ベクトル化する。
		*/
		void Normalize()
		{
			float length = GetLength();
			(*this) /= length;
		}

		/**
			@brief	このベクトルの向きを弧度法で取得する。
		*/
		float GetRadian() const
		{
			return atan2(Y, X);
		}

		/**
			@brief	このベクトルの向きを弧度法で設定する。
			@note 大きさを維持したまま、向きを変更する。
		*/
		void SetRadian(float value)
		{
			float length = GetLength();
			SinCos(value, Y, X);
			X *= length;
			Y *= length;
		}

		/**
			@brief	このベクトルの向きを度数法で取得する。
		*/
		float GetDegree() const
		{
			return RadianToDegree(atan2(Y, X));
		}

		/**
			@brief	このベクトルの向きを度数法で設定する。
		*/
		void SetDegree(float value)
		{
			float length = GetLength();
			SinCos(DegreeToRadian(value), Y, X);
			X *= length;
			Y *= length;
		}

		bool operator==(const Vector2DF& right);

		bool operator!=(const Vector2DF& right);

		bool operator > (const Vector2DF& o) const;
		bool operator < (const Vector2DF& o) const;

		Vector2DF operator-();

		Vector2DF operator+(const Vector2DF& right) const;

		Vector2DF operator-(const Vector2DF& right) const;

		Vector2DF operator*(const Vector2DF& right) const;

		Vector2DF operator/(const Vector2DF& right) const;

		Vector2DF operator*(float right) const;

		Vector2DF operator/(float right) const;

		Vector2DF& operator+=(const Vector2DF& right);

		Vector2DF& operator-=(const Vector2DF& right);

		Vector2DF& operator*=(const Vector2DF& right);

		Vector2DF& operator/=(const Vector2DF& right);

		Vector2DF& operator*=(float right);

		Vector2DF& operator/=(float right);
		
		/**
			@brief	内積を取得する。
			@param	v1	v1ベクトル
			@param	v2	v2ベクトル
			@return 内積v1・v2
		*/
		static float Dot(const Vector2DF& v1, const Vector2DF& v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y;
		}

		/**
		@brief	外積を取得する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return 外積v1×v2
		*/
		static float Cross(const Vector2DF& v1, const Vector2DF& v2)
		{
			return v1.X * v2.Y - v1.Y * v2.X;
		}

		/**
		@brief	2点間の距離を取得する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return v1とv2との距離
		*/
		static float Distance(const Vector2DF& v1, const Vector2DF& v2)
		{
			float dx = v1.X - v2.X;
			float dy = v1.Y - v2.Y;
			return sqrt(dx * dx + dy * dy);
		}

		/**
		@brief	加算する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return v1+v2
		*/
		static Vector2DF Add(Vector2DF v1, Vector2DF v2)
		{
			return Vector2DF(v1.X + v2.X, v1.Y + v2.Y);
		}

		/**
		@brief	減算する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return v1-v2
		*/
		static Vector2DF Subtract(Vector2DF v1, Vector2DF v2)
		{
			return Vector2DF(v1.X - v2.X, v1.Y - v2.Y);
		}

		/**
		@brief	除算する。
		@param	v1	値1
		@param	v2	値2
		@return	v1/v2
		*/
		static Vector2DF Divide(const Vector2DF& v1, const Vector2DF& v2)
		{
			return Vector2DF(v1.X / v2.X, v1.Y / v2.Y);
		}

		/**
		@brief	スカラーで除算する。
		@param	v1	値1
		@param	v2	値2
		@return	v1/v2
		*/
		static Vector2DF DivideByScalar(const Vector2DF& v1, float v2)
		{
			return Vector2DF(v1.X / v2, v1.Y / v2);
		}

		/**
			@brief	Vector2DI型に変換する。
			@return	Vector2DI型
		*/
		Vector2DI To2DI() const;
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	3次元ベクトル
*/
struct Vector3DF
{
public:
	/**
		@brief	X成分
	*/
	float	X;

	/**
		@brief	Y成分
	*/
	float	Y;

	/**
		@brief	Z成分
	*/
	float	Z;

	/**
		@brief	コンストラクタ
	*/
	Vector3DF();

	/**
	@brief	コンストラクタ
	@param	x	X成分
	@param	y	Y成分
	@param	z	Z成分
	*/
	Vector3DF( float x, float y, float z );

	bool operator == (const Vector3DF& o);
	bool operator != (const Vector3DF& o);
	bool operator > (const Vector3DF& o) const;
	bool operator < (const Vector3DF& o) const;

	Vector3DF operator-();

	Vector3DF operator + ( const Vector3DF& o ) const;

	Vector3DF operator - ( const Vector3DF& o ) const;

	Vector3DF operator * (const Vector3DF& o) const;

	Vector3DF operator / (const Vector3DF& o) const;

	Vector3DF operator * ( const float& o ) const;

	Vector3DF operator / ( const float& o ) const;

	Vector3DF& operator += ( const Vector3DF& o );

	Vector3DF& operator -= ( const Vector3DF& o );

	Vector3DF& operator *= (const Vector3DF& o);

	Vector3DF& operator /= (const Vector3DF& o);

	Vector3DF& operator *= ( const float& o );

	Vector3DF& operator /= ( const float& o );


	/**
	@brief	このベクトルの長さを取得する。
	*/
	float GetLength() const
	{
		return sqrt(GetSquaredLength());
	}

	/**
	@brief	このベクトルの長さの二乗を取得する。
	*/
	float GetSquaredLength() const
	{
		return X * X + Y * Y + Z * Z;
	}

	/**
	@brief	このベクトルの長さを設定する。
	*/
	void SetLength(float value)
	{
		float length = GetLength();
		(*this) *= (value / length);
	}

	/**
	@brief	このベクトルの単位ベクトルを取得する。
	*/
	Vector3DF GetNormal()
	{
		float length = GetLength();
		return Vector3DF(X / length, Y / length, Z / length);
	}

	/**
	@brief	このベクトルの単位ベクトル化する。
	*/
	void Normalize()
	{
		float length = GetLength();
		(*this) /= length;
	}

	/**
		@brief	内積を取得する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return 内積v1・v2
	*/
	static float Dot( const Vector3DF& v1, const Vector3DF& v2 );

	/**
		@brief	外積を取得する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return 外積v1×v2
		@note
		右手の親指がv1、人差し指がv2としたとき、中指の方向を返す。
	*/
	static Vector3DF Cross(const Vector3DF& v1, const Vector3DF& v2 );

	/**
	@brief	加算する。
	@param	v1	v1ベクトル
	@param	v2	v2ベクトル
	@return v1+v2
	*/
	static Vector3DF Add(Vector3DF v1, Vector3DF v2)
	{
		return Vector3DF(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
	}

	/**
		@brief	減算する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return v1-v2
	*/
	static Vector3DF Subtract(Vector3DF v1, Vector3DF v2);

	/**
	@brief	除算する。
	@param	v1	値1
	@param	v2	値2
	@return	v1/v2
	*/
	static Vector3DF Divide(const Vector3DF& v1, const Vector3DF& v2)
	{
		return Vector3DF(v1.X / v2.X, v1.Y / v2.Y, v1.Z / v2.Z);
	}

	/**
	@brief	スカラーで除算する。
	@param	v1	値1
	@param	v2	値2
	@return	v1/v2
	*/
	static Vector3DF DivideByScalar(const Vector3DF& v1, float v2)
	{
		return Vector3DF(v1.X / v2, v1.Y / v2, v1.Z / v2);
	}

	/**
		@brief	2点間の距離を取得する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return v1とv2の距離
	*/
	static float Distance(const Vector3DF& v1, const Vector3DF& v2);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	4次元ベクトル
	*/
	struct Vector4DF
	{
	public:
		/**
		@brief	X成分
		*/
		float	X;

		/**
		@brief	Y成分
		*/
		float	Y;

		/**
		@brief	Z成分
		*/
		float	Z;

		/**
		@brief	W成分
		*/
		float	W;

		/**
		@brief	コンストラクタ
		*/
		Vector4DF();

		/**
		@brief	コンストラクタ
		@param	x	X成分
		@param	y	Y成分
		@param	z	Z成分
		@param	w	W成分
		*/
		Vector4DF(float x, float y, float z, float w);


		/**
		@brief	このベクトルの長さを取得する。
		*/
		float GetLength() const
		{
			return sqrt(GetSquaredLength());
		}

		/**
		@brief	このベクトルの長さの二乗を取得する。
		*/
		float GetSquaredLength() const
		{
			return X * X + Y * Y + Z * Z + W * W;
		}

		/**
		@brief	このベクトルの長さを設定する。
		*/
		void SetLength(float value)
		{
			float length = GetLength();
			(*this) *= (value / length);
		}

		/**
		@brief	このベクトルの単位ベクトルを取得する。
		*/
		Vector4DF GetNormal()
		{
			float length = GetLength();
			return Vector4DF(X / length, Y / length, Z / length, W / length);
		}

		/**
		@brief	このベクトルの単位ベクトル化する。
		*/
		void Normalize()
		{
			float length = GetLength();
			(*this) /= length;
		}

		bool operator == (const Vector4DF& o);
		bool operator != (const Vector4DF& o);

		Vector4DF operator-();

		Vector4DF operator + (const Vector4DF& o) const;

		Vector4DF operator - (const Vector4DF& o) const;

		Vector4DF operator * (const Vector4DF& o) const;

		Vector4DF operator / (const Vector4DF& o) const;

		Vector4DF operator * (const float& o) const;

		Vector4DF operator / (const float& o) const;

		Vector4DF& operator += (const Vector4DF& o);

		Vector4DF& operator -= (const Vector4DF& o);

		Vector4DF& operator *= (const Vector4DF& o);

		Vector4DF& operator /= (const Vector4DF& o);

		Vector4DF& operator *= (const float& o);

		Vector4DF& operator /= (const float& o);

		/**
		@brief	内積を取得する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return 内積v1・v2
		*/
		static float Dot(const Vector4DF& v1, const Vector4DF& v2);

		/**
		@brief	2点間の距離を取得する。
		@param	v1	v1ベクトル
		@param	v2	v2ベクトル
		@return v1とv2の距離
		*/
		static float Distance(const Vector4DF& v1, const Vector4DF& v2);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

namespace asd
{
	/**
		@brief	3×3行列を表す構造体。
		@note
		[0,0][0,1]
		[1,0][1,1]
		*/
	struct Matrix33
	{
	public:
		/**
			@brief	オブジェクトを生成し、単位行列で初期化する。
		*/
		Matrix33();

		/**
			@brief	行列の値
			*/
		float Values[3][3];

		/**
			@brief	単位行列を設定する。
			*/
		Matrix33& SetIdentity();

		/**
		@brief	転置行列を設定する。
		@return	このインスタンスへの参照
		*/
		Matrix33& SetTransposed();

		/**
		@brief	逆行列を設定する。
		@return	このインスタンスへの参照
		*/
		Matrix33& SetInverted();

		/**
		@brief	逆行列を取得する。
		@return	逆行列
		*/
		Matrix33 GetInverted();

		/**
			@brief	平行移動の行列を設定する。
			*/
		Matrix33& SetTranslation(float x, float y);

		/**
		@brief	回転行列を設定する。
		*/
		Matrix33& SetRotation(float angle);

		/**
			@brief	拡大・縮小行列を設定する。
			*/
		Matrix33& SetScale(float x, float y);

		/**
		@brief	行列でベクトルを変形させる。
		@param	in	変形前ベクトル
		@return	変形後ベクトル
		*/
		Vector2DF Transform2D(const Vector2DF& in) const;

		/**
		@brief	行列でベクトルを変形させる。
		@param	in	変形前ベクトル
		@return	変形後ベクトル
		*/
		Vector3DF Transform3D(const Vector3DF& in) const;

		/**
		@brief	3×3行列の掛け算を計算する。
			*/
		Matrix33 operator*(const Matrix33& right);

		Vector3DF operator*(const Vector3DF& right);
	};
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	4×4行列を表す構造体
	@note
	M * V[x,y,z,1] の形<BR>
	[0,0][0,1][0,2][0,3]
	[1,0][1,1][1,2][1,3]
	[2,0][2,1][2,2][2,3]
	[3,0][3,1][3,2][3,3]
*/
struct Matrix44
{
private:

public:

	/**
		@brief	オブジェクトを生成し、単位行列で初期化する。
	*/
	Matrix44();

	/**
		@brief	行列の値
	*/
	float	Values[4][4];

	/**
		@brief	単位行列を設定する。
		@return	このインスタンスへの参照
	*/
	Matrix44& SetIdentity();

	/**
		@brief	転置行列を設定する。
		@return	このインスタンスへの参照
	*/
	Matrix44& SetTransposed();

	/**
		@brief	逆行列を設定する。
		@return	このインスタンスへの参照
	*/
	Matrix44& SetInverted();

	/**
		@brief	逆行列を取得する。
		@return	逆行列
	*/
	Matrix44 GetInverted() const;


	/**
		@brief	カメラ行列(右手系)を設定する。
		@param	eye	カメラの位置
		@param	at	カメラの注視点
		@param	up	カメラの上方向
		@return	このインスタンスへの参照
	*/
	Matrix44& SetLookAtRH( const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up );

	/**
		@brief	カメラ行列(左手系)を設定する。
		@param	eye	カメラの位置
		@param	at	カメラの注視点
		@param	up	カメラの上方向
		@return	このインスタンスへの参照
	*/
	Matrix44& SetLookAtLH(const Vector3DF& eye, const Vector3DF& at, const Vector3DF& up);

	/**
		@brief	射影行列(右手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetPerspectiveFovRH(float ovY, float aspect, float zn, float zf);

	/**
		@brief	OpenGL用射影行列(右手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetPerspectiveFovRH_OpenGL(float ovY, float aspect, float zn, float zf);

	/**
		@brief	射影行列(左手系)を設定する。
		@param	ovY	Y方向への視野角(ラジアン)
		@param	aspect	画面のアスペクト比
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetPerspectiveFovLH(float ovY, float aspect, float zn, float zf);

	/**
		@brief	正射影行列(右手系)を設定する。
		@param	width	横幅
		@param	height	縦幅
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetOrthographicRH(float width, float height, float zn, float zf);

	/**
		@brief	正射影行列(左手系)を設定する。
		@param	width	横幅
		@param	height	縦幅
		@param	zn	最近距離
		@param	zf	最遠距離
		@return	このインスタンスへの参照
	*/
	Matrix44& SetOrthographicLH(float width, float height, float zn, float zf);

	/**
		@brief	並行移動行列を設定する。
		@param	x	X方向移動量
		@param	y	Y方向移動量
		@param	z	Z方向移動量
		@return	このインスタンスへの参照
	*/
	Matrix44& SetTranslation(float x, float y, float z);

	/**
		@brief	X軸回転行列(右手)を設定する。
		@param	angle	X軸回転量(ラジアン)
		@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationX(float angle);

	/**
		@brief	Y軸回転行列(右手)を設定する。
		@param	angle	Y軸回転量(ラジアン)
		@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationY(float angle);

	/**
		@brief	Z軸回転行列(右手)を設定する。
		@param	angle	Z軸回転量(ラジアン)
		@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationZ(float angle);

	/**
	@brief	任意軸の反時計回転行列(右手)を設定する。
	@param	axis	軸
	@param	angle	回転量(ラジアン)
	@return	このインスタンスへの参照
	*/
	Matrix44& SetRotationAxis(const Vector3DF& axis, float angle);

	/**
	@brief	クオータニオンを元に回転行列(右手)を設定する。
	@param	x	クオータニオン
	@param	y	クオータニオン
	@param	z	クオータニオン
	@param	w	クオータニオン
	@return	このインスタンスへの参照
	*/
	Matrix44& SetQuaternion(float x, float y, float z, float w);

	/**
	@brief	拡大行列を設定する。
	@param	x	X方向拡大率
	@param	y	Y方向拡大率
	@param	z	Z方向拡大率
	@return	このインスタンスへの参照
	*/
	Matrix44& SetScale(float x, float y, float z);

	/**
	@brief	行列でベクトルを変形させる。
	@param	in	変形前ベクトル
	@return	変形後ベクトル
	*/
	Vector3DF Transform3D(const Vector3DF& in) const;

	/**
	@brief	行列でベクトルを変形させる。
	@param	in	変形前ベクトル
	@return	変形後ベクトル
	*/
	Vector4DF Transform4D(const Vector4DF& in) const;

	Matrix44 operator * (const Matrix44& right) const;

	Vector3DF operator*(const Vector3DF& right) const;

	Vector4DF operator*(const Vector4DF& right) const;

	/**
		@brief	乗算を行う。
		@param	o	出力先
		@param	in1	行列1
		@param	in2	行列2
		@return	出力先の参照
	*/
	static Matrix44& Mul( Matrix44& o, const Matrix44& in1, const Matrix44& in2 );

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


#include <array>

namespace asd
{
	/**
		@brief	長方形(整数)の構造体
	*/
	struct RectI
	{
	public:
		/**
			@brief	左上X座標
		*/
		int X;

		/**
			@brief	左上Y座標
		*/
		int Y;

		/**
			@brief	横幅
		*/
		int Width;
		
		/**
			@brief	縦幅
		*/
		int Height;

		/**
			@brief	コンストラクタ
		*/
		RectI();

		/**
			@brief	コンストラクタ
			@param	x	左上X座標
			@param	y	左上Y座標
			@param	width	横幅
			@param	height	縦幅
		*/
		RectI(int x, int y, int width, int height);

		/**
		@brief	コンストラクタ
		@param	position	左上座標
		@param	size	大きさ
		*/
		RectI(Vector2DI position, Vector2DI size);

		/**
			@brief	左上座標を取得する。
			@return	左上座標
		*/
		Vector2DI GetPosition() const;

		/**
			@brief	大きさを取得する。
			@return	大きさ
		*/
		Vector2DI GetSize() const;

		/**
			@brief	四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
			@return	座標
		*/
		std::array<Vector2DI, 4> GetVertexes() const;

		bool operator == (const RectI& other) const;

		/**
		@brief	RectF型に変換する。
		@return	RectF型
		*/
		RectF ToF() const;
	};
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <array>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
	/**
	@brief	長方形(浮動小数点)の構造体
	*/
	struct RectF
	{
	public:
		/**
		@brief	左上X座標
		*/
		float X;

		/**
		@brief	左上Y座標
		*/
		float Y;

		/**
		@brief	横幅
		*/
		float Width;

		/**
		@brief	縦幅
		*/
		float Height;

		/**
		@brief	コンストラクタ
		*/
		RectF();

		/**
		@brief	コンストラクタ
		@param	x	左上X座標
		@param	y	左上Y座標
		@param	width	横幅
		@param	height	縦幅
		*/
		RectF(float x, float y, float width, float height);

		/**
		@brief	コンストラクタ
		@param	position	左上座標
		@param	size	大きさ
		*/
		RectF(Vector2DF position, Vector2DF size);

		/**
		@brief	左上座標を取得する。
		@return	左上座標
		*/
		Vector2DF GetPosition() const;

		/**
		@brief	大きさを取得する。
		@return	大きさ
		*/
		Vector2DF GetSize() const;

		/**
		@brief	四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
		@return	座標
		*/
		std::array<Vector2DF, 4> GetVertexes() const;

		bool operator!=(const RectF& right);

		/**
		@brief	RectI型に変換する。
		@return	RectI型
		*/
		RectI ToI() const;
	};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#include<string>
#if defined(_MSC_VER)

#elif defined(__clang__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#include<typeinfo>
#if !__has_include(<cxxabi.h>)
#error "cxxabi.h is not found"
#endif
#elif defined(__GNUC__)
#include<cxxabi.h>
#include<cstdlib>
#include<cassert>
#include<typeinfo>
#endif

namespace asd
{
/**
	@brief	型から型名を人間の読める形の文字列として得る
	@detail	対応コンパイラ: msvc, gcc, clang.
*/
template<typename T> std::string GetTypeName()
{
#ifdef _MSC_VER
#ifndef _CPPRTTI
#error "/GR option is needed to get type names"
#endif
	return std::string(typeid(T).name());
#else
	int stat;
	std::string ret;
	char *pc = abi::__cxa_demangle(typeid(T).name(), 0, 0, &stat);
	switch (stat)
	{
	case 0:
		ret = std::string(pc);
		free(pc);
		return ret;
		break;
	case -1:
		assert(!"failed to memory allocation");
		return ret;
		break;
	case -2:
		assert(!"the type name couldn't be demangled");
		return ret;
		break;
	case -3:
		assert(!"there's an illegal argument");
		return ret;
		break;
	default:
		return ret;
		break;
	}
#endif
}

}

#include <stdio.h>
#include <cstddef>
#include <cassert>

namespace asd
{

/**
	@brief	確保されたメモリ領域の先頭ポインタと長さから、デバッグ時境界検査つき配列を提供する
	@details	確保されたメモリの所有権は移動しないので、使用する側でメモリを解放する必要がある
*/

template<typename T> class PointerArray
{
public:
	T* m_data; //!< 配列の先頭ポインタ
protected:
	size_t m_length; //!< 配列の長さ
public:
	typedef T Type; //!< 対象となっている型
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	配列の長さを得る
	*/
	size_t GetLength() const{return m_length;}
	
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	コンストラクタ
		@detail	T*はlength確保した配列と見なす。所有権は移動しない
		@param	idx	[in]	
	*/
	//! 
	PointerArray(T* ptr, size_t length): m_data(ptr), m_length(length){}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	idx番目の要素への参照を得る
		@detail	デバッグ時には範囲外チェックがつく
		@param	idx	[in]	要素のインデックス
	*/
	T& At(size_t idx)
	{
		assert(idx >= 0 && idx < m_length);
		return m_data[idx];
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
		@brief	idx番目の要素への参照を得る
		@detail	デバッグ時には範囲外チェックがつく
		@param	idx	[in]	要素のインデックス
	*/
	T& operator[](size_t idx)
	{
		return At(idx);
	}
};
}

#include<cstdint>

#ifdef _WIN32
/* win */
#include <Windows.h>
#elif ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
#include <chrono>
#else
/* *nix */
#include <sys/time.h>
#endif

namespace asd
{
/**
	@brief	パフォーマンスカウンタの現在の値をマイクロ秒単位で返す
*/
inline int64_t GetTime()
{
#ifdef _WIN32
	int64_t count, freq;
	if (QueryPerformanceCounter((LARGE_INTEGER*)&count))
	{
		if (QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
		{
			// オーバーフロー対策
			// return count * 1000000 / freq; と等価

			int64_t ret = 0;
			ret = count / freq;
			count -= ret * freq;
			ret *= 1000000;
			ret += count * 1000000 / freq;
			return ret;
		}
	}
	return 0;
#elif ( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#else
	struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif
}
}
#include<cassert>
#include<cstring>
namespace asd
{

/**
	@brief	型は異なるが同じ構造を持つ2変数の間でコピーする
	@details	T, Uが同じサイズかを検査し、そのサイズ分fromからtoにコピーする
	@warning	とても危険な関数なため、使用の際には細心の注意を要する。
	@param	from	[in]	コピー元のポインタ
	@param	to		[out]	コピー先のポインタ
*/
template<typename T, typename U> void TypeErasureCopy(T const* from, U* to)
{
	
#if __cplusplus >= 201103L
	static_assert(alignof(T) == alignof(U), "arguments 'from' and 'to' must be the same alignment.");
	static_assert(sizeof(T) == sizeof(U), "arguments 'from' and 'to' must be the same size.");
#else
	assert(sizeof(T) == sizeof(U));
#endif 
	memcpy(to, from, sizeof(T));
}
}
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	回転行列の計算順序
	*/
	enum class RotationOrder : int32_t
	{
		QUATERNION = 10,
		XZY = 11,
		XYZ = 12,
		ZXY = 13,
		ZYX = 14,
		YXZ = 15,
		YZX = 16,
		AXIS = 18,
	};

	/**
	@brief	キー間の補間方法
	*/
	enum class InterpolationType : int32_t
	{
		/**
		@brief	次のキーまで定数で補間
		*/
		Constant = 0,

		/**
		@brief	次のキーまで線形で補間
		*/
		Linear = 1,

		/**
		@brief	次のキーまで三次方程式で補間
		*/
		Cubic = 2,
	};

	/**
	@brief	Fカーブのキーフレーム
	*/
	struct FCurveKeyframe
	{
	public:
		/**
		@brief	時間と値
		*/
		Vector2DF				KeyValue;

		/**
		@brief	制御用ハンドル
		*/
		Vector2DF				LeftHandle;

		/**
		@brief	制御用ハンドル
		*/
		Vector2DF				RightHandle;

		/**
		@brief	補間方法
		*/
		InterpolationType		Interpolation;

		FCurveKeyframe()
		{
			Interpolation = InterpolationType::Linear;
		}
	};


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	色
	*/
	struct Color
	{
	public:

		/**
		@brief	赤
		*/
		uint8_t	R;

		/**
		@brief	緑
		*/
		uint8_t	G;

		/**
		@brief	青
		*/
		uint8_t	B;

		/**
		@brief	透明度
		*/
		uint8_t	A;

		/**
		@brief	コンストラクタ
		*/
		Color();

		/**
		@brief	コンストラクタ
		@param	r	赤
		@param	g	緑
		@param	b	青
		@param	a	透明度
		*/
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		Color operator*(const Color& right);

		Color& operator*=(const Color& right);

		bool operator==(const Color& right);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#include<vector>
#include<fstream>
#include<string>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<cstdint>
#include<climits>
#include<cassert>


namespace asd
{

class BinaryReader
{
protected:
	std::vector<uint8_t> data;
	std::size_t idx;
public:
	template<typename itrT> void ReadIn(itrT beg, itrT end)
	{
		data.assign(beg, end);
		idx = 0;
	}

	bool ReadFile(std::string const& filename) {
		std::ifstream ifs(filename, std::ios_base::in | std::ios_base::binary);
		if (!ifs){
			return false;
		}
		ifs.seekg(0, ifs.end);
		std::size_t length = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		data.reserve(length);
		ifs.read((char *)data.data(), length);
		return (bool)ifs;
	}

	template<typename T> inline T Get(){ assert(!"The type cannot be serializable."); return T();};
	bool IsEmpty()const{ return idx >= data.size(); }

	inline std::vector<uint8_t> Get(int32_t size)
	{
		
		if (idx + size > data.size()) { return std::vector<uint8_t>(); }
		std::vector<uint8_t> v(size);
		/*
		for (int i = 0; i < size; i++)
		{
			v.push_back(data.front());
			data.pop_front();
		}
		*/
		std::memcpy(v.data(), data.data() + idx, size);
		idx += size;
		return v;
	}

};



template<> inline int32_t BinaryReader::Get()
{
	size_t const size = sizeof(int32_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int32_t)."); return int32_t(); }
	int32_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;

}

template<> inline int32_t const& BinaryReader::Get()
{
	size_t const size = sizeof(int32_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int32_t)."); return int32_t(); }
	int32_t const& v = *((int32_t const*)(data.data() + idx));
	idx += size;
	return v;
}

template<> inline int16_t BinaryReader::Get()
{
	size_t const size = sizeof(int16_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int16_t)."); return int16_t(); }
	int16_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline int16_t const& BinaryReader::Get()
{
	size_t const size = sizeof(int16_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int16_t)."); return int16_t(); }
	int16_t const& v = *((int16_t const*)(data.data() + idx));
	idx += size;
	return v;
}

template<> inline uint16_t BinaryReader::Get()
{
	size_t const size = sizeof(uint16_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (uint16_t)."); return uint16_t(); }
	uint16_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline uint16_t const& BinaryReader::Get()
{
	size_t const size = sizeof(uint16_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (uint16_t)."); return uint16_t(); }
	uint16_t const& v = *((uint16_t const*)(data.data() + idx));
	idx += size;
	return v;
}

template<> inline int8_t BinaryReader::Get()
{
	size_t const size = sizeof(int8_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int8_t)."); return int8_t(); }
	int8_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline int8_t const& BinaryReader::Get()
{
	size_t const size = sizeof(int8_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (int8_t)."); return int8_t(); }
	int8_t const& v = *((int8_t const*)(data.data() + idx));
	idx += size;
	return v;
}

template<> inline uint8_t BinaryReader::Get()
{
	size_t const size = sizeof(uint8_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (uint8_t)."); return uint8_t(); }
	uint8_t v = 0;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline uint8_t const& BinaryReader::Get()
{
	size_t const size = sizeof(uint8_t);
	if (idx + size > data.size()) { assert(!"Lack of buffer (uint8_t)."); return uint8_t(); }
	uint8_t const& v = *((uint8_t const*)(data.data() + idx));
	idx += size;
	return v;
}


template<> inline std::string BinaryReader::Get()
{
	int32_t const len = Get<int32_t>();
	size_t const size = sizeof(char);

	if (idx + size * len > data.size()) { assert(!"Lack of buffer (string)."); return std::string(); }

	std::string v = std::string((char const*)(data.data() + idx), (size_t)len);
	idx += size * len;
	return v;
}
template<> inline astring BinaryReader::Get()
{
	int32_t const len = Get<int32_t>();
	size_t const size = sizeof(achar);

	if (idx + size * len > data.size()) { assert(!"Lack of buffer (astring)."); return asd::astring(); }

	asd::astring v = asd::astring((achar const*)(data.data() + idx), (size_t)len);
	idx += size * len;
	return v;
}

//template<> inline asd::achar* BinaryReader::Get()
//{
//	int8_t lenCs[4];
//	for (int i = 0; i < 4; i++)
//	{
//		assert(!data.empty());
//		if (data.empty()){
//			return nullptr;
//		}
//		lenCs[i] = data.front();
//		data.pop_front();
//	}
//
//	int32_t const len = *(static_cast<int32_t*>(static_cast<void*>(lenCs)));
//
//	asd::achar *achs = static_cast<asd::achar*>(calloc(len + 1, sizeof(asd::achar)));
//	int8_t charCs[2];
//
//	for (int32_t i = 0; i < len; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			assert(!data.empty());
//			if (data.empty()){
//				return achs;
//			}
//			charCs[j] = data.front();
//			data.pop_front();
//		}
//
//		achs[i] = *(static_cast<asd::achar*>(static_cast<void*>(charCs)));
//
//	}
//	return achs;
//}

template<> inline float BinaryReader::Get()
{
	size_t const size = sizeof(float);
	if (idx + size > data.size()) { assert(!"Lack of buffer (float)."); return float(); }
	float v = 0.0f;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline float const& BinaryReader::Get()
{
	size_t const size = sizeof(float);
	if (idx + size > data.size()) { assert(!"Lack of buffer (float)."); return float(); }
	float const& v = *((float const*)(data.data() + idx));
	idx += size;
	return v;
}

template<> inline bool BinaryReader::Get()
{
	size_t const size = sizeof(bool);
	if (idx + size > data.size()) { assert(!"Lack of buffer (bool)."); return bool(); }
	bool v = false;
	std::memcpy(&v, data.data() + idx, size);
	idx += size;
	return v;
}

template<> inline bool const& BinaryReader::Get()
{
	size_t const size = sizeof(bool);
	if (idx + size > data.size()) { assert(!"Lack of buffer (bool)."); return bool(); }
	bool const& v = *((bool const*)(data.data() + idx));
	idx += size;
	return v;
}


template<> inline Vector3DF BinaryReader::Get()
{
	Vector3DF v;
	v.X = Get<float>();
	v.Y = Get<float>();
	v.Z = Get<float>();

	return v;

}

template<> inline Vector2DF BinaryReader::Get()
{
	Vector2DF v;
	v.X = Get<float>();
	v.Y = Get<float>();

	return v;
}

template<> inline Color BinaryReader::Get()
{
	Color c;
	c.R = Get<uint8_t>();
	c.G = Get<uint8_t>();
	c.B = Get<uint8_t>();
	c.A = Get<uint8_t>();

	return c;
}

template<> inline RectI BinaryReader::Get()
{
	RectI v;
	v.X = Get<int32_t>();
	v.Y = Get<int32_t>();
	v.Width = Get<int32_t>();
	v.Height = Get<int32_t>();
	return v;
}

template<> inline RectF BinaryReader::Get()
{
	RectF v;
	v.X = Get<float>();
	v.Y = Get<float>();
	v.Width = Get<float>();
	v.Height = Get<float>();
	return v;
}

template<> inline FCurveKeyframe BinaryReader::Get()
{
	FCurveKeyframe keyframe;

	keyframe.KeyValue = Get<Vector2DF>();
	keyframe.LeftHandle = Get<Vector2DF>();
	keyframe.RightHandle = Get<Vector2DF>();
	keyframe.Interpolation = (InterpolationType)Get<int32_t>();

	return keyframe;
}

template<> inline RotationOrder BinaryReader::Get()
{
	RotationOrder rotationOrder;

	rotationOrder = (RotationOrder)Get<int32_t>();

	return rotationOrder;
}

template<> inline Matrix44 BinaryReader::Get()
{
	Matrix44 m;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			m.Values[j][i] = Get<float>();
		}
	}

	return m;

}

} 

#include<vector>
#include<cstdint>
#include<climits>
#include<fstream>
#include<string>

namespace asd
{

/**
@brief	数値、文字列をバイト列にシリアライズし、ファイルに書き出すクラス
@detail	対応する型はint32_t, int16_t, int8_t, uint8_t, std::string, astring, achar*
@warning	エンディアンは現状非互換、正当性の検査等はないので、
データを読み出す際はBinaryReaderで*書き出した順に*読み出すこと
*/
class BinaryWriter
{
protected:
	std::vector<int8_t> m_data; ///< シリアライズされたバイト列
public:
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	扱うデータが大きいことが予想される場合にあらかじめ容量を確保する
	@param	size	確保するサイズ
	*/

	void Reserve(size_t size)
	{
		m_data.reserve(size);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	シリアライズされたバイト列をconst参照で返す。
	@warning	本クラスのインスタンスより寿命を長く取る際はコピーなどして懸垂参照を避けること
	*/

	std::vector<int8_t> const& Get() const
	{
		return m_data;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int32_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int32_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int16_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	
	void Push(int16_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 2; i++)
		{
			m_data.push_back(pc[i]);
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	uint16_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/

	void Push(uint16_t content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 2; i++)
		{
			m_data.push_back(pc[i]);
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	int8_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(int8_t content){

		m_data.push_back(content);

	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	uint8_tをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする整数
	*/
	void Push(uint8_t content){

		m_data.push_back(*((uint8_t*)(&content)));

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	std::stringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(std::string const& content)
	{
		// TODO: string::lengthの返値が64bitでもたれていた場合に危険
		int32_t l = (int32_t)content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < l; i++)
		{
			m_data.push_back(static_cast<int8_t>(content.at(i)));
		}

	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	astringをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列
	*/
	void Push(astring const& content)
	{
		int32_t l = content.length();

		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&l));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (uint32_t i = 0; i < l; i++)
		{
			//copy
			achar a = content.at(i);
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			for (int j = 0; j < sizeof(achar) / sizeof(int8_t); j++) {
				m_data.push_back(pcs[j]);
			}
		}

	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	achar*をシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする文字列の先頭ポインタ
	@param	size	シリアライズする文字列のサイズ
	*/
	void Push(achar const* content, int32_t size)
	{
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&size));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
		for (int32_t i = 0; i < size; i++)
		{
			//copy
			achar a = content[i];
			int8_t* pcs = static_cast<int8_t*>(static_cast<void*>(&a));
			m_data.push_back(static_cast<int8_t>(pcs[0]));
			m_data.push_back(static_cast<int8_t>(pcs[1]));
		}

	}


	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	boolをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする実数
	*/
	void Push(bool content) {
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < sizeof(bool); i++)
		{
			m_data.push_back(pc[i]);
		}
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	floatをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズする実数
	*/
	void Push(float content){
		int8_t* pc = static_cast<int8_t*>(static_cast<void*>(&content));
		for (int i = 0; i < 4; i++)
		{
			m_data.push_back(pc[i]);
		}
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	asd::Vector3DFをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(Vector3DF const& content){
		Push(content.X);
		Push(content.Y);
		Push(content.Z);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	asd::Vector2DFをシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(Vector2DF const& content){
		Push(content.X);
		Push(content.Y);
	}

	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	asd::Matrix44をシリアライズし、バイト列の末尾に追加する
	@param	content	シリアライズするVector
	*/
	void Push(Matrix44 const& content){
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				Push(content.Values[j][i]);
			}
		}

	}



	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をストリームに書き出す。
	@param	os	書き出すストリーム
	@return 成功すればtrue, 失敗すればfalse
	*/

	bool WriteOut(std::ostream& os) const
	{
		if (!os){ return false; }
		for (auto itr = m_data.cbegin(); itr != m_data.cend(); itr++)
		{
			os << *itr;
		}
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	保持しているバイト列をファイルに書き出す。
	@param	filename	書き出すファイル名
	@param	isAppend	追記するか否か
	@return 成功すればtrue, 失敗すればfalse
	*/
	bool WriteOut(std::string const& filename, bool isAppend = false) const
	{
		std::ofstream os(
			filename,
			std::ios_base::binary | std::ios_base::out | (isAppend ? std::ios_base::app : std::ios_base::out)
		);
		if (!os) {
			return false;
		}
		if (!WriteOut(os)) { return false; }
		os.close();
		return true;
	}
	//-----------------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------------
	/**
	@brief	デストラクタ
	*/
	virtual ~BinaryWriter(){}
};
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Core;
	class Graphics;
	
	class Window;
	class Cursor;
	class Tool;

	class Keyboard;
	class Mouse;
	class Log;
	class Profiler;
	class LayerProfiler;
	class ProfilerViewer;
	class Renderer2D;

	class Joystick;
	class JoystickContainer;
	
	class File;
	class StaticFile;
	class StreamFile;

	class Sound;
	class SoundSource;

	class Texture;
	class Texture2D;
	class RenderTexture2D;
	class CubemapTexture;

	class ImagePackage;
	class MediaPlayer;

	class Shader2D;
	class Material2D;
	class Shader3D;
	class Material3D;
	class MaterialPropertyBlock;

	class Effect;

	class MassModel;
	class Model;
	class Terrain3D;

	class Font;

	class CoreChip2D;

	class Mesh;
	class Deformer;

	class AnimationSystem;
	class AnimationClip;
	class AnimationSource;
	class KeyframeAnimation;


	class ObjectSystemFactory;
	class CoreScene;
	class CoreLayer;

	class CoreShape;
	class CoreTriangleShape;
	class CoreRectangleShape;
	class CoreLineShape;
	class CoreCircleShape;
	class CoreArcShape;
	class CorePolygonShape;

	class CoreCollider2D;
	class CoreCircleCollider;
	class CoreLineCollider;
	class CorePolygonCollider;
	class CoreRectangleCollider;

	class CoreLayer2D;
	class CoreObject2D;
	class CoreDrawnObject2D;
	class CoreTextureObject2D;
	class CoreCameraObject2D;
	class CoreTextObject2D;
	class CoreEffectObject2D;
	class CoreMapObject2D;
	class CoreGeometryObject2D;

	class CoreLayer3D;
	class CoreObject3D;
	class CoreModelObject3D;
	class CoreCameraObject3D;
	class CoreEffectObject3D;
	class CoreDirectionalLightObject3D;
	class CoreMassModelObject3D;
	class CoreTerrainObject3D;

	class CorePostEffect;
	class CoreTransition;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	
	/**
		@brief	起動時に自動的に生成されるレイヤーの種類
	*/
	enum class AutoGeneratedLayerType : int32_t
	{
		Layer2D,
		Layer3D,
		Nothing,
	};

	/**
		@brief	レイヤーの種類
	*/
	enum class LayerType : int32_t
	{
		Layer2D,
		Layer3D,
	};

	enum class WritingDirection :int32_t
	{
		Vertical,
		Horizontal,
	};

	/**
		@brief	フレームレートの制御方法
	*/
	enum class FramerateMode : int32_t
	{
		/**
			@brief	固定フレームレート
		*/
		Constant,

		/**
			@brief	可変フレームレート
		*/
		Variable,
	};

	enum class TextureFormat : int32_t
	{
		R8G8B8A8_UNORM = 0,
		R16G16B16A16_FLOAT = 11,
		R32G32B32A32_FLOAT = 1,
		R8G8B8A8_UNORM_SRGB = 2,
		R16G16_FLOAT = 3,
		R8_UNORM = 4,

		BC1 = 5,
		BC2 = 6,
		BC3 = 7,
		BC1_SRGB = 8,
		BC2_SRGB = 9,
		BC3_SRGB = 10,

	};

	enum class TextureClassType : int32_t
	{
		Texture2D = 0,
		RenderTexture2D = 1,
		CubemapTexture = 2,
		DepthBuffer = 3,
	};

	/**
		@brief	描画時のブレンドモードを表す列挙体
	*/
	enum class AlphaBlendMode : int32_t
	{
		/// <summary>
		/// 不透明
		/// </summary>
		Opacity = 0,
		/// <summary>
		/// 透明
		/// </summary>
		Blend = 1,
		/// <summary>
		/// 加算
		/// </summary>
		Add = 2,
		/// <summary>
		/// 減算
		/// </summary>
		Sub = 3,
		/// <summary>
		/// 乗算
		/// </summary>
		Mul = 4,
		/// <summary>
		/// 全加算(内部処理用)
		/// </summary>
		AddAll = 5,
		/// <summary>
		/// 全不透明(内部処理用)
		/// </summary>
		OpacityAll = 6,
	};

	enum class TextureFilterType : int32_t
	{
		Nearest = 0,
		Linear = 1,
	};

	enum class TextureWrapType : int32_t
	{
		Repeat = 0,
		Clamp = 1,
	};

	enum class CullingType : int32_t
	{
		Front = 0,
		Back = 1,
		Double = 2,
	};

	/**
		@brief	描画方法
	*/
	enum class GraphicsDeviceType : int32_t
	{
		Default = 0,		///< 実行環境で最も安定している描画方法(初期化時に使用)
		DirectX11 = 1,		///< DirectX11
		OpenGL = 2,			///< OpenGL
	};

	/**
		@brief	ウインドウ表示位置
	*/
	enum class WindowPositionType : int32_t
	{
		Default = 0,		///< OSによる自動選択
		Centering = 1,		///< プライマリモニタの中央
	};

	/**
		@brief	カラースペース
		@note
		リニアスペースが設定されている場合、画像を逆ガンマ補正して読み込み、描画結果はガンマ補正して出力される。
	*/
	enum class ColorSpaceType : int32_t
	{
		GammaSpace = 0,	///< ガンマスペース
		LinearSpace = 1, ///< リニアスペース
	};

	/**
	@brief	イメージパッケージ内の要素の種類
	*/
	enum class ImagePackageElementType : int32_t
	{
		Image = 0,	///< 画像
		Button, ///< ボタン
	};

	/**
		@brief	イメージパッケージ内の要素の種類(追加情報)
	*/
	enum class ImagePackageAdditionalElementType : int32_t
	{
		None,		///< なし
		Normal,		///< ボタンの時、通常状態で表示される
		Pressed,	///< ボタンの時、押されている状態で表示される
		Hovered,	///< ボタンの時、マウスが重なっている状態で表示される
	};

	/**
	@brief	ボタンの押し状態を示す列挙型
	*/
	enum class ButtonState : int32_t
	{
		Push,	///<	ボタンをこのフレームで押した状態
		Release,	///<	ボタンをこのフレームで離した状態
		Hold,	///<	ボタンを押し続けている状態
		Free,	///<	ボタンを離し続けている状態
	};


	enum class Object2DType : int32_t
	{
		Unknown,
		Texture,
		Camera,
		Effect,
		Text,
		Map,
		Geometry,
	};

	enum class RenderedObject3DType : int32_t
	{
		Unknown,
		Mesh,
		Camera,
		Effect,
		DirectionalLight,
		MassObject,
		Terrain,
	};

	/**
		@brief	シェーダーに対して外部から設定可能なプロパティの変数の種類
	*/
	enum class ShaderVariableType : int32_t
	{
		Unknown,
		Float,
		Vector2DF,
		Vector3DF,
		Vector4DF,
		Vector4DF_Array,
		Matrix44,
		Matrix44_Array,
		Texture2D,
		CubemapTexture,
	};

	/**
	@brief	3D描画時に表示されるバッファ
	*/
	enum class VisualizedBufferType : int32_t
	{
		FinalImage,
		DiffuseColor,
		SpecularColor,
		Normal,
		Smoothness,
		Environment,
		Occlusion,
	};

	/**
		@brief	描画設定のクラス
	*/
	class RenderSettings
	{
	public:
		/**
			@brief	遅延レンダリングから軽量レンダリングに変更し高速に描画するか?
		*/
		bool IsLightweightMode;
		
		/**
			@brief	画面に表示されるバッファ
		*/
		VisualizedBufferType VisualizedBuffer;

		RenderSettings()
		{
			IsLightweightMode = false;
			VisualizedBuffer = VisualizedBufferType::FinalImage;
		}
	};

	/**
	@brief	参照カウンタのインターフェース
	*/
	class IReference
	{
	public:
		/**
			@brief	参照カウンタを加算する。
			@return	加算後の参照カウンタ
		*/
		virtual int AddRef() = 0;

		/**
		@brief	参照カウンタを取得する。
		@return	参照カウンタ
		*/
		virtual int GetRef() = 0;

		/**
		@brief	参照カウンタを減算する。0になった時、インスタンスを削除する。
		@return	減算後の参照カウンタ
		*/
		virtual int Release() = 0;
	};

	/**
		@brief	非同期読み込みの進捗
	*/
	enum class LoadState : int32_t
	{
		Loading,
		WaitSync,
		Loaded,
		Failed
	};

	/**
		@brief	ツールで表示するダイアログの種類
	*/
	enum class ToolDialogStyle {
		Info,
		Warning,
		Error,
		Question
	};

	/**
		@brief	ツールで表示するダイアログのボタンの種類
	*/
	enum class ToolDialogButtons {
		OK,
		OKCancel,
		YesNo,
		Quit
	};

	/**
		@brief	ツールで表示するダイアログの結果
	*/
	enum class ToolDialogSelection {
		OK,
		Cancel,
		Yes,
		No,
		Quit,
		None,
		Error
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <memory>
#include <atomic>


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	参照カウンタオブジェクト
	*/
	class ReferenceObject
		: public IReference
	{
	private:
		mutable std::atomic<int32_t> m_reference;

	public:
		ReferenceObject();

		virtual ~ReferenceObject();

		virtual int AddRef();

		virtual int GetRef();

		virtual int Release();
	};

#if !SWIG
	/**
	@brief	全ての参照カウンタの値の合計を取得する。
	@return	参照カウンタ合計値
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	int32_t GetGlobalReferenceCount();

	/**
	@brief	全ての参照カウンタの値を取得し、0ならDLLを解放する。
	@note
	実体はEngineに存在するのでそちらを参照する。
	*/
	void CheckDLL();

	template <typename T>
	struct ReferenceDeleter
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);
		}
	};

	template <typename T>
	struct ReferenceDeleterWithReleaseDLL
	{
		void operator ()(T* p)
		{
			auto p_ = ((IReference*) p);
			SafeRelease(p_);

			auto ref = GetGlobalReferenceCount();
			if (ref == 0)
			{
				CheckDLL();
			}
		}
	};

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtr(T* p)
	{		
		return std::shared_ptr<T>(p, ReferenceDeleter<T>());
	}

	template <typename T>
	static std::shared_ptr<T> CreateSharedPtrWithReleaseDLL(T* p)
	{
		return std::shared_ptr<T>(p, ReferenceDeleterWithReleaseDLL<T>());
	}

#endif

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};


#include <stdio.h>
#include <stdint.h>
#include <memory>
#include <vector>


namespace asd {


class File;
class MediaPlayer;
class Sound;
class SoundSource;
class StaticFile;
class StreamFile;
class Tool;
class AutoGeneratedWrapperAccessor;

/**
	@brief ファイルの操作に関する処理を提供するクラス 
*/
class File {
	void* self = nullptr;
	bool isCtrlSelf = false;
	friend class File;
	friend class MediaPlayer;
	friend class Sound;
	friend class SoundSource;
	friend class StaticFile;
	friend class StreamFile;
	friend class Tool;
	friend class Graphics;
	friend class AutoGeneratedWrapperAccessor;
	File(void* self, bool isCtrlSelf);
	public:
	/**
		@brief 
	*/
	virtual ~File();
	
	/**
		@brief 一括読込のファイルを生成する。 
		@param path ファイルパス 
		@return ファイル 
	*/
	std::shared_ptr<StaticFile> CreateStaticFile(const char16_t* path);
	
	/**
		@brief 一括読込のファイルを非同期に生成する。 
		@param path ファイルパス 
		@return ファイル 
	*/
	std::shared_ptr<StaticFile> CreateStaticFileAsync(const char16_t* path);
	
	/**
		@brief 逐次読込のファイルを生成する。 
		@param path ファイルパス 
		@return ファイル 
	*/
	std::shared_ptr<StreamFile> CreateStreamFile(const char16_t* path);
	
	/**
		@brief ファイル操作する時のルートのディレクトリを追加する。 
		@param path ルートのディレクトリのパス 
	*/
	void AddRootDirectory(const char16_t* path);
	
	/**
		@brief ファイル操作する時のルートのパスワード付パッケージを追加する。 
		@param path パッケージのパス 
		@param password パスワード 
	*/
	void AddRootPackageWithPassword(const char16_t* path,const char16_t* password);
	
	/**
		@brief ファイル操作する時のルートのパッケージを追加する。 
		@param path パッケージのパス 
	*/
	void AddRootPackage(const char16_t* path);
	
	/**
		@brief 追加されたパッケージを全て消去する。 
	*/
	void ClearRootDirectories();
	
	/**
		@brief ファイルが存在するか取得する。 
		@param path ファイルのパス 
		@return ファイルが存在するか? 
	*/
	bool Exists(const char16_t* path) const;
	
};

/**
	@brief 映像ファイルを再生するためのクラス 
*/
class MediaPlayer {
	void* self = nullptr;
	bool isCtrlSelf = false;
	friend class File;
	friend class MediaPlayer;
	friend class Sound;
	friend class SoundSource;
	friend class StaticFile;
	friend class StreamFile;
	friend class Tool;
	friend class Graphics;
	friend class AutoGeneratedWrapperAccessor;
	MediaPlayer(void* self, bool isCtrlSelf);
	public:
	/**
		@brief 
	*/
	virtual ~MediaPlayer();
	
	/**
		@brief 映像を再生する。 
		@param isLoopingMode ループ再生するか? 
	*/
	bool Play(bool isLoopingMode);
	
	/**
		@brief 映像を読み込む。 
		@param path ファイルのパス 
	*/
	bool Load(const char16_t* path);
	
	/**
		@brief 現在の映像をテクスチャに書き込む。 
		@param target 書き込み先 
	*/
	bool WriteToTexture2D(std::shared_ptr<Texture2D> target);
	
	/**
		@brief 読み込んだ映像の大きさを取得する。 
	*/
	Vector2DI GetSize() const;
	
	/**
		@brief 現在再生中の映像のフレーム番号を取得する。 
	*/
	int32_t GetCurrentFrame() const;
	
};

/**
	@brief 音を管理するクラス 
*/
class Sound {
	void* self = nullptr;
	bool isCtrlSelf = false;
	friend class File;
	friend class MediaPlayer;
	friend class Sound;
	friend class SoundSource;
	friend class StaticFile;
	friend class StreamFile;
	friend class Tool;
	friend class Graphics;
	friend class AutoGeneratedWrapperAccessor;
	Sound(void* self, bool isCtrlSelf);
	public:
	/**
		@brief 
	*/
	virtual ~Sound();
	
	/**
		@brief 音を読み込む。 
		@param path パス 
		@param isDecompressed 解凍するか? 
		@return 音源 
	*/
	std::shared_ptr<SoundSource> CreateSoundSource(const char16_t* path,bool isDecompressed);
	
	/**
		@brief 音を再生する。 
		@param soundSource 音源 
		@return ID 
	*/
	int32_t Play(std::shared_ptr<SoundSource> soundSource);
	
	/**
		@brief 音が再生中か、取得する。 
		@param id ID 
		@return 再生中か? 
	*/
	bool GetIsPlaying(int32_t id);
	
	/**
		@brief 全ての再生中の音を停止する。 
	*/
	void StopAll();
	
	/**
		@brief 指定した音を停止する。 
		@param id ID 
	*/
	void Stop(int32_t id);
	
	/**
		@brief 指定した音を一時停止する。 
		@param id ID 
	*/
	void Pause(int32_t id);
	
	/**
		@brief 指定した一時停止中の音の一時停止を解除する。 
		@param id ID 
	*/
	void Resume(int32_t id);
	
	/**
		@brief 指定した音の音量を設定する。 
		@param id ID 
		@param volume 音量(0.0～1.0) 
	*/
	void SetVolume(int32_t id,float volume);
	
	/**
		@brief 指定した音をフェードインさせる。 
		@param id ID 
		@param second フェードインに使用する時間(秒) 
	*/
	void FadeIn(int32_t id,float second);
	
	/**
		@brief 指定した音をフェードアウトさせる。 
		@param id ID 
		@param second フェードアウトに使用する時間(秒) 
	*/
	void FadeOut(int32_t id,float second);
	
	/**
		@brief 任意の音量に音量を一定時間かけて変更する。 
		@param id 音のID 
		@param second 変更にかかる時間(秒) 
		@param targetedVolume 変更先の音量(0.0～1.0) 
	*/
	void Fade(int32_t id,float second,float targetedVolume);
	
	/**
		@brief 再生速度変更するかを取得する。 
		@param id 音のID 
		@return 再生速度変更するか? 
	*/
	bool GetIsPlaybackSpeedEnabled(int32_t id);
	
	/**
		@brief 再生速度変更するかを設定する。 
		@param id 音のID 
		@param isPlaybackSpeedEnabled 再生速度変更するか? 
	*/
	void SetIsPlaybackSpeedEnabled(int32_t id,bool isPlaybackSpeedEnabled);
	
	/**
		@brief 再生速度を取得する。 
		@param id 音のID 
		@return 再生速度(比率) 
	*/
	float GetPlaybackSpeed(int32_t id);
	
	/**
		@brief 再生速度を設定する。 
		@param id 音のID 
		@param playbackSpeed 再生速度(比率) 
	*/
	void SetPlaybackSpeed(int32_t id,float playbackSpeed);
	
	/**
		@brief パン位置を取得する。 
		@param id 音のID 
		@return パン位置, 0.0で中央, -1.0で左, 1.0で右。 
	*/
	float GetPanningPosition(int32_t id);
	
	/**
		@brief パン位置を設定する。 
		@param id 音のID 
		@param panningPosition パン位置, 0.0で中央, -1.0で左, 1.0で右。 
	*/
	void SetPanningPosition(int32_t id,float panningPosition);
	
};

/**
	@brief 音源のクラス 
*/
class SoundSource {
	void* self = nullptr;
	bool isCtrlSelf = false;
	friend class File;
	friend class MediaPlayer;
	friend class Sound;
	friend class SoundSource;
	friend class StaticFile;
	friend class StreamFile;
	friend class Tool;
	friend class Graphics;
	friend class AutoGeneratedWrapperAccessor;
	SoundSource(void* self, bool isCtrlSelf);
	public:
	/**
		@brief 
	*/
	virtual ~SoundSource();
	
	/**
		@brief ループポイントの開始地点(秒)を取得する。 
		@return 開始地点(秒) 
	*/
	float GetLoopStartingPoint() const;
	
	/**
		@brief ループポイントの開始地点(秒)を設定する。 
		@param startingPoint 開始地点(秒) 
	*/
	void SetLoopStartingPoint(float startingPoint);
	
	/**
		@brief ループポイントの終了地点(秒)を取得する。 
		@return 終了地点(秒) 
	*/
	float GetLoopEndPoint() const;
	
	/**
		@brief ループポイントの終了地点(秒)を設定する。 
		@param endPoint 終了地点(秒) 
	*/
	void SetLoopEndPoint(float endPoint);
	
	/**
		@brief ループするかを取得する。 
		@return ループするか? 
	*/
	bool GetIsLoopingMode() const;
	
	/**
		@brief ループするかを設定する。 
		@param isLoopingMode ループするか? 
	*/
	void SetIsLoopingMode(bool isLoopingMode);
	
	/**
		@brief 音の長さを取得する。 
		@return 長さ(秒) 
	*/
	float GetLength();
	
};

/**
	@brief 一括してファイルを読み込むクラス 
*/
class StaticFile {
	void* self = nullptr;
	bool isCtrlSelf = false;
	friend class File;
	friend class MediaPlayer;
	friend class Sound;
	friend class SoundSource;
	friend class StaticFile;
	friend class StreamFile;
	friend class Tool;
	friend class Graphics;
	friend class AutoGeneratedWrapperAccessor;
	StaticFile(void* self, bool isCtrlSelf);
	public:
	/**
		@brief 
	*/
	virtual ~StaticFile();
	
	/**
		@brief 読み込まれたバッファを取得する。 
		@return バッファ 
	*/
	const std::vector<uint8_t>& GetBuffer() const;
	
	/**
		@brief ファイルのパスを取得する。 
		@return パス 
	*/
	const char16_t* GetFullPath() const;
	
	/**
		@brief 読み込まれたバッファの先頭のポインタを取得する。 
		@return ポインタ 
	*/
	void* GetData();
	
	/**
		@brief 読み込まれたバッファのサイズを取得する。 
		@return サイズ 
	*/
	int32_t GetSize();
	
	/**
		@brief パッケージ内からファイルが読み込まれているかを取得する。 
		@return パッケージ内からファイルが読み込まれているか? 
	*/
	bool GetIsInPackage() const;
	
	/**
		@brief ファイルの非同期読み込みの進捗を取得する。 
		@return ファイルの非同期読み込みの進捗を表す列挙子。 
	*/
	LoadState GetLoadState() const;
	
};

/**
	@brief 部分的にファイルを読み込むクラス 
*/
class StreamFile {
	void* self = nullptr;
	bool isCtrlSelf = false;
	friend class File;
	friend class MediaPlayer;
	friend class Sound;
	friend class SoundSource;
	friend class StaticFile;
	friend class StreamFile;
	friend class Tool;
	friend class Graphics;
	friend class AutoGeneratedWrapperAccessor;
	StreamFile(void* self, bool isCtrlSelf);
	public:
	/**
		@brief 
	*/
	virtual ~StreamFile();
	
	/**
		@brief ファイルのサイズを取得する。 
		@return サイズ 
	*/
	int32_t GetSize() const;
	
	/**
		@brief ファイル内で現在読み込んでいる位置を取得する。 
		@return 現在読み込んでいる位置 
	*/
	int32_t GetCurrentPosition() const;
	
	/**
		@brief ファイルからデータを読み込み、一時的なバッファに格納する。 
		@param size 読み込むサイズ 
		@return 読み込んだサイズ 
	*/
	int32_t Read(int32_t size);
	
	/**
		@brief 読み込まれた一時的なバッファの先頭のポインタを取得する。 
		@return ポインタ 
	*/
	void* GetTempBuffer();
	
	/**
		@brief 読み込まれた一時的なバッファのサイズを取得する。 
		@return サイズ 
	*/
	int32_t GetTempBufferSize();
	
	
/**
	@brief	指定したサイズ分、ファイルを読み込む。
	@param	buffer	出力先
	@param	size	読み込まれるサイズ
*/
void Read(std::vector<uint8_t>& buffer, int32_t size)
{
	auto result = Read(size);
	buffer.resize(result);
	memcpy(buffer.data(), GetTempBuffer(), result);
}

};

/**
	@brief ツールを開発するための低レイヤーな命令群のクラス 
*/
class Tool {
	void* self = nullptr;
	bool isCtrlSelf = false;
	friend class File;
	friend class MediaPlayer;
	friend class Sound;
	friend class SoundSource;
	friend class StaticFile;
	friend class StreamFile;
	friend class Tool;
	friend class Graphics;
	friend class AutoGeneratedWrapperAccessor;
	Tool(void* self, bool isCtrlSelf);
	public:
	/**
		@brief 
	*/
	virtual ~Tool();
	
	/**
		@brief フルスクリーンでウインドウの表示を開始する。 
		@param name ウインドウ名 
		@param offset ウインドウ上部の位置のオフセット 
	*/
	bool BeginFullscreen(const char16_t* name,int32_t offset);
	
	/**
		@brief ウインドウの表示を開始する。 
		@param name ウインドウ名 
	*/
	bool Begin(const char16_t* name);
	
	/**
		@brief ウインドウの表示を終了する。 
	*/
	void End();
	
	/**
		@brief 分割線を表示する。 
	*/
	void Separator();
	
	/**
		@brief 次に表示する要素を改行せずに表示する。 
	*/
	void SameLine();
	
	/**
		@brief テキストを表示する。 
		@param text テキスト名 
	*/
	void Text(const char16_t* text);
	
	/**
		@brief ボタンを表示する。 
		@param label ラベル名 
	*/
	bool Button(const char16_t* label);
	
	/**
		@brief 画像を表示する。 
		@param user_texture 画像 
		@param size 大きさ 
	*/
	void Image(std::shared_ptr<Texture2D> user_texture,const Vector2DF& size);
	
	/**
		@brief コンボボックスを表示する。 
		@param label ラベル名 
		@param preview_value コンボボックスの先頭に表示される文字列 
	*/
	bool BeginCombo(const char16_t* label,const char16_t* preview_value);
	
	/**
		@brief コンボボックスを表示を終了する。 
	*/
	void EndCombo();
	
	/**
		@brief 文字を入力する。 
		@param label ラベル名 
		@param buf 文字が格納されるバッファ 
		@param buf_size バッファサイズ 
	*/
	bool InputText(const char16_t* label,int8_t* buf,int32_t buf_size);
	
	/**
		@brief 整数を入力する。 
		@param label ラベル名 
		@param v 値 
	*/
	bool InputInt(const char16_t* label,int* v);
	
	/**
		@brief 色を変更する。 
		@param label ラベル名 
		@param vs 色(RGB+A) 
	*/
	bool ColorEdit4(const char16_t* label,float* vs);
	
	/**
		@brief 選択可能な文字列を表示する。 
		@param label ラベル名 
		@param selected 選択されているか? 
	*/
	bool Selectable(const char16_t* label,bool selected);
	
	/**
		@brief リストボックスを表示する。 
		@param label ラベル名 
		@param current_item 選択されているアイテム 
		@param items アイテム(;区切り) 
	*/
	bool ListBox(const char16_t* label,int* current_item,const char16_t* items);
	
	/**
		@brief 画面上のメニューバーの表示を開始する。 
	*/
	bool BeginMainMenuBar();
	
	/**
		@brief 画面上のメニューバーの表示を終了する。 
	*/
	void EndMainMenuBar();
	
	/**
		@brief メニューバー(ウインドウ等)の表示を開始する。 
	*/
	bool BeginMenuBar();
	
	/**
		@brief メニューバーの表示を終了する。 
	*/
	void EndMenuBar();
	
	/**
		@brief メニューの表示を開始する。 
		@param label ラベル名 
	*/
	bool BeginMenu(const char16_t* label);
	
	/**
		@brief メニューの表示を終了する。 
	*/
	void EndMenu();
	
	/**
		@brief メニューアイテムを表示する。 
		@param label ラベル名 
		@param shortcut ショートカット 
		@param p_selected 選択されているか? 
	*/
	bool MenuItem(const char16_t* label,const char16_t* shortcut,bool* p_selected);
	
	/**
		@brief 行数を設定する。 
		@param count 行数 
	*/
	void Columns(int count);
	
	/**
		@brief 次の行に移動する。 
	*/
	void NextColumn();
	
	/**
		@brief 現在の行のインデックスを取得する。 
	*/
	int GetColumnIndex();
	
	/**
		@brief 行の幅を取得する。 
		@param column_index インデックス 
	*/
	float GetColumnWidth(int column_index);
	
	/**
		@brief 行の幅を設定する。 
		@param column_index インデックス 
		@param width 幅 
	*/
	void SetColumnWidth(int column_index,float width);
	
	/**
		@brief 現在選択されているアイテムにフォーカスを設定する。 
	*/
	void SetItemDefaultFocus();
	
	/**
		@brief ファイルを開くダイアログを開く。 
		@param filterList フィルタ 
		@param defaultPath 最初に表示するディレクトリのパス 
		@return 開くパス 
	*/
	const char16_t* OpenDialog(const char16_t* filterList,const char16_t* defaultPath);
	
	/**
		@brief ファイルを保存するダイアログを開く。 
		@param filterList フィルタ 
		@param defaultPath 最初に表示するディレクトリのパス 
		@return 保存するパス 
	*/
	const char16_t* SaveDialog(const char16_t* filterList,const char16_t* defaultPath);
	
	/**
		@brief フォルダを選択するダイアログを開く。 
		@param defaultPath 最初に表示するディレクトリのパス 
		@return フォルダのパス 
	*/
	const char16_t* PickFolder(const char16_t* defaultPath);
	
	/**
		@brief フォントを追加する。 
		@param filename フォントへのパス 
		@param size_pixels フォントサイズ 
	*/
	void AddFontFromFileTTF(const char16_t* filename,float size_pixels);
	
	/**
		@brief ダイアログを表示する。 
		@param message メッセージ 
		@param title タイトル 
		@param style ダイアログの種類 
		@param buttons ボタンの種類 
	*/
	ToolDialogSelection ShowDialog(const char16_t* message,const char16_t* title,ToolDialogStyle style,ToolDialogButtons buttons);
	
};


};



#include <cstdint>




namespace asd{

	/**
	@brief マウスの左右ボタンの状態を扱うクラス
	*/
	class SideButton
	{
	private:
		ButtonState m_mouseButtonState;
	public:
		/**
		@brief ボタンの押し状態を返す
		@return ボタンの押し状態
		*/
		virtual ButtonState GetButtonState() const;

		SideButton(){}
		SideButton(ButtonState mouseButtonState,bool doubleClicked);
	};
};




namespace asd{

	/**
	@brief マウスの中ボタンの状態を扱うクラス
	*/
	class MiddleButton
	{
		friend class Mouse;

	private:
		ButtonState m_mouseButtonState;
		double m_rotation;
	public:
		/**
		@brief ボタンの押し状態を返す
		*/
		virtual ButtonState GetButtonState() const;

		/**
		@brief ホイールの回転度合いを返す
		*/
		virtual float GetRotation() const;
		MiddleButton(ButtonState mouseButtonState, float rotation);
		MiddleButton(){}
	};
};

#include <memory>

namespace asd{


	/**
	@brief マウスのボタン一覧を示す列挙型
	*/
	enum class MouseButtons : int32_t
	{
		/**
		@brief 左ボタン
		*/
		ButtonLeft = 0,

		/**
		@brief 右ボタン
		*/
		ButtonRight = 1,

		/**
		@brief 中央ボタン
		*/
		ButtonMiddle = 2,

		/**
		@brief サブボタン1
		*/
		SubButton1 = 3,

		/**
		@brief サブボタン2
		*/
		SubButton2 = 4,

		/**
		@brief サブボタン3
		*/
		SubButton3 = 5,

		/**
		@brief サブボタン4
		*/
		SubButton4 = 6,

		/**
		@brief サブボタン5
		*/
		SubButton5 = 7,
	};
	
	/**
	@brief マウスからの入力を取得するクラス
	*/
	class Mouse
	{
	public:
		/**
		@brief 非推奨である。(GetButtonInputStateを代わりに使用する)
		@note
		*/
		virtual SideButton* GetLeftButton() const = 0;


		/**
		@brief 非推奨である。(GetButtonInputStateを代わりに使用する)
		@note
		*/
		virtual SideButton* GetRightButton() const = 0;


		/**
		@brief 非推奨である。(GetButtonInputStateを代わりに使用する)
		*/
		virtual MiddleButton* GetMiddleButton() const = 0;


		/**
		@brief マウスポインタの位置を取得する。
		*/
		virtual Vector2DF GetPosition() const = 0;

		/**
			@brief マウスポインタの位置を設定する。
		*/
		virtual void SetPosition(Vector2DF pos) = 0;

		/**
		@brief 指定したボタンの入力状態を取得を取得する。
		*/
		virtual ButtonState GetButtonInputState(MouseButtons mouseButton) const = 0;


		/**
		@brief ホイールの回転度合いを取得する。
		*/
		virtual float GetWheel() const = 0;

	};
};

namespace asd{
	/**
	@brief	Altseedで扱うキーボードのキー一覧
	*/
	enum class Keys : int32_t
	{
		Unknown,
		Space,			///< スペース
		Apostrophe,		///< ： (US配列では ` )
		Comma,			///< ，
		Minus,			///< －
		Period,			///< ．
		Slash,			///< ／
		Num0,			///< 0
		Num1,			///< 1
		Num2,			///< 2
		Num3,			///< 3
		Num4,			///< 4
		Num5,			///< 5
		Num6,			///< 6
		Num7,			///< 7
		Num8,			///< 8
		Num9,			///< 9
		Semicolon,		///< ；
		Equal,			///< ＾ (US配列では ＝ )
		A,				///< A
		B,				///< B
		C,				///< C
		D,				///< D
		E	,			///< E
		F,				///< F
		G,				///< G
		H,				///< H
		I,				///< I
		J,				///< J
		K,				///< K
		L,				///< L
		M,				///< M
		N,				///< N
		O,				///< O
		P,				///< P
		Q,				///< Q
		R,				///< R
		S,				///< S
		T,				///< T
		U,				///< U
		V,				///< V
		W,				///< W
		X,				///< X
		Y,				///< Y
		Z,				///< Z
		LeftBracket,	///< @ (US配列では ［ )
		Backslash,		///< ] (US配列では ＼ )
		RightBracket,	///< [ (US配列では ］ )
		GraveAccent,	///< 半角/全角
		World1,
		World2,
		Escape,			///< Esc
		Enter,			///< Enter
		Tab,			///< Tab 
		Backspace,		///< BackSpace
		Insert,			///< Insert 
		Delete,			///< Delete 
		Right,			///< →
		Left,			///< ←
		Down,			///< ↓
		Up,				///< ↑
		PageUp,			///< PageUp
		PageDown,		///< PageDown
		Home,			///< Home 
		End,			///< End
		CapsLock,		///< CapsLock
		ScrollLock,		///< ScrollLock
		NumLock,		///< NumLock
		PrintScreen,	///< PrintScreen
		Pause,			///< Pause
		F1,				///< F1
		F2,				///< F2
		F3,				///< F3
		F4,				///< F4
		F5,				///< F5
		F6,				///< F6
		F7,				///< F7
		F8,				///< F8
		F9,				///< F9
		F10,			///< F10
		F11,			///< F11
		F12,			///< F12
		F13,			///< F13
		F14,			///< F14
		F15,			///< F15
		F16,			///< F16
		F17,			///< F17
		F18,			///< F18
		F19,			///< F19
		F20,			///< F20
		F21,			///< F21
		F22,			///< F22
		F23,			///< F23
		F24,			///< F24
		F25,			///< F25
		Keypad0,		///< テンキーの0
		Keypad1,		///< テンキーの1
		Keypad2,		///< テンキーの2
		Keypad3,		///< テンキーの3
		Keypad4,		///< テンキーの4
		Keypad5,		///< テンキーの5
		Keypad6,		///< テンキーの6
		Keypad7,		///< テンキーの7
		Keypad8,		///< テンキーの8
		Keypad9,		///< テンキーの9
		KeypadDecimal,	///< テンキーの．
		KeypadDivide,	///< テンキーの／
		KeypadMultiply,	///< テンキーの＊
		KeypadSubstract,///< テンキーの－
		KeypadAdd,		///< テンキーの＋
		KeypadEnter,	///< テンキーのEnter
		KeypadEqual,	///< テンキーの＝
		LeftShift,		///< 左Shift
		LeftControl,	///< 左Ctrl
		LeftAlt,		///< 左Alt
		LeftWin,		///< 左Win
		RightShift,		///< 右Shift
		RightControl,	///< 右Ctrl
		RightAlt,		///< 右Alt
		RightWin,		///< 右Win
		Menu,			///< コンテキストメニュー
		Last,
		MAX
	};

	/**
	@brief キーボードからの入力を取得するクラス
	*/
	class Keyboard
	{
	public:
		/**
		@brief 特定のキーの押し状態をKeyState列挙型で返す
		@param 押し状態を調べたいキー
		@return 押し状態
		*/
		virtual ButtonState GetKeyState(Keys key) const = 0;
	};

};

namespace asd{

	/**
		@brief	ジョイステイックの種類
	*/
	enum class JoystickType : int32_t
	{
		Other,
		PS4,
		XBOX360,
	};

	/**
		@brief	ジョイステイックのボタンの種類
	*/
	enum class JoystickButtonType : int32_t
	{
		Start,
		Select,

		LeftUp,
		LeftDown,
		LeftLeft,
		LeftRight,

		RightUp,	///< PSx Triangle
		RightRight,	///< PSx Circle
		RightLeft,	///< PSx Square
		RightDown,	///< PSx Cross

		L1,
		R1,
		L2,
		R2,
		L3,
		R3,

		LeftStart,	///< XBOX360 Start, PS4 Options
		RightStart,	///< XBOX360 Select, PS4 TouchPad

		Max,
	};

	/**
		@brief	ジョイステイックの軸の種類
	*/
	enum class JoystickAxisType : int32_t
	{
		LeftH,
		LeftV,
		RightH,
		RightV,
		L2,
		R2,

		Max,
	};

	/**
	@brief	ジョイステイックのクラス
	*/
	class Joystick
	{
	public:
		Joystick(){}
		virtual ~Joystick(){}

		/**
		@brief	ジョイスティックの名称を取得する。
		@return	名称
		*/
		virtual const asd::achar* GetJoystickName() = 0;

		/**
			@brief	ジョイスティックの種類を取得する。
		*/
		virtual JoystickType GetJoystickType() const = 0;

		/**
		@brief	ジョイスティックのボタンの合計数を返す
		*/
		virtual int GetButtonsCount() = 0;

		/**
		@brief	ジョイスティックの軸の合計数を返す
		*/
		virtual int GetAxesCount() = 0;

		/**
		@brief	指定したボタンの押し状態を返す
		@param	at	何番目のボタンの押し状態を取得するか
		@return 押し状態
		*/
		virtual ButtonState GetButtonState(int at) = 0;

		/**
			@brief	特定のジョイステイックの場合(PS4)に指定したボタンの押し状態を取得する。
			@param	type	ボタンの種類
			@return 押し状態
		*/
		virtual ButtonState GetButtonStateAt(JoystickButtonType type) = 0;

		/**
		@brief	指定した軸の倒し具合を返す
		@param	at	何番目の軸配列の倒し具合を取得するか
		@return 倒し具合
		*/
		virtual float GetAxisState(int at) = 0;

		/**
		@brief	特定のジョイステイックの場合(PS4)に指定した指定した軸の倒し具合を取得する。
		@param	type	軸の種類
		@return 倒し具合
		*/
		virtual float GetAxisStateAt(JoystickAxisType type) = 0;
	};
};



namespace asd{
	class JoystickContainer
	{
	public:
		JoystickContainer(){}
		virtual ~JoystickContainer(){}

		/**
		@brief	引数に指定した番号のジョイスティックが接続されているかを取得する。
		@param	at	何番目のジョイスティックの接続を確認するか
		@return 接続されているか否か
		*/
		virtual bool GetIsPresentAt(int at) = 0;

		/**
		@brief	引数に指定した番号のジョイスティッククラスを取得する。
		@param	at	何番目のジョイスティッククラスを取得するか
		@return 該当するジョイスティッククラス
		*/
		virtual Joystick* GetJoystickAt(int at) = 0;

		/**
		@brief	全てのジョイスティックの接続状態を更新する。
		*/
		virtual void RefreshAllJoysticks() = 0;
	};
};

#include <string>
#include <memory>

namespace asd
{
	enum class LogLevel : int32_t
	{
		Error = 1 << 1,
		Critical = 1 << 2,
		Warning = 1 << 3,
		Information = 1 << 4,
		All = 2147483647	// INT_MAXと等しい
	};

	class Log
	{
	public:
		virtual ~Log()
		{
		}

		/**
			@brief	ログ ファイルへ文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const achar* text, LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ見出しとして文字列を書き込む。
			@param	text	[in]	書き込む文字列
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const achar* text, LogLevel level = LogLevel::All) = 0;

#if !_WIN32 && !SWIG
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const wchar_t* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const wchar_t* text, LogLevel level = LogLevel::All) = 0;
#endif

#ifndef SWIG
	public:
		/**
		@brief	ログ ファイルへ文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void Write(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLine(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteStrongly(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ強調された文字列を書き込み、改行する。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteLineStrongly(const char* text, LogLevel level = LogLevel::All) = 0;

		/**
		@brief	ログ ファイルへ見出しとして文字列を書き込む。
		@param	text	[in]	書き込む文字列
		@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHeading(const char* text, LogLevel level = LogLevel::All) = 0;
#endif

		/**
			@brief	ログ ファイルへ水平線を書き込む。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void WriteHorizontalRule(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへ表形式(<table> 要素)での書き込みを開始する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void BeginTable(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログ ファイルへの表形式(<table> 要素)での書き込みを終了する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void EndTable(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	表形式での出力において、新しい行に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeRow(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	表形式での出力において、新しい列に移動する。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void ChangeColumn(LogLevel level = LogLevel::All) = 0;

		/**
			@brief	ログの出力が可能であるかどうかを表す真偽値を取得する。
			@param	level	[in]	ログの出力が可能か?
		*/
		virtual bool GetIsValid() = 0;

		/**
			@brief	ログの出力レベルを設定する。ログ出力メソッドに指定した出力レベルがこの値より小さければログが出力される。
			@param	level	[in]	ログの出力レベル
		*/
		virtual void SetOutputLevel(LogLevel level) = 0;
	};
}
#include <list>

namespace asd
{
	/**
		@brief	プログラムの実行状況(計算時間や、使っているプロセッサ)を記録するクラス。
	*/
	class Profiler : public ReferenceObject
	{
	protected:
		Profiler(){}
		virtual ~Profiler(){}

	public:
		/**
			@brief	計算の開始時間と、使用するプロセッサ番号を記録する。
			@param	id		プロファイリングを識別するID。Start関数は、IDの一致するEnd関数と対応する。
		*/
		virtual void Start(int id) = 0;

		/**
			@brief	計算の終了時間を記録し、プロファイリング結果を蓄積する。
			@param	id		プロファイリングを識別するID。End関数は、IDの一致するStart関数と対応する。
		*/
		virtual void End(int id) = 0;
	};
}



namespace asd
{
	class Texture
		: public IReference
	{
	protected:
		Texture() {}
		virtual ~Texture() {}

	public:

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() = 0;
	};
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	
	class TextureLockInfomation
	{
		friend class Texture2D;
		friend class Texture2D_Imp_DX11;
		friend class Texture2D_Imp_GL;

	private:
		void*		pixels = nullptr;
		int32_t		pitch = 0;
		Vector2DI	size;
	public:

		void* GetPixels() const
		{
			return pixels;
		}

		int32_t GetPitch() const
		{
			return pitch;
		}

		Vector2DI GetSize() const
		{
			return size;
		}

	};

	class Texture2D
		: public Texture
	{
	protected:
		TextureClassType	m_type;
		
	public:
		Texture2D(){}
		virtual ~Texture2D(){}

		/**
			@brief	テクスチャのサイズを取得する。
			@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
			@brief	テクスチャのフォーマットを取得する。
			@return	フォーマット
		*/
		virtual TextureFormat GetFormat() const = 0;

		/**
		@brief	テクスチャをファイルに保存する。
		@param	path	出力先
		@return	成否
		*/
		virtual bool Save(const achar* path) = 0;

		/**
			@brief	テクスチャをロックし編集可能にする。
			@param	info	テクスチャ情報
			@return	成否
		*/
		virtual bool Lock(TextureLockInfomation* info) = 0;

		/**
		@brief	テクスチャをアンロックする。
		*/
		virtual void Unlock() = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() { return TextureClassType::Texture2D; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class RenderTexture2D
		: public Texture2D
	{
	public:
		RenderTexture2D(){}
		virtual ~RenderTexture2D(){}

		/**
		@brief	テクスチャのサイズを取得する。
		@return	サイズ
		*/
		virtual Vector2DI GetSize() const = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() { return TextureClassType::RenderTexture2D; }
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}



namespace asd
{
	/**
	@brief	キューブマップ
	*/
	class CubemapTexture
		: public Texture
	{
	protected:
		CubemapTexture() {}
		virtual ~CubemapTexture() {}

	public:

		/**
			@brief	ミップマップ数を取得する。
			@return	ミップマップ数
		*/
		virtual int32_t GetMipmapCount() const = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() override { return TextureClassType::CubemapTexture; }
	};
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	シェーダー(2D)
	*/
	class Shader2D
		: public IReference
	{
	protected:
		Shader2D(){}
		virtual ~Shader2D(){}
	public:
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	シェーダー(3D)
	*/
	class Shader3D
		: public IReference
	{
	protected:
		Shader3D(){}
		virtual ~Shader3D(){}
	public:
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material2D
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual Texture2D* GetTexture2D_(const achar* name) = 0;		
		virtual Shader2D* GetShader2D_() = 0;

		Material2D(){}
		virtual ~Material2D(){}
	public:
		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

		virtual Vector4DF GetVector4DF(const achar* name) = 0;
		virtual void SetVector4DF(const achar* name, Vector4DF value) = 0;

		virtual void SetTexture2D(const achar* name, Texture2D* value) = 0;
		virtual void SetShader2D(Shader2D* shader) = 0;

		virtual TextureFilterType GetTextureFilterType(const achar* name) = 0;
		virtual void SetTextureFilterType(const achar* name, TextureFilterType filter) = 0;

		virtual TextureWrapType GetTextureWrapType(const achar* name) = 0;
		virtual void SetTextureWrapType(const achar* name, TextureWrapType wrap) = 0;

#if! SWIG
		std::shared_ptr<Texture2D> GetTexture2D(const achar* name)
		{
			auto v = GetTexture2D_(name);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetTexture2D(const achar* name, std::shared_ptr<Texture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		void SetTexture2D(const achar* name, std::shared_ptr<RenderTexture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		std::shared_ptr<Shader2D> GetShader2D()
		{
			auto v = GetShader2D_();
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetShader2D(std::shared_ptr<Shader2D> shader)
		{
			SetShader2D(shader.get());
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Material3D
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual Texture2D* GetTexture2D_(const achar* name) = 0;
		virtual Shader3D* GetShader3D_() = 0;

		Material3D(){}
		virtual ~Material3D(){}
	public:
		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

		virtual Vector4DF GetVector4DF(const achar* name) = 0;
		virtual void SetVector4DF(const achar* name, Vector4DF value) = 0;

		virtual void SetTexture2D(const achar* name, Texture2D* value) = 0;
		virtual void SetShader3D(Shader3D* shader) = 0;

		virtual TextureFilterType GetTextureFilterType(const achar* name) = 0;
		virtual void SetTextureFilterType(const achar* name, TextureFilterType filter) = 0;

		virtual TextureWrapType GetTextureWrapType(const achar* name) = 0;
		virtual void SetTextureWrapType(const achar* name, TextureWrapType wrap) = 0;

#if! SWIG
		std::shared_ptr<Texture2D> GetTexture2D(const achar* name)
		{
			auto v = GetTexture2D_(name);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetTexture2D(const achar* name, std::shared_ptr<Texture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		void SetTexture2D(const achar* name, std::shared_ptr<RenderTexture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		std::shared_ptr<Shader3D> GetShader3D()
		{
			auto v = GetShader3D_();
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetShader3D(std::shared_ptr<Shader3D> shader)
		{
			SetShader3D(shader.get());
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}




namespace asd
{
	/**
		@brief	マテリアルへのパラメーターの集合
	*/
	class MaterialPropertyBlock
		: public IReference
	{
		friend class Accessor;

	protected:
		MaterialPropertyBlock() {}
		virtual ~MaterialPropertyBlock() {}

		/**
		@brief	テクスチャを取得する。
		@name	名称
		*/
		virtual Texture2D* GetTexture2D_(const achar* name) = 0;
	
	public:

		virtual float GetFloat(const achar* name) = 0;
		virtual void SetFloat(const achar* name, float value) = 0;

		virtual Vector2DF GetVector2DF(const achar* name) = 0;
		virtual void SetVector2DF(const achar* name, Vector2DF value) = 0;

		virtual Vector3DF GetVector3DF(const achar* name) = 0;
		virtual void SetVector3DF(const achar* name, Vector3DF value) = 0;

		virtual Vector4DF GetVector4DF(const achar* name) = 0;
		virtual void SetVector4DF(const achar* name, Vector4DF value) = 0;

		virtual TextureFilterType GetTextureFilterType(const achar* name) = 0;
		virtual void SetTextureFilterType(const achar* name, TextureFilterType filter) = 0;

		virtual TextureWrapType GetTextureWrapType(const achar* name) = 0;
		virtual void SetTextureWrapType(const achar* name, TextureWrapType wrap) = 0;

		virtual void SetTexture2D(const achar* name, Texture2D* value) = 0;

#if! SWIG
		std::shared_ptr<Texture2D> GetTexture2D(const achar* name)
		{
			auto v = GetTexture2D_(name);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		void SetTexture2D(const achar* name, std::shared_ptr<Texture2D> value)
		{
			SetTexture2D(name, value.get());
		}

		void SetTexture2D(const achar* name, std::shared_ptr<RenderTexture2D> value)
		{
			SetTexture2D(name, value.get());
		}
#endif
	};
}




namespace asd
{
	/**
	@brief	フォントの情報が記録されているクラス
	*/
	class Font
		:public IReference
	{
	private:

	protected:
		Font(){}
		virtual ~Font(){}
	public:
		/**
		@brief	描画テキストと描画方向を与えると、その文字の描画領域を返す。
		@param	text	描画テキスト
		@param	writingDirection	描画方向
		@return	文字の描画領域
		*/
		virtual Vector2DI CalcTextureSize(const achar* text, WritingDirection writingDirection) = 0;

		/**
		@brief	指定した文字の代わりに画像を表示する組み合わせを追加する。
		@param	c	文字
		@param	texture	画像
		*/
		virtual void AddImageGlyph(const achar* c, Texture2D* texture) = 0;

#if !SWIG
		/**
		@brief	指定した文字の代わりに画像を表示する組み合わせを追加する。
		@param	c	文字
		@param	texture	画像
		*/
		void AddImageGlyph(const achar* c, std::shared_ptr<Texture2D> texture)
		{
			AddImageGlyph(c, texture.get());
		}
#endif
	};
}





namespace asd
{
	/**
	@brief	複数の画像が格納されているクラス
	*/
	class ImagePackage
		: public IReference
	{
		friend class Accessor;
	private:

	protected:
		ImagePackage() {}
		virtual ~ImagePackage(){}

		virtual Texture2D* GetImage_(int32_t index) = 0;

	public:
		
		/**
			@brief	格納されている画像の枚数を取得する。
			@return	画像の枚数
		*/
		virtual int32_t GetImageCount() = 0;

		/**
			@brief	格納されている画像の名称を取得する。
			@param	index	インデックス
			@return	名称
		*/
		virtual const achar* GetImageName(int32_t index) = 0;

		/**
		@brief	格納されている画像が配置される領域を取得する。
		@param	index	インデックス
		@return	領域
		*/
		virtual RectI GetImageArea(int32_t index) = 0;

		/**
		@brief	格納されている画像の種類を取得する。
		@param	index	インデックス
		@return	画像の種類
		*/
		virtual ImagePackageElementType GetElementType(int32_t index) = 0;

		/**
		@brief	格納されている画像の追加情報の種類を取得する。
		@param	index	インデックス
		@return	追加情報の種類
		*/
		virtual ImagePackageAdditionalElementType GetAdditionalElementType(int32_t index) = 0;

#if !SWIG
		/**
			@brief	画像を取得する。
			@param	index	インデックス
			@return	画像
		*/
		std::shared_ptr<Texture2D> GetImage(int32_t index)
		{
			auto image = GetImage_(index);
			SafeAddRef(image);
			return CreateSharedPtrWithReleaseDLL(image);
		}
#endif
	};
}



namespace asd
{
	/**
		@brief	エフェクトの情報が記録されているクラス
	*/
	class Effect
		: public IReference
	{
	private:

	protected:
		Effect(){}
		virtual ~Effect(){}
	public:

	};
}






namespace asd
{
	/**
		@brief	メッシュを変形させるためのクラス
	*/
	class Deformer
		: public IReference
	{
	protected:
		Deformer() {}
		virtual ~Deformer() {}

	public:	
		/**
			@brief	ボーンを追加する。
			@param	name	ボーンの名称
			@param	parentBoneIndex	親ボーンのインデックス(親がない場合は-1)
			@param	rotationOrder	ボーンの回転行列の計算方法
			@param	localMat	ボーンのローカル変形行列
		*/
		virtual void AddBone(const achar* name, int32_t parentBoneIndex, RotationOrder rotationOrder, const Matrix44& localMat, const Vector3DF& translation, const Vector4DF& rotation, const Vector3DF& scaling) = 0;
	};
}





namespace asd
{
	/**
		@brief	3Dメッシュのクラス
	*/
	class Mesh
		: public IReference
	{
	protected:
		Mesh(){}
		virtual ~Mesh(){}
	public:

		/**
			@brief	頂点を追加する。
			@param	position	座標
			@param	normal		法線
			@param	binormal	従法線
			@param	uv1			UV1
			@param	uv2			UV2
			@param	color		頂点色
			@param	boneWeights	ボーンのウエイト
			@param	boneIndexes	ボーンのインデックス
		*/
		virtual void AddVertex(
			const Vector3DF& position,
			const Vector3DF& normal,
			const Vector3DF& binormal,
			const Vector2DF& uv1,
			const Vector2DF& uv2,
			const Color& color,
			int32_t boneWeights,
			int32_t boneIndexes) = 0;

		/**
			@brief	面を追加する。
			@param	index1	頂点インデックス1
			@param	index2	頂点インデックス2
			@param	index3	頂点インデックス3
			@param	materialIndex	材質インデックス
		*/
		virtual void AddFace(int32_t index1, int32_t index2, int32_t index3, int32_t materialIndex) = 0;

		/**
			@brief	ボーンとの接続設定を追加する。
			@param	targetIndex	対象ボーンインデックス
			@param	boneToMesh	ボーンの行列をメッシュの行列に変換する行列
		*/
		virtual void AddBoneConnector(int32_t targetIndex, const Matrix44& boneToMesh) = 0;

		/**
			@brief	材質を追加する。
			@return	材質のインデックス
		*/
		virtual int32_t AddMaterial() = 0;

		/**
			@brief	設定した値をGPUに送信する。
		*/
		virtual void SendToGPUMemory() = 0;

		/**
		@brief	内部シェーダーを使用する場合のカラーテクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetColorTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合の法線テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetNormalTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合の金属度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetMetalnessTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	内部シェーダーを使用する場合の面平滑度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetSmoothnessTexture(int32_t materialIndex, Texture2D* texture) = 0;

		/**
		@brief	材質を設定する。
		@param	materialIndex	材質のインデックス
		@param	material		材質
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		virtual void SetMaterial(int32_t materialIndex, Material3D* material) = 0;

#if !SWIG
		/**
		@brief	内部シェーダーを使用する場合のカラーテクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetColorTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetColorTexture(materialIndex, texture.get());
		}

		/**
		@brief	内部シェーダーを使用する場合の法線テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetNormalTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetNormalTexture(materialIndex, texture.get());
		}

		/**
		@brief	内部シェーダーを使用する場合の金属度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetMetalnessTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetMetalnessTexture(materialIndex, texture.get());
		}


		/**
		@brief	内部シェーダーを使用する場合の面平滑度テクスチャを設定する。
		@param	materialIndex	材質のインデックス
		@param	テクスチャ
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetSmoothnessTexture(int32_t materialIndex, std::shared_ptr<Texture2D> texture)
		{
			SetSmoothnessTexture(materialIndex, texture.get());
		}

		/**
		@brief	材質を設定する。
		@param	materialIndex	材質のインデックス
		@param	material		材質
		@note
		AddMaterialCountを実行した後でないと無効になる。
		*/
		void SetMaterial(int32_t materialIndex, std::shared_ptr<Material3D> material)
		{
			SetMaterial(materialIndex, material.get());
		}
#endif
	};
};



namespace asd
{
	/**
		@brief	3Dモデルクラス
	*/
	class Model
		: public IReference
	{
		friend class Accessor;
	protected:
		Model(){}
		virtual ~Model(){}

		virtual Mesh* GetMesh_(int32_t index) = 0;
		virtual AnimationClip* GetAnimationClip_(int32_t index) = 0;
	public:
		
		/**
		@brief	モデルが持つアニメーションクリップの名称を取得する。
		@param	index	アニメーションクリップのインデックス
		@return	アニメーションクリップの名称
		*/
		virtual const achar* GetAnimationClipName(int32_t index) = 0;

#if! SWIG
		/**
		@brief	モデルが持つアニメーションクリップを取得する。
		@param	index	アニメーションクリップのインデックス
		@return	アニメーションクリップ
		*/
		std::shared_ptr<AnimationClip> GetAnimationClip(int32_t index)
		{
			auto v = GetAnimationClip_(index);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}

		/**
			@brief	メッシュを取得する。
			@param	index	メッシュのインデックス
			@return	メッシュ
		*/
		std::shared_ptr<Mesh> GetMesh(int32_t index)
		{
			auto v = GetMesh_(index);
			SafeAddRef(v);
			return CreateSharedPtrWithReleaseDLL(v);
		}
#endif
		/**
			@brief	モデルが持つメッシュの個数を取得する。
			@return	メッシュの個数
		*/
		virtual int32_t GetMeshCount() const = 0;

		/**
		@brief	モデルが持つアニメーションクリップの個数を取得する。
		@return	アニメーションクリップの個数
		*/
		virtual int32_t GetAnimationClipCount() const = 0;
	};
}






namespace asd
{
	/**
	@brief	大量描画が可能な3Dモデルクラス
	*/
	class MassModel
		: public IReference
	{
	private:

	protected:
		MassModel(){}
		virtual ~MassModel(){}
	public:

		/**
		@brief	モデルが持つアニメーションの個数を取得する。
		@return	アニメーションの個数
		*/
		virtual int32_t GetAnimationCount() const = 0;

		/**
		@brief	モデルが持つアニメーションの名称を取得する。
		@param	index	アニメーションのインデックス
		@return	アニメーションの名称
		*/
		virtual const achar* GetAnimationName(int32_t index) const = 0;

		/**
		@brief	モデルが持つアニメーションの長さ(60フレーム単位)を取得する。
		@param	name	アニメーション名
		@return	アニメーションの長さ
		*/
		virtual float GetAnimationLength(const achar* name) const = 0;

		/**
		@brief	アニメーションがループするかを取得する。
		@param	name	アニメーション名
		@return	ループするか?
		*/
		virtual bool GetIsAnimationLoopingMode(const achar* name) const = 0;

		/**
		@brief	アニメーションがループするかを設定する。
		@param	name	アニメーション名
		@return	isLoopingMode	ループするか?
		*/
		virtual void SetIsAnimationLoopingMode(const achar* name, bool isLoopingMode) = 0;

		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		virtual void SetMaterial(Material3D* material) = 0;

#if !SWIG
		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		void SetMaterial(std::shared_ptr<Material3D> material)
		{
			SetMaterial(material.get());
		}
#endif
	};
}



namespace asd
{
	/**
	@brief	地形のクラス
	*/
	class Terrain3D
		: public IReference
	{
		friend class Accessor;
	protected:
		Terrain3D(){}
		virtual ~Terrain3D(){}

	public:

		/**
			@brief	マップに対する変更を適用してメッシュを生成する。
		*/
		virtual bool Commit() = 0;

		/**
			@brief	編集可能な情報を破棄して固定化する。
		*/
		virtual void Fix() = 0;

		/**
			@brief	衝突判定を削除する。
		*/
		virtual void ClearCollisions() = 0;

		/**
			@brief	新規に地形を作成する。
			@param	gridSize		グリッド1つ当たりの大きさ
			@param	gridWidthCount	横方向のグリッド数
			@param	gridHeightCount	縦方向のグリッド数
		*/
		virtual void New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount) = 0;

		/**
			@brief	メモリから地形のデータを読み込む。
			@param	buffer	バッファ
			@note
			テクスチャのパスは保存されないので、読み込んだ後にAddSurfaceする必要がある。
		*/
		virtual void LoadFromMemory(const std::vector<uint8_t>& buffer) = 0;

		/**
			@brief	メモリに地形のデータを保存する。
			@return	地形のデータ
		*/
		virtual std::vector<uint8_t> SaveToMemory() = 0;

		/**
			@brief	素材を追加する。
			@param	name	素材名
			@param	size	素材のテクスチャ1枚あたりの大きさ
			@param	color	色テクスチャのパス
			@param	normal	法線テクスチャのパス
			@param	metalness	スペキュラテクスチャのパス
		*/
		virtual void AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* metalness) = 0;

		/**
			@brief	素材インデックスを取得する。
			@param	name	素材名
			@return	素材インデックス
		*/
		virtual int32_t GetSurfaceIndex(const achar* name) = 0;

		/**
			@brief	崖のテクスチャを設定する。
			@param	diffuseTexture		色テクスチャ
			@param	normalTexture		法線テクスチャ
			@param	metalnessTexture	金属度テクスチャ
		*/
		virtual void SetCliffTexture(Texture2D* diffuseTexture, Texture2D* normalTexture, Texture2D* metalnessTexture) = 0;

		/**
			@brief	素材を円形に割り当てる。
			@param	surfaceIndex	素材インデックス
			@param	x	円形の中心座標
			@param	y	円形の中心座標
			@param	radius	円形の半径
			@param	value	割り当てる値(-255～255)
			@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		virtual void SetMaterial(Material3D* material) = 0;

		/**
		@brief	円形に地形を上下させる。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	値
		@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void RaiseHeightWithCircle(float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	円形に地形を指定した高度に設定する。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	高度
		@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void ChangeHeightWithCircle(float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	円形に地形を平滑化させる。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	平滑度(0～1)
		@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void SmoothHeightWithCircle(float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	円形に崖の生成を伴う地形の上下をさせる。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	値
		*/
		virtual void ChangeCliffesWithCircle(float x, float y, float radius, int32_t value) = 0;

		/**
		@brief	光線を飛ばし、衝突した位置を取得する。
		@param	from	光線を飛ばす元の位置
		@param	to		光線を飛ばす先の位置
		@return	光線が地形に衝突した位置。衝突しない場合、NaNを返す。
		*/
		virtual Vector3DF CastRay(const Vector3DF& from, const Vector3DF& to) = 0;
#if !SWIG
		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		void SetMaterial(std::shared_ptr<Material3D> material)
		{
			SetMaterial(material.get());
		}

		/**
		@brief	崖のテクスチャを設定する。
		@param	diffuseTexture		色テクスチャ
		@param	normalTexture		法線テクスチャ
		@param	metalnessTexture	金属度テクスチャ
		*/
		virtual void SetCliffTexture(std::shared_ptr<Texture2D> diffuseTexture, std::shared_ptr<Texture2D> normalTexture, std::shared_ptr<Texture2D> metalnessTexture)
		{
			SetCliffTexture(diffuseTexture.get(), normalTexture.get(), metalnessTexture.get());
		}
#endif
	};
}




namespace asd
{
	/**
		@brief	キーフレームで指定するアニメーションのクラス
	*/
	class KeyframeAnimation
		: public IReference
	{
	protected:
		KeyframeAnimation() {}
		virtual ~KeyframeAnimation() {}

	public:
		/**
			@brief	名称を取得する。
			@return	名称
		*/
		virtual const achar* GetName() = 0;

		/**
			@brief	名称を指定する。
			@param	name	名称
		*/
		virtual void SetName(const achar* name) = 0;

		/**
			@brief	キーフレームを追加する。
			@param	kf	キーフレーム
		*/
		virtual void AddKeyframe(const FCurveKeyframe& kf) = 0;

		/**
			@brief	指定した時間の値を取得する。
			@param	time	時間
			@return	値
		*/
		virtual float GetValue(float time) = 0;
	};
}



namespace asd
{
	/**
		@brief	アニメーションの元データのクラス
	*/
	class AnimationSource
		: public IReference
	{
	public:
		AnimationSource() {}
		virtual ~AnimationSource() {}

		/**
			@brief	ボーンごとのアニメーションを追加する。
			@param	keyframeAnimation	アニメーション
		*/
		virtual void AddAnimation(KeyframeAnimation* keyframeAnimation) = 0;

		/**
		@brief	ボーンごとのアニメーションを削除する。
		@param	keyframeAnimation	アニメーション
		*/
		virtual void RemoveAnimation(KeyframeAnimation* keyframeAnimation) = 0;

		/**
		@brief	アニメーションの長さ(60フレーム単位)を取得する。
		@return	アニメーションの長さ
		*/
		virtual float GetLength() const = 0;

#if !SWIG
		/**
		@brief	ボーンごとのアニメーションを追加する。
		@param	keyframeAnimation	アニメーション
		*/
		void AddAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			AddAnimation(keyframeAnimation.get());
		}

		/**
		@brief	ボーンごとのアニメーションを削除する。
		@param	keyframeAnimation	アニメーション
		*/
		void RemoveAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			RemoveAnimation(keyframeAnimation.get());
		}
#endif
	};
}





namespace asd
{
	/**
		@brief	アニメーションソースを元に情報が付加されたアニメーションクラス
	*/
	class AnimationClip
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual AnimationSource* GetSource_() = 0;
	public:

		AnimationClip() {}
		virtual ~AnimationClip() {}

		/**
			@brief	ソースを設定する。
			@param	source	ソース
		*/
		virtual void SetSource(AnimationSource* source) = 0;

		/**
		@brief	ループするかを取得する。
		@return	ループするか?
		*/
		virtual bool GetIsLoopingMode() const = 0;

		/**
		@brief	ループするかを設定する。
		@return	isLoopingMode	ループするか?
		*/
		virtual void SetIsLoopingMode(bool isLoopingMode) = 0;

#if !SWIG
		/**
		@brief	ソースを取得する。
		@return	source	ソース
		*/
		std::shared_ptr<AnimationSource> GetSource()
		{
			auto o = GetSource_();
			SafeAddRef(o);
			return CreateSharedPtrWithReleaseDLL(o);
		}

		/**
		@brief	ソースを設定する。
		@param	source	ソース
		*/
		void SetSource(std::shared_ptr<AnimationSource>& source)
		{
			SetSource(source.get());
		}
#endif
	};
}



namespace asd
{
	/**
		@brief	アニメーションに関するインスタンスを生成するためのクラス
	*/
	class AnimationSystem
	{
		friend class Accessor;

	protected:
		AnimationSystem(){}
		virtual ~AnimationSystem(){}

		virtual AnimationClip* CreateAnimationClip_() = 0;

		virtual AnimationSource* CreateAnimationSource_() = 0;

		virtual KeyframeAnimation* CreateKeyframeAnimation_() = 0;

	public:

#if !SWIG
		/**
			@brief	アニメーションクリップのインスタンスを生成する。
			@return	インスタンス
		*/
		std::shared_ptr<AnimationClip> CreateAnimationClip()
		{
			auto o = CreateAnimationClip_();
			return CreateSharedPtrWithReleaseDLL(o);
		}

		/**
		@brief	アニメーションソースのインスタンスを生成する。
		@return	インスタンス
		*/
		std::shared_ptr<AnimationSource> CreateAnimationSource()
		{
			auto o = CreateAnimationSource_();
			return CreateSharedPtrWithReleaseDLL(o);
		}

		/**
		@brief	キーフレームアニメーションのインスタンスを生成する。
		@return	インスタンス
		*/
		std::shared_ptr<KeyframeAnimation> CreateKeyframeAnimation()
		{
			auto o = CreateKeyframeAnimation_();
			return CreateSharedPtrWithReleaseDLL(o);
		}
#endif
	};
}



namespace asd {

class Graphics
	: public ReferenceObject
{
protected:
	virtual Texture2D* CreateTexture2D_(const achar* path) = 0;
	virtual Texture2D* CreateTexture2DAsRawData_(const achar* path) = 0;
	virtual Texture2D* CreateEmptyTexture2D_(int32_t width, int32_t height, TextureFormat format) = 0;
	virtual Texture2D* CreateEditableTexture2D_(const achar* path) = 0;

	virtual RenderTexture2D* CreateRenderTexture2D_(int32_t width, int32_t height, TextureFormat format) = 0;
	
	//virtual CubemapTexture* CreateCubemapTextureFrom6ImageFiles_(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom) = 0;
	//virtual CubemapTexture* CreateCubemapTextureFromMipmapImageFiles_(const achar* path, int32_t mipmapCount) = 0;
	//virtual CubemapTexture* CreateCubemapTextureFromSingleImageFile_(const achar* path) = 0;
	virtual CubemapTexture* CreateCubemapTexture_(const achar* path) = 0;

	virtual Shader2D* CreateShader2D_( const achar* shaderText) = 0;
	virtual Shader2D* CreateShader2DFromBinary_(const achar* path) = 0;
	virtual Material2D* CreateMaterial2D_(Shader2D* shader) = 0;

	virtual Shader3D* CreateShader3D_(const achar* shaderText) = 0;
	virtual Material3D* CreateMaterial3D_(Shader3D* shader) = 0;

	virtual MaterialPropertyBlock* CreateMaterialPropertyBlock_() = 0;

	virtual Mesh* CreateMesh_() = 0;
	virtual Deformer* CreateDeformer_() = 0;
	virtual Model* CreateModel_(const achar* path) = 0;
	virtual MassModel* CreateMassModelFromModelFile_(const achar* path) = 0;
	virtual MassModel* CreateMassModel_(const achar* path) = 0;
	virtual Terrain3D* CreateTerrain3D_() = 0;

	virtual Effect* CreateEffect_(const achar* path) = 0;
	virtual Font* CreateFont_(const achar* path) = 0;
	virtual Font* CreateDynamicFont_(const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor) = 0;

	virtual ImagePackage* CreateImagePackage_(const achar* path) = 0;
	virtual MediaPlayer* CreateMediaPlayer_() = 0;

public:
	Graphics(){}
	virtual ~Graphics(){}

#ifndef SWIG

	/**
	@brief	テクスチャを生成する。
	@param	path	画像ファイルへの相対パス
	@return	テクスチャ
	@warning	読み込める画像形式はPNGまたはDDSのみ。
	*/
	std::shared_ptr<Texture2D> CreateTexture2D(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateTexture2D_(path)); }

	/**
	@brief	テクスチャを生成する。
	@param	path	画像ファイルへの相対パス
	@return	テクスチャ
	@note
	画素がRGB空間にマッピングされている時に使用する。(ノーマルマップ等)
	@warning	読み込める画像形式はPNGまたはDDSのみ。
	*/
	std::shared_ptr<Texture2D> CreateTexture2DAsRawData(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateTexture2DAsRawData_(path)); }

	/**
	@brief	テクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@param	format	フォーマット
	@return	テクスチャ
	*/
	std::shared_ptr<Texture2D> CreateEmptyTexture2D(int32_t width, int32_t height, TextureFormat format)
	{
		return CreateSharedPtrWithReleaseDLL(CreateEmptyTexture2D_(width, height, format));
	}

	/**
	@brief	編集可能なテクスチャを生成する。
	@param	path	パス
	@return	テクスチャ
	*/
	std::shared_ptr<Texture2D> CreateEditableTexture2D(const achar* path) { return CreateSharedPtrWithReleaseDLL(CreateEditableTexture2D_(path)); }

	/**
	@brief	描画先として指定可能なテクスチャを生成する。
	@param	width	横幅
	@param	height	縦幅
	@param	format	フォーマット
	@warning	読み込める画像形式はPNGのみ。
	@return	テクスチャ
	*/
	std::shared_ptr<RenderTexture2D> CreateRenderTexture2D(int32_t width, int32_t height, TextureFormat format){ return CreateSharedPtrWithReleaseDLL(CreateRenderTexture2D_(width, height, format)); }

	/**
	@brief	6枚の画像ファイルからキューブマップテクスチャを生成する。
	@param	front	前方向の画像ファイルへの相対パス
	@param	left	左方向の画像ファイルへの相対パス
	@param	back	後ろ方向の画像ファイルへの相対パス
	@param	right	右方向の画像ファイルへの相対パス
	@param	top		上方向の画像ファイルへの相対パス
	@param	bottom	下方向の画像ファイルへの相対パス
	@warning	読み込める画像形式はPNGまたはDDSのみ。
	@return	キューブマップ
	*/
	//std::shared_ptr<CubemapTexture> CreateCubemapTextureFrom6ImageFiles(const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
	//{
	//	return CreateSharedPtrWithReleaseDLL(
	//		CreateCubemapTextureFrom6ImageFiles_(
	//		front, left, back, right, top, bottom));
	//}

	/**
	@brief	複数の画像ファイルからミップマップ付のキューブマップテクスチャを生成する。
	@param	path		ファイルの名称の先頭
	@param	mipmapCount	ミップマップ数
	@warning	読み込める画像形式はPNGまたはDDSのみ。
	@return	キューブマップ
	*/
	//std::shared_ptr<CubemapTexture> CreateCubemapTextureFromMipmapImageFiles(const achar* path, int32_t mipmapCount)
	//{
	//	return CreateSharedPtrWithReleaseDLL(
	//		CreateCubemapTextureFromMipmapImageFiles_(
	//		path, mipmapCount));
	//}

	/**
	@brief	1つのファイルからミップマップ付のキューブマップテクスチャを生成する。
	@param	path		画像ファイルへの相対パス
	@return	キューブマップ
	@warning	読み込める画像形式はDDSのみ。
	*/
	//std::shared_ptr<CubemapTexture> CreateCubemapTextureFromSingleImageFile(const achar* path)
	//{
	//	return CreateSharedPtrWithReleaseDLL(
	//		CreateCubemapTextureFromSingleImageFile_(
	//		path));
	//}

	/**
	@brief	1つのファイルからミップマップ付のキューブマップテクスチャを生成する。
	@param	path		画像ファイルへの相対パス
	@return	キューブマップ
	@warning	読み込める画像形式はDDSのみ。
	*/
	std::shared_ptr<CubemapTexture> CreateCubemapTexture(const achar* path)
	{
		return CreateSharedPtrWithReleaseDLL(
			CreateCubemapTexture_(
			path));
	}

	/**
	@brief	シェーダー(2D)を生成する。
	@param	shaderText						シェーダーのコード
	@return	シェーダー(2D)
	*/
	std::shared_ptr<Shader2D> CreateShader2D(const achar* shaderText)
	{
		return CreateSharedPtrWithReleaseDLL(CreateShader2D_(shaderText));
	}

	/**
	@brief	シェーダー(2D)を生成する。(未実装)
	@param	path	ピクセルシェーダーのバイナリ
	@return	シェーダー(2D)
	*/
	std::shared_ptr<Shader2D> CreateShader2DFromBinary(const achar* path)
	{
		return CreateSharedPtrWithReleaseDLL(CreateShader2DFromBinary_(path));
	}

	/**
	@brief	マテリアル(2D)を生成する。
	@param	shader	シェーダー
	@return	マテリアル(2D)
	*/
	std::shared_ptr<Material2D> CreateMaterial2D(std::shared_ptr<Shader2D> shader)
	{
		auto material = CreateMaterial2D_(shader.get());
		return CreateSharedPtrWithReleaseDLL(material);
	}

	/**
	@brief	シェーダー(3D)を生成する。
	@param	shaderText						シェーダーのコード
	@return	シェーダー(3D)
	*/
	std::shared_ptr<Shader3D> CreateShader3D(const achar* shaderText)
	{
		return CreateSharedPtrWithReleaseDLL(CreateShader3D_(shaderText));
	}

	/**
	@brief	マテリアル(3D)を生成する。
	@param	shader	シェーダー
	@return	マテリアル(3D)
	*/
	std::shared_ptr<Material3D> CreateMaterial3D(std::shared_ptr<Shader3D> shader)
	{
		auto material = CreateMaterial3D_(shader.get());
		return CreateSharedPtrWithReleaseDLL(material);
	}

	/**
	@brief	マテリアルプロパティブロックを生成する。
	@return	マテリアルプロパティブロック
	*/
	std::shared_ptr<MaterialPropertyBlock> CreateMaterialPropertyBlock()
	{
		auto block = CreateMaterialPropertyBlock_();
		return CreateSharedPtrWithReleaseDLL(block);
	}

	/**
	@brief	メッシュを生成する。
	@return	メッシュ
	*/
	std::shared_ptr<Mesh> CreateMesh()
	{
		auto mesh = CreateMesh_();
		return CreateSharedPtrWithReleaseDLL(mesh);
	}

	/**
	@brief	デフォーマーを生成する。
	@return	デフォーマー
	*/
	std::shared_ptr<Deformer> CreateDeformer()
	{
		auto deformer = CreateDeformer_();
		return CreateSharedPtrWithReleaseDLL(deformer);
	}

	/**
	@brief	モデルを生成する。
	@param	path	パス
	@return	モデル
	*/
	std::shared_ptr<Model> CreateModel(const achar* path)
	{
		auto model = CreateModel_(path);
		return CreateSharedPtrWithReleaseDLL(model);
	}

	/**
	@brief	大量描画用モデルを生成する。
	@param	path	パス
	@return	大量描画用モデル
	*/
	std::shared_ptr<MassModel> CreateMassModelFromModelFile(const achar* path)
	{
		auto model = CreateMassModelFromModelFile_(path);
		return CreateSharedPtrWithReleaseDLL(model);
	}

	/**
	@brief	大量描画用モデルを生成する。
	@param	path	パス
	@return	大量描画用モデル
	*/
	std::shared_ptr<MassModel> CreateMassModel(const achar* path)
	{
		auto model = CreateMassModel_(path);
		return CreateSharedPtrWithReleaseDLL(model);
	}

	/**
	@brief	地形を生成する。
	@return	地形
	*/
	std::shared_ptr<Terrain3D> CreateTerrain3D()
	{
		auto terrain = CreateTerrain3D_();
		return CreateSharedPtrWithReleaseDLL(terrain);
	}

	/**
	@brief	エフェクトを生成する。
	@param	path	パス
	@return	エフェクト
	*/
	std::shared_ptr<Effect> CreateEffect(const achar* path)
	{
		auto effect = CreateEffect_(path);
		return CreateSharedPtrWithReleaseDLL(effect);
	}

#undef CreateFont
	/**
	@brief	フォントを生成する。
	@param	path	パス
	@return	フォント
	*/
	std::shared_ptr<Font> CreateFont(const achar* path)
	{
		auto font = CreateFont_(path);
		return CreateSharedPtrWithReleaseDLL(font);
	}

	/**
	@brief	必要に応じて動的に生成されるフォントを生成する。
	@param	font			フォント名/フォントパス
	@param	fontSize		フォントサイズ
	@param	color			フォントの色
	@param	outlineSize		外枠の太さ
	@param	outlineColor	外枠の色
	@return	フォント
	@note
	文字を表示する時に必要な文字の分だけフォントを生成するフォントクラスを生成する。
	fontには、フォント名、もしくはフォントファイルへのパスを指定する。
	何もfontに文字を指定しない、もしくはnullだと標準フォントが使用される。
	事前に専用のフォントファイルを用意する必要はないが、アプリケーションを実行する環境に指定したフォントが存在する必要がある。
	*/
	std::shared_ptr<Font> CreateDynamicFont(const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor)
	{
		auto font_ = CreateDynamicFont_(font, fontSize, color, outlineSize, outlineColor);
		return CreateSharedPtrWithReleaseDLL(font_);
	}


	/**
	@brief	画像パッケージを生成する。
	@param	path	パス
	@return	画像パッケージ
	*/
	std::shared_ptr<ImagePackage> CreateImagePackage(const achar* path)
	{
		auto ip = CreateImagePackage_(path);
		return CreateSharedPtrWithReleaseDLL(ip);
	}

#ifdef __ALTSEED_HEADER__
	/**
	@brief	メディアプレイヤーを生成する。
	@return	メディアプレイヤー
	*/
	std::shared_ptr<MediaPlayer> CreateMediaPlayer()
	{
		auto mp = CreateMediaPlayer_();
		return std::shared_ptr<MediaPlayer>(new MediaPlayer(mp, true));
	}
#endif

#endif

	/**
	@brief	1フレーム間に実行された描画命令の回数を取得する。
	@return	描画命令の回数
	@note
	現在、エフェクトの描画回数はカウントされない。
	*/
	virtual int32_t GetDrawCallCount() const = 0;

	/**
		@brief	現在使用済みのVRAM容量を取得する。
		@return	VRAM容量
		@note
		値は推測値である。
	*/
	virtual int32_t GetUsedVRAMSize() const = 0;

	/**
	@brief	描画ランタイムの種類を取得する。
	@return	種類
	*/
	virtual GraphicsDeviceType GetGraphicsDeviceType() const = 0;

};

}

namespace asd
{
	/**
	@brief	図形の種類
	*/
	enum class ShapeType
	{
		/**
		@brief	円
		*/
		CircleShape,

		/**
		@brief	弧
		*/
		ArcShape,

		/**
		@brief	線分
		*/
		LineShape,

		/**
		@brief	矩形
		*/
		RectangleShape,

		/**
		@brief	三角形
		*/
		TriangleShape,

		/**
		@brief	多角形
		*/
		PolygonShape
	};
}


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	カーソルを表すクラス
	*/
	class Cursor
		: public IReference
	{
	protected:
		Cursor(){}
		virtual ~Cursor(){}
	public:

	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
#if !SWIG
	typedef void(ASD_STDCALL *CoreFuncPtr)(Core*);
#endif

	class CoreOption
	{
	public:
		bool IsFullScreen;
		GraphicsDeviceType GraphicsDevice;
		bool IsReloadingEnabled;
		bool IsWindowResizable = false;
		WindowPositionType WindowPosition;
		ColorSpaceType ColorSpace;
	};

	class Core
		: public ReferenceObject
	{
	private:

	public:

#if !SWIG
		/**
			@brief	インスタンスが破棄された際に呼ばれる関数ポインタを設定する。
			@param	func	破棄された際に呼ばれる関数
		*/
		virtual void SetRemovedFunctionPpointer(CoreFuncPtr func) = 0;
#endif

		/**
		@brief	初期化を行う。
		@param	title	タイトル
		@param	width	横幅
		@param	height	縦幅
		@param	isFullScreen	フルスクリーンで起動するかどうか?
		@param	isOpenGLMode	OpenGLで起動するかどうか?
		@return	成否
		*/
		virtual bool Initialize(const achar* title, int32_t width, int32_t height, CoreOption option) = 0;

		/**
		@brief	初期化を行う。
		@param	handle1	ハンドル
		@param	handle2	ハンドル
		@param	width	横幅
		@param	height	縦幅
		@param	isOpenGLMode	OpenGLで起動するかどうか?
		@return	成否
		*/
		virtual bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, CoreOption option) = 0;

		/**
			@brief	タイトルを設定する。
			@param	title	タイトル
		*/
		virtual void SetTitle(const achar* title) = 0;

		/**
		@brief	イベントを実行し、進行可否を判断する。
		@return	進行可能か?
		*/
		virtual bool DoEvents() = 0;

		/**
		@brief	更新処理を行う。
		*/
		virtual void Update() = 0;

		/**
		@brief	終了処理を行う。
		*/
		virtual void Terminate() = 0;

		/**
		@brief	更新されたリソースのリロードを行う。
		*/
		virtual void Reload() = 0;

		/**
			@brief	描画機能の準備をする。
		*/
		virtual void BeginDrawing() = 0;

		/**
			@brief	描画機能の使用を完了する。
		*/
		virtual void EndDrawing() = 0;

		/**
			@brief	シーンをウインドウに描画する。
		*/
		virtual void DrawSceneToWindow(CoreScene* scene) = 0;

		/**
			@brief	画面遷移しつつシーンをウインドウに描画する。
		*/
		virtual void DrawSceneToWindowWithTransition(CoreScene* nextScene, CoreScene* previousScene, CoreTransition* transition) = 0;

		/**
			@brief	現在のコアの内部情報を描画する。
		*/
		virtual void Draw() = 0;

		/**
			@brief	現在のシーンを変更します。
		*/
		virtual void ChangeScene(CoreScene* scene) = 0;

		/**
			@brief	ウインドウを閉じる。
		*/
		virtual void Close() = 0;

		/**
			@brief	マウスカーソルを作成する。
			@param	path	画像のパス
			@param	hot		カーソルの相対座標
			@return	カーソル
		*/
		virtual Cursor* CreateCursor(const achar* path, Vector2DI hot) = 0;

		/**
			@brief	マウスカーソルを設定する。
			@param	cursor	カーソル
		*/
		virtual void SetCursor(Cursor* cursor) = 0;

		/**
		@brief	クリップボードの内容を取得する。
		@return	クリップボード
		*/
		virtual const achar* GetClipboardString() = 0;

		/**
		@brief	クリップボードの内容を設定する。
		@param	s	文字列
		*/
		virtual void SetClipboardString(const achar* s) = 0;

		/**
			@brief	フルスクリーンモードかどうか設定する。
			@param	isFullscreenMode	フルスクリーンモードか
		*/
		virtual void SetIsFullscreenMode(bool isFullscreenMode) = 0;

		/**
			@brief	スクリーンショットをpngとして保存する。
			@param	path	出力先
		*/
		virtual void TakeScreenshot(const achar* path) = 0;

		/**
			@brief	スクリーンショットをgifアニメーションとして録画する。
			@param	path	出力先
			@param	frame	録画フレーム数
			@param	frequency_rate	録画頻度(例えば、1だと1フレームに1回保存、0.5だと2フレームに2回保存)
			@param	scale	ウインドウサイズに対する画像サイズの拡大率(ウインドウサイズが320の場合、0.5を指定すると160の画像が出力される)
			@note
			実行してから一定時間の間、録画を続ける。
			録画が終了するまでにアプリケーションが終了された場合、終了した時点までの録画結果が出力される。
		*/
		virtual void CaptureScreenAsGifAnimation(const achar* path, int32_t frame, float frequency_rate, float scale) = 0;

		/**
			@brief	1フレームで経過した時間を実時間(秒)で取得する。
			@return	経過時間時間(秒)
		*/
		virtual float GetDeltaTime() const = 0;

		/**
		@brief	1フレームで経過した時間を外部から設定する。
		@param	deltaTime	経過時間(秒)
		@note
		基本的に開発者は使用する必要はない。
		何らかの理由で無理やり経過時間を指定する場合に使用する。
		*/
		virtual void SetDeltaTime(float deltaTime) = 0;

		/**
			@brief	現在のFPSを取得する。
			@return FPS
		*/
		virtual float GetCurrentFPS() = 0;

		/**
			@brief	目標FPSを取得する。
			@return	FPS
		*/
		virtual int32_t GetTargetFPS() = 0;

		/**
			@brief	目標FPSを設定する。
			@param	fps	FPS
		*/
		virtual void SetTargetFPS(int32_t fps) = 0;

		/**
		@brief	時間を指定可能なオブジェクトの実時間あたりの進行速度を取得する。
		@return	進行速度
		*/
		virtual float GetTimeSpan() const = 0;

		/**
		@brief	時間を指定可能なオブジェクトの実時間あたりの進行速度を設定する。
		@param	timeSpan	進行速度
		*/
		virtual void SetTimeSpan(float timeSpan) = 0;

		/**
		@brief	フレームレートの制御方法を取得する。
		@return	制御方法
		*/
		virtual FramerateMode GetFramerateMode() const = 0;

		/**
		@brief	フレームレートの制御方法を設定する。
		@param	framerateMode	制御方法
		*/
		virtual void SetFramerateMode(FramerateMode framerateMode) = 0;

		/**
			@brief	Windowsの場合、ウインドウのハンドルを返す。
			@return	ウインドウハンドル
		*/
		virtual void* GetWindowHandle() const = 0;

		/**
		@brief キーボードの入力を扱うKeyboardクラスを取得する。
		*/
		virtual Keyboard* GetKeyboard() = 0;

		/**
		@brief マウスの入力を扱うMouseクラスを取得する。
		*/
		virtual Mouse* GetMouse() = 0;

		/**
		@brief 接続されているジョイスティックの入力を扱うJoystickContainerクラスを取得する。
		*/
		virtual JoystickContainer* GetJoystickContainer() = 0;

		/**
		@brief 通常のファイルとパックファイルに対する読み込みをサポートするFileクラスを取得する。
		*/
		virtual File* GetFile() = 0;

		/**
			@brief	ログ出力を扱うLogクラスを取得する。
		*/
		virtual Log* GetLogger() = 0;

		/**
			@brief	プロファイリング機能を提供するProfilerクラスを取得する。
		*/
		virtual Profiler* GetProfiler() = 0;

		virtual LayerProfiler* GetLayerProfiler() = 0;

		/**
			@brief	描画を扱うグラフィックスクラスを取得する。
		*/
		virtual Graphics* GetGraphics() = 0;

		virtual Sound* GetSound() = 0;

		virtual ObjectSystemFactory* GetObjectSystemFactory() = 0;

		virtual AnimationSystem* GetAnimationSyatem() = 0;

		/**
		@brief	ウィンドウサイズを取得する。
		*/
		virtual Vector2DI GetWindowSize() = 0;

		virtual void SetWindowSize(Vector2DI size) = 0;

		virtual bool GetProfilerVisibility() const = 0;

		virtual void SetProfilerVisibility(bool visible) = 0;

		virtual void OpenTool() = 0;

		virtual void CloseTool() = 0;

		virtual Tool* GetTool() = 0;
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
};



#include <vector>
#include <memory>

namespace asd
{
	class Engine;

	/**
		@brief	ポストエフェクトを適用するクラス
	*/
	class PostEffect
	{
		friend class Engine;
		friend class Layer;
		friend class CameraObject3D;
		friend class PostEffectGrayScale;
		friend class PostEffectGaussianBlur;
		friend class PostEffectLightBloom;
		friend class PostEffectSepia;

	private:
		std::shared_ptr<CorePostEffect>	m_corePostEffect;

		CorePostEffect* GetCoreObject() const;

		bool	isEnabled = true;

	protected:
		/**
		@brief	オーバーライドして、毎フレーム描画される処理を記述できる。
		*/
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) {}

		/**
		@brief	マテリアルを用いてテクスチャに画像を描画する。
		@param	target		描画先
		@param	material	マテリアル
		*/
		void DrawOnTexture2DWithMaterial(std::shared_ptr<RenderTexture2D> target, std::shared_ptr<Material2D> material);

	public:
		PostEffect();
		virtual ~PostEffect();

		/**
			@brief	このポストエフェクトが有効かどうか、取得する。
			@return	有効かどうか
		*/
		bool GetIsEnabled() const;

		/**
		@brief	このポストエフェクトが有効かどうか、設定する。
		@param	value	有効かどうか
		*/
		void SetIsEnabled(bool value);
	};
}


#include<memory>
#include <vector>


namespace asd
{

	/**
	@brief	グレースケール化するポストエフェクトクラス
	*/

	class PostEffectGrayScale : public PostEffect
	{
	private:
		std::shared_ptr<asd::Material2D>	material2d;

	protected:
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;

	public:
		PostEffectGrayScale();
		virtual ~PostEffectGrayScale() = default;
	};

}

#include<memory>
#include <vector>


namespace asd
{

	/**
	@brief	セピア調化するポストエフェクトクラス
	*/

	class PostEffectSepia : public PostEffect
	{
	private:
		std::shared_ptr<asd::Material2D>	material2d;

	protected:
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;

	public:
		PostEffectSepia();
		virtual ~PostEffectSepia() = default;
	};

}

#include<memory>
#include <vector>


namespace asd
{

	/**
	@brief	ガウスぼかしをかけるポストエフェクトクラス
	*/
	class PostEffectGaussianBlur : public PostEffect
	{
	private:
		std::shared_ptr<Material2D>	material2dX;
		std::shared_ptr<Material2D>	material2dY;
		std::shared_ptr<RenderTexture2D>	tempTexture;

		float intensity = 5.0f;

	protected:
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;

	public:
		PostEffectGaussianBlur();
		virtual ~PostEffectGaussianBlur() = default;

		/**
		@brief ぼかしの強さを設定する。
		@detail 実質的にはガウス関数の分散にあたる値の平方根
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }
	};

}

#include<memory>
#include <vector>

namespace asd
{

	/**
		@brief	輝度の高い画素のみをぼかして加算するポストエフェクト
	*/
	class PostEffectLightBloom : public PostEffect
	{
	private:
		std::shared_ptr<asd::Material2D>	material2dX_Lum, material2dX, material2dY, materialSum, downsample;

		std::shared_ptr<RenderTexture2D>	tempTexture0;
		std::shared_ptr<RenderTexture2D>	tempTexture1;
		std::shared_ptr<RenderTexture2D>	tempTexture2;
		std::shared_ptr<RenderTexture2D>	tempTexture3;

		std::shared_ptr<RenderTexture2D>	downsampledTexture0;
		std::shared_ptr<RenderTexture2D>	downsampledTexture1;
		std::shared_ptr<RenderTexture2D>	downsampledTexture2;
		std::shared_ptr<RenderTexture2D>	downsampledTexture3;

		float intensity = 5.0f;
		float threshold = 1.0f;
		float exposure = 1.0f;
		bool isLuminanceMode = false;

	protected:
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;

	public:
		PostEffectLightBloom();
		virtual ~PostEffectLightBloom() = default;

		/**
		@brief ぼかしの強さを設定する。
		@note
		実質的にはガウス関数の分散にあたる値の平方根。
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }

		/**
		@brief	ぼかすピクセルの閾値を設定する。
		@note
		この閾値を超えた値がぼかされ加算される。
		*/
		void SetThreshold(float const value){ threshold = value; }

		/**
			@brief	露光の強さを指定する。
			@note
			この値が高いほどぼかされる値の輝度が高くなる。
		*/
		void SetExposure(float const value) { exposure = value; }

		/**
		@brief	輝度を参照するか指定する。
		@note
		この値がtrueだと、RGBではなく輝度からぼかされる色を決定する。
		*/
		void SetIsLuminanceMode(bool value) { isLuminanceMode = value; }
	};

}


namespace asd
{
	/**
		@brief	シーン遷移時の効果を適用するクラス
	*/
	class Transition
	{
		friend class Engine;

	private:
		std::shared_ptr<CoreTransition> coreTransition;

		std::shared_ptr<CoreTransition> GetCoreTransition() { return coreTransition; }

	public:
		typedef std::shared_ptr<Transition> Ptr;

		Transition();
		virtual ~Transition();

		/**
		@brief	オーバーライドして、このオブジェクトの更新処理を記述できる。
		*/
		virtual void OnUpdate();


		/**
		@brief	シーンが切り替わっているか取得する。
		@return	シーンが切り替わっているか?
		*/
		bool IsSceneChanged() const;

		/**
		@brief	実際にシーンを次のシーンに切り替える。
		*/
		void ChangeScene();

		/**
		@brief	シーン遷移処理が終了したか取得する。
		@return	シーン遷移処理が終了したか?
		*/
		bool GetIsFinished() const;

		/**
		@brief	シーン遷移処理を終了する。
		*/
		void Finish();

		/**
		@brief	次のシーンを3角形に貼り付けて描画する。
		@param	pos1	座標1(0～1)
		@param	pos2	座標2(0～1)
		@param	pos3	座標3(0～1)
		@param	col1	頂点カラー1
		@param	col2	頂点カラー2
		@param	col3	頂点カラー3
		@param	uv1		UV1(0～1)
		@param	uv2		UV2(0～1)
		@param	uv3		UV3(0～1)
		@note
		ChangeSceneを実行するまでは無効である。
		*/
		virtual void DrawTriangleWithNextScene(
			Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
			Color col1, Color col2, Color col3,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3);

		/**
		@brief	次のシーンを4角形に貼り付けて描画する。
		@param	upperLeftPos	左上の描画位置(0～1)
		@param	upperRightPos	右上の描画位置(0～1)
		@param	lowerRightPos	右下の描画位置(0～1)
		@param	lowerLeftPos	左下の描画位置(0～1)
		@param	upperLeftCol	左上の頂点色
		@param	upperRightCol	右上の頂点色
		@param	lowerRightCol	右下の頂点色
		@param	lowerLeftCol	左下の頂点色
		@param	upperLeftUV		左上のUV値(0～1)
		@param	upperRightUV	右上のUV値(0～1)
		@param	lowerRightUV	右下のUV値(0～1)
		@param	lowerLeftUV		左下のUV値(0～1)
		@note
		ChangeSceneを実行するまでは無効である。
		*/
		void DrawRectangleWithNextScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV);

		/**
		@brief	前のシーンを3角形に貼り付けて描画する。
		@param	pos1	座標1
		@param	pos2	座標2
		@param	pos3	座標3
		@param	col1	頂点カラー1
		@param	col2	頂点カラー2
		@param	col3	頂点カラー3
		@param	uv1		UV1
		@param	uv2		UV2
		@param	uv3		UV3
		@note
		ChangeSceneを実行すると前のシーンの描画は停止し、静止画として扱われる。
		*/
		void DrawTriangleWithPreviousScene(
			Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
			Color col1, Color col2, Color col3,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3);

		/**
		@brief	前のシーンを4角形に貼り付けて描画する。
		@param	upperLeftPos	左上の描画位置(0～1)
		@param	upperRightPos	右上の描画位置(0～1)
		@param	lowerRightPos	右下の描画位置(0～1)
		@param	lowerLeftPos	左下の描画位置(0～1)
		@param	upperLeftCol	左上の頂点色
		@param	upperRightCol	右上の頂点色
		@param	lowerRightCol	右下の頂点色
		@param	lowerLeftCol	左下の頂点色
		@param	upperLeftUV		左上のUV値(0～1)
		@param	upperRightUV	右上のUV値(0～1)
		@param	lowerRightUV	右下のUV値(0～1)
		@param	lowerLeftUV		左下のUV値(0～1)
		@note
		ChangeSceneを実行すると前のシーンの描画は停止し、静止画として扱われる。
		*/
		void DrawRectangleWithPreviousScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV);
	};
}


namespace asd
{
	/**
	@brief	フェードしながらシーン遷移する効果を適用するクラス
	*/
	class TransitionFade
		: public Transition
	{
	private:
		float fadeinDuration = 0;
		float fadeoutDuration = 0;
		float time = 0;

	public:
		/**
			@brief	コンストラクタ
			@param	fadeoutDuration	フェードアウトにかかる実時間(秒)
			@param	fadeinDuration	フェードインにかかる実時間(秒)
		*/
		TransitionFade(float fadeoutDuration, float fadeinDuration);
		virtual ~TransitionFade();

		void OnUpdate() override;
	};
}

namespace asd
{
	class ICommitable
	{
	public:
		virtual ~ICommitable()
		{
		}
		virtual void Commit() = 0;
	};
}

namespace asd
{
	template<typename TComponent>
	class IComponentRegisterable
	{
	public:
		virtual ~IComponentRegisterable()
		{
		}
		virtual void Register(const std::shared_ptr<TComponent>& component) = 0;
		virtual void Unregister(const std::shared_ptr<TComponent>& component) = 0;
	};
}

namespace asd
{
	template<typename TComponent>
	class IImmediateComponentManager
	{
	protected:
		typedef std::shared_ptr<TComponent> ComponentPtr;

	public:
		virtual ~IImmediateComponentManager()
		{
		}

		virtual void ImmediatelyAddComponent(const ComponentPtr& component, astring key) = 0;
		virtual void ImmediatelyRemoveComponent(astring key) = 0;
	};
}

namespace asd
{
	template<typename TObject>
	class IImmediateObjectManager
	{
	protected:
		typedef std::shared_ptr<TObject> ObjectPtr;

	public:
		virtual ~IImmediateObjectManager()
		{
		}

		virtual void ImmediatelyAddObject(const ObjectPtr& object, bool raiseEvent) = 0;
		virtual void ImmediatelyRemoveObject(const ObjectPtr& object, bool raiseEvent) = 0;
	};
}

namespace asd
{
	class IImmediatelyDisposable
	{
	public:
		virtual ~IImmediatelyDisposable()
		{
		}

		virtual void Dispose() = 0;
		virtual void DisposeImmediately() = 0;
	};
}

namespace asd
{
	template<typename TObject>
	class IObjectRegisterable
	{
	public:
		virtual ~IObjectRegisterable()
		{
		}

		virtual bool GetIsAlive() const = 0;
		virtual void Register(const std::shared_ptr<TObject>& object) = 0;
		virtual void Unregister(const std::shared_ptr<TObject>& object) = 0;
		virtual void AddToCore(const std::shared_ptr<TObject>& object) = 0;
		virtual void RemoveFromCore(const std::shared_ptr<TObject>& object) = 0;
	};
}

namespace asd
{
	enum class RegistrationCommand
	{
		Add, Remove
	};
}


#include <memory>

namespace asd
{
	class Object2D;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	asd::Object2D クラスに登録することができるコンポーネント。
	*/
	class Object2DComponent
		: public IImmediatelyDisposable
		, public std::enable_shared_from_this<Object2DComponent>
	{
		friend class Object2D;
		friend class ComponentManager<Object2DComponent>;

	private:
		Object2D* m_owner;
		bool m_isUpdated;
		bool m_isAlive;
		astring m_key;

		void Update();
		void RaiseOnAdded();
		void RaiseOnRemoved();
		void RaiseOnDisposed();
		void SetOwner(Object2D* value);

		void DisposeImmediately();

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
			*/
		virtual void OnUpdate() = 0;

		virtual void OnObjectAdded()
		{
		}

		virtual void OnObjectRemoved()
		{
		}

		virtual void OnObjectDisposed()
		{
		}

	public:
		typedef std::shared_ptr<Object2DComponent> Ptr;

		/**
			@brief	コンストラクタ
			*/
		Object2DComponent();
		/**
			@brief	デストラクタ
			*/
		virtual ~Object2DComponent()
		{
		}

		/**
			@brief	このインスタンスを保持している2Dオブジェクトのインスタンスを取得する。
			*/
		Object2D* GetOwner() const;

		/**
			@brief	このコンポーネントが更新されるかどうかを取得する。
			*/
		bool GetIsUpdated() const;
		/**
			@brief	このコンポーネントが更新されるかどうかを設定する。
			*/
		void SetIsUpdated(bool value);

		astring GetKey() const;

		void SetKey(astring value);


		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
			*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
			*/
		void Dispose();
	};
}



#include <memory>

namespace asd
{
	class Layer2D;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	asd::Layer2D クラスに登録することができるコンポーネント。
	*/
	class Layer2DComponent
		: public std::enable_shared_from_this<Layer2DComponent>
		, public IImmediatelyDisposable
	{
		friend class Layer2D;
		friend class ComponentManager<Layer2DComponent>;

	private:
		Layer2D* m_layer;
		bool m_isUpdated;
		bool m_isAlive;
		astring m_key;

		void Update();
		void RaiseOnAdded();
		void RaiseOnRemoved();
		void RaiseOnDisposed();
		void SetOwner(Layer2D* value);

		void DisposeImmediately();

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

		virtual void OnLayerAdded()
		{
		}

		virtual void OnLayerRemoved()
		{
		}

		virtual void OnLayerDisposed()
		{
		}

	public:
		typedef std::shared_ptr<Layer2DComponent> Ptr;

		/**
			@brief	コンストラクタ
		*/
		Layer2DComponent();
		/**
			@brief	デストラクタ
		*/
		virtual ~Layer2DComponent()
		{
		}

		/**
			@brief	このインスタンスを保持しているレイヤーのインスタンスを取得する。
		*/
		Layer2D* GetOwner() const;

		/**
			@brief	このコンポーネントが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このコンポーネントが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		astring GetKey() const;
		void SetKey(astring value);


		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
		*/
		void Dispose();
	};
}



#include <memory>

namespace asd
{
	class Scene;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	asd::Scene クラスに登録することができるコンポーネント。
	*/
	class SceneComponent
		: public std::enable_shared_from_this<SceneComponent>
		, public IImmediatelyDisposable
	{
		friend class Scene;
		friend class ComponentManager<SceneComponent>;

	private:
		Scene* m_scene;
		bool m_isUpdated;
		bool m_isAlive;
		astring m_key;

		void Update();
		void RaiseOnRegistered();
		void RaiseOnStartUpdating();
		void RaiseOnTransitionFinished();
		void RaiseOnTransitionBegin();
		void RaiseOnStopUpdating();
		void RaiseOnUnregistered();
		void RaiseOnDisposed();
		void SetOwner(Scene* value);

		void DisposeImmediately();

	protected:
		/**
			@brief	オーバーライドして、このコンポーネントの更新処理を記述できる。
		*/
		virtual void OnUpdate() = 0;

		virtual void OnSceneRegistered()
		{
		}

		virtual void OnStartSceneUpdating()
		{
		}

		virtual void OnSceneTransitionFinished()
		{
		}

		virtual void OnSceneTransitionBegin()
		{
		}

		virtual void OnStopSceneUpdating()
		{
		}

		virtual void OnSceneUnregistered()
		{
		}

		virtual void OnSceneDisposed()
		{
		}

	public:
		typedef std::shared_ptr<SceneComponent> Ptr;

		/**
			@brief	コンストラクタ
		*/
		SceneComponent();
		/**
			@brief	デストラクタ
		*/
		virtual ~SceneComponent()
		{
		}

		/**
			@brief	このインスタンスを保持しているシーンのインスタンスを取得する。
		*/
		Scene* GetOwner() const;

		/**
			@brief	このコンポーネントが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このコンポーネントが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		astring GetKey() const;

		void SetKey(astring value);

		/**
			@brief	このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返します。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このコンポーネントを破棄します。
		*/
		void Dispose();
	};
}

#include <memory>
#include <map>

namespace asd
{
	class Scene;

	/**
		@brief	オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer
		: public std::enable_shared_from_this<Layer>
		, public IImmediatelyDisposable
	{
		friend class Scene;
	public:
		typedef std::shared_ptr<Layer> Ptr;

	protected:
		std::shared_ptr<CoreLayer>	m_commonObject;
		bool m_isUpdated;
		bool m_isUpdatedCurrent = false;
		bool m_isDrawn;
		bool m_isAlive;
		int m_updatePriority;
		float m_updateFrequency;
		float m_updateTimer;

		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;

	private:
		Scene* m_scene;
		astring m_name;

		virtual void SetScene(Scene* scene);

		std::shared_ptr<CoreLayer> GetCoreLayer() const;

		void DisposeImmediately();
		virtual void DisposeInternal() = 0;
		virtual void BeginUpdating() = 0;
		virtual void EndUpdateting() = 0;
		virtual void UpdateInternal() = 0;
		virtual void DrawAdditionally() = 0;

		virtual void RaiseOnAdded() = 0;
		virtual void RaiseOnRemoved() = 0;
		virtual void Update();
		void Draw();
		void BeginDrawing();
		void EndDrawing();

	protected:

		/**
			@brief	オーバーライドして、このレイヤーがシーンに登録されたときの処理を記述できる。
		*/
		virtual void OnAdded();

		/**
			@brief	オーバーライドして、このレイヤーがシーンから登録解除されたときの処理を記述できる。
		*/
		virtual void OnRemoved();
		
		/**
			@brief	オーバーライドして、このレイヤーが更新される前の処理を記述できる。
		*/
		virtual void OnUpdating();
		
		/**
			@brief	オーバーライドして、このレイヤーが更新された後の処理を記述できる。
		*/
		virtual void OnUpdated();
	
		/**
			@brief	オーバーライドして、このレイヤーの描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

		/**
			@brief	オーバーライドして、このレイヤーがVanishメソッドによって破棄されるときの処理を記述できる。
		*/
		virtual void OnVanish();
		
		/**
			@brief	オーバーライドして、このレイヤーが破棄されるときの処理を記述できる。
		*/
		virtual void OnDispose();

	public:
		Layer();
		virtual ~Layer();

		/**
			@brief	このインスタンスを管理している asd.Scene クラスのインスタンスを取得する。
			@return	シーン
		*/
		virtual Scene* GetScene() const;

		/**
			@brief	レイヤーの更新を実行するかどうか取得する。
		*/
		bool GetIsUpdated() const;

		/**
			@brief	レイヤーの更新を実行するかどうか設定する。
			@param	value	レイヤーの更新を実行するかどうか
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	レイヤーを描画するどうか取得する。
		*/
		bool GetIsDrawn() const;
		/**
		@brief	レイヤーを描画するどうか設定する。
		@param	value	レイヤーを描画するどうか
		*/
		void SetIsDrawn(bool value);

		/**
			@brief	このレイヤーが有効かどうかを取得する。
		*/
		bool GetIsAlive() const;

		/**
		@brief	このレイヤーの描画優先度を取得する。この値が大きいほど手前に描画される。
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このレイヤーの描画優先度を設定する。この値が大きいほど手前に描画される。
		@param	value	優先度
		*/
		void SetDrawingPriority(int value);

		/**
		@brief	ポストエフェクトを追加する。
		@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
		@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

		/**
			@brief	このレイヤーを破棄する。
		*/
		void Dispose();

		/**
			@brief	レイヤーの種類を取得する。
			@return	レイヤーの種類
		*/
		virtual LayerType GetLayerType() const = 0;

		/**
			@brief	このレイヤーの前回の更新時間を取得する。
		*/
		int GetTimeForUpdate() const;

		virtual int GetObjectCount() const = 0;

		/**
			@brief	このレイヤーの名前を取得する。
		*/
		astring GetName() const;
		/**
			@brief	このレイヤーの名前を設定する。
		*/
		void SetName(astring value);

		/**
			@brief	このレイヤーの更新の優先順位を取得する。
		*/
		int GetUpdatePriority() const;
		/**
			@brief	このレイヤーの更新の優先順位を設定する。
		*/
		void SetUpdatePriority(int value);

		/**
			@brief	このレイヤーの１フレームごとの更新回数を取得する。
		*/
		float GetUpdateFrequency() const;

		/**
			@brief	このレイヤーの１フレームごとの更新回数を設定する。
		*/
		void SetUpdateFrequency(float value);
	};
}



namespace asd
{
	class GeometryObject2D;
	class Layer2D;

	/**
	@brief	当たり判定や図形描画に用いられる図形を扱うクラス。
	*/
	class Shape
	{
		friend class GeometryObject2D;
		friend class Layer2D;
	protected:
		virtual std::shared_ptr<CoreShape> GetCoreShape() const = 0;
	public:

		/**
		@brief	図形のタイプを取得する。
		@return	図形のタイプ
		*/
		virtual ShapeType GetShapeType() const = 0;

		/**
		@brief 指定した図形と衝突しているか否かを取得する。
		@param shape 衝突を調べる図形
		@return 衝突しているか否か
		*/
		bool GetIsCollidedWith(std::shared_ptr<Shape> shape);
	};
};


namespace asd
{
	/**
	@brief	図形としての三角形を扱うクラス。
	*/
	class TriangleShape :public Shape
	{
	private:
		std::shared_ptr<CoreTriangleShape> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		TriangleShape();
		virtual ~TriangleShape();

		/**
		@brief	指定したインデックスの頂点の座標を取得する。
		@param	index	座標を取得する頂点のインデックス（0 <= index < 3）
		@return	頂点の座標
		*/
		Vector2DF GetPointByIndex(int index) const;

		/**
		@brief	指定したインデックスの頂点の座標を設定する。
		@param	point	インデックスで指定した頂点に設定する座標
		@param	index	座標を設定する頂点のインデックス（0 <= index < 3）
		*/
		void SetPointByIndex(Vector2DF point, int index);

		/**
		@brief	指定したインデックスの頂点のUVを取得する。
		@param	index	UVを取得する頂点のインデックス（0 <= index < 3）
		@return	頂点のUV
		*/
		Vector2DF GetUVByIndex(int index) const;

		/**
		@brief	指定したインデックスの頂点のUVを設定する。
		@param	uv	インデックスで指定した頂点に設定するUV
		@param	index	UVを設定する頂点のインデックス（0 <= index < 3）
		*/
		void SetUVByIndex(Vector2DF uv, int index);

		ShapeType GetShapeType() const override;
	};
}


namespace asd
{
	/**
	@brief	図形としての円を扱うクラス。
	*/
	class CircleShape :public Shape
	{
	private:
		std::shared_ptr<CoreCircleShape> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		CircleShape();
		virtual ~CircleShape();

		/**
		@brief	この円の中心座標を取得する。
		@return	この円の中心座標
		*/
		Vector2DF GetPosition() const;

		/**
		@brief	この円の中心座標を設定する。
		@param	position	この円の中心座標
		*/
		void SetPosition(Vector2DF position);

		/**
		@brief	この円の外径を取得する。
		@return	この円の外径
		*/
		float GetOuterDiameter() const;

		/**
		@brief	この円の外径を設定する。
		@param	outerDiameter	この円の外径
		*/
		void SetOuterDiameter(float outerDiameter);

		/**
		@brief	この円の内径を取得する。
		@return	この円の内径
		*/
		float GetInnerDiameter() const;

		/**
		@brief	この円の内径を設定する。
		@param	innerDiameter	この円の内径
		*/
		void SetInnerDiameter(float innerDiameter);

		/**
		@brief	この円の中心を軸とした回転角度を取得する。
		@return	この円の中心を軸とした回転角度
		*/
		float GetAngle() const;

		/**
		@brief	この円の中心を軸とした回転角度を設定する。
		@param	angle	この円の中心を軸とした回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief	この円の外周、内周上の曲がり角（滑らかさ）の数を取得する。
		@return	外周、内周状の曲がり角の数
		*/
		int GetNumberOfCorners() const;

		/**
		@brief	この円の外周、内周上の曲がり角（滑らかさ）の数を設定する。
		@param	numberOfCorners	外周、内周状の曲がり角の数
		*/
		void SetNumberOfCorners(int numberOfCorners);

		ShapeType GetShapeType() const;
	};
}


namespace asd
{
	/**
	@brief	図形としての弧を扱うクラス。
	*/
	class ArcShape :public Shape
	{
	private:
		std::shared_ptr<CoreArcShape> m_coreObject;
	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		ArcShape();
		virtual ~ArcShape();

		/**
		@brief	この弧の中心座標を取得する。
		@return	この弧の中心座標
		*/
		Vector2DF GetPosition() const;

		/**
		@brief	この弧の中心座標を設定する。
		@param	position	この弧の中心座標
		*/
		void SetPosition(Vector2DF position);

		/**
		@brief	この弧の外径を取得する。
		@return	この弧の外径
		*/
		float GetOuterDiameter() const;

		/**
		@brief	この弧の外径を設定する。
		@param	outerDiameter	この弧の外径
		*/
		void SetOuterDiameter(float outerDiameter);

		/**
		@brief	この弧の内径を取得する。
		@return	この弧の内径
		*/
		float GetInnerDiameter() const;

		/**
		@brief	この弧の内径を設定する。
		@param	innerDiameter	この弧の内径
		*/
		void SetInnerDiameter(float innerDiameter);

		/**
		@brief	この弧の中心を軸とした回転角度を取得する。
		@return	この弧の中心を軸とした回転角度
		*/
		float GetAngle() const;

		/**
		@brief	この弧の中心を軸とした回転角度を設定する。
		@param	angle	この弧の中心を軸とした回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief	この弧の外周、内周上の曲がり角（滑らかさ）の数を取得する。
		@return	外周、内周状の曲がり角の数
		*/
		int GetNumberOfCorners() const;

		/**
		@brief	この弧の外周、内周上の曲がり角（滑らかさ）の数を設定する。
		@param	numberOfCorners	外周、内周状の曲がり角の数
		*/
		void SetNumberOfCorners(int numberOfCorners);

		/**
		@brief	この弧の始点となる外周、内周の曲がり角の番号を取得する。(0 <= StartingCorner <= NumberOfCorners)
		@return	始点となる外周、内周の曲がり角の番号
		*/
		int GetStartingCorner() const;

		/**
		@brief	この弧の始点となる外周、内周の曲がり角の番号を設定する。(0 <= StartingCorner <= NumberOfCorners)
		@param	startingCorner	始点となる外周、内周の曲がり角の番号
		*/
		void SetStartingCorner(int startingCorner);

		/**
		@brief	この弧の終点となる外周、内周の曲がり角の番号を取得する。(0 <= EndingCorner <= NumberOfCorners)
		@return	終点となる外周、内周の曲がり角の番号
		*/
		int GetEndingCorner() const;

		/**
		@brief	この弧の終点となる外周、内周の曲がり角の番号を設定する。(0 <= EndingCorner <= NumberOfCorners)
		@param	endingCorner	終点となる外周、内周の曲がり角の番号
		*/
		void SetEndingCorner(int endingCorner);

		ShapeType GetShapeType() const override;
	};
}


namespace asd
{
	/**
	@brief	図形としての矩形を扱うクラス。
	*/
	class RectangleShape :public Shape
	{
	private:
		std::shared_ptr<CoreRectangleShape> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		RectangleShape();
		virtual ~RectangleShape();

		/**
		@brief	矩形の描画範囲を取得する。
		@return	矩形の描画範囲
		*/
		RectF GetDrawingArea() const;

		/**
		@brief	矩形の描画範囲を設定する。
		@param	drawingArea	矩形の描画範囲
		*/
		void SetDrawingArea(RectF drawingArea);

		/**
		@brief	矩形の頂点別UVを取得する。
		@return	矩形の頂点別UV
		*/
		RectF GetUV() const;

		/**
		@brief	矩形の頂点別UVを設定する。
		@param	uv	矩形の頂点別UV
		*/
		void SetUV(RectF uv);

		/**
		@brief	矩形の回転角度を取得する。
		@return	矩形の回転角度
		*/
		float GetAngle() const;

		/**
		@brief	矩形の回転角度を設定する。
		@param	angle	矩形の回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief	矩形の回転の際の中心座標を取得する。
		@return	矩形の回転の際の中心座標
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	矩形の回転の際の中心座標を設定する。
		@centerPosition	矩形の回転の際の中心座標
		*/
		void SetCenterPosition(Vector2DF centerPosition);

		ShapeType GetShapeType() const override;
	};
}


namespace asd
{
	/**
	@brief	図形としての線分を扱うクラス。
	*/
	class LineShape :public Shape
	{
	private:
		std::shared_ptr<CoreLineShape> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		LineShape();
		virtual ~LineShape();

		/**
		@brief	線分の始点座標を取得する。
		@return	線分の視点座標
		*/
		Vector2DF GetStartingPosition() const;

		/**
		@brief	線分の視点座標を設定する。
		@param	startingPosition	線分の始点座標
		*/
		void SetStartingPosition(Vector2DF startingPosition);

		/**
		@brief	線分の終点座標を取得する。
		@return	線分の終点座標
		*/
		Vector2DF GetEndingPosition() const;

		/**
		@brief	線分の終点座標を設定する。
		@param	endingPosition	線分の終点座標
		*/
		void SetEndingPosition(Vector2DF endingPosition);

		/**
		@brief	線分の太さを取得する。
		@return	線分の太さ
		*/
		float GetThickness() const;

		/**
		@brief	線分の太さを設定する。
		@param	thickness	線分の太さ
		*/
		void SetThickness(float thickness);

		ShapeType GetShapeType() const override;
	};
}


namespace asd
{
	/**
	@brief	図形としての多角形を扱うクラス。
	*/
	class PolygonShape :public Shape
	{
	private:
		std::shared_ptr<CorePolygonShape> m_coreObject;
		std::vector<std::shared_ptr<CoreShape>> holeShapes;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		PolygonShape();
		virtual ~PolygonShape();

		/**
		@brief	ポリゴンを構成する頂点を追加する。
		@return	新規に追加する頂点の座標
		*/
		void AddVertex(Vector2DF vertex);

		/**
		@brief	ポリゴンを構成する頂点を全て削除する。
		*/
		void ClearVertexes();

		/**
		@brief	多角形に空ける穴となるオブジェクトを追加する。
		@param	holeShape	穴となるオブジェクトのインスタンス
		@return	正しく追加されたかどうか
		*/
		bool AddHole(std::shared_ptr<CoreShape> holeShape);

		/**
		@brief	多角形に空ける穴となるオブジェクトを削除する。
		@param	holeShape	穴となるオブジェクトのインスタンス
		@return	正しく削除されたかどうか
		*/
		bool RemoveHole(std::shared_ptr<CoreShape> holeShape);

		/**
		@brief	追加された穴となるオブジェクトの一覧を取得する。
		@return	追加された穴となるオブジェクトの一覧
		*/
		std::vector<std::shared_ptr<CoreShape>>& GetHoleShapes();

		/**
		@brief	ポリゴンを構成する頂点の数量を取得する。
		@return	ポリゴンを構成する頂点の数量
		*/
		int GetVertexesNum() const;

		ShapeType GetShapeType() const override;
	};
}


namespace asd
{
	/**
	@brief	2Dコライダの形状タイプ
	*/
	enum class Collider2DShapeType{

		/**
		@brief	円形
		*/
		Circle,

		/**
		@brief	矩形
		*/
		Rectangle,

		/**
		@brief	線形
		*/
		Line,

		/**
		@brief	ポリゴン
		*/
		Polygon,
	};
}

namespace asd {

	enum class CollisionType {
		Enter,
		Stay,
		Exit
	};

	class CoreCollision2D;

	class CoreCollision2DEvent {
		CollisionType collisionType;
		CoreCollision2D* collision;
	public:

		CoreCollision2DEvent(CollisionType collisionType, CoreCollision2D* collision) {
			this->collisionType = collisionType;
			this->collision = collision;
		}

		CollisionType GetCollisionType() {
			return collisionType;
		}

		CoreCollision2D* GetCollision(){
			return collision;
		}
	};
}



namespace asd
{

	class Scene;
	class Object2D;
	class Object3D;

	/**
	@brief	2Dのコライダクラス
	*/
	class Collider2D
	{
		friend class Object2D;
		std::weak_ptr<Object2D> ownerObject2D;
	protected:
		virtual std::shared_ptr<CoreCollider2D> GetCoreCollider() const = 0;
	public:

		/**
		@brief	コライダの形状を取得する。
		@return	コライダの形状
		*/
		virtual Collider2DShapeType GetShapeType() const = 0;

		/**
		@brief	このコライダを保持しているObject2Dを取得する
		*/
		std::weak_ptr<Object2D> GetOwnerObject() const;

		/**
		@brief	このコライダのタグを取得する
		@return	コライダのタグ
		*/
		const achar* GetTag() const;

		/**
		@brief	このコライダのタグを設定する
		@param	tag	コライダのタグ
		*/
		void SetTag(const achar* tag);

		/**
		@brief	このコライダを可視化する成否を設定する（デバッグ用）
		@param	isVisible コライダの可視化の成否
		*/
		void SetVisible(bool isVisible);

		/**
		@brief	このコライダを可視化する成否を取得する（デバッグ用）
		@return コライダの可視化の成否
		*/
		bool GetVisible();
	};
};

#include <vector>
#include <memory>

namespace asd {

	/**
	2Dコライダの衝突情報を扱うクラス
	*/
	class Collision2DInfo {
		std::shared_ptr<Collider2D> selfCollider;
		std::shared_ptr<Collider2D> theirsCollider;
		CollisionType collisionType;

	public:
		Collision2DInfo(std::shared_ptr<Collider2D> selfCollider, std::shared_ptr<Collider2D> theirsCollider, CollisionType collisionType) {
			this->selfCollider = selfCollider;
			this->theirsCollider = theirsCollider;
			this->collisionType = collisionType;
		}

		virtual ~Collision2DInfo() {}

		/**
		brief	衝突した自分自身のコライダを取得する
		*/
		std::shared_ptr<Collider2D> GetSelfCollider() {
			return selfCollider;
		}

		/**
		@brief	衝突した相手のコライダを取得する
		*/
		std::shared_ptr<Collider2D> GetTheirsCollider() {
			return theirsCollider;
		}

		/**
		@brief	衝突状態の種類を取得する
		*/
		CollisionType GetCollisionType() {
			return collisionType;
		}
	};
}



namespace asd {

	/**
	@brief	円形のコライダクラス
	*/
	class CircleCollider : public Collider2D {
	private:
		std::shared_ptr<CoreCircleCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;

	public:

		CircleCollider();
		virtual ~CircleCollider();

		/**
		@brief	この円の中心座標を取得する。
		@return	この円の中心座標
		*/
		Vector2DF GetCenter() const;

		/**
		@brief	この円の中心座標を設定する。
		@param	position	この円の中心座標
		*/
		void SetCenter(Vector2DF position);

		/**
		@brief	この円の半径を取得する。
		@return	この円の半径
		*/
		float GetRadius() const;

		/**
		@brief	この円の半径を設定する。
		@param	radius	この円の半径
		*/
		void SetRadius(float radius);

		Collider2DShapeType GetShapeType() const override;
	};
}



namespace asd {

	/**
	@brief	線形のコライダクラス
	*/
	class LineCollider : public Collider2D{
	private:
		std::shared_ptr<CoreLineCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;

	public:

		LineCollider();
		virtual ~LineCollider();

		/**
		@brief	この線分の始点を取得する
		@return	この線分の始点
		*/
		Vector2DF GetStartingPosition() const;

		/**
		@brief	この線分の始点を設定する
		@param	startingPosition	この線分の始点
		*/
		void SetStartingPosition(Vector2DF startingPosition);

		/**
		@brief	この線分の終点を取得する
		@return	この線分の終点
		*/
		Vector2DF GetEndingPosition() const;

		/**
		@brief	この線分の終点を設定する
		@param	endingPosition	この線分の終点
		*/
		void SetEndingPosition(Vector2DF endingPosition);

		/**
		@brief	この線分の太さを設定する
		@return	この線分の太さ
		*/
		float GetThickness() const;

		/**
		@brief	この線分の太さを取得する
		@param	thickness	この線分の太さ
		*/
		void SetThickness(float thickness);

		Collider2DShapeType GetShapeType() const override;
	};
}


namespace asd {

	/**
	@brief	矩形のコライダクラス
	*/
	class RectangleCollider : public Collider2D{
	private:
		std::shared_ptr<CoreRectangleCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;
	public:
		RectangleCollider();
		virtual ~RectangleCollider();

		/**
		@brief	この矩形の範囲を取得する
		@return	矩形の範囲
		*/
		RectF GetArea() const;

		/**
		@brief	この矩形の範囲を設定する
		@param	area	矩形の範囲
		*/
		void SetArea(RectF area);

		Collider2DShapeType GetShapeType() const override;
	};
}

namespace asd {

	/**
	ポリゴンのコライダクラス
	*/
	class PolygonCollider : public Collider2D
	{
	private:
		std::shared_ptr<CorePolygonCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;
	public:
		PolygonCollider();
		~PolygonCollider();

		/**
		@brief	このポリゴンが正しい形式であることの成否を取得する
		@return	このポリゴンが正しい形式であることの成否
		*/
		bool GetIsValid();

		/**
		@brief	このポリゴンに新たな頂点を追加する
		@param	vertex	追加する新たな頂点の座標
		*/
		void AddVertex(Vector2DF vertex);


		/**
		@brief	このポリゴンの頂点全てをクリアする
		*/
		void ClearVertexes();

		/**
		@brief	このポリゴンの頂点数を取得する
		@return	ポリゴンの頂点数
		*/
		int GetVertexesNum() const;

		/**
		@brief	このポリゴンのindex番目の頂点位置を設定する
		@param	index	ポリゴンの頂点のインデックス
		@param	vertex	設定する頂点座標
		*/
		void SetVertex(int index, Vector2DF vertex);

		/**
		@brief	このポリゴンのindex番目の頂点位置を取得する
		@param	index	ポリゴンの頂点のインデックス
		@param	vertex	取得したポリゴンの頂点座標
		*/
		Vector2DF GetVertex(int index) const;

		Collider2DShapeType GetShapeType() const override;
	};
}

namespace asd
{
	/**
		@brief	子オブジェクトの管理に関する同期の設定を表す値を提供するクラス。
	*/
	class ChildManagementMode
	{
	public:
		typedef int Flags;

		enum : Flags
		{
			/**
				@brief	何も同期しない。
			*/
			Nothing = 0,
			/**
				@brief	レイヤーへの登録状況を同期する。
			*/
			RegistrationToLayer = 1,
			/**
				@brief	親オブジェクトが破棄されたとき、子オブジェクトも破棄する。
			*/
			Disposal = 2,
			/**
				@brief	親オブジェクトが更新されないとき、子オブジェクトも更新しない。
			*/
			IsUpdated = 4,
			/**
				@brief	親オブジェクトが描画されないとき、子オブジェクトも描画されない。
			*/
			IsDrawn = 8,
		};
	};
}


namespace asd
{
	/**
		@brief	オブジェクトの親子関係の同期方法を表す列挙体
	*/
	enum class ChildTransformingMode : int32_t
	{
		/**
			@brief	位置のみを同期する。
		*/
		Position = 0,

		/**
			@brief	位置・角度・拡大率を同期する。
		*/
		All = 1,

		/**
			@brief	同期を行わない。
		*/
		Nothing = 2,
	};
}

namespace asd
{
	/**
		@brief	子オブジェクトの描画に関する同期の設定を表すフラグを提供するクラス。
		*/
	class ChildDrawingMode
	{
	public:
		typedef int Flags;

		enum : Flags
		{
			/**
				@brief	何も同期しない。
			*/
			Nothing = 0,
			/**
				@brief	描画の際に親の色を子に乗算する。
			*/
			Color = 1,
			/**
				@brief	描画の際に親の描画優先度を子の描画優先度に足す。
			*/
			DrawingPriority = 2,
		};
	};
}



namespace asd
{
	class MapObject2D;

	/**
	@brief	マップオブジェクト2Dで描画するテクスチャの情報を扱うチップクラス。
	*/
	class Chip2D
	{
		friend class MapObject2D;
	private:
		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<CoreChip2D> m_coreObject;
	protected:
		std::shared_ptr<CoreChip2D> GetCoreObject() const;

	public:

		Chip2D();
		~Chip2D(){}

		/**
		@brief	描画するテクスチャを設定する。
		@param	texture	描画するテクスチャ
		*/
		void SetTexture(std::shared_ptr<Texture2D> texture);


		/**
		@brief	描画するテクスチャを取得する。
		@return	描画するテクスチャ
		*/
		std::shared_ptr<Texture2D> GetTexture();


		/**
		@brief	このチップのテクスチャ上の描画範囲を取得する。
		@return	テクスチャ上の描画範囲
		*/
		RectF GetSrc() const;


		/**
		@brief	このチップのテクスチャ上の描画範囲を設定する。
		@param	src	テクスチャ上の描画範囲
		*/
		void SetSrc(RectF src);

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な位置を取得する。
		@return 相対的な描画位置
		*/
		Vector2DF GetPosition() const;

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な位置を設定する。
		@param	position	相対的な描画位置
		*/
		void SetPosition(Vector2DF position);

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な回転角度を取得する。
		@return 相対的な回転角度
		*/
		float GetAngle() const;

		/**
		@brief　このチップのマップオブジェクト2Dに対する相対的な回転角度を設定する。
		@param	angle	相対的な回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な拡大率を取得する。
		@return 相対的な拡大率
		*/
		Vector2DF GetScale() const;

		/**
		@brief　このチップのマップオブジェクト2Dに対する相対的な拡大率を設定する。
		@param	scale	相対的な拡大率
		*/
		void SetScale(Vector2DF scale);

		/**
		@brief	このチップを描画する際の中心座標を取得する。
		@return	描画の際の中心座標
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	このチップを描画する際の中心座標を取得する。
		@param	position	描画の際の中心座標
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このチップのテクスチャを描画する際に合成する色を取得する。
		@return	合成する色
		*/
		Color GetColor() const;

		/**
		@brief	このチップのテクスチャを描画する際に合成する色を設定する。
		@param	color	合成する色
		*/
		void SetColor(Color color);

		/**
		@brief　このチップのテクスチャ描画の左右反転フラグを取得する。
		@return	テクスチャ描画の左右反転フラグ
		*/
		bool GetTurnLR() const;

		/**
		@brief　このチップのテクスチャ描画の左右反転フラグを設定する。
		@param	turnLR	テクスチャ描画の左右反転フラグ
		*/
		void SetTurnLR(bool turnLR);

		/**
		@brief　このチップのテクスチャ描画の上下反転フラグを取得する。
		@return	テクスチャ描画の上下反転フラグ
		*/
		bool GetTurnUL() const;

		/**
		@brief　このチップのテクスチャ描画の上下反転フラグを設定する。
		@param	turnLR	テクスチャ描画の上下反転フラグ
		*/
		void SetTurnUL(bool turnUL);

		/**
		@brief	このチップの描画優先度を取得する。
		@return	描画優先度
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このチップの描画優先度を設定する。
		@param	描画優先度
		*/
		void SetDrawingPriority(int priority);

		/**
		@brief　このチップのブレンディング モードを取得する。
		@return	ブレンディング モード
		*/
		AlphaBlendMode GetAlphaBlendMode() const;

		/**
		@brief　このチップのブレンディング モードを設定する。
		@param	alphaBlend	ブレンディング モード
		*/
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend);

		/**
		@brief　このチップのテクスチャを描画する際のテクスチャフィルタの種類を設定する。
		@param	textureFilterType	テクスチャフィルタのタイプ
		*/
		void SetTextureFilterType(TextureFilterType textureFilterType);

		/**
		@brief　このチップのテクスチャを描画する際のテクスチャフィルタの種類を取得する。
		@return	テクスチャフィルタのタイプ
		*/
		TextureFilterType GetTextureFilterType() const;
	};
}
#include <memory>
#include <list>
#include <functional>
#include <unordered_set>
#include <unordered_map>

namespace asd
{
	class Layer2D;
	template<typename TObject> class ObjectManager;
	class EventToManageFamilyship2D;
	class EventToManageDrawnFamilyship2D;

	/**
		@brief	画面上に描画される2Dオブジェクトを表すクラス。
	*/
	class Object2D
		: public std::enable_shared_from_this<Object2D>
		, public IComponentRegisterable<Object2DComponent>
		, public IImmediatelyDisposable
	{
		friend class Layer2D;
		friend class DrawnObject2D;
		friend class ObjectManager<Object2D>;
		friend class Object2DComponent;
		friend class EventToManageFamilyship2D;
		friend class EventToManageDrawnFamilyship2D;

	public:
		typedef std::shared_ptr<Object2D> Ptr;

	private:
		class ParentInfo2D
		{
		private:
			Object2D* m_parent;
			ChildManagementMode::Flags m_managementMode;

		public:
			typedef std::shared_ptr<ParentInfo2D> Ptr;

			ParentInfo2D(Object2D* parent, ChildManagementMode::Flags managementMode)
				: m_parent(parent)
				, m_managementMode(managementMode)
			{
			}

			Object2D* GetParent() const
			{
				return m_parent;
			}

			ChildManagementMode::Flags GetManagementMode() const
			{
				return m_managementMode;
			}
		};

		Layer2D* m_owner;
		ParentInfo2D::Ptr m_parentInfo;
		std::list<Object2D::Ptr> m_children;
		std::unordered_set<std::shared_ptr<Collider2D>> myColliders;
		std::shared_ptr<ComponentManager<Object2DComponent>> m_componentManager;
		std::list<std::shared_ptr<Collision2DInfo>> collisions2DInfo;

		bool m_isUpdated;
		bool m_isDrawn;
		int m_updatePriority;
		std::function<void(int)> m_onUpdatePriorityChanged;

		void RaiseOnAdded();
		void RaiseOnRemoved();

		void RaiseOnCollisionCallbacks();

		void Update();
		void DrawAdditionally();
		void SetLayer(Layer2D* layer);
		virtual CoreObject2D* GetCoreObject() const = 0;

		void SyncContainerWithChild(const Object2D::Ptr& child);

		void Register(const Object2DComponent::Ptr& component);
		void Unregister(const Object2DComponent::Ptr& component);
		void DisposeImmediately();
		void ImmediatelyRemoveComponent(astring key);
		void ImmediatelyAddChild(
			const Object2D::Ptr& child,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode);
		void ImmediatelyRemoveChild(const Object2D::Ptr& child);


	protected:

		/**
			@brief	オーバーライドして、この2Dオブジェクトがレイヤーに登録されたときの処理を記述できる。
		*/
		virtual void OnAdded();
		/**
			@brief	オーバーライドして、この2Dオブジェクトがレイヤーから登録解除されたときの処理を記述できる。
		*/
		virtual void OnRemoved();
		/**
			@brief	オーバーライドして、この2Dオブジェクトが破棄される際の処理を記述できる。
		*/
		virtual void OnDispose();		
		/**
			@brief	オーバーライドして、このオブジェクトの更新処理を記述できる。
		*/
		virtual void OnUpdate();
		/**
			@brief	オーバーライドして、このオブジェクトに対する追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally();

		/**
		@brief	オーバーライドして、この2Dオブジェクトが別の2Dオブジェクトのコライダと衝突開始する際の処理を記述できる。
		@param	collisionInfo	衝突に関する情報
		*/
		virtual void OnCollisionEnter(std::shared_ptr<Collision2DInfo> collisionInfo) {}
		/**
		@brief	オーバーライドして、この2Dオブジェクトが別の2Dオブジェクトのコライダと衝突が継続している際の処理を記述できる。
		@param	collisionInfo	衝突に関する情報
		*/
		virtual void OnCollisionStay(std::shared_ptr<Collision2DInfo> collisionInfo) {}
		/**
		@brief	オーバーライドして、この2Dオブジェクトが別の2Dオブジェクトのコライダと衝突終了した際の処理を記述できる。
		@param	collisionInfo	衝突に関する情報
		*/
		virtual void OnCollisionExit(std::shared_ptr<Collision2DInfo> collisionInfo) {}




		/**
		@brief	通常の描画に加えてテクスチャを描画する。
		@param	upperLeftPos	テクスチャの左上の描画位置
		@param	upperRightPos	テクスチャの右上の描画位置
		@param	lowerRightPos	テクスチャの右下の描画位置
		@param	lowerLeftPos	テクスチャの左下の描画位置
		@param	upperLeftCol	テクスチャの左上の頂点色
		@param	upperRightCol	テクスチャの右上の頂点色
		@param	lowerRightCol	テクスチャの右下の頂点色
		@param	lowerLeftCol	テクスチャの左下の頂点色
		@param	upperLeftUV		テクスチャの左上のUV値
		@param	upperRightUV	テクスチャの右上のUV値
		@param	lowerRightUV	テクスチャの右下のUV値
		@param	lowerLeftUV		テクスチャの左下のUV値
		@param	texture			描画するテクスチャ
		@param	alphaBlend		アルファブレンドの種類
		@param	priority		描画の優先順位(大きいほど前面に描画される)
		@note
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		/**
		@brief	通常の描画に加えて専用のシェーダーを用いて2Dスプライトを描画する。
		@param	upperLeftPos	テクスチャの左上の描画位置
		@param	upperRightPos	テクスチャの右上の描画位置
		@param	lowerRightPos	テクスチャの右下の描画位置
		@param	lowerLeftPos	テクスチャの左下の描画位置
		@param	upperLeftCol	テクスチャの左上の頂点色
		@param	upperRightCol	テクスチャの右上の頂点色
		@param	lowerRightCol	テクスチャの右下の頂点色
		@param	lowerLeftCol	テクスチャの左下の頂点色
		@param	upperLeftUV		テクスチャの左上のUV値
		@param	upperRightUV	テクスチャの右上のUV値
		@param	lowerRightUV	テクスチャの右下のUV値
		@param	lowerLeftUV		テクスチャの左下のUV値
		@param	material		表示に使用するマテリアル	
		@param	alphaBlend		アルファブレンドの種類
		@param	priority		描画の優先順位(大きいほど前面に描画される)
		@note
		専用のシェーダーを用いてスプライトを描画する。
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteWithMaterialAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			std::shared_ptr<Material2D> material, AlphaBlendMode alphaBlend, int32_t priority);

		/**
		@brief	通常の描画に加えて文字列を描画する。
		@param	pos					描画位置
		@param	color				頂点色
		@param	font				フォント
		@param	text				文字列
		@param	writingDirection	行方向
		@param	alphaBlend		アルファブレンドの種類
		@param	priority		描画の優先順位(大きいほど前面に描画される)
		@note
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawTextAdditionally(Vector2DF pos, Color color, std::shared_ptr<Font> font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawRectangleAdditionally(RectF drawingArea, Color color, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawCircleAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawArcAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawShapeAdditionally(std::shared_ptr<Shape> shape, Color color, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

	public:
		/**
			@brief	コンストラクタ
		*/
		Object2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Object2D();

		/**
			@brief	カメラに表示するグループを取得する。
			@return	グループ
			@note
			カメラのグループと他のオブジェクトのグループで&でビット演算して1以上だった場合、そのカメラにオブジェクトが描画されるようになる。

		*/
		int32_t GetCameraGroup() const;

		/**
		@brief	カメラに表示するグループを設定する。
		@param	group	グループ
		*/
		void SetCameraGroup(int32_t group);

		/**
			@brief	このオブジェクトが更新されるかどうかを取得する。
		*/
		bool GetIsUpdated() const;
		/**
			@brief	このオブジェクトが更新されるかどうかを設定する。
		*/
		void SetIsUpdated(bool value);

		/**
			@brief	このオブジェクトが描画されるかどうかを取得する。
		*/
		bool GetIsDrawn() const;

		/**
			@brief	このオブジェクトが描画されるかどうかを設定する。
		*/
		void SetIsDrawn(bool value);

		/**
			@brief	このオブジェクトが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返す。
		*/
		bool GetIsAlive() const;

		/**
			@brief	このオブジェクトを破棄する。
		*/
		void Dispose();

		/**
			@brief	このオブジェクトを保持しているレイヤーを取得する。
		*/
		Layer2D* GetLayer() const;

		/**
			@brief	指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
			@param	child	追加する子オブジェクト
			@param	managementMode	子オブジェクトの管理に関する同期設定。フラグをOR演算でつなげて複数指定することができる。
			@param	transformingmode	子オブジェクトの変形に関する同期設定。
		*/
		void AddChild(const Object2D::Ptr& child, ChildManagementMode::Flags managementMode, ChildTransformingMode transformingMode);
		/**
			@brief	指定した子オブジェクトをこのインスタンスから削除する。
			@param	child	削除する子オブジェクト
		*/
		void RemoveChild(const Object2D::Ptr& child);
		/**
			@brief	このオブジェクトが保持している子オブジェクトを含むコンテナを取得する。
		*/
		const std::list<Object2D::Ptr>& GetChildren() const;
		/**
			@brief	このオブジェクトの親オブジェクトを取得する。親がいなければnullptrを返す。
		*/
		Object2D* GetParent() const;

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Object2DComponent::Ptr& component, astring key);
		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		Object2DComponent::Ptr GetComponent(astring key);
		/**
			@brief	指定したコンポーネントを削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		bool RemoveComponent(astring key);


		/**
			@brief	このオブジェクトを描画する位置を取得する。
		*/
		Vector2DF GetPosition() const;
		/**
			@brief	このオブジェクトを描画する位置を設定する。
		*/
		void SetPosition(Vector2DF position);
		/**
			@brief	このオブジェクトを描画する実際の位置を取得する。親子関係がある場合に、親の位置を考慮した値を取得できる。
		*/
		Vector2DF GetGlobalPosition();

		/**
			@brief	このオブジェクトを描画する際の回転角度を取得する。
			@note
			回転角度は度で表記され、回転方向は時計回りである。
		*/
		float GetAngle() const;
		/**
			@brief	このオブジェクトを描画する際の回転角度を設定する。
			@note
			回転角度は度で表記され、回転方向は時計回りである。
		*/
		void SetAngle(float value);

		/**
			@brief	このオブジェクトを描画する際の拡大率を取得する。
		*/
		Vector2DF GetScale() const;
		/**
			@brief	このオブジェクトを描画する際の拡大率を設定する。
		*/
		void SetScale(Vector2DF value);

		/**
			@brief	このオブジェクトの更新の優先順位を取得する。
		*/
		int GetUpdatePriority() const;
		/**
			@brief	このオブジェクトの更新の優先順位を設定する。
		*/
		void SetUpdatePriority(int value);

		/**
			@brief	このオブジェクトが親子関係を考慮して最終的に更新されるかどうかの真偽値を取得します。
		*/
		bool GetAbsoluteBeingUpdated() const;
		/**
			@brief	このオブジェクトが親子関係を考慮して最終的に描画されるかどうかの真偽値を取得します。
		*/
		bool GetAbsoluteBeingDrawn() const;

		/**
			@brief	衝突判定用のコライダを追加する
			@param	collider	コライダ
		*/
		void AddCollider(std::shared_ptr<Collider2D> collider);


		/**
			@brief	衝突判定用のコライダを削除する
			@param	collider	コライダ
		*/
		void RemoveCollider(std::shared_ptr<Collider2D> collider);

		/**
		@brief	現時点での衝突情報の一覧を取得する
		@return	現時点での衝突情報の一覧
		*/
		std::list<std::shared_ptr<Collision2DInfo>> &GetCollisionsInfo();
	};
}
#include <memory>

namespace asd
{
	class EventToManageDrawnFamilyship2D;

	class DrawnObject2D
		: public Object2D
	{
		friend class EventToManageDrawnFamilyship2D;

	public:
		typedef std::shared_ptr<DrawnObject2D> Ptr;

	private:
		virtual CoreDrawnObject2D* GetCoreDrawnObject() const = 0;

		void ImmediatelyAddDrawnChild(
			const DrawnObject2D::Ptr& child,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drawingMode);

	public:

		/**
			@brief	描画する際に合成する色を取得する。
		*/
		Color GetColor() const;
		/**
			@brief	描画する際に合成する色を設定する。
		*/
		void SetColor(Color value);

		/**
			@brief	このオブジェクトの描画優先度を取得する。
		*/
		int GetDrawingPriority() const;
		/**
			@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int value);

		/**
			@brief	親子関係を考慮して最終的に描画時にこのオブジェクトに合成する色を取得する。
		*/
		Color GetAbsoluteColor() const;
		/**
			@brief	親子関係を考慮したこのオブジェクトの最終的な描画優先度を取得する。
		*/
		int GetAbsoluteDrawingPriority() const;

		/**
			@brief	描画に関する同期設定を指定して、指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
			@param	child	追加する子オブジェクト
			@param	managementMode	子オブジェクトの管理に関する同期設定。フラグをOR演算でつなげて複数指定することができる。
			@param	transformingmode	子オブジェクトの変形に関する同期設定。
			@param	drawingMode	子オブジェクトの描画に関する同期設定。フラグをOR演算でつなげて複数指定することができる。
		*/
		void AddDrawnChild(
			const DrawnObject2D::Ptr& child,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drawingMode);
	};
}
#include <memory>

namespace asd
{
	/**
	@brief	テクスチャを描画するクラス。
	*/
	class TextureObject2D : public DrawnObject2D
	{
	private:
		std::shared_ptr<CoreTextureObject2D> m_coreObject;
		std::shared_ptr<Texture2D> m_texturePtr;

		CoreObject2D* GetCoreObject() const;
		CoreDrawnObject2D* GetCoreDrawnObject() const override;

	public:
		typedef std::shared_ptr<TextureObject2D> Ptr;

		/**
			@brief	コンストラクタ
		*/
		TextureObject2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~TextureObject2D();


		/**
			@brief	描画するテクスチャを取得する。
		*/
		std::shared_ptr<Texture2D> GetTexture() const;
		/**
			@brief	描画するテクスチャを設定する。
		*/
		void SetTexture(std::shared_ptr<Texture2D> texture);

		/**
		@brief	テクスチャ上の描画範囲を取得する。
		*/
		RectF GetSrc() const;

		/**
		@brief	テクスチャ上の描画範囲を設定する。
		@param value 描画するテクスチャ上の範囲。(フィールドのいずれかが0より小さかったらテクスチャ全体を描画する。)
		*/
		void SetSrc(RectF value);

		/**
			@brief	描画する際の中心座標を取得する。
		*/
		Vector2DF GetCenterPosition() const;
		/**
			@brief	描画する際の中心座標を設定する。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
			@brief	左右反転して描画するかどうかを取得する。
		*/
		bool GetTurnLR() const;
		/**
			@brief	左右反転して描画するかどうかを設定する。
		*/
		void SetTurnLR(bool turnLR);

		/**
			@brief	上下反転して描画するかどうかを取得する。
		*/
		bool GetTurnUL() const;
		/**
			@brief	上下反転して描画するかどうかを設定する。
		*/
		void SetTurnUL(bool turnUL);

		/**
			@brief	このオブジェクトのブレンディング モードを取得する。
		*/
		AlphaBlendMode GetAlphaBlendMode() const;
		/**
			@brief	このオブジェクトのブレンディング モードを設定する。
		*/
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend);

		/**
		@brief　このオブジェクトを描画する際のテクスチャフィルタの種類を設定する。
		*/
		void SetTextureFilterType(TextureFilterType textureFilterType);

		/**
		@brief　このオブジェクトを描画する際のテクスチャフィルタの種類を取得する。
		*/
		TextureFilterType GetTextureFilterType() const;

		/**
		@brief このオブジェクトを描画する際に使用されるマテリアルを取得する。
		*/
		std::shared_ptr<Material2D> GetMaterial();

		/**
		@brief このオブジェクトを描画する際に使用されるマテリアルを設定する。
		@param	material	マテリアル
		*/
		void SetMaterial(const std::shared_ptr<Material2D>& material);
	};
}


namespace asd
{
	/**
		@brief	2Dレイヤーの一部を描画するためのカメラ
	*/
	class CameraObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreCameraObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<CameraObject2D> Ptr;

		CameraObject2D();
		virtual ~CameraObject2D();

		/**
		@brief	このオブジェクトの描画優先度を取得する。
		*/
		int GetDrawingPriority() const;
		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int value);

		/**
			@brief	描画元の領域を取得する。
			@return	描画元
		*/
		RectI GetSrc() const;

		/**
		@brief	描画元の領域を設定する。
		@param	value	描画元
		*/
		void SetSrc(RectI value);

		/**
		@brief	描画先の領域を取得する。
		@return	描画先
		*/
		RectI GetDst() const;

		/**
		@brief	描画先の領域を設定する。
		@param	value	描画先
		*/
		void SetDst(RectI value);

		/**
		@brief	画面に描画せずにテクスチャに描画するか、を取得する。
		@return	画面に描画せずにテクスチャに描画するか
		*/
		bool GetIsOffscreenMode();

		/**
		@brief	画面に描画せずにテクスチャに描画するか、を設定する。
		@param	value	画面に描画せずにテクスチャに描画するか
		*/
		void SetIsOffscreenMode(bool value);

		/**
		@brief	カメラが描画した結果をテクスチャとして取得する。
		@return	描画結果
		*/
		std::shared_ptr<Texture2D> GetTexture();

		/**
		@brief　カメラが描画した結果を描画する際のテクスチャフィルタの種類を設定する。
		*/
		void SetTextureFilterType(TextureFilterType textureFilterType);

		/**
		@brief　カメラが描画した結果を描画する際のテクスチャフィルタの種類を取得する。
		*/
		TextureFilterType GetTextureFilterType() const;
	};
}


namespace asd
{
	/**
	@brief	文字列の描画を扱うクラス。
	*/
	class TextObject2D : public DrawnObject2D
	{
	private:
		std::shared_ptr<CoreTextObject2D> m_coreObject;
		std::shared_ptr<Font> m_font;

		CoreObject2D* GetCoreObject() const;
		CoreDrawnObject2D* GetCoreDrawnObject() const override;

	public:
		typedef std::shared_ptr<TextObject2D> Ptr;

		TextObject2D();
		virtual ~TextObject2D();

		/**
		@brief	このオブジェクトを描画する際の描画方向を取得する。
		*/
		WritingDirection GetWritingDirection() const;

		/**
		@brief	このオブジェクトを描画する際のフォントを取得する。
		*/
		std::shared_ptr<Font> GetFont() const;

		/**
		@brief 描画するテキストを取得する。
		*/
		const achar* GetText();

		/**
		@brief	このオブジェクトの原点位置を取得する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を取得する。
		*/
		bool GetTurnLR() const;

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を取得する。
		*/
		bool GetTurnUL() const;

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを取得します。
		*/
		AlphaBlendMode GetAlphaBlendMode() const;

		/**
		@brief	このオブジェクトを描画する際の描画方向を設定します。
		*/
		void SetWritingDirection(WritingDirection writingDirection);

		/**
		@brief	このオブジェクトを描画する際のフォントを設定する。
		*/
		void SetFont(std::shared_ptr<Font> font);

		/**
		@brief 描画するテキストを設定する。
		*/
		void SetText(const achar* text);

		/**
		@brief	このオブジェクトの原点位置を設定する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を設定する。
		*/
		void SetTurnLR(bool turnLR);

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を設定する。
		*/
		void SetTurnUL(bool turnUL);

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを設定する。
		*/
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend);

		/**
		@brief　このオブジェクトを描画する際のテクスチャフィルタの種類を設定する。
		*/
		void SetTextureFilterType(TextureFilterType textureFilterType);

		/**
		@brief　このオブジェクトを描画する際のテクスチャフィルタの種類を取得する。
		*/
		TextureFilterType GetTextureFilterType() const;

		/**
		@brief テキストを描画する際の文字間値を設定する。
		*/
		void SetLetterSpacing(float letterSpacing);

		/**
		@brief テキストを描画する際の文字間値を取得する。
		*/
		float GetLetterSpacing() const;

		/**
		@brief テキストを描画する際の行間値を設定する。
		*/
		void SetLineSpacing(float lineSpacing);

		/**
		@brief テキストを描画する際の行間値を取得する。
		*/
		float GetLineSpacing() const;

		/**
		@brief	文字の色等を変更できるモードか、取得する。
		*/
		bool GetIsRichTextMode() const;

		/**
		@brief	文字の色等を変更できるモードか、設定する。
		*/
		void SetIsRichTextMode(bool value);

		/**
			@brief このオブジェクトを描画する際に使用されるマテリアルを取得する。
		*/
		std::shared_ptr<Material2D> GetMaterial();

		/**
			@brief このオブジェクトを描画する際に使用されるマテリアルを設定する。
			@param	material	マテリアル
		*/
		void SetMaterial(const std::shared_ptr<Material2D>& material);
	};
}


namespace asd
{
	class EffectObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreEffectObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<EffectObject2D> Ptr;

		EffectObject2D();
		virtual ~EffectObject2D();

		/**
		@brief	描画に使用するエフェクトを取得する。
		@return	エフェクト
		*/
		std::shared_ptr<Effect> GetEffect();

		/**
		@brief	描画に使用するエフェクトを設定する。
		@param	effect	エフェクト
		*/
		void SetEffect(std::shared_ptr<Effect> effect);


		/**
		@brief	設定されている全てのエフェクトを再生する。
		@return	再生されたエフェクトのID
		*/
		int32_t Play();

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		/**
		@brief	このオブジェクトから再生されたエフェクトを表示状態にする。
		*/
		void Show();

		/**
		@brief	このオブジェクトから再生されたエフェクトを非表示状態にする。
		*/
		void Hide();

		/**
		@brief	このオブジェクトから再生されたエフェクトが再生中か取得する。
		@return	再生中か?
		*/
		bool GetIsPlaying();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value);

		/**
		@brief	このオブジェクトから再生されるエフェクトの角度(X軸)を取得する。
		@return	value	回転量(度)
		@note
		*/
		float GetEffectRotationX() const;

		/**
		@brief	このオブジェクトから再生されるエフェクトの角度(X軸)を指定する。
		@param	value	回転量(度)
		@note
		エフェクトの形状が画面に垂直な方向に長い場合、2D表示では予期せぬ見た目で表示されることがある。
		この時、X軸を中心にエフェクトを回転することで、違和感のない表示をできるようにする。
		*/
		void SetEffectRotationX(float value);

		/**
		@brief	このオブジェクトから再生されるエフェクトの角度(Y軸)を取得する。
		@return	value	回転量(度)
		@note
		*/
		float GetEffectRotationY() const;

		/**
		@brief	このオブジェクトから再生されるエフェクトの角度(Y軸)を指定する。
		@param	value	回転量(度)
		@note
		エフェクトの形状が画面に垂直な方向に長い場合、2D表示では予期せぬ見た目で表示されることがある。
		この時、Y軸を中心にエフェクトを回転することで、違和感のない表示をできるようにする。
		*/
		void SetEffectRotationY(float value);

		/**
		@brief	このオブジェクトから再生されるエフェクトの向きを取得する。
		@return	value	回転量(度)
		@note
		互換性のために残されている。
		*/
		float GetEffectRotation() const;

		/**
		@brief	このオブジェクトから再生されるエフェクトの向きを指定する。
		@param	value	回転量(度)
		@note
		2D表示だとエフェクトが画面に対して前後に表示されるように作成されていた場合、予期せぬ見た目で表示されてしまうことがある。
		そのため、Y軸を中心にエフェクトを回転させ2Dとして違和感のない表示ができるようにする。
		互換性のために残されている。
		*/
		void SetEffectRotation(float value);

		/**
		@brief	このオブジェクトの描画優先度を取得する。
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);
	};
}
#include <memory>

namespace asd
{
	/**
	@brief	テクスチャをチップとして高速に描画するクラス。
	*/
	class MapObject2D : public Object2D
	{
		friend class Chip2D;
	private:
		std::shared_ptr<CoreMapObject2D> m_coreObject;
		std::vector<std::shared_ptr<Chip2D>> m_chips;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<MapObject2D> Ptr;

		/**
		@brief	コンストラクタ
		*/
		MapObject2D();
		/**
		@brief	デストラクタ
		*/
		virtual ~MapObject2D();

		/**
		@brief	このオブジェクトの原点位置を取得する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	このオブジェクトの描画優先度を取得します。
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このオブジェクトの原点位置を設定する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);

		/**
		@brief	このオブジェクトに描画チップを追加する。
		*/
		bool AddChip(std::shared_ptr<Chip2D> chip);

		/**
		@brief	このオブジェクトから描画チップを削除する。
		*/
		bool RemoveChip(std::shared_ptr<Chip2D> chip);

		/**
		@brief	このオブジェクトに追加されている描画チップを全て削除する。
		*/
		void Clear();

		/**
		@brief　このオブジェクトが保有しているチップを全て取得する。
		*/
		std::vector<std::shared_ptr<Chip2D>>& GetChips();
	};
}


namespace asd
{
	/**
	@brief	図形の描画を扱うクラス。
	*/
	class GeometryObject2D : public DrawnObject2D
	{
	private:
		std::shared_ptr<CoreGeometryObject2D> m_coreObject;
		std::shared_ptr<Texture2D> texturePtr;
		std::shared_ptr<Shape> shapePtr;

		CoreObject2D* GetCoreObject() const;
		CoreDrawnObject2D* GetCoreDrawnObject() const override;

	public:
		typedef std::shared_ptr<GeometryObject2D> Ptr;

		GeometryObject2D();
		virtual ~GeometryObject2D();

		/**
		@brief	描画に使用する図形を取得する。
		@return 図形
		*/
		std::shared_ptr<Shape> GetShape() const ;

		/**
		@brief	描画に使用する図形を設定する。
		@param	shape	図形
		*/
		void SetShape(std::shared_ptr<Shape> shape);

		/**
		@brief	このオブジェクトのブレンディング モードを取得する。
		@return	ブレンディングモード
		*/
		AlphaBlendMode GetAlphaBlendMode() const;

		/**
		@brief	このオブジェクトのブレンディング モードを設定する。
		@param	alphaBlend	ブレンディングモード
		*/
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend);

		/**
		@brief	このオブジェクトを描画する際の中心座標を設定する。
		@param	centerPosition	描画する際の中心座標
		*/
		void SetCenterPosition(Vector2DF centerPosition);

		/**
		@brief	このオブジェクトを描画する際の中心座標を取得する。
		@return	描画する際の中心座標
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	このオブジェクトを描画する際のテクスチャフィルタの種類を設定する。
		@param	textureFilterType	描画する際のテクスチャフィルタの種類
		*/
		void SetTextureFilterType(TextureFilterType textureFilterType);

		/**
		@brief	このオブジェクトを描画する際のテクスチャフィルタの種類を取得する。
		@return	描画する際のテクスチャフィルタの種類
		*/
		TextureFilterType GetTextureFilterType() const;

		/**
		@brief	このオブジェクトを描画に使用するエフェクトを設定する。
		@param	texture	テクスチャ
		*/
		void SetTexture(std::shared_ptr<Texture2D> texture);

		/**
		@brief	このオブジェクトを描画に使用するエフェクトを取得する。
		@return	テクスチャ
		*/
		std::shared_ptr<Texture2D> GetTexture() const;
	};
}
#include <memory>
#include <list>
#include <map>


namespace asd
{
	template<typename TObject> class ObjectManager;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	*/
	class Layer2D
		: public Layer
		, public IObjectRegisterable<Object2D>
		, public IComponentRegisterable<Layer2DComponent>
	{
		friend class Scene;
		friend class Object2D;
		friend class Layer2DComponent;

	public:
		typedef std::shared_ptr<Layer2D> Ptr;

	private:
		std::shared_ptr<CoreLayer2D>	m_coreLayer;
		std::shared_ptr<ObjectManager<Object2D>> m_objects;
		std::shared_ptr<ComponentManager<Layer2DComponent>> m_components;
		std::unordered_map<int64_t, std::shared_ptr<Collider2D>> collidersMap;

		void BeginUpdating() override;
		void EndUpdateting() override;
		void UpdateInternal() override;
		void DrawAdditionally() override;
		void DisposeInternal() override;

		void ImmediatelyRemoveObject(const Object2D::Ptr& object, bool raiseEvent);
		void ImmediatelyRemoveComponent(astring key);
		void Register(const Object2D::Ptr& object);
		void Unregister(const Object2D::Ptr& object);
		void AddToCore(const Object2D::Ptr& object) override;
		void RemoveFromCore(const Object2D::Ptr& object) override;
		void Register(const Layer2DComponent::Ptr& component) override;
		void Unregister(const Layer2DComponent::Ptr& component) override;

		void RaiseOnAdded() override;
		void RaiseOnRemoved() override;

	protected:

	public:
		/**
			@brief	コンストラクタ
		*/
		Layer2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~Layer2D();

		bool GetIsAlive() const;

		/**
			@brief	指定した2Dオブジェクトをこのインスタンスに追加する。
			@param	object	追加するオブジェクト
		*/
		void AddObject(const Object2D::Ptr& object);
		/**
			@brief	指定した2Dオブジェクトをこのインスタンスから削除する。
			@param	object	削除するオブジェクト
		*/
		void RemoveObject(const Object2D::Ptr& object);
		/**
			@brief	このレイヤーに登録されている2Dオブジェクトのリストを取得する。
			@return	登録されているオブジェクトのリスト
		*/
		std::list<Object2D::Ptr> GetObjects() const;

		/**
		@brief	レイヤーが所持する全ての2Dオブジェクトをこのインスタンスから削除する。
		*/
		void Clear();

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const Layer2DComponent::Ptr& component, astring key);

		/**
			@brief	指定したキーを持つコンポーネントを取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		Layer2DComponent::Ptr GetComponent(astring key);

		/**
			@brief	指定したコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		bool RemoveComponent(astring key);

		/**
			@brief	通常の描画に加えてテクスチャを描画する。
			@param	upperLeftPos	テクスチャの左上の描画位置
			@param	upperRightPos	テクスチャの右上の描画位置
			@param	lowerRightPos	テクスチャの右下の描画位置
			@param	lowerLeftPos	テクスチャの左下の描画位置
			@param	upperLeftCol	テクスチャの左上の頂点色
			@param	upperRightCol	テクスチャの右上の頂点色
			@param	lowerRightCol	テクスチャの右下の頂点色
			@param	lowerLeftCol	テクスチャの左下の頂点色
			@param	upperLeftUV		テクスチャの左上のUV値
			@param	upperRightUV	テクスチャの右上のUV値
			@param	lowerRightUV	テクスチャの右下のUV値
			@param	lowerLeftUV		テクスチャの左下のUV値
			@param	texture			描画するテクスチャ
			@param	alphaBlend		アルファブレンドの種類
			@param	priority		描画の優先順位(大きいほど前面に描画される)
			@note
			OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteAdditionally(
			const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
			const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
			const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
			std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, int32_t priority);

		/**
		@brief	通常の描画に加えて専用のシェーダーを用いて2Dスプライトを描画する。
		@param	upperLeftPos	テクスチャの左上の描画位置
		@param	upperRightPos	テクスチャの右上の描画位置
		@param	lowerRightPos	テクスチャの右下の描画位置
		@param	lowerLeftPos	テクスチャの左下の描画位置
		@param	upperLeftCol	テクスチャの左上の頂点色
		@param	upperRightCol	テクスチャの右上の頂点色
		@param	lowerRightCol	テクスチャの右下の頂点色
		@param	lowerLeftCol	テクスチャの左下の頂点色
		@param	upperLeftUV		テクスチャの左上のUV値
		@param	upperRightUV	テクスチャの右上のUV値
		@param	lowerRightUV	テクスチャの右下のUV値
		@param	lowerLeftUV		テクスチャの左下のUV値
		@param	material		表示に使用するマテリアル
		@param	alphaBlend		アルファブレンドの種類
		@param	priority		描画の優先順位(大きいほど前面に描画される)
		@note
		専用のシェーダーを用いてスプライトを描画する。
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteWithMaterialAdditionally(
			const Vector2DF& upperLeftPos, const Vector2DF& upperRightPos, const Vector2DF& lowerRightPos, const Vector2DF& lowerLeftPos,
			const Color& upperLeftCol, const Color& upperRightCol, const Color& lowerRightCol, const Color& lowerLeftCol,
			const Vector2DF& upperLeftUV, const Vector2DF& upperRightUV, const Vector2DF& lowerRightUV, const Vector2DF& lowerLeftUV,
			std::shared_ptr<Material2D> material, AlphaBlendMode alphaBlend, int32_t priority);

		/**
			@brief	通常の描画に加えて文字列を描画する。
			@param	pos					描画位置
			@param	color				頂点色
			@param	font				フォント
			@param	text				文字列
			@param	writingDirection	行方向
			@param	alphaBlend		アルファブレンドの種類
			@param	priority		描画の優先順位(大きいほど前面に描画される)
			@note
			OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawTextAdditionally(Vector2DF pos, Color color, std::shared_ptr<Font> font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawRectangleAdditionally(const RectF& drawingArea, const Color& color, const RectF& uv, const std::shared_ptr<Texture2D>& texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawRotatedRectangleAdditionally(RectF drawingArea, Color color, Vector2DF rotationCenter, float angle, RectF uv, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawTriangleAdditionally(Vector2DF position1, Vector2DF position2, Vector2DF position3, Color color, Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawCircleAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawArcAdditionally(asd::Vector2DF center, float outerDiameter, float innerDiameter, Color color, int vertNum, int startingVerticalAngle, int endingVerticalAngle, float angle, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawLineAdditionally(Vector2DF point1, Vector2DF point2, float thickness, Color color, AlphaBlendMode alphaBlend, int32_t priority);

		void DrawShapeAdditionally(std::shared_ptr<Shape> shape, Color color, std::shared_ptr<Texture2D> texture, AlphaBlendMode alphaBlend, int32_t priority);

		/**
		@brief	レイヤーの種類を取得する。
		@return	レイヤーの種類
		*/
		LayerType GetLayerType() const override { return LayerType::Layer2D; }

		int GetObjectCount() const;

		/**
			@brief	エフェクトの歪みが有効かどうか、取得する。
			@return	有効/無効
		*/
		bool GetIsDistortionEnabled() const;

		/**
			@brief	エフェクトの歪みが有効かどうか、設定する。
			@param	value	有効/無効
		*/
		void SetIsDistortionEnabled(bool value);

	};
}

#include <functional>

namespace asd
{
	class Layer3D;
	template<class TObject> class ObjectManager;

	/**
		@brief	更新・描画処理を行う単位となる3Dオブジェクトの機能を提供するクラス
	*/
	class Object3D
		: public std::enable_shared_from_this<Object3D>
		, public IImmediatelyDisposable
	{
		friend class Layer3D;
		friend class ObjectManager<Object3D>;

	public:
		typedef std::shared_ptr<Object3D> Ptr;

	private:

		Layer3D*		m_owner;

		bool m_isUpdated;
		bool m_isDrawn;
		bool m_isAlive;
		int m_updatePriority;
		std::function<void(int)> m_onUpdatePriorityChanged;
		
		void RaiseOnAdded();
		void RaiseOnRemoved();
		void Update();
		void SetLayer(Layer3D* layer);
		CoreObject3D* GetCoreObject() const;
		void DisposeImmediately();

	protected:
		CoreObject3D*	m_commonObject;

		/**
		@brief	ユーザーはオーバーライドしてはいけない。
		*/
		virtual void OnUpdateInternal() {}

	protected:

		/**
			@brief	オーバーライドして、この3Dオブジェクトがレイヤーに登録されたときの処理を記述できる。
		*/
		virtual void OnAdded();

		/**
			@brief	オーバーライドして、この3Dオブジェクトがレイヤーから登録解除されたときの処理を記述できる。
		*/
		virtual void OnRemoved();

		/**
			@brief	オーバーライドして、この3Dオブジェクトが破棄されるときの処理を記述できる。
		*/
		virtual void OnDispose();

		/**
			@brief	オーバーライドして、この3Dオブジェクトの更新処理を記述することができる。
		*/
		virtual void OnUpdate() = 0;

		/**
			@brief	オーバーライドして、この3Dオブジェクトに関する追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() = 0;

	public:
		/**
			@brief	コンストラクタ
		*/
		Object3D();

		/**
			@brief	デストラクタ
		*/
		virtual ~Object3D();

		/**
		@brief	オブジェクトの更新を実行するかどうか取得する。
		*/
		bool GetIsUpdated() const;

		/**
		@brief	オブジェクトの更新を実行するかどうか設定する。
		@param	value	オブジェクトの更新を実行するかどうか
		*/
		void SetIsUpdated(bool value);

		/**
		@brief	オブジェクトを描画するどうか取得する。
		*/
		bool GetIsDrawn() const;
		/**
		@brief	オブジェクトを描画するどうか設定する。
		@param	value	オブジェクトを描画するどうか
		*/
		void SetIsDrawn(bool value);

		/**
		@brief	オブジェクトが生存しているか取得する。
		@return	生存しているか
		*/
		bool GetIsAlive() const;

		/**
		@brief	オブジェクトを破棄する。
		*/
		void Dispose();

		/**
		@brief	このインスタンスを管理している asd.Layer3D クラスのインスタンスを取得する。
		*/
		Layer3D* GetLayer() const;

		/**
			@brief	このインスタンスの親に対する現在の座標を取得する。
			@return	座標
		*/
		Vector3DF GetPosition() const;

		/**
			@brief	このインスタンスの親に対する現在の座標を設定する。
			@param	position	座標
		*/
		void SetPosition(Vector3DF position);

		/**
		@brief	このインスタンスの親に対する現在の角度を取得する。
		@return	角度
		*/
		Vector3DF GetRotation() const;

		/**
		@brief	このインスタンスの親に対する現在の角度を設定する。
		@param	angle	角度
		*/
		void SetRotation(Vector3DF angle);

		/**
		@brief	このインスタンスの親に対する現在の拡大率を取得する。
		@return	拡大率
		*/
		Vector3DF GetScale() const;

		/**
		@brief	このインスタンスの親に対する現在の拡大率を設定する。
		@param	scale	拡大率
		*/
		void SetScale(Vector3DF scale);

		/**
		@brief	モデル行列を直接取得する。
		@return	モデル行列
		*/
		Matrix44 GetLocalMatrix();

		/**
		@brief	モデル行列を直接設定する。
		@param	localMat	モデル行列
		@note
		この値を設定した場合、通常の移動や回転は無効化される。
		不具合が存在するため、一時的に無効化
		*/
		//void SetLoalMatrixDirectly(Matrix44 localMat);

		/**
			@brief	このインスタンスの更新の優先順位を取得する。
		*/
		int GetUpdatePriority() const;
		/**
			@brief	このインスタンスの更新の優先順位を設定する。
		*/
		void SetUpdatePriority(int value);


		/**
		@brief	通常の描画に加えてテクスチャを描画する。
		@param	upperLeftPos	テクスチャの左上の描画位置
		@param	upperRightPos	テクスチャの右上の描画位置
		@param	lowerRightPos	テクスチャの右下の描画位置
		@param	lowerLeftPos	テクスチャの左下の描画位置
		@param	upperLeftCol	テクスチャの左上の頂点色
		@param	upperRightCol	テクスチャの右上の頂点色
		@param	lowerRightCol	テクスチャの右下の頂点色
		@param	lowerLeftCol	テクスチャの左下の頂点色
		@param	upperLeftUV		テクスチャの左上のUV値
		@param	upperRightUV	テクスチャの右上のUV値
		@param	lowerRightUV	テクスチャの右下のUV値
		@param	lowerLeftUV		テクスチャの左下のUV値
		@param	texture			描画するテクスチャ
		@param	alphaBlend		アルファブレンドの種類
		@param	depthWrite		深度を書き込むか?
		@param	depthTest		深度を比較するか?
		@note
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest);
	};
}



namespace asd
{
	class PostEffect;

	/**
	@brief	カメラの機能を提供するクラス
	*/
	class CameraObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreCameraObject3D> m_coreObject;

		std::vector<std::shared_ptr<PostEffect>>	m_postEffects;

		void OnUpdateInternal() override;

	protected:
		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		CameraObject3D();
		virtual ~CameraObject3D();

		/**
			@brief	画面サイズを取得する。
			@return	画面サイズ
		*/
		Vector2DI GetWindowSize() const;

		/**
			@brief	画面サイズを設定する。
			@param	size	画面サイズ
		*/
		void SetWindowSize(const Vector2DI& size);

		/**
			@brief	カメラが見る座標を取得する。
			@return	カメラが見る座標
		*/
		Vector3DF GetFocus();

		/**
			@brief	カメラが見る座標を設定する。
			@param	focus	カメラが見る座標
		*/
		void SetFocus(const Vector3DF& focus);

		/**
		@brief	画角を取得する。
		@return	画角
		*/
		float GetFieldOfView();

		/**
		@brief	画角を設定する。
		@param	fov	画角
		*/
		void SetFieldOfView(float fov);

		/**
		@brief	画角を取得する。
		@return	画角
		*/
		float GetZFar();

		/**
		@brief	画角を設定する。
		@param	zfar	画角
		*/
		void SetZFar(float zfar);

		/**
		@brief	描画する最遠距離を取得する。
		@return	描画する最遠距離
		*/
		float GetZNear();

		/**
		@brief	描画する最近距離を設定する。
		@param	znear	描画する最近距離
		*/
		void SetZNear(float znear);

		/**
		@brief	プロジェクション行列を直接取得する。
		@return	プロジェクション行列
		*/
		Matrix44 GetProjectionMatrixDirectly();

		/**
		@brief	プロジェクション行列を直接設定する。
		@param	projectionMatrix	プロジェクション行列
		@note
		この値を直接設定した場合、プロジェクション行列に関わるパラメーターは無効化される。
		*/
		void SetProjectionMatrixDirectly(Matrix44 projectionMatrix);

		/**
		@brief	描画先のフォーマットがHDRか取得する。
		@return	HDRか?
		*/
		bool GetHDRMode() const;

		/**
		@brief	描画先のフォーマットをHDRにするか設定する。
		@param	value	HDRか?
		*/
		void SetHDRMode(bool value);

		/**
		@brief	ポストエフェクトを追加する。
		@param	postEffect	ポストエフェクト
		*/
		void AddPostEffect(const std::shared_ptr<PostEffect>& postEffect);

		/**
		@brief	ポストエフェクトを全て消去する。
		*/
		void ClearPostEffects();

	};
}



namespace asd
{
	/**
	@brief	無限遠光源の機能を提供するクラス
	*/
	class DirectionalLightObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreDirectionalLightObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		DirectionalLightObject3D();
		virtual ~DirectionalLightObject3D();

		/**
			@brief	光源の色を取得する。
			@return	光源の色
		*/
		Color GetColor();

		/**
		@brief	光源の色を設定する。
		@param	color	光源の色
		*/
		void SetColor(const Color& color);

		/**
			@brief	光源の強さを取得する。
			@return	光源の強さ
			@note
			デフォルトは1.0である。
		*/
		float GetIntensity();

		/**
		@brief	光源の強さを設定する。
		@param	intensity	光源の強さ
		@note
		デフォルトは1.0である。
		*/
		void SetIntensity(float intensity);
	};
}



namespace asd
{
	/**
	@brief	エフェクトの機能を提供するクラス
	*/
	class EffectObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreEffectObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		EffectObject3D();
		virtual ~EffectObject3D();

		/**
			@brief	描画に使用するエフェクトを設定する。
			@param	effect	エフェクト
		*/
		void SetEffect(std::shared_ptr<Effect> effect);


		/**
		@brief	設定されている全てのエフェクトを再生する。
		@return	再生されたエフェクトのID
		*/
		int32_t Play();

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		/**
		@brief	このオブジェクトから再生されたエフェクトを表示状態にする。
		*/
		void Show();

		/**
		@brief	このオブジェクトから再生されたエフェクトを非表示状態にする。
		*/
		void Hide();

		/**
		@brief	このオブジェクトから再生されたエフェクトが再生中か取得する。
		@return	再生中か?
		*/
		bool GetIsPlaying();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value);
	};
}



namespace asd
{
	/**
	@brief	メッシュを表示する3Dオブジェクトの機能を提供するクラス
	*/
	class ModelObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreModelObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		ModelObject3D();
		virtual ~ModelObject3D();

		/**
		@brief	描画に使用するモデルを設定する。
		@param	model	モデル
		*/
		void SetModel(std::shared_ptr<Model>& model);

		/**
		@brief	描画に使用するメッシュを追加する。
		@param	meshGroupIndex	メッシュグループのインデックス
		@param	mesh	メッシュ
		*/
		void AddMesh(std::shared_ptr<Mesh> mesh);

		/**
		@brief	描画に使用するデフォーマーを設定する。
		@param	meshGroupIndex	メッシュグループのインデックス
		@param	deformer	デフォーマー
		*/
		void SetDeformer(std::shared_ptr<Deformer> deformer);

		
		/**
		@brief	アニメーションクリップを取得する。
		@param	name	アニメーションクリップの名称
		@return	アニメーションクリップ
		*/
		std::shared_ptr<AnimationClip> GetAnimationClip(const achar* name);

		/**
		@brief	アニメーションを再生する。
		@param	index	対象のインデックス
		@param	name	アニメーションの名称
		*/
		void PlayAnimation(int32_t index, const achar* name);

		/**
		@brief	アニメーションを停止する。
		@param	index	対象のインデックス
		*/
		void StopAnimation(int32_t index);

		/**
		@brief	アニメーションの合成比率を設定する。
		@param	index	対象のインデックス
		@param	weight	合成比率
		*/
		void SetAnimationWeight(int32_t index, float weight);

		/**
		@brief	アニメーションをクロスフェードで再生する。
		@param	index	対象のインデックス
		@param	name	アニメーションの名称
		@param	time	クロスフェードにかかる時間(秒)
		*/
		void CrossFadeAnimation(int32_t index, const achar* name, float time);

		/**
		@brief	アニメーションが再生中か取得する。
		@param	index	対象のインデックス
		@return	再生中か?
		*/
		bool GetIsAnimationPlaying(int32_t index);

		/**
		@brief	材質個別に描画時のパラメーターを設定する。
		@param	meshIndex	メッシュのインデックス
		@param	materialIndex	材質のインデックス
		@param	block	プロパティ
		@note
		通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		*/
		void SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, std::shared_ptr<MaterialPropertyBlock> block);
	};
}



namespace asd
{
	/**
	@brief	大量にメッシュを表示する3Dオブジェクトの機能を提供するクラス
	*/
	class MassModelObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreMassModelObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		MassModelObject3D();
		virtual ~MassModelObject3D();

		/**
		@brief	描画に使用するモデルを設定する。
		@param	model	モデル
		*/
		void SetModel(std::shared_ptr<MassModel>& model);

		/**
		@brief	アニメーションを再生する。
		@param	name	アニメーションの名称
		*/
		void PlayAnimation(const achar* name);

		/**
		@brief	アニメーションを停止する。
		*/
		void StopAnimation();

		/**
		@brief	アニメーションをクロスフェードで再生する。
		@param	name	アニメーションの名称
		@param	time	クロスフェードにかかる時間(秒)
		*/
		void CrossFadeAnimation(const achar* name, float time);

		/**
		@brief	アニメーションが再生中か取得する。
		@return	再生中か?
		*/
		bool GetIsAnimationPlaying();

		/**
		@brief	材質個別に描画時のパラメーターを設定する。
		@param	block	プロパティ
		@note
		通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		*/
		void SetMaterialPropertyBlock(std::shared_ptr<MaterialPropertyBlock> block);
	};
}



namespace asd
{
	/**
	@brief	地形を描画するクラス
	*/
	class TerrainObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreTerrainObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		TerrainObject3D();
		virtual ~TerrainObject3D();

		/**
			@brief	地形を設定する。
			@param	terrain	地形
		*/
		void SetTerrain(std::shared_ptr<Terrain3D> terrain);

		/**
		@brief	材質個別に描画時のパラメーターを設定する。
		@param	block	プロパティ
		@note
		通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		*/
		void SetMaterialPropertyBlock(std::shared_ptr<MaterialPropertyBlock> block);
	};
}



namespace asd
{
	template<typename TObject> class ObjectManager;

	/**
		@brief	3Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス
		*/
	class Layer3D
		: public Layer
		, public IObjectRegisterable<Object3D>
	{
		friend class Scene;
		friend class Object3D;

	public:
		typedef std::shared_ptr<Object3D> ObjectPtr;

	private:

		std::shared_ptr<CoreLayer3D>	m_coreLayer;
		std::shared_ptr<ObjectManager<Object3D>>	m_objects;

		void BeginUpdating();
		void EndUpdateting();

		void DrawAdditionally();
		void UpdateInternal();
		void DisposeInternal();

		void ImmediatelyRemoveObject(const Object3D::Ptr& object);

		bool GetIsAlive() const;
		void Register(const Object3D::Ptr& object);
		void Unregister(const Object3D::Ptr& object);
		void AddToCore(const Object3D::Ptr& object);
		void RemoveFromCore(const Object3D::Ptr& object);

		void RaiseOnAdded() override;
		void RaiseOnRemoved() override;

	protected:

	public:
		/**
			@brief	コンストラクタ
			*/
		Layer3D(RenderSettings settings = RenderSettings());

		/**
			@brief	デストラクタ
			*/
		virtual ~Layer3D();

		/**
			@brief	描画設定を取得する。
			@return	描画設定
			*/
		RenderSettings GetRenderSettings() const;

		/**
			@brief	描画設定を設定する。
			@param	settings	描画設定
			*/
		void SetRenderSettings(RenderSettings settings);

		/**
			@brief	このレイヤーに指定した3Dオブジェクトを追加する。
			@param	object	追加する3Dオブジェクト
			*/
		void AddObject(const Object3D::Ptr& object);

		/**
			@brief	このレイヤーから指定した3Dオブジェクトを削除する。
			@param	object	削除される3Dオブジェクト
			*/
		void RemoveObject(const Object3D::Ptr& object);
		/**
		@brief	このレイヤーに登録されている3Dオブジェクトのリストを取得する。
		@return	登録されているオブジェクトのリスト
		*/
		std::list<Object3D::Ptr> GetObjects() const;

		/**
		@brief	通常の描画に加えてテクスチャを描画する。
		@param	upperLeftPos	テクスチャの左上の描画位置
		@param	upperRightPos	テクスチャの右上の描画位置
		@param	lowerRightPos	テクスチャの右下の描画位置
		@param	lowerLeftPos	テクスチャの左下の描画位置
		@param	upperLeftCol	テクスチャの左上の頂点色
		@param	upperRightCol	テクスチャの右上の頂点色
		@param	lowerRightCol	テクスチャの右下の頂点色
		@param	lowerLeftCol	テクスチャの左下の頂点色
		@param	upperLeftUV		テクスチャの左上のUV値
		@param	upperRightUV	テクスチャの右上のUV値
		@param	lowerRightUV	テクスチャの右下のUV値
		@param	lowerLeftUV		テクスチャの左下のUV値
		@param	texture			描画するテクスチャ
		@param	alphaBlend		アルファブレンドの種類
		@param	depthWrite		深度を書き込むか?
		@param	depthTest		深度を比較するか?
		@note
		OnDrawAdditionallyの中以外では実行してはいけない。
		*/
		void DrawSpriteAdditionally(Vector3DF upperLeftPos, Vector3DF upperRightPos, Vector3DF lowerRightPos, Vector3DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			std::shared_ptr<Texture2D>  texture, AlphaBlendMode alphaBlend, bool depthWrite, bool depthTest);

		/**
		@brief	環境光の強さを取得する。
		@return	環境光の強さ
		@note
		デフォルトは1.0である。
		*/
		float GetAmbientColorIntensity();

		/**
		@brief	環境光の強さを設定する。
		@param	ambientColorIntensity	環境光の強さ
		@note
		デフォルトは1.0である。
		*/
		void SetAmbientColorIntensity(float ambientColorIntensity);

		/**
			@brief	空方向の環境光の色を設定する。
			@param	color	色
			*/
		void SetSkyAmbientColor(Color color);

		/**
		@brief	地面方向の環境光の色を設定する。
		@param	color	色
		*/
		void SetGroundAmbientColor(Color color);

		/**
		@brief	テクスチャによる環境の色の強さを設定する。
		@param	environmentDiffuseColorIntensity	拡散色の強さ
		@param	environmentSpecularColorIntensity	スペキュラ色の強さ
		@note
		デフォルトは1.0である。
		*/
		void SetEnvironmentColorIntensity(float environmentDiffuseColorIntensity, float environmentSpecularColorIntensity);

		/**
			@brief	テクスチャによる環境の色を設定する。
			@param	diffuseColor	拡散色
			@param	specularColor	スペキュラ色
			*/
		void SetEnvironmentColor(std::shared_ptr<CubemapTexture> diffuseColor, std::shared_ptr<CubemapTexture> specularColor);

		/**
			@brief	SSAOのサンプリングする半径を取得する。
			@return	SSAOのサンプリングする半径
			*/
		float GetSSAO_Radius();

		/**
		@brief	SSAOのサンプリングする半径を設定すする。
		@param	value	SSAOのサンプリングする半径
		*/
		void SetSSAO_Radius(float value);

		/**
		@brief	SSAOのバイアスを取得する。
		@return	SSAOのバイアス
		*/
		float GetSSAO_Bias();

		/**
		@brief	SSAOのバイアスを設定すする。
		@param	value	SSAOのバイアス
		*/
		void SetSSAO_Bias(float value);

		/**
		@brief	SSAOの強さを取得する。
		@return	SSAOの強さ
		*/
		float GetSSAO_Intensity();

		/**
		@brief	SSAOの強さを設定すする。
		@param	value	SSAOの強さ
		*/
		void SetSSAO_Intensity(float value);

		/**
		@brief	SSAOの最大距離を取得する。
		@return	SSAOの最大距離
		*/
		float GetSSAO_FarPlain();

		/**
		@brief	SSAOの最大距離を設定すする。
		@param	value	SSAOの最大距離
		*/
		void SetSSAO_FarPlain(float value);

		/**
		@brief	高さ方向のフォグが有効かどうか、を取得する。
		*/
		bool GetIsHeightFogEnabled();
		
		/**
		@brief	高さ方向のフォグが有効かどうか、を設定する。
		*/
		void SetIsHeightFogEnabled(bool value);

		/**
		@brief	高さ方向のフォグの濃度を取得する。
		*/
		float GetHeightFogDensity();
		
		/**
		@brief	高さ方向のフォグの濃度を設定する。
		*/
		void SetHeightFogDensity(float value);

		/**
		@brief	高さ方向のフォグの色を取得する。
		*/
		Color GetHeightFogColor();

		/**
		@brief	高さ方向のフォグの色を設定する。
		*/
		void SetHeightFogColor(Color value);

		/**
		@brief	高さ方向のフォグの減衰率を取得する。
		@note
		この値が大きいほど、高い位置のフォグが薄くなる。
		*/
		float GetHeightFogFalloff();
		
		/**
		@brief	高さ方向のフォグの減衰率を設定する。
		*/
		void SetHeightFogFalloff(float value);

		/**
		@brief	高さ方向のフォグの開始距離を取得する。
		@note
		この値よりカメラに対して近い位置には霧が生じなくなる。
		*/
		float GetHeightFogStartDistance();

		/**
		@brief	高さ方向のフォグの開始距離を設定する。
		*/
		void SetHeightFogStartDistance(float value);

		bool GetIsOceanEnabled();
		void SetIsOceanEnabled(bool value);
		RectF GetOceanArea();
		void SetOceanArea(RectF value);
		float GetOceanHeight();
		void SetOceanHeight(float value);
		float GetOceanGridSize();
		void SetOceanGridSize(float value);
		Color GetOceanColor();
		void SetOceanColor(Color color);
		float GetOceanDensity();
		void SetOceanDensity(float value);
		Texture2D* GetOceanNormalMap();
		void SetOceanNormalMap(std::shared_ptr<Texture2D> value);

		/**
		@brief	レイヤーの種類を取得する。
		@return	レイヤーの種類
		*/
		LayerType GetLayerType() const override { return LayerType::Layer3D; }

		int GetObjectCount() const;
	};
}




namespace asd
{
	class Engine;
	class EventToChangeScene;
	class EventToManageLayer;
	class Layer;
	template<typename TComponent> class ComponentManager;

	/**
		@brief	画面を表し、レイヤーを管理するクラス。
	*/
	class Scene
		: public std::enable_shared_from_this<Scene>
		, public IComponentRegisterable<SceneComponent>
		, public IImmediatelyDisposable
	{
		friend class Engine;
		friend class EventToChangeScene;
		friend class EventToManageLayer;
		friend class Layer;
		friend class SceneComponent;

	public:
		typedef std::shared_ptr<Scene> Ptr;

	private:
		std::shared_ptr<CoreScene> m_coreScene;
		std::vector<Layer::Ptr> m_layersToDraw;
		std::vector<Layer::Ptr> m_layersToUpdate;
		std::shared_ptr<ComponentManager<SceneComponent>> m_componentManager;
		bool alreadyFirstUpdate;
		bool m_isAlive;

		void RaiseOnRegistered();
		void RaiseOnStartUpdating();
		void RaiseOnTransitionFinished();
		void RaiseOnTransitionBegin();
		void RaiseOnStopUpdating();
		void RaiseOnUnregistered();

		void Update();
		void Draw();
		void CommitChanges();
		void UpdateComponents();

		void ImmediatelyAddLayer(const Layer::Ptr& layer, bool raiseEvent);
		void ImmediatelyRemoveLayer(const Layer::Ptr& layer, bool raiseEvent);
		void ImmediatelyRemoveComponent(astring key);
		void DisposeImmediately();
		void Register(const SceneComponent::Ptr& component);
		void Unregister(const SceneComponent::Ptr& component);

	protected:
		/**
		@brief	オーバーライドして、このシーンがエンジンに登録されたときに実行する処理を記述できる。
		*/
		virtual void OnRegistered();

		/**
		@brief	オーバーライドして、最初のシーン更新時に実行する処理を記述する。
		*/
		virtual void OnStartUpdating();

		/**
		@brief	オーバーライドして、トランジション終了時に実行する処理を記述する。
		*/
		virtual void OnTransitionFinished();

		/**
		@brief	オーバーライドして、このシーンから別のシーンに切り替わる際に実行される処理を記述する。
		*/
		virtual void OnTransitionBegin();

		/**
			@brief	オーバーライドして、このシーンの更新が止まるときに実行する処理を記述できる。
		*/
		virtual void OnStopUpdating();

		/**
			@brief	オーバーライドして、このシーンがエンジンから登録解除されたときに実行する処理を記述できる。
		*/
		virtual void OnUnregistered();

		/**
		@brief	オーバーライドして、このシーンが破棄される際に実行される処理を記述する。
		*/
		virtual void OnDispose();

		/**
			@brief	オーバーライドして、このシーンを更新する直前の処理を記述できる。
		*/
		virtual void OnUpdating();
		/**
			@brief	オーバーライドして、このシーンを更新した直後の処理を記述できる。
		*/
		virtual void OnUpdated();

	public:
		/**
			@brief	コンストラクタ
		*/
		Scene();
		/**
			@brief	デストラクタ
		*/
		virtual ~Scene();

		/**
			@brief	描画先のフォーマットがHDRか取得する。
			@return	HDRか?
		*/
		bool GetHDRMode() const;

		/**
			@brief	描画先のフォーマットをHDRにするか設定する。
			@param	value	HDRか?
		*/
		void SetHDRMode(bool value);

		/**
			@brief	指定したレイヤーをこのインスタンスに追加する。
			@param	layer	追加するレイヤー
		*/
		void AddLayer(const Layer::Ptr& layer);
		/**
			@brief	指定したレイヤーをこのインスタンスから削除する。
			@param	layer	削除するレイヤー
		*/
		void RemoveLayer(const Layer::Ptr& layer);

		/**
			@brief	指定したコンポーネントをこのインスタンスに追加する。
			@param	component	追加するコンポーネント
			@param	key			コンポーネントに関連付けるキー
		*/
		void AddComponent(const SceneComponent::Ptr& component, astring key);
		/**
			@brief	キーの示すコンポーネントをこのインスタンスから取得する。
			@param	key		取得するコンポーネントを示すキー
		*/
		SceneComponent::Ptr GetComponent(astring key);
		/**
			@brief	キーの示すコンポーネントをこのインスタンスから削除する。
			@param	key		削除するコンポーネントを示すキー
		*/
		bool RemoveComponent(astring key);

		/**
			@brief	全てのレイヤーとポストエフェクトが描画され終わった画面をテクスチャとして取得する。
			@return	画面
			@note
			テクスチャの内容はシーンが描画されるたびに変わる。
			主にシーン遷移の際に使用する。
		*/
		std::shared_ptr<RenderTexture2D> GetEffectedScreen();

		/**
			@brief	所属しているレイヤーを取得する。
			@return	所属しているレイヤー
		*/
		const std::vector<Layer::Ptr>& GetLayers() const;

		bool GetIsAlive() const;

		void Dispose();
	};
}



//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <queue>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	// 先行宣言

	class Scene;
	class Object2D;
	class Object3D;

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
	@brief	初期化時に設定するオプションのクラス
	*/
	struct EngineOption
	{
		/**
		@brief	フルスクリーンで起動するか?
		*/
		bool			IsFullScreen = false;

		/**
		@brief	描画に使用するデバイス
		*/
		GraphicsDeviceType	GraphicsDevice = GraphicsDeviceType::Default;

		/**
		@brief	ウインドウの初期配置
		*/
		WindowPositionType	WindowPosition = WindowPositionType::Default;

		/**
		@brief	リソースの再読み込みを有効にするかどうか?
		*/
		bool IsReloadingEnabled = false;

		/**
		@brief	起動時に自動的に生成されるシーンとレイヤーの種類
		*/
		AutoGeneratedLayerType	AutoGeneratedLayer = AutoGeneratedLayerType::Layer2D;

		/**
		@brief	カラースペースの種類
		*/
		ColorSpaceType ColorSpace = ColorSpaceType::LinearSpace;
	};

	class Engine
	{
		friend class EventToChangeScene;
		friend class Scene;
		friend class Layer;
		friend class DrawnObject2D;
		friend class Object2D;
		friend class Object3D;
		friend class Object2DComponent;
		friend class Layer2DComponent;
		friend class SceneComponent;
		template<typename TObject> friend class ObjectManager;
		template<typename TComponent> friend class ComponentManager;

	private:
		class SceneTransitionState
		{
		public:
			virtual std::shared_ptr<SceneTransitionState> Proceed()
			{
				return nullptr;
			}
			virtual void Draw() = 0;
			virtual void Update()
			{
			}
			virtual void ForceToComplete()
			{
			}
		};

		class NeutralState : public SceneTransitionState
		{
		public:
			void Draw() override;
			std::shared_ptr<SceneTransitionState> Proceed() override;
		};

		class FadingOutState : public SceneTransitionState
		{
		private:
			std::shared_ptr<Transition> m_transition;
			bool m_doAutoDispose;

		public:
			FadingOutState(std::shared_ptr<Transition> transition, std::shared_ptr<Scene> nextScene, bool doAutoDispose);
			std::shared_ptr<SceneTransitionState> Proceed() override;
			void Draw() override;
			void Update() override;
			// ForceToCompleteを実装せず、フェードアウト中にさらに画面遷移しようとしたときの動作を保証しない
		};

		class FadingInState : public SceneTransitionState
		{
		private:
			std::shared_ptr<Transition> m_transition;
			std::shared_ptr<Scene> m_previousScene;
			bool m_doAutoDispose;

		public:
			FadingInState(std::shared_ptr<Transition> transition, std::shared_ptr<Scene> previousScene, bool doAutoDispose);
			std::shared_ptr<SceneTransitionState> Proceed() override;
			void Draw() override;
			void Update() override;
			void ForceToComplete() override;
		};

		class QuicklyChangingState : public SceneTransitionState
		{
		private:
			bool m_doAutoDispose;

		public:
			QuicklyChangingState(std::shared_ptr<Scene> nextScene, bool doAutoDispose);
			std::shared_ptr<SceneTransitionState> Proceed() override;
			void Draw() override;
			void ForceToComplete() override;
		};

	private:
		typedef std::shared_ptr<Scene> ScenePtr;

		static Core*					m_core;

		static Keyboard* m_keyboard ;
		static Mouse* m_mouse ;
		static JoystickContainer* m_joystickContainer ;
		static Log* m_logger ;
		static Profiler* m_profiler ;
		static LayerProfiler* m_layerProfiler;
		static Sound*	m_sound;
		static File*	m_file;
		static Graphics* m_graphics ;
		static ObjectSystemFactory* m_objectSystemFactory ;
		static AnimationSystem* m_animationSyatem ;
		static Tool*	tool;

		static std::shared_ptr<Scene>	m_currentScene;
		static std::shared_ptr<Scene>	m_nextScene;
		static std::shared_ptr<SceneTransitionState> m_transitionState;
		static std::queue<std::shared_ptr<ICommitable>> m_changesToCommit;

		static bool HasDLL(const char* path);
		static bool CheckDLL();
		static bool GenerateCore();

		static void CommitChange();

		Engine();
		~Engine();

	public:
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool Initialize(const achar* title, int32_t width, int32_t height, EngineOption option);

		/**
			@brief	初期化を行う。
			@param	handle1	ハンドル
			@param	handle2	ハンドル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool InitializeByExternalWindow(void* handle1, void* handle2, int32_t width, int32_t height, EngineOption option);

		/**
		@brief	タイトルを設定する。
		@param	title	タイトル
		*/
		static void SetTitle(const achar* title);

		/**
			@brief	イベントを実行し、進行可否を判断する。
			@return	進行可能か?
		*/
		static bool DoEvents();

		/**
			@brief	更新処理を行う。
		*/
		static void Update();

		/**
			@brief	終了処理を行う。
		*/
		static void Terminate();

		/**
		@brief	マウスカーソルを作成する。
		@param	path	画像のパス
		@param	hot		カーソルの相対座標
		@return	カーソル
		*/
		static std::shared_ptr<Cursor> CreateCursor(const achar* path, Vector2DI hot);

		/**
		@brief	マウスカーソルを設定する。
		@param	cursor	カーソル
		*/
		static void SetCursor(std::shared_ptr<Cursor> cursor);

		/**
		@brief	クリップボードの内容を取得する。
		@return	クリップボード
		*/
		static const achar* GetClipboardString();

		/**
		@brief	クリップボードの内容を設定する。
		@param	s	文字列
		*/
		static void SetClipboardString(const achar* s);

		/**
		@brief	フルスクリーンモードかどうか設定する。
		@param	isFullscreenMode	フルスクリーンモードか
		@note
		現状、DirectXのみ有効である。
		*/
		static void SetIsFullscreenMode(bool isFullscreenMode);

		/**
			@brief	内部の参照数を取得する。
			@note
			内部オブジェクト間の参照の数を取得する。
			正しく内部オブジェクトが破棄されているか調査するためのデバッグ用である。
		*/
		static int32_t GetReferenceCount();

		/**
		@brief	一番最初に追加された2Dレイヤーにオブジェクトを追加する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool AddObject2D(std::shared_ptr<Object2D> o);

		/**
		@brief	一番最初に追加された2Dレイヤーからオブジェクトを削除する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool RemoveObject2D(std::shared_ptr<Object2D> o);

		/**
		@brief	一番最初に追加された3Dレイヤーにオブジェクトを追加する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool AddObject3D(std::shared_ptr<Object3D> o);

		/**
		@brief	一番最初に追加された3Dレイヤーからオブジェクトを削除する。
		@param	o	オブジェクト
		@return	成否
		*/
		static bool RemoveObject3D(std::shared_ptr<Object3D> o);

		/**
			@brief	描画する対象となるシーンを変更する。
			@param	scene	次のシーン
		*/
		static void ChangeScene(std::shared_ptr<Scene> scene, bool doAutoDispose = true);

		/**
		@brief	描画する対象となるシーンを画面遷移効果ありで変更する。
		@param	scene	次のシーン
		@param	transition	画面遷移効果
		*/
		static void ChangeSceneWithTransition(
			std::shared_ptr<Scene> scene,
			const std::shared_ptr<Transition>& transition,
			bool doAutoDispose = true);

		/**
		@brief	スクリーンショットを保存する。
		@param	path	出力先
		@note
		Windowsの場合、pngとjpg形式の保存に対応している。他のOSではpng形式の保存に対応している。
		形式の種類は出力先の拡張子で判断する。
		*/
		static void TakeScreenshot(const achar* path);

		/**
		@brief	スクリーンショットをgifアニメーションとして録画する。
		@param	path	出力先
		@param	frame	録画フレーム数
		@param	frequency_rate	録画頻度(例えば、1だと1フレームに1回保存、0.5だと2フレームに1回保存)
		@param	scale	ウインドウサイズに対する画像サイズの拡大率(ウインドウサイズが320の場合、0.5を指定すると160の画像が出力される)
		@note
		実行してから一定時間の間、録画を続ける。
		録画が終了するまでにアプリケーションが終了された場合、終了した時点までの録画結果が出力される。
		*/
		static void CaptureScreenAsGifAnimation(const achar* path, int32_t frame, float frequency_rate, float scale);

		/**
		@brief	1フレームで経過した実時間(秒)を取得する。
		@return	経過時間(秒)
		*/
		static float GetDeltaTime();

		/**
		@brief	1フレームで経過した時間を外部から設定する。
		@param	deltaTime	経過時間(秒)
		@note
		基本的に開発者は使用する必要はない。
		何らかの理由で無理やり経過時間を指定する場合に使用する。
		*/
		static void SetDeltaTime(float deltaTime);

		static std::shared_ptr<Scene> GetCurrentScene();

		/**
		@brief	現在のFPSを取得する。
		@return FPS
		*/
		static float GetCurrentFPS();

		/**
		@brief	目標FPSを取得する。
		@return	FPS
		*/
		static int32_t GetTargetFPS();

		/**
		@brief	目標FPSを設定する。
		@param	fps	FPS
		*/
		static void SetTargetFPS(int32_t fps);

		/**
		@brief	時間を指定可能なオブジェクトの実時間あたりの進行速度を取得する。
		@return	進行速度
		*/
		static float GetTimeSpan();

		/**
		@brief	時間を指定可能なオブジェクトの実時間あたりの進行速度を設定する。
		@param	timeSpan	進行速度
		*/
		static void SetTimeSpan(float timeSpan);

		/**
		@brief	フレームレートの制御方法を取得する。
		@return	制御方法
		*/
		static FramerateMode GetFramerateMode();

		/**
		@brief	フレームレートの制御方法を設定する。
		@param	framerateMode	制御方法
		*/
		static void SetFramerateMode(FramerateMode framerateMode);

		/**
			@brief	ウインドウを閉じる。
		*/
		static void Close();

		/**
		@brief	Windowsの場合、ウインドウのハンドルを取得する。
		@return	ウインドウハンドル
		*/
		static void* GetWindowHandle();

		/**
		@brief キーボードクラスを取得する。
		@return キーボード
		*/
		static Keyboard* GetKeyboard();

		/**
		@brief マウスクラスを取得する。
		@return マウス
		*/
		static Mouse* GetMouse();

		/**
		@brief	ログクラスを取得する。
		@return	ログクラス
		*/
		static Log* GetLogger();

		/**
		@brief	プロファイラクラスを取得する。
		@return	プロファイラクラス
		*/
		static Profiler* GetProfiler();

		/**
		@brief	ジョイスティックコンテナクラスを取得する。
		@return	ジョイスティックコンテナクラス
		*/
		static JoystickContainer* GetJoystickContainer();
		
		/**
		@brief	ファイルクラスを取得する。
		@return	ファイルクラス
		*/
		static File* GetFile();

		/**
		@brief	Graphicsクラスを取得する。
		*/
		static Graphics* GetGraphics();

		/**
		@brief	Soundクラスを取得する。
		*/
		static Sound* GetSound();

		/**
		@brief	AnimationSyatemクラスを取得する。
		preturn	AnimationSyatemクラス
		*/
		static AnimationSystem* GetAnimationSyatem();

		/**
		@brief	ウィンドウのサイズを取得する。
		@return	ウィンドウのサイズ
		*/
		static Vector2DI GetWindowSize();

		/**
		@brief	ウインドウのサイズを設定する。
		@param	size	ウインドウのサイズ
		*/
		static void SetWindowSize(Vector2DI size);

		static bool GetProfilerVisibility();

		static void SetProfilerVisibility(bool visibility);

		/**
		@brief	ツールを開く。
		*/
		static void OpenTool();

		/**
		@brief	ツールを閉じる。
		*/
		static void CloseTool();

		/**
		@brief	Toolクラスを取得する。
		*/
		static Tool* GetTool();

		/**
		@brief	更新されたリソースのリロードを行う。
		*/
		static void Reload();

#if _WIN32

#else
		/**
			@brief	初期化を行う。
			@param	title	タイトル
			@param	width	横幅
			@param	height	縦幅
			@param	option	オプション
			@return	成否
		*/
		static bool Initialize(const wchar_t* title, int32_t width, int32_t height, EngineOption option)
		{
			return Initialize( ToAString(title).c_str(), width, height, option );
		}
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}

