#pragma once
#if _DEBUG
#define ALLOWED_LOG_VERBOSITY 4
#include <Core/Logger/Logger.h>
#else
// Logs a message with specified verbosity, category, line, message, and format arguments.
#define LOG(verbosity, category, line, message, ...) void()

// Logs a message with current line and specified verbosity, category, line, message, and format arguments.
#define LOG_L(verbosity, category, message, ...) void()

// Logs a message with Verbose verbosity and specified category, message, and format arguments.
#define LOG_VERBOSE(category, message, ...) void()
// Logs a message with current line, Verbose verbosity and specified category, line, message, and format arguments.
#define LOG_VERBOSE_L(category, message, ...) void()

// Logs a message with Info verbosity and specified category, message, and format arguments.
#define LOG_INFO(category, message, ...) void()
// Logs a message with current line, Info verbosity and specified category, line, message, and format arguments.
#define LOG_INFO_L(category, message, ...) void()

// Logs a message with Warning verbosity and specified category, message, and format arguments.
#define LOG_WARNING(category, message, ...) void()
// Logs a message with current line, Warning verbosity and specified category, line, message, and format arguments.
#define LOG_WARNING_L(category, message, ...) void()


// Logs a message with Error verbosity and specified category, message, and format arguments.
#define LOG_ERROR(category, message, ...) void()
// Logs a message with current line, Error verbosity and specified category, line, message, and format arguments.
#define LOG_ERROR_L(category, message, ...) void()

// Logs a message with Fatal verbosity and specified category, message, and format arguments.
#define LOG_FATAL(category, message, ...) void()
// Logs a message with current line, Fatal verbosity and specified category, line, message, and format arguments.
#define LOG_FATAL_L(category, message, ...) void()
#endif

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>
#include <d3d11_1.h>
#endif

#include "Result.h"
using namespace graphics::result;

#include "GAssert.h"

#define _STRINGIFY_INTERNAL(resolvedExpression) #resolvedExpression
#define STRINGIFY(expression) _STRINGIFY_INTERNAL(expression)

#define _WSTRINGIFY_INTERNAL(resolvedExpression) L#resolvedExpression
#define WSTRINGIFY(expression) _WSTRINGIFY_INTERNAL(expression)