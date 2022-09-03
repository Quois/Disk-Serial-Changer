#pragma once
#include <Windows.h>
#include <iostream>

typedef LONGLONG LCN;
typedef LCN* PLCN;

#pragma pack(1)		//while parse DBR,must indicate the pack(1)

//
//  Define the Packed and Unpacked BIOS Parameter Block
//

typedef struct _PACKED_BIOS_PARAMETER_BLOCK {

    UCHAR  BytesPerSector[2];                               //  offset = 0x000
    UCHAR  SectorsPerCluster[1];                            //  offset = 0x002
    UCHAR  ReservedSectors[2];                              //  offset = 0x003 (zero)
    UCHAR  Fats[1];                                         //  offset = 0x005 (zero)
    UCHAR  RootEntries[2];                                  //  offset = 0x006 (zero)
    UCHAR  Sectors[2];                                      //  offset = 0x008 (zero)
    UCHAR  Media[1];                                        //  offset = 0x00A
    UCHAR  SectorsPerFat[2];                                //  offset = 0x00B (zero)
    UCHAR  SectorsPerTrack[2];                              //  offset = 0x00D
    UCHAR  Heads[2];                                        //  offset = 0x00F
    UCHAR  HiddenSectors[4];                                //  offset = 0x011 (zero)
    UCHAR  LargeSectors[4];                                 //  offset = 0x015 (zero)

} PACKED_BIOS_PARAMETER_BLOCK;                              //  sizeof = 0x019

typedef PACKED_BIOS_PARAMETER_BLOCK* PPACKED_BIOS_PARAMETER_BLOCK;

typedef struct BIOS_PARAMETER_BLOCK {

    USHORT BytesPerSector;
    UCHAR  SectorsPerCluster;
    USHORT ReservedSectors;
    UCHAR  Fats;               // Number of FATs         : always 0
    USHORT RootEntries;        // Root directory entries : always 0
    USHORT Sectors;            // Total logical sectors  : always 0
    UCHAR  Media;              // Media descriptor
    USHORT SectorsPerFat;
    USHORT SectorsPerTrack;
    USHORT Heads;              // Number of heads
    ULONG  HiddenSectors;
    ULONG  LargeSectors;       // Large total logical sectors

} BIOS_PARAMETER_BLOCK;

typedef BIOS_PARAMETER_BLOCK* PBIOS_PARAMETER_BLOCK;

typedef struct _PACKED_BOOT_SECTOR {

	UCHAR Jump[3];                                                  //  offset = 0x000
	UCHAR Oem[8];                                                   //  offset = 0x003
	BIOS_PARAMETER_BLOCK PackedBpb;                                 //  offset = 0x00B
	UCHAR Unused[4];                                                //  offset = 0x024
	LONGLONG NumberSectors;                                         //  offset = 0x028
	LCN MftStartLcn;                                                //  offset = 0x030
	LCN Mft2StartLcn;                                               //  offset = 0x038
	CHAR ClustersPerFileRecordSegment;                              //  offset = 0x040
	UCHAR Reserved0[3];
	CHAR DefaultClustersPerIndexAllocationBuffer;                   //  offset = 0x044
	UCHAR Reserved1[3];
	LONGLONG SerialNumber;                                          //  offset = 0x048
	ULONG Checksum;                                                 //  offset = 0x050
	UCHAR BootStrap[0x200 - 0x044];                                   //  offset = 0x054

} PACKED_BOOT_SECTOR;                                               //  sizeof = 0x200

typedef PACKED_BOOT_SECTOR* PPACKED_BOOT_SECTOR;


class nt_volume {
public:

	nt_volume() = default;
	~nt_volume();

	int open_volume(const char* path);
	int read_sector(uint32_t sector, char* buffer, int sector_size);
	int write_sector(uint32_t sector, char* buffer, int sector_size);
	int close_volume();

private:
	HANDLE h_volume = nullptr;
	bool open = false;
};