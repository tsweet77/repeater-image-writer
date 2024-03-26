/*
Intention Repeater Image Writer v2.1 (by Claude 3 Opus and Anthro Teacher)
Writes an intention to a 1920x1080 BMP.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <random>

struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

void writeBytes(std::ofstream& file, const void* data, size_t size) {
    file.write(static_cast<const char*>(data), size);
}

void writeInt32(std::ofstream& file, int32_t value) {
    unsigned char bytes[4];
    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
    writeBytes(file, bytes, 4);
}

void writeBMPHeader(std::ofstream& file, int width, int height) {
    const int headerSize = 54;
    const int imageSize = width * height * 3;
    const int fileSize = headerSize + imageSize;

    // Write BMP header
    file.put(0x42); // 'B'
    file.put(0x4D); // 'M'
    writeInt32(file, fileSize);
    writeInt32(file, 0); // Reserved
    writeInt32(file, headerSize);
    writeInt32(file, 40); // Info header size
    writeInt32(file, width);
    writeInt32(file, height);
    writeInt32(file, 0x00180001); // 24 bits per pixel
    writeInt32(file, 0); // No compression
    writeInt32(file, imageSize);
    writeInt32(file, 0); // Horizontal resolution
    writeInt32(file, 0); // Vertical resolution
    writeInt32(file, 0); // Number of colors
    writeInt32(file, 0); // Important colors
}

RGB getColorForChar(char c, int lowestASCII, int range, int rRand, int gRand, int bRand) {
    int asciiValue = static_cast<int>(c);
    
    // Calculate the normalized value within the range
    double normalizedValue = static_cast<double>(asciiValue - lowestASCII) / range;
    
    // Generate RGB values based on the normalized value and random values
    unsigned char r = static_cast<unsigned char>(std::round(normalizedValue * rRand));
    unsigned char g = static_cast<unsigned char>(std::round(normalizedValue * gRand));
    unsigned char b = static_cast<unsigned char>(std::round(normalizedValue * bRand));
    
    return {r, g, b};
}

int main() {
    std::string intention;
    std::cout << "Enter Intention: ";
    std::getline(std::cin, intention);

    std::string filename;
    std::cout << "Filename (.bmp): ";
    std::getline(std::cin, filename);

    // Add ".png" extension if not already present
    if (filename.substr(filename.size() - 4) != ".bmp") {
        filename += ".bmp";
    }

    int highestASCII = *std::max_element(intention.begin(), intention.end());
    int lowestASCII = *std::min_element(intention.begin(), intention.end());
    int range = highestASCII - lowestASCII;

    // Generate random values for R, G, and B
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);

    int rRand = distrib(gen);
    int gRand = distrib(gen);
    int bRand = distrib(gen);

    std::vector<RGB> pixels;
    for (char c : intention) {
        RGB color = getColorForChar(c, lowestASCII, range, rRand, gRand, bRand);
        pixels.push_back(color);
    }

    const int width = 1920;
    const int height = 1080;

    std::ofstream bmpFile(filename, std::ios::binary);
    if (!bmpFile) {
        std::cout << "Failed to create the BMP file." << std::endl;
        return 1;
    }

    writeBMPHeader(bmpFile, width, height);

    int pixelIndex = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            RGB pixel = pixels[pixelIndex];
            writeBytes(bmpFile, &pixel, sizeof(RGB));
            pixelIndex = (pixelIndex + 1) % pixels.size();
        }
    }

    bmpFile.close();

    std::cout << filename << " written." << std::endl;

    return 0;
}