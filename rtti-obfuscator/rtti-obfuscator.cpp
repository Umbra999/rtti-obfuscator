#include <iostream>
#include <fstream>
#include <random>
#include <unordered_set>
#include <filesystem>
#include <regex>

static std::unordered_set<std::string> renamedTypes;

static std::string getRandomString(const size_t len)
{
	std::string s(len, '\0');

	static thread_local std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<unsigned int> dist(0, 0xFF);

	for (size_t i = 0; i < len; ++i) 
	{
		s[i] = static_cast<char>(dist(rng));
	}

	return s;
}

static std::string replaceRTTITypes(const std::string& contents, const std::regex& reg)
{
	std::string result;
	std::sregex_iterator it(contents.begin(), contents.end(), reg);
	std::sregex_iterator end;

	size_t lastPos = 0;

	for (; it != end; ++it) {
		const std::smatch& match = *it;

		std::string prefix = match[1].str();
		std::string typeName = match[2].str();

		auto matchStart = match.position(0);
		auto matchLength = match.length(0);

		result.append(contents, lastPos, matchStart - lastPos);

		size_t length = 1 + prefix.size() + typeName.size() + 2;
		size_t maxNewLength = 3;

		std::string newTypeName;
		do 
		{
			newTypeName = getRandomString(length);
			if (newTypeName.size() > maxNewLength)
			{
				std::fill(newTypeName.begin() + maxNewLength, newTypeName.end(), '\0');
			}
		}
		while (renamedTypes.contains(newTypeName));

		renamedTypes.emplace(newTypeName);

		result.append(newTypeName);
		result.push_back('\0');

		lastPos = matchStart + matchLength;
	}

	result.append(contents, lastPos, std::string::npos);

	return result;
}

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			throw std::exception("No path to binary provided");
		}

		std::string path = argv[1];
		std::ifstream fs(path, std::fstream::binary);
		if (fs.fail())
		{
			throw std::exception("Binary could not be opened");
		}

		std::stringstream ss;
		ss << fs.rdbuf();
		auto contents = ss.str();

		std::regex reg(R"(\.(\?AV|PEAV)(.+?)@@\0)");
		contents = replaceRTTITypes(contents, reg);

		std::filesystem::path p(path);
		auto outputPath = (p.parent_path() / (p.stem().string() + p.extension().string())).string();

		std::ofstream os(outputPath, std::ofstream::trunc | std::ofstream::binary);
		if (!os.write(contents.data(), contents.size()))
		{
			throw std::runtime_error("Could not write to file " + outputPath);
		}

		std::cout << "Successfully obfuscated rtti" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		system("pause");
		return 1;
	}

	return 0;
}