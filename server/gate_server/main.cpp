#include <iostream>
#include "GateServer.hpp"

int main(int argc, char** argv)
{
	USE_NETWORK;
	GateServer app;
	app.run();
	return 0;
}