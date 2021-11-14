#include "eppch.h"
#include "Offline.h"

#include "Epoch/Renderer/Offline/Object/Sphere.h"
#include "Epoch/Renderer/Offline/Core/RayMaterial.h"

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Epoch {

  bool Offline::BeginScene()
  {
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 1080;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int max_depth = 50;
	const int samples_per_pixel = 100;

	m_RendererID = new char[image_width * image_height * 3];
	// Camera

	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = Point(0, 0, 0);
	auto horizontal = Vec3(viewport_width, 0, 0);
	auto vertical = Vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);

	// Material
	auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	auto material_left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
	auto material_right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	// World
	HittableList world;
	world.Add(std::make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100.0, material_ground));
	world.Add(std::make_shared<Sphere>(Point(0.0, 0.0, -1.0), 0.5, material_center));
	world.Add(std::make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
	world.Add(std::make_shared<Sphere>(Point(1.0, 0.0, -1.0), 0.5, material_right));

	//world.Add(std::make_shared<Sphere>(Point(0.5, 0, -1), 0.5, material_ground));
	//world.Add(std::make_shared<Sphere>(Point(-0.5, 0, -1), 0.5));
	//world.Add(std::make_shared<Sphere>(Point(0, -100.5, -1), 100));

	// Render

	int x = 0;

	for (int j = image_height - 1; j >= 0; --j)
	{
	  Progress = 1.0f - ((float)j / (float)(image_height - 1));
	  for (int i = 0; i < image_width; ++i)
	  {
		Color pixel_color(0, 0, 0);
		for (int s = 0; s < samples_per_pixel; ++s)
		{
		  auto u = float(i + random_float()) / (image_width - 1);
		  auto v = float(j + random_float()) / (image_height - 1);

		  Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);

		  pixel_color += RayColor(ray, world, max_depth);
		}

		auto r = pixel_color.x();
		auto g = pixel_color.y();
		auto b = pixel_color.z();

		// Divide the color by the number of samples.
		auto scale = 1.0 / samples_per_pixel;
		//r *= scale;
		//g *= scale;
		//b *= scale;
		r = sqrt(r * scale);
		g = sqrt(g * scale);
		b = sqrt(b * scale);

		float ir = 256 * clamp(r, 0.0, 0.999);
		float ig = 256 * clamp(g, 0.0, 0.999);
		float ib = 256 * clamp(b, 0.0, 0.999);

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

  Color Offline::RayColor(const Ray& ray, const Hittable& world, int depth)
  {
	HitRecord rec;

	if (depth <= 0)
	  return Color(0, 0, 0);

	if (world.Hit(ray, 0.001, infinity, rec)) {
	  //return 0.5 * (rec.normal + Color(1, 1, 1));

	  Ray scattered;
	  Color attenuation;
	  if (rec.material->Scatter(ray, rec, attenuation, scattered))
		return attenuation * RayColor(scattered, world, depth - 1);
	  return Color(0, 0, 0);

	  //Point target = rec.p + rec.normal + Random_unit_vector();
	  //return 0.5 * RayColor(Ray(rec.p, target - rec.p), world, depth - 1);
	}
	Vec3 unit_direction = normalize(ray.GetDirection());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);

	//if (t > 0.0f)
	//{
	//  Vec3 normal = normalize(ray.At(t) - Point(0, 0, -1));
	//  return 0.5 * Color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
	//}

	//Vec3 dirNormalize = normalize(ray.GetDirection());
	//t = 0.5 * (dirNormalize.y() + 1.0);
	//return (1.0 - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
  }


}