#include "serial_changer.hpp"

#include "util.hpp"
#include "volume_helper.hpp"

std::vector<std::string> serial_changer::get_fixed_drives()
{
	std::vector<std::string> valid;

	for (int i = 'A'; i <= 'Z'; i++) {
		char buf[8];
		sprintf_s(buf, "%c:\\", (char)i);
		if (GetDriveTypeA(buf) == 3) {
			valid.push_back(std::string(buf));
		}
	}

	return valid;
}

serial_changer::serial_changer(std::string drive)
	: m_drive(drive) 
{ }

int serial_changer::change_serial(uint64_t serial)
{
	nt_volume vol;
	if (!vol.open_volume(m_drive.c_str())) {
		print_error("Could not open handle to volume!");
		return FALSE;
	}

	print_info("Opened handle to volume %s", m_drive.c_str());

	char sector[512];
	if (!vol.read_sector(0, sector, 512)) {
		print_error("Could not read boot sector!");
		return FALSE;
	}

	PPACKED_BOOT_SECTOR boot_sector = (PPACKED_BOOT_SECTOR)sector;
	if (strncmp("NTFS", (const char*)boot_sector->Oem, 5) == 0) {
		print_error("Invalid OEM ID!");
		return FALSE;
	}

	print_info("Valid OEM ID!");
	print_info("Serial number: 0x%I64X", boot_sector->SerialNumber);
	print_warn("Patching serial...");

	boot_sector->SerialNumber = serial;
	if (!vol.write_sector(0, sector, 512)) {
		print_error("Could not write boot sector!");
		return FALSE;
	}
	print_info("New serial number: 0x%I64X", boot_sector->SerialNumber);

	return TRUE;
}

int serial_changer::random_serial()
{
	std::random_device rand;
	std::mt19937 engine(rand());

	std::uniform_int_distribution<uint64_t> dist;

	return change_serial(dist(engine));
}
