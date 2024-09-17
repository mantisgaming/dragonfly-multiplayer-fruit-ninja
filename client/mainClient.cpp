
#include <iostream>
#include "ClientSample.h"
#include <SharedSample.h>

int main(int argc, char** argv) {
	std::cout << ClientSample::clientString << std::endl;
	std::cout << SharedSample::sharedString << std::endl;
}