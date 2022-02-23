using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using H264WriterDLL;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

namespace CSharpEncodeImage
{
    // render a red image once!
    public class RenderRedImage : FrameRenderer
    {
        public bool Render(int width, int height, int fps, int frame_cnt, UInt32[] pixels)
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
    }

    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("HasH264() Software: {0}\n", H264Writer.HasH264(Processing.Software));
            Console.WriteLine("HasH264() Hardware: {0}\n", H264Writer.HasH264(Processing.HardwareAcceleration));
            Console.WriteLine("HasHEVC() Software: {0}\n", H264Writer.HasHEVC(Processing.Software));
            Console.WriteLine("HasHEVC() Hardware: {0}\n", H264Writer.HasHEVC(Processing.HardwareAcceleration));
            string musicFile = "";
            string videoFile = "C:\\temp\\RedVideoCSharp.mp4";

            RenderRedImage frameRenderer = new RenderRedImage();

            H264Writer writer = new H264Writer(musicFile, videoFile, VideoCodec.H264, Processing.HardwareAcceleration, 640, 480, 30, 60000, frameRenderer, 4000000, 0, 100, RateControlMode.Quality, 100);
            if (writer.IsValid())
            {
                if (writer.Process())
                {
                    Console.WriteLine("Video written successfully!\n");
                    return;
                }
            }
            Console.WriteLine("Video write failed!\n");

        }
    }

    /*
    // render a jpeg once!
    public class RenderJPG : FrameRenderer
    {
        public bool Render(int width, int height, int fps, int frame_cnt, UInt32[] pixels)
        {
            if (frame_cnt == 0)
            {
                Bitmap bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                Bitmap jpg = new Bitmap("image\\yes.jpg", true);
                Graphics g = Graphics.FromImage(bmp);
                float w_ratio_bmp = bmp.Width / (float)bmp.Height;
                float w_ratio_jpg = jpg.Width / (float)jpg.Height;

                SolidBrush brush = new SolidBrush(Color.Black);
                g.FillRectangle(brush, 0, 0, bmp.Width, bmp.Height);

                if (w_ratio_jpg >= w_ratio_bmp)
                {
                    int width2 = bmp.Width;
                    int height2 = (int)((bmp.Width / (float)jpg.Width) * jpg.Height);
                    g.DrawImage(jpg, 0, (bmp.Height - height2) / 2, width2, height2);
                }
                else
                {
                    int width2 = (int)((bmp.Height / (float)jpg.Height) * jpg.Width);
                    int height2 = bmp.Height;
                    g.DrawImage(jpg, (bmp.Width - width2) / 2, 0, width2, height2);
                }

                BitmapData bitmapData = new BitmapData();
                Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);

                bmp.LockBits(
                    rect,
                    ImageLockMode.ReadOnly,
                    PixelFormat.Format32bppArgb,
                    bitmapData);

                unsafe
                {
                    uint* pixelsSrc = (uint*)bitmapData.Scan0;

                    if (pixelsSrc==null)
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
                }
                bmp.UnlockBits(bitmapData);

                bmp.Dispose();
                jpg.Dispose();
                brush.Dispose();
                g.Dispose();
            }
            return true;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            string musicFile = "";
            string videoFile = "C:\\temp\\JpgVideoCSharp.mp4";

            RenderJPG frameRenderer = new RenderJPG();

            H264Writer writer = new H264Writer(musicFile, videoFile, VideoCodec.H264, Processing.HardwareAcceleration, 640, 480, 30, 10000, frameRenderer, 4000000, 0, 100, RateControlMode.Quality, 100);
            if (writer.IsValid())
            {
                if (writer.Process())
                {
                    Console.WriteLine("Video written successfully!\n");
                    return;
                }
            }
            Console.WriteLine("Video write failed!\n");

        }
    }
    */

    /*
    // render 2 jpegs!
    public class Render2JPG : FrameRenderer
    {
       public Render2JPG(string img1, string img2)
        {
            g = null;
            g2 = null;
            bmp = null;
            bmp2 = null;
            jpg1 = new Bitmap(img1, true);
            jpg2 = new Bitmap(img2, true);
        }
        ~Render2JPG()
        {
            jpg1.Dispose();
            jpg2.Dispose();
            bmp.Dispose();
            bmp2.Dispose();
            g.Dispose();
            g2.Dispose();
        }

        public bool Render(int width, int height, int fps, int frame_cnt, UInt32[] pixels)
        {
            if (bmp == null)
            {
                bmp = new Bitmap(width, height, PixelFormat.Format32bppArgb);
                bmp2 = new Bitmap(width, height, PixelFormat.Format32bppArgb);
                g = Graphics.FromImage(bmp);
                g2 = Graphics.FromImage(bmp2);
            }
            byte alpha = 0;
            float frame_duration = 1000.0f / fps;
            float time = frame_cnt * frame_duration;
            if (time <= 1000.0)
                alpha = 0;
            else if (time >= 2000.0)
                alpha = 255;
            else
                alpha = (byte)(int)(((time) - 1000.0) / 1000.0 * 255);

            float w_ratio_bmp = bmp.Width / (float)bmp.Height;
            float w_ratio_jpg = jpg1.Width / (float)jpg1.Height;

            SolidBrush brush = new SolidBrush(Color.Black);
            g.FillRectangle(brush, 0, 0, bmp.Width, bmp.Height);

            if (w_ratio_jpg >= w_ratio_bmp)
            {
                int width2 = bmp.Width;
                int height2 = (int)((bmp.Width / (float)jpg1.Width) * jpg1.Height);
                g.DrawImage(jpg1, 0, (bmp.Height - height2) / 2, width2, height2);
                g2.DrawImage(jpg2, 0, (bmp2.Height - height2) / 2, width2, height2);
            }
            else
            {
                int width2 = (int)((bmp.Height / (float)jpg1.Height) * jpg1.Width);
                int height2 = bmp.Height;
                g.DrawImage(jpg1, (bmp.Width - width2) / 2, 0, width2, height2);
                g2.DrawImage(jpg2, (bmp2.Width - width2) / 2, 0, width2, height2);
            }

            BitmapData bitmapData = new BitmapData();
            BitmapData bitmapData2 = new BitmapData();
            Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);

            bmp.LockBits(
                rect,
                ImageLockMode.ReadOnly,
                PixelFormat.Format32bppArgb,
                bitmapData);

            bmp2.LockBits(
                rect,
                ImageLockMode.ReadOnly,
                PixelFormat.Format32bppArgb,
                bitmapData2);

            unsafe
            {
                uint* pixelsSrc = (uint*)bitmapData.Scan0;
                uint* pixelsSrc2 = (uint*)bitmapData2.Scan0;

                if (pixelsSrc == null || pixelsSrc2 == null)
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
            }
            bmp.UnlockBits(bitmapData);
            bmp2.UnlockBits(bitmapData2);

            brush.Dispose();

            return true;
        }

	    private uint Alphablend(uint dest, uint source, byte nAlpha, byte nAlphaFinal)
        {
            byte nInvAlpha = (byte) ~nAlpha;

            byte nSrcRed = (byte)((source & 0xff0000) >> 16);
            byte nSrcGreen = (byte)((source & 0xff00) >> 8);
            byte nSrcBlue = (byte)(source & 0xff);

            byte nDestRed = (byte)((dest & 0xff0000) >> 16);
            byte nDestGreen = (byte)((dest & 0xff00) >> 8);
            byte nDestBlue = (byte)(dest & 0xff);

            byte nRed = (byte)((nSrcRed * nAlpha + nDestRed * nInvAlpha) >> 8);
            byte nGreen = (byte)((nSrcGreen * nAlpha + nDestGreen * nInvAlpha) >> 8);
            byte nBlue = (byte)((nSrcBlue * nAlpha + nDestBlue * nInvAlpha) >> 8);

            return (uint)(nAlphaFinal << 24 | nRed << 16 | nGreen << 8 | nBlue);
        }

        private Bitmap jpg1;
        private Bitmap jpg2;
        private Graphics g;
        private Graphics g2;
        private Bitmap bmp;
        private Bitmap bmp2;
    }

    class Program
    {
        static void Main(string[] args)
        {
            string musicFile = "";
            string videoFile = "C:\\temp\\TwoJpgVideoCSharp.mp4";

            Render2JPG frameRenderer = new Render2JPG("image\\first.jpg", "image\\second.jpg");

            H264Writer writer = new H264Writer(musicFile, videoFile, VideoCodec.H264, Processing.HardwareAcceleration, 640, 480, 30, 3000, frameRenderer, 4000000, 0, 100, RateControlMode.Quality, 100);
            if (writer.IsValid())
            {
                if (writer.Process())
                {
                    Console.WriteLine("Video written successfully!\n");
                    return;
                }
            }
            Console.WriteLine("Video write failed!\n");

        }
    }
    */

    /*
    // text animation!
    public class RenderText : FrameRenderer
    {
        public RenderText(string img1, string img2)
        {
            g = null;
            g2 = null;
            bmp = null;
            bmp2 = null;
            jpg1 = new Bitmap(img1, true);
            jpg2 = new Bitmap(img2, true);
        }
        ~RenderText()
        {
            jpg1.Dispose();
            jpg2.Dispose();
            bmp.Dispose();
            bmp2.Dispose();
            g.Dispose();
            g2.Dispose();
        }

        public bool Render(int width, int height, int fps, int frame_cnt, UInt32[] pixels)
        {
            if (bmp == null)
            {
                bmp = new Bitmap(width, height, PixelFormat.Format32bppArgb);
                bmp2 = new Bitmap(width, height, PixelFormat.Format32bppArgb);
                g = Graphics.FromImage(bmp);
                g2 = Graphics.FromImage(bmp2);
            }
            Bitmap renderbmp = new Bitmap(width, height, PixelFormat.Format32bppArgb);
            Graphics render_g = Graphics.FromImage(renderbmp);

            float rectProgress = 0.0f;
            float textProgress = 0.0f;
            float frame_duration = 1000.0f / fps;
            float time = frame_cnt * frame_duration;

            SolidBrush brush = new SolidBrush(Color.Black);
            render_g.FillRectangle(brush, 0, 0, width, height);
            g.FillRectangle(brush, 0, 0, width, height);

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

            g.DrawImage(jpg1, (width - jpg1.Width) / 2, (height / 2) - (int)(jpg1.Height * textProgress), jpg1.Width, jpg1.Height);
            g.FillRectangle(brush, 0, height / 2 - 4, width, height / 2 + 4);
            render_g.DrawImage(bmp, 0, 0, width, height);

            g2.DrawImage(jpg2, (width - jpg2.Width) / 2, (int)((height / 2 - jpg2.Height) + (int)(jpg2.Height * textProgress)), jpg2.Width, jpg2.Height);
            g2.FillRectangle(brush, 0, 0, width, height / 2 + 4);
            render_g.DrawImage(bmp2, 0, height / 2 + 4, new Rectangle(0, height / 2 + 4, width, height / 2 - 4), GraphicsUnit.Pixel);

            SolidBrush whitebrush = new SolidBrush(Color.White);
            int start_x = (width - (int)(rectWidth * rectProgress)) / 2;
            int pwidth = (int)(rectWidth * rectProgress);
            render_g.FillRectangle(whitebrush, start_x, (height - rectHeight) / 2, pwidth, rectHeight);

            BitmapData bitmapData = new BitmapData();
            Rectangle rect = new Rectangle(0, 0, width, height);

            renderbmp.LockBits(
                rect,
                ImageLockMode.ReadOnly,
                PixelFormat.Format32bppArgb,
                bitmapData);

            unsafe
            {
                uint* pixelsSrc = (uint*)bitmapData.Scan0;

                if (pixelsSrc == null)
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
            }
            renderbmp.UnlockBits(bitmapData);

            renderbmp.Dispose();
            brush.Dispose();
            whitebrush.Dispose();
            render_g.Dispose();

            return true;
        }

        private Bitmap jpg1;
        private Bitmap jpg2;
        private Graphics g;
        private Graphics g2;
        private Bitmap bmp;
        private Bitmap bmp2;
    }

    class Program
    {
        static void Main(string[] args)
        {
            string musicFile = "";
            string videoFile = "C:\\temp\\TextVideoCSharp.mp4";

            RenderText frameRenderer = new RenderText("image\\Mandy.png", "image\\Frenzy.png");

            H264Writer writer = new H264Writer(musicFile, videoFile, VideoCodec.H264, Processing.HardwareAcceleration, 640, 480, 60, 2000, frameRenderer, 4000000, 0, 100, RateControlMode.Quality, 100);
            if (writer.IsValid())
            {
                if (writer.Process())
                {
                    Console.WriteLine("Video written successfully!\n");
                    return;
                }
            }
            Console.WriteLine("Video write failed!\n");

        }
    }
    */
}
