#define _CRT_SECURE_NO_WARNINGS
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <iostream>

constexpr int channelsNum = STBI_rgb_alpha;

void Image::AddVerticalPadding(const int windowWidth, const int windowHeight)
{
	const int padding = (width * windowHeight - windowWidth * height) / (2 * windowWidth);

	const int paddingVectorSize = width * padding * channels;

	std::vector<unsigned char> newData(2 * paddingVectorSize, 0);

	newData.insert(newData.begin() + paddingVectorSize, data.cbegin(), data.cend());

	height = height + 2 * padding;

	data = newData;
}

void Image::AddHorizontalPadding(const int windowWidth, const int windowHeight)
{
	const int padding = (windowWidth * height - width * windowHeight) / (2 * windowHeight);

	const int paddingVectorWidth = padding * channels;

	const int paddedImageWidth = (width + 2 * padding) * channels;

	const int vectorSizeWithPadding = paddedImageWidth * height;

	std::vector<unsigned char> newData(vectorSizeWithPadding, 0);

	for (size_t row{ 0 }; row < height; row++)
	{
		for (size_t column{ 0 }; column < width * channels; column++)
		{
			newData.at(column + paddingVectorWidth + paddedImageWidth * row) = data.at(column + width * channels * row);
		}
	}

	width = width + 2 * padding;

	data = newData;
}

Image::Image(const std::string& imagePath) : width(0), height(0), channels(0), data({})
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* stbiData = stbi_load(imagePath.c_str(), &width, &height, &channels, channelsNum);

	channels = channelsNum;

	if (stbiData == nullptr)
	{
		std::cout << "Couldn't load image: " << imagePath << std::endl;
		return;
	}

	size_t dataSize = width * height * channels;

	data = std::vector<unsigned char>(stbiData, stbiData + dataSize);

	stbi_image_free(stbiData);
}

Image::Image(const std::string& imagePath, const int windowWidth, const int windowHeight) : Image(imagePath)
{
	if (!IsImageProper())
	{
		return;
	}

	double windowRatio = static_cast<double>(windowWidth) / static_cast<double>(windowHeight);

	double imageRatio = static_cast<double>(width) / static_cast<double>(height);

	if (windowRatio >= imageRatio)
	{
		AddHorizontalPadding(windowWidth, windowHeight);
	}
	else
	{
		AddVerticalPadding(windowWidth, windowHeight);
	}
}

Image::Image(int width, int height, int channels) : width(width), height(height), channels(channels)
{
	size_t dataSize = width * height * channels;

	data = std::vector<unsigned char>(dataSize);
}

void Image::Save(const std::string& imagePath)
{
	stbi_flip_vertically_on_write(true);

	int result = stbi_write_png(imagePath.c_str(), width, height, channels, data.data(), width * channels);

	if (result == 0)
	{
		std::cout << "Couldn't save image " << imagePath << std::endl;
	}
}

int Image::Width() const
{
	return width;
}

int Image::Height() const
{
	return height;
}

int Image::Channels() const
{
	return channels;
}

const std::vector<unsigned char>& Image::Data() const
{
	return data;
}

std::vector<unsigned char>& Image::Data()
{
	return data;
}

bool Image::IsImageProper() const
{
	if (width == 0 || height == 0)
	{
		return false;
	}

	return true;
}
