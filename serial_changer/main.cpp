#include <Windows.h>
#include <iostream>
#include <random>

#include "volume_helper.hpp"
#include "util.hpp"
#include "serial_changer.hpp"

int main(int argc, char** argv) {
	init_print();

	std::vector<std::string> drives = serial_changer::get_fixed_drives();
	for (const auto& path : drives) {
		print_info("Drive: %s", path.c_str());
		serial_changer changer(path);

		print_warn("Change serial number?");
		if (MessageBoxA(NULL, "Change disk serial?", "Continue?", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK) {
			changer.random_serial();
		}
		else {
			print_warn("Exiting...!");
		}
	}

	return 0;
}