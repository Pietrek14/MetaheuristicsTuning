#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_map>


namespace logging
{
	class csv_reader
	{
	public:

		explicit csv_reader(const std::string& filename)
		{
			file_.open("metaheuristics results\\" + filename);
			if (!file_.is_open())
				throw std::runtime_error("Failed to open CSV file: " + filename);

			read_header();
		}

		const std::vector<std::string>& headers() const
		{
			return headers_;
		}

		bool read_row(std::vector<std::string>& row)
		{
			row.clear();

			std::string line;
			if (!std::getline(file_, line))
				return false;

			split_line(line, row);
			return true;
		}


	private:

		std::ifstream file_;
		std::vector<std::string> headers_;

		void read_header()
		{
			std::string line;
			if (!std::getline(file_, line))
				throw std::runtime_error("Failed to read header from CSV file.");

			split_line(line, headers_);
		}


		static void split_line(const std::string& line, std::vector<std::string>& values)
		{
			std::stringstream line_stream(line);
			std::string value;
			while (std::getline(line_stream, value, ','))
				values.push_back(value);
		}
	};
}