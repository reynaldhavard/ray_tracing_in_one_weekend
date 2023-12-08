
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"

#include <iostream>
#include <memory>


int main()
{
    hittable_list world;

    auto ground_material = lambertian(color(0.5, 0.5, 0.5));
    auto ground_sphere = sphere(point3(0, -1000, 0), 1000);
    world.add(ground_sphere, ground_material);

    for (int a = -11; a < 11; ++a){
        for (int b = -11; b < 11; ++b){
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9){
                material mat;

                if (choose_mat < 0.8){
                    // diffuse
                    auto albedo = color::random() * color::random();
                    mat = lambertian(albedo);
                }
                else if (choose_mat < 0.95){
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    mat = metal(albedo, fuzz);
                }
                else {
                    mat = dielectric(1.5);
                }
                auto s = sphere(center, 0.2);
                world.add(s, mat);
            }
        }
    }

    auto material1 = dielectric(1.5);
    auto s1 = sphere(point3(0, 1, 0), 1.0);
    world.add(s1, material1);

    auto material2 = lambertian(color(0.4, 0.2, 0.1));
    auto s2 = sphere(point3(-4, 1, 0), 1.0);
    world.add(s2, material2);

    auto material3 = metal(color(0.7, 0.6, 0.5), 0.0);
    auto s3 = sphere(point3(4, 1, 0), 1.0);
    world.add(s3, material3);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 32;
    cam.max_depth = 8;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.initialize();

    SDL sdl(cam.image_width, cam.image_height);

    cam.render(world, sdl);

    // Keep window open until we close it
    while (true){
        if (sdl.quit()){
            break;
        }
        sdl.render();
    }

    return 0;

}
