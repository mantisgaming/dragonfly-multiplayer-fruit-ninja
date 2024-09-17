
#include <iostream>
#include "ServerSample.h"
#include <SharedSample.h>

int main(int argc, char** argv) {
	std::cout << ServerSample::serverString << std::endl;
	std::cout << SharedSample::sharedString << std::endl;
}