#include "eppch.h"
#include "Offline.h"

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Epoch/Math/Vec.h"
#include "Epoch/Renderer/Offline/Core/Ray.h"

namespace Epoch {

  bool Offline::BeginScene()
  {
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 1080;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	m_RendererID = new char[image_width * image_height * 3];
	// Camera

	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = Point(0, 0, 0);
	auto horizontal = Vec3(viewport_width, 0, 0);
	auto vertical = Vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

	int x = 0;

	for (int j = image_height - 1; j >= 0; --j)
	{
	  Progress = 1 - j / image_height - 1;

	  for (int i = 0; i < image_width; ++i)
	  {
		auto u = float(i) / (image_width - 1);
		auto v = float(j) / (image_height - 1);

		Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);

		Color pixel_color = RayColor(ray);

		float ir = 255.999 * pixel_color.x();
		float ig = 255.999 * pixel_color.y();
		float ib = 255.999 * pixel_color.z();

		m_RendererID[x * 3 + 0] = ir;
		m_RendererID[x * 3 + 1] = ig;
		m_RendererID[x * 3 + 2] = ib;

		x++;
	  }
	}

	stbi_write_png("assets/textures/Offline.png", image_width, image_height, 3, m_RendererID, image_width * 3);
	stbi_write_bmp("assets/textures/Offline.bmp", image_width, image_height, 3, m_RendererID);
	stbi_write_tga("assets/textures/Offline.tga", image_width, image_height, 3, m_RendererID);
	stbi_write_jpg("assets/textures/Offline.jpg", image_width, image_height, 3, m_RendererID, 95);
	//stbi_write_hdr("assets/textures/wr6x5_regular.hdr", 6, 5, 3, img6x5_rgbf);

	//stbi_flip_vertically_on_write(1);

	//stbi_write_png("assets/textures/wr6x5_flip.png", 6, 5, 3, img6x5_rgb, 6 * 3);
	//stbi_write_bmp("assets/textures/wr6x5_flip.bmp", 6, 5, 3, img6x5_rgb);
	//stbi_write_tga("assets/textures/wr6x5_flip.tga", 6, 5, 3, img6x5_rgb);
	//stbi_write_jpg("assets/textures/wr6x5_flip.jpg", 6, 5, 3, img6x5_rgb, 95);
	//stbi_write_hdr("assets/textures/wr6x5_flip.hdr", 6, 5, 3, img6x5_rgbf);
	return true;
  }

  Color Offline::RayColor(const Ray& ray)
  {
	Vec3 dirNormalize = normalize(ray.GetDirection());
	auto t = 0.5 * (dirNormalize.y() + 1.0);
	return (1.0 - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
  }

}