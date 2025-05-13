#pragma once
#include <vector>
#include <string>

class Image
{
private:
	int width;
	int height;
	int channels;
	std::vector<unsigned char> data;

	void AddVerticalPadding(const int windowWidth, const int windowHeight);
	void AddHorizontalPadding(const int windowWidth, const int windowHeight);
public:
	Image(const std::string& imagePath);
	Image(const std::string& imagePath, const int windowWidth, const int windowHeight);
	Image(int width, int height, int channels);

	void Save(const std::string& imagePath);

	int Width() const;
	int Height() const;
	int Channels() const;
	const std::vector<unsigned char>& Data() const;
	std::vector<unsigned char>& Data();

	bool IsImageProper() const;
};

