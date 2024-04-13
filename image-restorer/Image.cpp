#include "Image.h"

using namespace std;
using namespace Eigen;

Image::Image(string filename) :removed(), restored() {
	ifstream file(filename);
	if (!file) {
		throw runtime_error("Unable to open file in Image constructor");
	}

	string header;
	file >> header;
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  

	if (header != "P2") {
		throw runtime_error("Invalid PGM file");
	}

	while (file.peek() == '#') {
		file.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	
	file >> width >> height >> max_val;
	original.resize(width * height);
	for (int i = 0; i < width * height; i++) {
		file >> original(i);
	}

	file.close();
}

void Image::remove(int percent) {
	if (width <= 2 || height <= 2) {
		throw runtime_error("Image width and height must be greater than 2");
	}

	removed = original;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> X(1, width - 2);
	uniform_int_distribution<> Y(1, height- 2);

	int n = static_cast<int>((width - 2) * (height - 2) * percent / 100);

	for (int i = 0; i < n; i++) {
		int& pixel = removed[width * Y(gen) + X(gen)];

		if (pixel == max_val) {
			i--;
			continue;
		}

		pixel = 0;
	}
}
void Image::saveRemoved() {
	if (removed.size() == 0) {
		throw runtime_error("Removed image cannot be empty");
	}

	ofstream file("imageRemovedPixels.pgm");
	if (!file) {
		throw runtime_error("Unable to open file in saveRemoved");
	}

	file << "P2\n" << "# pixels removed\n" << width << " " << height << endl << max_val << endl;

	for (int i = 0; i < width * height; i++) {
		file << removed(i) << endl;
	}
}

void Image::restore() {
	int size = width * height;
	SparseMatrix<double> m(size, size);
	
	for (int i = 0; i < size; i++) {
		if (removed[i] == 0) {
			m.insert(i, i) = 4.;

			if (i - 1 >= 0) {
				m.insert(i, i - 1) = -1.;
			}
			if (i + 1 < size) {
				m.insert(i, i + 1) = -1.;
			}
			if (i - width >= 0) {
				m.insert(i, i - width) = -1.;
			}
			if (i + width < size) {
				m.insert(i, i + width) = -1.;
			}
		}
		else {
			m.insert(i, i) = 1.;
		}
	}

	BiCGSTAB<SparseMatrix<double>, IncompleteLUT<double>> solver;
	solver.compute(m);
	if (solver.info() != Eigen::Success) {
		cout << "BiCGSTAB failed. \n";
		return;
	}

	VectorXd removed_d = removed.cast<double>();
	VectorXd restored_d = solver.solve(removed_d);
	if (solver.info() != Eigen::Success) {
		std::cout << "Solving failed. \n";
		return;
	}

	restored = restored_d.cast<int>();
}
void Image::saveRestored() {
	if (restored.size() == 0) {
		throw runtime_error("Restored image cannot be empty");
	}

	ofstream file("imageRestoredPixels.pgm");
	if (!file) {
		throw runtime_error("Unable to open file in saveRestored");
	}

	file << "P2\n" << "# pixels restored\n" << width << " " << height << endl << max_val << endl;

	for (int i = 0; i < width * height; i++) {
		file << restored(i) << endl;
	}
}