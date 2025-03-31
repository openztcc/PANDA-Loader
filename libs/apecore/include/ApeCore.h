#ifndef APECORE_H
#define APECORE_H

// APE.CORE - ZT1 Graphics Parser
// by Eric Galvan (Goosifer.IO)
// https://github.com/openztcc/APE.Core
// Licensed under MIT (see LICENSE)
//
// This library parses ZT1 game sprites and animations. Please feel
// free to use in your own projects and tools.
//
// Credits to Jeff Bostoen for his fantastic documentation on the ZT1 file formats:
// https://github.com/jbostoen/ZTStudio/wiki/ZT1-Graphics-Explained
//
// Credits to the original author of the STB image library (for PNG writing):
// - Sean Barrett (released under public domain/MIT)
// 
// Version 0.6.3

#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <cstring>

#define MAGIC "FATZ"
#define APE_CORE_VERSION "0.6.4"

// if FATZ is first 4 bytes, additional 5 bytes ahead
// The ninth byte is a boolean value that specifies if there is an 
// a background frame 

// -------------------------------- APE Graphic Structures

struct Header
{
    uint32_t speed; // animation speed in ms
    uint32_t palNameSize;
    std::vector<char> palName;
    uint32_t frameCount;
};

struct PixelBlock 
{
    uint8_t offset; // How many transparent pixels before drawing
    uint8_t colorCount; // How many colors in this block
    std::vector<uint8_t> colors; // Color indexes in pal
};

struct PixelSet 
{
    uint8_t blockCount; // How many pixel blocks
    std::vector<PixelBlock> blocks; // The pixel blocks
};

struct Frame
{
    uint32_t frameSize; // in bytes
    uint16_t height;
    uint16_t width;
    int16_t y; // y offset
    int16_t x; // x offset
    uint8_t unk1; // unknown bytes
    uint8_t unk2; // unknown bytes
    std::vector<PixelSet> pixelSets; // The pixel sets  
};

// -------------------------------- PAL Colors

struct Color 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// -------------------------------- Standard Pixel Output

struct OutputBuffer 
{
    uint8_t* pixels; // continuous array of pixels: i.e. {0,0,0,255,255,255,255,...}
    int width;
    int height;
    int offsetX;
    int offsetY;
    int channels;
};

class ApeCore
{
    public: 
        ApeCore();
        virtual ~ApeCore();

        int load(std::string fileName, int colorProfile = 0, std::string ioPal = "");
        int save(std::string fileName);
        int exportToPNG(std::string fileName, OutputBuffer output);
        int getFrameCount();
        OutputBuffer** apeBuffer();
        std::string getPalLocation();
        std::vector<Frame>& getFrames();
        std::vector<Color>& getColors() { return colors; }
        static int validateGraphicFile(std::string fileName);
        static int validatePaletteFile(std::string fileName);
        static int hasBackgroundFrame(std::string fileName);
        // return header info
        static Header getHeader(std::string fileName);

    private:
        int readPal(std::string fileName);
        void writePal(std::string fileName);
        static int hasMagic(std::ifstream &input);
        int writeBuffer();

        std::ifstream input;
        std::ifstream pal;
        OutputBuffer** frameBuffers;
        Header header;
        std::vector<Frame> frames;
        std::vector<std::vector<PixelBlock>> pixelBlocks;
        std::vector<Color> colors;
        bool hasBackground;
        int colorModel;
        std::string palLocation;
    };

#endif // APECORE_H