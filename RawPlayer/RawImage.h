


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 预处理
#pragma once
#include <Windows.h>
#include "../CSConv/CSConv.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define MAX_Frame				0x7FFFFFFF
#define MAX_FrameRate			100
#define MIN_Resolution			8
#define MAX_Resolution			2048
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 色度空间
typedef enum tagCOLORSPACE
{
	CS_YV12,
	CS_I420,
	CS_YUY2,
	CS_YVYU,
	CS_UYVY,
	CS_VYUY,
	CS_RGB32,
	CS_RGB24,
	CS_RGB16,
	CS_RGB15,
	CS_ALL,
	CS_BMP,
	CS_MAX,
}
COLORSPACE, *PCOLORSPACE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 色度空间字符串
const TCHAR c_tzColorSpace[][8] =
{
	TEXT("YV12"),
	TEXT("I420"),
	TEXT("YUY2"),
	TEXT("YVYU"),
	TEXT("UYVY"),
	TEXT("VYUY"),
	TEXT("RGB32"),
	TEXT("RGB24"),
	TEXT("RGB16"),
	TEXT("RGB15")
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RAWIMAGE 类
typedef struct tacRAWIMAGE
{
	COLORSPACE m_csColorSpace;

	UINT m_uWidth;
	INT m_iHeight;

	UINT m_uXStride;
	UINT m_uUvStride;

	INT m_iFrameStep;
	UINT m_uFrameRate;
}
RAWIMAGE, *PRAWIMAGE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRawImage 类
class CRawImage: public RAWIMAGE
{
public:
	static DWORD m_dwCpuFeature;				// CPU 特性
	static const PVOID (*m_pvCSConv)[CS_ALL];	// 转换函数

public:
	UINT m_uHeight;								// 高度
	UINT m_uFrameSize;							// 帧大小

public:
	// 初始化
	static DWORD Initialize();

	// 获取 CPU 特性
	static DWORD GetCpuFeature(PTSTR ptzText = NULL);

public:
	// 构造
	inline CRawImage() {Reset();}

	// 构造
	inline CRawImage(CRawImage &riOther, COLORSPACE csColorSpace) {*this = riOther; Update(csColorSpace);}

	// 重置
	inline VOID Reset() {memset(this, 0, sizeof(RAWIMAGE));}

	// 赋值
	inline VOID operator=(RAWIMAGE &riOther) {memcpy(this, &riOther, sizeof(RAWIMAGE));}

	// 比较
	inline VOID operator==(CRawImage &riOther) {memcmp(this, &riOther, sizeof(CRawImage));}

public:
	// 更新
	VOID Update();

	// 转换
	PBYTE Convert(CRawImage &riDst, PBYTE pbDst, PBYTE pbSrc);

private:
	// 获取跨度
	VOID GetStride();

public:
	// 获取 UV 分量
	VOID GetUV(PBYTE pbX, PBYTE *ppbU, PBYTE *ppbV);

public:
	// 更新
	inline VOID Update(COLORSPACE csColorSpace) {m_csColorSpace = csColorSpace; GetStride();}

	// 转换
	inline PBYTE Convert(COLORSPACE csDst, PBYTE pbDst, PBYTE pbSrc) {CRawImage riDst(*this, csDst); return Convert(riDst, pbDst, pbSrc);}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
