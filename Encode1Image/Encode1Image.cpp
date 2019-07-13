// Encode1Image.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../Common/H264Writer.h"

/*
int main()
{
	std::vector<std::wstring> encoders;
	if (H264Writer::EnumVideoEncoder(encoders, Processing::Software, VideoCodec::H264))
	{
		for (size_t i = 0; i < encoders.size(); ++i)
		{
			printf("Video Encoder: %S\n", encoders[i].c_str());
		}
	}
	else
	{
		printf("H264Writer::EnumVideoEncoder failed!\n");
	}
	return 0;
}
*/

/*
class RenderRedImage : public FrameRenderer
{
public:

	// render a red image once!
	bool Render(int width, int height, int fps, int frame_cnt, UINT32* pixels) override
	{
		if (frame_cnt == 0)
		{
			for (int col = 0; col < width; ++col)
			{
				for (int row = 0; row < height; ++row)
				{
					int index = row * width + col;
					pixels[index] = 0xffff0000;
				}
			}
		}
		return true;
	}
};

int main()
{
	std::wstring musicFile(L"");
	std::wstring videoFile(L"C:\\temp\\RedVideo.mp4");

	RenderRedImage frameRenderer;

	H264Writer writer(musicFile.c_str(), videoFile.c_str(), VideoCodec::H264, 640, 480, 30, 5000, &frameRenderer);
	if (writer.IsValid())
	{
		if (writer.Process())
		{
			printf("Video written successfully!\n");
			return 0;
		}
	}
	printf("Video write failed!\n");

}
*/

/*
#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class RenderJPG : public FrameRenderer
{
public:
	RenderJPG()
	{
		// Initialize GDI+ so that we can load the JPG
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	}
	~RenderJPG()
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

	// render a jpg once!
	bool Render(int width, int height, int fps, int frame_cnt, UINT32* pixels) override
	{
		using namespace Gdiplus;

		if (frame_cnt == 0)
		{
			Bitmap bmp(width, height, PixelFormat32bppARGB);
			Bitmap jpg(L"image\\yes.jpg", TRUE);
			Graphics g(&bmp);
			float w_ratio_bmp = bmp.GetWidth() / (float)bmp.GetHeight();
			float w_ratio_jpg = jpg.GetWidth() / (float)jpg.GetHeight();

			SolidBrush brush(Color::Black);
			g.FillRectangle(&brush, 0, 0, bmp.GetWidth(), bmp.GetHeight());

			if (w_ratio_jpg >= w_ratio_bmp)
			{
				int width2 = bmp.GetWidth();
				int height2 = (int)((bmp.GetWidth() / (float)jpg.GetWidth()) * jpg.GetHeight());
				g.DrawImage(&jpg, 0, (bmp.GetHeight() - height2) / 2, width2, height2);
			}
			else
			{
				int width2 = (int)((bmp.GetHeight() / (float)jpg.GetHeight()) * jpg.GetWidth());
				int height2 = bmp.GetHeight();
				g.DrawImage(&jpg, (bmp.GetWidth() - width2) / 2, 0, width2, height2);
			}

			BitmapData bitmapData;
			Rect rect(0, 0, bmp.GetWidth(), bmp.GetHeight());

			bmp.LockBits(
				&rect,
				ImageLockModeRead,
				PixelFormat32bppARGB,
				&bitmapData);

			UINT* pixelsSrc = (UINT*)bitmapData.Scan0;

			if (!pixelsSrc)
				return false;

			int stride = bitmapData.Stride >> 2;

			for (int col = 0; col < width; ++col)
			{
				for (int row = 0; row < height; ++row)
				{
					int indexSrc = (height - 1 - row) * stride + col;
					int index = row * width + col;
					pixels[index] = pixelsSrc[indexSrc];
				}
			}

			bmp.UnlockBits(&bitmapData);

		}
		return true;
	}

private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
};

int main()
{
	std::wstring musicFile(L"");
	std::wstring videoFile(L"C:\\temp\\JpgVideo.mp4");

	RenderJPG frameRenderer;
	H264Writer writer(musicFile.c_str(), videoFile.c_str(), VideoCodec::H264, 640, 480, 30, 10000, &frameRenderer);
	if (writer.IsValid())
	{
		if (writer.Process())
		{
			printf("Video written successfully!\n");
			return 0;
		}
	}
	printf("Video write failed!\n");
}
*/

