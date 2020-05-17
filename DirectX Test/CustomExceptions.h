#pragma once
#include <exception>

class CException : public std::exception{};

class KeyboardException : public CException{};

class WindowException : public CException{};

class DeviceException : public CException{};