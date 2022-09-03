#include "volume_helper.hpp"

nt_volume::~nt_volume()
{
	close_volume();
}

int nt_volume::open_volume(const char* path)
{
	char drive_path[16];
	sprintf_s(drive_path, "\\\\.\\%c:", path[0]);
	h_volume = CreateFileA(drive_path, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		NULL, NULL);
	if (h_volume == INVALID_HANDLE_VALUE) return 0;
	open = true;
	return 1;
}

int nt_volume::read_sector(uint32_t sector, char* buffer, int sector_size)
{
	if (!open) return 0;

	if (SetFilePointer(h_volume, sector, NULL, FILE_BEGIN) == -1) {
		return 0;
	}

	DWORD read;
	if (!ReadFile(h_volume, buffer, sector_size, &read, 0)) {
		return 0;
	}

	return 1;
}

int nt_volume::write_sector(uint32_t sector, char* buffer, int sector_size)
{
	if (!open) return 0;

	if (SetFilePointer(h_volume, sector, NULL, FILE_BEGIN) == -1) {
		return 0;
	}

	DWORD written;
	if (!WriteFile(h_volume, buffer, sector_size, &written, 0)) {
		return 0;
	}

	return 1;
}

int nt_volume::close_volume()
{
	if (open) {
		CloseHandle(h_volume);
		return 0;
	}

	return 1;
}
