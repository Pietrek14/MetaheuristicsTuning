#pragma once

#include "objective.h"
#include "field.h"

#include <format>
#include <string_view>
#include <string>
#include <map>
#include <mutex>
#include <vector>


using std::make_format_args;
using std::string_view;
using std::string;
using std::map;


inline std::mutex println_mutex;


template <typename... Args>
void print(const string_view fmt_str, Args&&... args)
{
	auto fmt_args{ make_format_args(args...) };
	string outstr{ vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

template <typename... Args>
void println(const string_view fmt_str = "", Args&&... args)
{
	std::lock_guard<std::mutex> lock(println_mutex);

	print(fmt_str, args...);
	print("\n");
}

inline void print_fields(const std::vector<core::field_t>& fields)
{
	std::lock_guard<std::mutex> lock(println_mutex);

	for (const auto& field : fields)
		print("{}: {}, ", field.name, field.value);
	print("\n");
}