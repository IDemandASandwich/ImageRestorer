#include "Image.h"

int main(void)
{
	try {
		Image image("me.pgm");
		image.remove(30);
		image.saveRemoved();
		image.restore();
		image.saveRestored();
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Caught a runtime_error exception: " << e.what() << '\n';
	}
	
	return 0;
}
