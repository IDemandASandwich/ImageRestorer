#include "Image.h"

int main(void)
{
	try {
		Image image("meMedium.pgm");	// change to your photo
		image.remove(99);				// how many % of pixels to remove
		image.saveRemoved();
		image.restore();
		image.saveRestored();
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Caught a runtime_error exception: " << e.what() << '\n';
	}
	
	return 0;
}