/*
#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class Render2JPG : public FrameRenderer
{
public:
	Render2JPG(const std::wstring& img1, const std::wstring& img2)
	: jpg1(nullptr), jpg2(nullptr), g(nullptr), g2(nullptr), bmp(nullptr), bmp2(nullptr)
	{
		// Initialize GDI+ so that we can load the JPG
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
		jpg1 = new Gdiplus::Bitmap(img1.c_str(), TRUE);
		jpg2 = new Gdiplus::Bitmap(img2.c_str(), TRUE);
	}
	~Render2JPG()
	{
		delete jpg1;
		delete jpg2;
		delete bmp;
		delete bmp2;
		delete g;
		delete g2;
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

	// render 2 jpg
	// This function takes a long time.
	bool Render(int width, int height, int fps, int frame_cnt, UINT32* pixels) override
	{
		using namespace Gdiplus;

		if (bmp == nullptr)
		{
			bmp = new Bitmap(width, height, PixelFormat32bppARGB);
			bmp2 = new Bitmap(width, height, PixelFormat32bppARGB);
			g = new Graphics(bmp);
			g2 = new Graphics(bmp2);
		}
		BYTE alpha = 0;
		float frame_duration = 1000.0 / fps;
		float time = frame_cnt * frame_duration;
		if (time <= 1000.0)
			alpha = 0;
		else if (time >= 2000.0)
			alpha = 255;
		else
			alpha = (BYTE)(int)(((time)-1000.0) / 1000.0 * 255);

		float w_ratio_bmp = bmp->GetWidth() / (float)bmp->GetHeight();
		float w_ratio_jpg = jpg1->GetWidth() / (float)jpg1->GetHeight();

		SolidBrush brush(Color::Black);
		g->FillRectangle(&brush, 0, 0, bmp->GetWidth(), bmp->GetHeight());

		if (w_ratio_jpg >= w_ratio_bmp)
		{
			int width2 = bmp->GetWidth();
			int height2 = (int)((bmp->GetWidth() / (float)jpg1->GetWidth()) * jpg1->GetHeight());
			g->DrawImage(jpg1, 0, (bmp->GetHeight() - height2) / 2, width2, height2);
			g2->DrawImage(jpg2, 0, (bmp2->GetHeight() - height2) / 2, width2, height2);
		}
		else
		{
			int width2 = (int)((bmp->GetHeight() / (float)jpg1->GetHeight()) * jpg1->GetWidth());
			int height2 = bmp->GetHeight();
			g->DrawImage(jpg1, (bmp->GetWidth() - width2) / 2, 0, width2, height2);
			g2->DrawImage(jpg2, (bmp2->GetWidth() - width2) / 2, 0, width2, height2);
		}

		BitmapData bitmapData;
		BitmapData bitmapData2;
		Rect rect(0, 0, bmp->GetWidth(), bmp->GetHeight());

		bmp->LockBits(
			&rect,
			ImageLockModeRead,
			PixelFormat32bppARGB,
			&bitmapData);

		bmp2->LockBits(
			&rect,
			ImageLockModeRead,
			PixelFormat32bppARGB,
			&bitmapData2);

		UINT* pixelsSrc = (UINT*)bitmapData.Scan0;
		UINT* pixelsSrc2 = (UINT*)bitmapData2.Scan0;

		if (!pixelsSrc || !pixelsSrc2)
			return false;

		int stride = bitmapData.Stride >> 2;

		for (int col = 0; col < width; ++col)
		{
			for (int row = 0; row < height; ++row)
			{
				int indexSrc = (height - 1 - row) * stride + col;
				int index = row * width + col;
				pixels[index] = Alphablend(pixelsSrc2[indexSrc], pixelsSrc[indexSrc], alpha, 0xff);
			}
		}

		bmp->UnlockBits(&bitmapData);
		bmp2->UnlockBits(&bitmapData2);

		return true;
	}

private:
	inline UINT Alphablend(UINT dest, UINT source, BYTE nAlpha, BYTE nAlphaFinal)
	{
		BYTE nInvAlpha = ~nAlpha;

		BYTE nSrcRed = (source & 0xff0000) >> 16;
		BYTE nSrcGreen = (source & 0xff00) >> 8;
		BYTE nSrcBlue = (source & 0xff);

		BYTE nDestRed = (dest & 0xff0000) >> 16;
		BYTE nDestGreen = (dest & 0xff00) >> 8;
		BYTE nDestBlue = (dest & 0xff);

		BYTE nRed = (nSrcRed * nAlpha + nDestRed * nInvAlpha) >> 8;
		BYTE nGreen = (nSrcGreen * nAlpha + nDestGreen * nInvAlpha) >> 8;
		BYTE nBlue = (nSrcBlue * nAlpha + nDestBlue * nInvAlpha) >> 8;

		return nAlphaFinal << 24 | nRed << 16 | nGreen << 8 | nBlue;
	}
	private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
	Gdiplus::Bitmap* jpg1;
	Gdiplus::Bitmap* jpg2;
	Gdiplus::Graphics* g;
	Gdiplus::Graphics* g2;
	Gdiplus::Bitmap* bmp;
	Gdiplus::Bitmap* bmp2;

};

int main()
{
	std::wstring musicFile(L"");
	std::wstring videoFile(L"C:\\temp\\TwoJpgVideo.mp4");

	// Warning JPG1 and JPG2 must have the same dimensions
	Render2JPG frameRenderer(L"image\\first.jpg", L"image\\second.jpg");
	H264Writer writer(musicFile.c_str(), videoFile.c_str(), VideoCodec::H264, 640, 480, 30, 3000, &frameRenderer);
	if (writer.IsValid())
	{
		if (writer.Process())
		{
			printf("Video written successfully!\n");
			return 0;
		}
	}
	printf("Video write failed!\n");
}
*/


