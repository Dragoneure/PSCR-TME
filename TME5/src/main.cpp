#include "Vec3D.h"
#include "Rayon.h"
#include "Scene.h"
#include "DrawJob.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <random>

using namespace std;
using namespace pr;


int main () {

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	// on pose une graine basee sur la date
	default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	// definir la Scene : resolution de l'image
	Scene scene (1000,1000);
	// remplir avec un peu d'aléatoire
	fillScene(scene, re);
	
	// lumieres 
	vector<Vec3D> lights;
	lights.reserve(3);
	lights.emplace_back(Vec3D(50, 50, -50));
	lights.emplace_back(Vec3D(50, 50, 120));
	lights.emplace_back(Vec3D(200, 0, 120));

	// les points de l'ecran, en coordonnées 3D, au sein de la Scene.
	// on tire un rayon de l'observateur vers chacun de ces points
	const Scene::screen_t & screen = scene.getScreenPoints();

	// Les couleurs des pixels dans l'image finale
	Color * pixels = new Color[scene.getWidth() * scene.getHeight()];

	// pour chaque pixel, calculer sa couleur
	for (int x =0 ; x < scene.getWidth() ; x++) {
		for (int  y = 0 ; y < scene.getHeight() ; y++) {
			// le point de l'ecran par lequel passe ce rayon
			auto & screenPoint = screen[y][x];
			// le rayon a inspecter
			Rayon  ray(scene.getCameraPos(), screenPoint);

			int targetSphere = findClosestInter(scene, ray);

			if (targetSphere == -1) {
				// keep background color
				continue ;
			} else {
				const Sphere & obj = *(scene.begin() + targetSphere);
				// pixel prend la couleur de l'objet
				Color finalcolor = computeColor(obj, ray, scene.getCameraPos(), lights);
				// le point de l'image (pixel) dont on vient de calculer la couleur
				Color & pixel = pixels[y*scene.getHeight() + x];
				// mettre a jour la couleur du pixel dans l'image finale.
				pixel = finalcolor;
			}

		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	    std::cout << "Total time "
	              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
	              << "ms.\n";

	exportImage("toto.ppm",scene.getWidth(), scene.getHeight() , pixels);

	return 0;
}

