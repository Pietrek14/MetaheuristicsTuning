#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <format>
#include <stdexcept>
#include <ios>


namespace logging
{
	class csv_writer
	{
	public:

		explicit csv_writer(
			const std::string& base_filename,
			const std::vector<std::string>& headers)
		{
			auto now = std::chrono::floor<std::chrono::seconds>(
				std::chrono::system_clock::now());
			std::string filename = std::format(
				"{}_{:%Y%m%d_%H%M%S}.csv", base_filename, now);

			file_.open("metaheuristics results\\" + filename, std::ios::trunc);
			if (!file_.is_open())
				throw std::runtime_error("Failed to open CSV file: " + filename);

			for (size_t i{}; i < headers.size(); ++i)
			{
				file_ << headers[i];
				if (i + 1 < headers.size())
					file_ << ',';
			}
			file_ << '\n';
			file_.flush();
		}

		void write_row(const std::vector<std::string>& values)
		{
			for (size_t i{}; i < values.size(); ++i)
			{
				file_ << values[i];
				if (i + 1 < values.size())
					file_ << ',';
			}
			file_ << '\n';
			file_.flush();
		}


	private:
		std::ofstream file_;
	};
}