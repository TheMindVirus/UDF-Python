#ifndef DEBUG_H
#define DEBUG_H

#ifndef DEBUG_BUFFSIZE
#define DEBUG_BUFFSIZE   4096
#endif//DEBUG_BUFFSIZE

#ifndef DEBUG_FILENAME
#define DEBUG_FILENAME   "%SystemRoot%\\Temp\\UMDFDriver.txt"
#endif//DEBUG_FILENAME

#ifdef NDEBUG
#define debug(...)
#else
#define debug(...)   DebugLogToFile(__VA_ARGS__);

void DebugLogToFile(LPSTR format, ...)
{
	HANDLE file = NULL;
	CHAR path[DEBUG_BUFFSIZE] = "";
	CHAR buffer[DEBUG_BUFFSIZE] = "";
	DWORD written = 0;
	va_list va = va_list();
	va_start(va, format);
	vsprintf_s(buffer, DEBUG_BUFFSIZE, format, va);
	va_end(va);
	strcat_s(buffer, DEBUG_BUFFSIZE, "\n");

	ExpandEnvironmentStringsA(DEBUG_FILENAME, path, (DWORD)strlen(DEBUG_FILENAME));
	file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == file) { return; }

	SetFilePointer(file, 0, NULL, FILE_END);
	WriteFile(file, buffer, (DWORD)strlen(buffer), &written, NULL);
	CloseHandle(file);
}

#endif//NDEBUG

#endif//DEBUG_H