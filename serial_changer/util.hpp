#pragma once
#include <Windows.h>
#include <iostream>

#define print_info(format, ...) printf_color(9, '+', format, __VA_ARGS__)
#define print_warn(format, ...) printf_color(14, '!', format, __VA_ARGS__)
#define print_error(format, ...) printf_color(12, '-', format, __VA_ARGS__)

static HANDLE out_handle = nullptr;
static WORD orig_attr = 0;

static void init_print() {
	out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(out_handle, &info);

	orig_attr = info.wAttributes;
}

static void printf_color(int attr, char sym, const char* format, ...) {
	out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(out_handle, attr);
	
	va_list va;
	va_start(va, format);

	char buffer[512];

	//vprintf(format, va);
	vsprintf_s(buffer, format, va);
	printf("[%c] %s\n", sym, buffer);

	va_end(va);

	SetConsoleTextAttribute(out_handle, orig_attr);
}