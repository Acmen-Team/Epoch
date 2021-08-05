#include "eppch.h"
#include "Offline.h"

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Epoch {

  bool Offline::BeginScene()
  {
	const int image_width = 256;
	const int image_height = 256;

	//unsigned char img6x5_rgb[256 * 256 * 3];

	int x = 0;

	for (int j = image_height - 1; j >= 0; --j)
	{
	  Progress = 1 - j / image_height - 1;

	  for (int i = 0; i < image_width; ++i)
	  {
		auto r = float(i) / (image_width - 1);
		auto g = float(j) / (image_height - 1);
		auto b = 0.25f;

		float ir = 255.999 * r;
		float ig = 255.999 * g;
		float ib = 255.999 * b;

		m_RendererID[x * 3 + 0] = ir;
		m_RendererID[x * 3 + 1] = ig;
		m_RendererID[x * 3 + 2] = ib;

		x++;
	  }
	}


	// for (i = 0; i < 256 * 256; i++) {
	   //img6x5_rgb[i * 3 + 0] = float(i) / (256 - 1) * 255.999f;
	   //img6x5_rgb[i * 3 + 1] = 10;
	   //img6x5_rgb[i * 3 + 2] = 64;

	   //img6x5_rgbf[i * 3 + 0] = on ? 1.0f : 0.0f;
	   //img6x5_rgbf[i * 3 + 1] = 0.0f;
	   //img6x5_rgbf[i * 3 + 2] = on ? 0.0f : 1.0f;
	// }

	//m_RendererID = img6x5_rgb;

	stbi_write_png("assets/textures/Offline.png", 256, 256, 3, m_RendererID, 256 * 3);
	stbi_write_bmp("assets/textures/Offline.bmp", 256, 256, 3, m_RendererID);
	stbi_write_tga("assets/textures/Offline.tga", 256, 256, 3, m_RendererID);
	stbi_write_jpg("assets/textures/Offline.jpg", 256, 256, 3, m_RendererID, 95);
	//stbi_write_hdr("assets/textures/wr6x5_regular.hdr", 6, 5, 3, img6x5_rgbf);

	//stbi_flip_vertically_on_write(1);

	//stbi_write_png("assets/textures/wr6x5_flip.png", 6, 5, 3, img6x5_rgb, 6 * 3);
	//stbi_write_bmp("assets/textures/wr6x5_flip.bmp", 6, 5, 3, img6x5_rgb);
	//stbi_write_tga("assets/textures/wr6x5_flip.tga", 6, 5, 3, img6x5_rgb);
	//stbi_write_jpg("assets/textures/wr6x5_flip.jpg", 6, 5, 3, img6x5_rgb, 95);
	//stbi_write_hdr("assets/textures/wr6x5_flip.hdr", 6, 5, 3, img6x5_rgbf);
	return true;
  }

}