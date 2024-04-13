#pragma once

#include <iostream>
#include <fstream>
#include <random>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

using namespace std;
using namespace Eigen;

class Image
{
private:
	VectorXi original;
	VectorXi removed;
	VectorXi restored;
	int width;
	int height;
	int max_val;

public:
	Image(string filename);
	void remove(int percent);
	void saveRemoved();
	void restore();
	void saveRestored();

};

