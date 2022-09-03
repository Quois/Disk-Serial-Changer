#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <random>

class serial_changer {
public:
	static std::vector<std::string> get_fixed_drives();

	serial_changer(std::string drive);

	int change_serial(uint64_t serial);
	int random_serial();

private:
	std::string m_drive;
};