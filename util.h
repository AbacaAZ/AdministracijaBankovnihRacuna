#pragma once

char* allocateStringMem(const char* str, const int n);
char* inputString(char* const str, int (*condition)(const char* const));
void safeFree(void** const p);
void safeFreeArr(void*** const pArr, const int n);