#include <Gdiplus.h>
#pragma comment(lib, "gdiplus.lib")


class RenderText : public FrameRenderer
{
public:
	RenderText(const std::wstring& img1, const std::wstring& img2) 
		: jpg1(nullptr), jpg2(nullptr), g(nullptr), g2(nullptr), bmp(nullptr), bmp2(nullptr)
	{
		// Initialize GDI+ so that we can load the JPG
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
		jpg1 = new Gdiplus::Bitmap(img1.c_str(), TRUE);
		jpg2 = new Gdiplus::Bitmap(img2.c_str(), TRUE);
	}
	~RenderText()
	{
		delete jpg1;
		delete jpg2;
		delete bmp;
		delete bmp2;
		delete g;
		delete g2;
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}


	// render text
	bool Render(int width, int height, int fps, int frame_cnt, UINT32* pixels) override
	{
		using namespace Gdiplus;

		if (bmp == nullptr)
		{
			bmp = new Bitmap(width, height, PixelFormat32bppARGB);
			bmp2 = new Bitmap(width, height, PixelFormat32bppARGB);
			g = new Graphics(bmp);
			g2 = new Graphics(bmp2);
		}

		Bitmap renderbmp(width, height, PixelFormat32bppARGB);
		Graphics render_g(&renderbmp);

		float rectProgress = 0.0f;
		float textProgress = 0.0f;
		float frame_duration = 1000.0f / fps;
		float time = frame_cnt * frame_duration;

		SolidBrush brush(Color::Black);
		render_g.FillRectangle(&brush, 0, 0, width, height);
		g->FillRectangle(&brush, 0, 0, width, height);

		int rectHeight = 4;

		int rectWidth = (int)(width * 0.8f);
		if (time >= 1000.0f)
			rectProgress = 1.0f;
		else
			rectProgress = time / 1000.0f;


		if (time >= 2000.0f)
			textProgress = 1.0f;
		else if (time <= 1000.0f)
			textProgress = 0.0f;
		else
			textProgress = (time - 1000.0f) / 1000.0f;

		g->DrawImage(jpg1, (width - jpg1->GetWidth()) / 2, (height / 2) - (int)(jpg1->GetHeight() * textProgress), jpg1->GetWidth(), jpg1->GetHeight());
		g->FillRectangle(&brush, 0, height / 2 - 4, width, height / 2 + 4);
		render_g.DrawImage(bmp, 0, 0, width, height);

		g2->DrawImage(jpg2, (width - jpg2->GetWidth()) / 2, (int)((height / 2 - jpg2->GetHeight()) + (int)(jpg2->GetHeight() * textProgress)), jpg2->GetWidth(), jpg2->GetHeight());
		g2->FillRectangle(&brush, 0, 0, width, height / 2 + 4);
		render_g.DrawImage(bmp2, 0, height / 2 + 4, 0, height / 2 + 4, width, height / 2 - 4, Gdiplus::UnitPixel);

		SolidBrush whitebrush(Color::White);
		int start_x = (width - (int)(rectWidth * rectProgress)) / 2;
		int pwidth = (int)(rectWidth * rectProgress);
		render_g.FillRectangle(&whitebrush, start_x, (height - rectHeight) / 2, pwidth, rectHeight);

		BitmapData bitmapData;
		Rect rect(0, 0, width, height);

		renderbmp.LockBits(
			&rect,
			ImageLockModeRead,
			PixelFormat32bppARGB,
			&bitmapData);

		UINT* pixelsSrc = (UINT*)bitmapData.Scan0;

		if (!pixelsSrc)
			return false;

		int stride = bitmapData.Stride >> 2;

		for (int col = 0; col < width; ++col)
		{
			for (int row = 0; row < height; ++row)
			{
				int indexSrc = (height - 1 - row) * stride + col;
				int index = row * width + col;
				pixels[index] = pixelsSrc[indexSrc];
			}
		}

		renderbmp.UnlockBits(&bitmapData);

		return true;
	}
private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
	Gdiplus::Bitmap* jpg1;
	Gdiplus::Bitmap* jpg2;
	Gdiplus::Graphics* g;
	Gdiplus::Graphics* g2;
	Gdiplus::Bitmap* bmp;
	Gdiplus::Bitmap* bmp2;

};
int main()
{
	std::wstring musicFile(L"");
	std::wstring videoFile(L"C:\\temp\\TextVideo.mp4");

	RenderText frameRenderer(L"image\\Mandy.png", L"image\\Frenzy.png");

	H264Writer writer(musicFile.c_str(), videoFile.c_str(), VideoCodec::H264, 640, 480, 60, 2000, &frameRenderer);
	if (writer.IsValid())
	{
		if (writer.Process())
		{
			printf("Video written successfully!\n");
			return 0;
		}
	}
	printf("Video write failed!\n");
	return 1;
}
