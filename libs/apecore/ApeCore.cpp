#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"
#include "include/ApeCore.h"

ApeCore::ApeCore()
{
    hasBackground = false;
    header.speed = 0;
    header.palNameSize = 0;
    header.frameCount = 0;
    header.palName = std::vector<char>();
    frames = std::vector<Frame>();
    pixelBlocks = std::vector<std::vector<PixelBlock>>();
    colors = std::vector<Color>();
    input = std::ifstream();
    input.exceptions(static_cast<std::ios_base::iostate>(
        std::ifstream::failbit | std::ifstream::badbit));
    colorModel = 0;
    palLocation = "";

    frameBuffers = new OutputBuffer*[1];
}

ApeCore::~ApeCore()
{ 
    if (input.is_open()) 
    {
        input.close();
    }

    if (pal.is_open()) 
    {
        pal.close();
    }

    // free frame buffers
    int numBuffers = getFrameCount();
    for (int i = 0; i < numBuffers; i++) 
    {
        delete[] frameBuffers[i]->pixels;
        delete frameBuffers[i];
    }

    // free colors
    colors.clear();

    // free frames
    frames.clear();

    // free pixel blocks
    for (std::vector<PixelBlock> &blocks : pixelBlocks) 
    {
        blocks.clear();
    }

    // free header
    header.palName.clear();    
}

OutputBuffer** ApeCore::apeBuffer()
{
    return frameBuffers;
}

std::vector<Frame>& ApeCore::getFrames() 
{
    return frames;
}

int ApeCore::getFrameCount() 
{
    return header.frameCount;
}

std::string ApeCore::getPalLocation() 
{
    return palLocation;
}


int ApeCore::hasMagic(std::ifstream &input)
{
    char magic[5] = {0};
    input.read(magic, 4);

    // std::cout << "\tMagic Bytes: " << magic << std::endl;
    
    // read at least 4 bytes
    // if less than 4 bytes, not FATZ
    if (input.gcount() < 4) 
    {
        input.clear();  
        input.seekg(0, std::ios::beg);
        return 0;
    }

    // test for FATZ
    if (strcmp(magic, MAGIC) != 0) {
        input.clear();
        input.seekg(0, std::ios::beg);
        return 0;
    }

    // FATZ found
    input.clear();
    input.seekg(0, std::ios::beg);
    return 1;
}

int ApeCore::readPal(std::string fileName) 
{
    std::cout << "Reading palette: " << fileName << std::endl;
    
    pal.open(fileName, std::ios::binary);
    if (!pal.is_open()) {
        std::cerr << "ERROR: Could not open palette file: " << fileName << std::endl;
        return -1;
    }

    // Read color count (4 bytes, little-endian)
    uint16_t colorCount = 0;
    pal.read(reinterpret_cast<char*>(&colorCount), 2);

    // Skip 2
    pal.seekg(2, std::ios::cur);
    
    std::cout << "\tColor count: " << colorCount << std::endl;

    // Validate color count
    if (colorCount == 0 || colorCount > 256) {
        std::cerr << "ERROR: Invalid color count: " << colorCount << std::endl;
        pal.close();
        return -2;
    }

    // Clear and prepare colors vector
    colors.clear();
    colors.reserve(256);  // Always ensure 256 colors

    // Read each color (ABGR format, 4 bytes per color)
    for (uint32_t i = 0; i < colorCount; i++) 
    {
        // uint32_t abgr;
        // pal.read(reinterpret_cast<char*>(&abgr), 4);

        Color color;
        pal.read(reinterpret_cast<char*>(&color), 4);

        // Convert RGBA to BGRA if necessary
        if (colorModel == 1) {
            std::swap(color.r, color.b);
        }


        colors.push_back(color);

        // Debug output
        // std::cout << "\tColor " << i << ": R=" << static_cast<int>(color.r) 
        //           << " G=" << static_cast<int>(color.g) 
        //           << " B=" << static_cast<int>(color.b) 
        //           << " A=" << static_cast<int>(color.a) 
        //         //   << " (Raw ARGB: " << std::hex << color.a << color.r << color.g << color.b << std::dec << ")"
        //           << std::endl;
    }

    pal.close();

    // Fill remaining colors if necessary
    while (colors.size() < 256) {
        colors.push_back({0, 0, 0, 255});  // Fill with black (fully opaque)
    }

    return 1;
}


int ApeCore::writeBuffer() 
{
    if (frames.empty()) {
        return 0;
    }

    int numBuffers = getFrameCount();
    frameBuffers = new OutputBuffer*[numBuffers];

    for (Frame &frame : frames) 
    {
        int index = &frame - &frames[0];
        OutputBuffer output = OutputBuffer();

        // Set dimensions and format
        output.width = static_cast<int>(frame.width);
        output.height = static_cast<int>(frame.height);
        output.offsetX = static_cast<int>(frame.x);
        output.offsetY = static_cast<int>(frame.y);
        output.channels = 4;  // RGBA/BGRA
        
        // Calculate buffer size and initialize with transparent pixels
        size_t bufferSize = output.width * output.height * output.channels;
        output.pixels = new uint8_t[bufferSize];
        for (size_t i = 0; i < bufferSize; i += 4) {
            output.pixels[i] = 0;     // R
            output.pixels[i + 1] = 0; // G
            output.pixels[i + 2] = 0; // B
            output.pixels[i + 3] = 0; // A (Fully transparent)
        }

        // Process each row
        for (int row = 0; row < frame.height; row++) 
        {
            if (row >= frame.pixelSets.size()) {
                std::cerr << "ERROR: Row " << row << " exceeds pixelSet count!" << std::endl;
                continue;
            }

            PixelSet &pixelSet = frame.pixelSets[row];
            int xPos = 0;  // Reset horizontal position for each new row

            // Process each block in the row
            for (PixelBlock &pixelBlock : pixelSet.blocks) 
            {
                // Apply offset from current position
                xPos += pixelBlock.offset;

                // Skip if this is an end-of-line block or empty block
                if (pixelBlock.colorCount == 0) {
                    continue;
                }

                // Process each color in the block
                for (uint8_t colorIndex : pixelBlock.colors) 
                {
                    // Bounds checking
                    if (xPos >= output.width) {
                        break;  // Stop if we've exceeded the width
                    }

                    // Validate color index
                    if (colorIndex >= colors.size()) {
                        std::cerr << "ERROR: Out-of-bounds color index! (" 
                                 << (int)colorIndex << ")" << std::endl;
                        continue;
                    }

                    // Calculate pixel position in buffer
                    size_t pixelIndex = (row * output.width + xPos) * output.channels;
                    
                    // Ensure we don't write outside the buffer
                    if (pixelIndex + 3 >= bufferSize) {
                        std::cerr << "ERROR: Buffer overflow prevented at position " 
                                 << pixelIndex << std::endl;
                        break;
                    }

                    // Get color from palette
                    Color &color = colors[colorIndex];

                    // Write pixel data according to color model
                    if (colorModel == 1) {  // BGRA mode
                        output.pixels[pixelIndex] = color.b;
                        output.pixels[pixelIndex + 1] = color.g;
                        output.pixels[pixelIndex + 2] = color.r;
                        output.pixels[pixelIndex + 3] = color.a;
                    } else {  // RGBA mode
                        output.pixels[pixelIndex] = color.r;
                        output.pixels[pixelIndex + 1] = color.g;
                        output.pixels[pixelIndex + 2] = color.b;
                        output.pixels[pixelIndex + 3] = color.a;
                    }
                    
                    xPos++;  // Move to next horizontal position
                }
            }
        }
        
        // Store the completed buffer
        frameBuffers[index] = new OutputBuffer(output);
    }

    return 1;
}// Color model 0 = RGBA
// Color model 1 = BGRA
int ApeCore::load(std::string fileName, int colorModel, std::string ioPal)
{
    this->colorModel = colorModel;

    input.open(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    if (!input.is_open()) {
        return -1;
    }

    // ------------------------------- read header
    // Note: if bg frame exists, not counted in frameCount
    // std::cout << "Header" << std::endl;

    // check if fatz
    if (hasMagic(input)) {
        // skip 8 bytes
        input.seekg(8, std::ios::cur);
        // read 9th byte
        input.read((char*)&hasBackground, 1);
        // std::cout << "\tType: is fatz" << std::endl;
        // std::cout << "\thasBackground: " << hasBackground << std::endl;
    } else {
        // std::cout << "\tType: not fatz" << std::endl;
    }

    input.read((char*)&header.speed, 4); // animation speed in ms
    input.read((char*)&header.palNameSize, 4); // size of palette name
    header.palName.resize(header.palNameSize); // resize to size
    input.read(header.palName.data(), header.palNameSize); // read palette name
    input.read((char*)&header.frameCount, 4); // number of frames
    frames.resize(header.frameCount); // resize frames to frame count

    if (ioPal.empty()) 
        palLocation = std::string(header.palName.data());
    else
        palLocation = ioPal;

    if (hasBackground) {
        header.frameCount += 1;
        frames.resize(header.frameCount);
    }

    // print header
    std::cout << "\tspeed: " << header.speed << " ms" << std::endl;
    std::cout << "\tpalNameSize: " << header.palNameSize << " bytes" << std::endl;
    std::cout << "\tpalName: " << header.palName.data() << std::endl;
    std::cout << "\tframeCount: " << header.frameCount << std::endl;
    std::cout << "\tframes: " << frames.size() << std::endl;

    // ------------------------------- read palette
    ApeCore::readPal(palLocation);

    // ------------------------------- read frames
    for (int i = 0; i < header.frameCount; i++) {
        Frame frame;
        input.read((char*)&frame.frameSize, 4);
        input.read((char*)&frame.height, 2);
        input.read((char*)&frame.width, 2);
        input.read((char*)&frame.y, 2);
        input.read((char*)&frame.x, 2);
        input.read((char*)&frame.unk1, 1); // always 0?
        input.read((char*)&frame.unk2, 1); // always 0?

        // read pixel sets
        for (int j = 0; j < frame.height; j++) {
            PixelSet pixelSet;
            input.read((char*)&pixelSet.blockCount, 1); // how many pixel blocks
            pixelSet.blocks.resize(pixelSet.blockCount); // resize to block count
            for (int k = 0; k < pixelSet.blockCount; k++) { // read each block
                PixelBlock block;
                input.read((char*)&block.offset, 1); // offset
                input.read((char*)&block.colorCount, 1); // color count
                block.colors.resize(block.colorCount); // resize to color count
                input.read((char*)block.colors.data(), block.colorCount); // colors
                pixelSet.blocks[k] = block; // store block
            }

            // // TODO: test issues that might arise from this
            // // Possible issue: skips some pixels and leaves them blank
            // if (pixelSet.blockCount == 0) {
            //     pixelSet.blocks.push_back(PixelBlock{0, 0, std::vector<uint8_t>()});
            // }
            frame.pixelSets.push_back(pixelSet); // store pixel set
        }

        // store frame
        frames[i] = frame;

        // print frame
        // std::cout << "Frame " << i << std::endl;
        // std::cout << "\tframeSize: " << frame.frameSize << " bytes" << std::endl;
        // std::cout << "\theight: " << (int)frame.height << " px" << std::endl;
        // std::cout << "\twidth: " << (int)frame.width << " px" << std::endl;
        // std::cout << "\ty: " << (int)frame.y << std::endl;
        // std::cout << "\tx: " << (int)frame.x << std::endl;
        // std::cout << "\tunk1: " << (int)frame.unk1 << std::endl;
        // std::cout << "\tunk2: " << (int)frame.unk2 << std::endl;
        // std::cout << "\tpixelSets: " << frame.pixelSets.size() << std::endl;
        // for (int j = 0; j < frame.pixelSets.size(); j++) {
        //     std::cout << "\t\tpixelSet " << j << std::endl;
        //     std::cout << "\t\t\tblockCount: " << (int)frame.pixelSets[j].blockCount << std::endl;
        //     for (int k = 0; k < frame.pixelSets[j].blocks.size(); k++) {
        //         std::cout << "\t\t\tblock " << k << std::endl;
        //         std::cout << "\t\t\t\toffset: " << (int)frame.pixelSets[j].blocks[k].offset << std::endl;
        //         std::cout << "\t\t\t\tcolorCount: " << (int)frame.pixelSets[j].blocks[k].colorCount << std::endl;
        //         std::cout << "\t\t\t\tcolors: ";
        //         for (int l = 0; l < frame.pixelSets[j].blocks[k].colorCount; l++) {
        //             std::cout << (int)frame.pixelSets[j].blocks[k].colors[l] << " ";
        //         }
        //         std::cout << std::endl;
        //     }
        // }
    }

    input.close();

    // write output buffer
    if (ApeCore::writeBuffer() > 0) {
        std::cout << "Wrote output buffer" << std::endl;
    } else if (ApeCore::writeBuffer() == 0) {
        std::cout << "No frames to write" << std::endl;
    } else {
        std::cout << "Failed to write output buffer" << std::endl;
    }

    return 1;
}

int ApeCore::save(std::string fileName)
{
    std::ofstream output(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::out));
    if (!output.is_open()) {
        return -1;
    }

    if (hasBackground) 
    {
        // WRITE: FATZ
        output.write(MAGIC, 4);
    }

    // -------------------------------------- write header
    output.write((char*)&header.speed, 4); // speed in ms
    output.write((char*)&header.palNameSize, 4); // size of palette name
    output.write(header.palName.data(), header.palNameSize); // palette name
    output.write((char*)&header.frameCount, 4); // frame count

    // write frames
    // TODO: write a Buffer reader and convert to frames and pal
    for (Frame &frame : frames) {
        output.write((char*)&frame.frameSize, 4);
        output.write((char*)&frame.height, 2);
        output.write((char*)&frame.width, 2);
        output.write((char*)&frame.x, 2);
        output.write((char*)&frame.y, 2);
        output.write((char*)&frame.unk1, 1);
        output.write((char*)&frame.unk2, 1);

        // write pixel sets
        for (PixelSet &pixelSet : frame.pixelSets) {
            output.write((char*)&pixelSet.blockCount, 1);
            for (PixelBlock &block : pixelSet.blocks) {
                output.write((char*)&block.offset, 1);
                output.write((char*)&block.colorCount, 1);
                output.write((char*)block.colors.data(), block.colorCount);
            }
        }
    }

    output.close();

    // write palette
    // ApeCore::writePal(palLocation);

    return 1;
}

// Does a simple validation to see if file is valid APE graphic
// 1. Checks if file is open = Not valid
// 2. Checks if first 4 bytes are FATZ = Valid
// 3. Checks if palette name is empty = Not valid
// 4. Checks if palette name has '.pal' extension = Valid
int ApeCore::validateGraphicFile(std::string fileName) 
{
    std::ifstream graphic(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    int isValid = 0;
    Header hdr;
    
    // if file is not open, return false
    if (!graphic.is_open()) {
        // immediately return false if file is not open
        return 0;
    }

    // if has magic bytes FATZ
    if (hasMagic(graphic)) {
        isValid = 1;

        // skip 9 bytes
        graphic.seekg(9, std::ios::cur);
    }
    // if no magic bytes, it means first 9 bytes don't exist
    // following checks are not necessary:
    // - MAGIC BYTES - 4 bytes
    // - UNKNOWN NULL BYTES - 4 bytes
    // - HAS BACKGROUND - 1 byte
    // does not mean file is invalid, just not a FATZ file or ZTAF (reverse FATZ)

    graphic.read((char*)&hdr.speed, 4); // speed in ms
    graphic.read((char*)&hdr.palNameSize, 4); // size of palette name
    hdr.palName.resize(hdr.palNameSize); // resize to size
    graphic.read(hdr.palName.data(), hdr.palNameSize); // read palette name
    graphic.read((char*)&hdr.frameCount, 4); // frame count

    // if pal name is empty, return false
    if (hdr.palName.empty() || hdr.palNameSize == 0 || hdr.palNameSize < 0) {
        // if no palette exists then immediately return false
        return 0;
    }

    std::string palette(hdr.palName.data());

    // if pal name has '.pal' extension, return true
    if (palette.find(".pal") != std::string::npos) {
        isValid = 1;
    }

    graphic.close();
    return isValid;
}

// Does a simple validation to see if file is valid APE palette
// Not a comprehensive check, just a quick validation of the first few bytes
// Loading in the palette later can return early if the rest is not valid
int ApeCore::validatePaletteFile(std::string fileName) 
{
    std::ifstream palette(fileName, static_cast<std::ios_base::openmode>(std::ios::binary | std::ios::in));
    bool isValid = false;
    // if (!palette.is_open()) {
    //     return 0;
    // }

    // // Get header info
    // Header hdr = getHeader(fileName);

    // // if pal name is empty, return false
    // if (hdr.palName.empty() || hdr.palNameSize == 0 || hdr.palNameSize < 0) {
    //     // if no palette exists then immediately return false
    //     return 0;
    // }

    // // Make sure file has .pal extension
    // std::string paletteName(hdr.palName.data());
    // if (paletteName.find(".pal") == std::string::npos) {
    //     isValid = 1;
    // } 
    // else {
    //     return 0;
    // }

    // // Read color count (4 bytes, little-endian)
    // uint16_t colorCount = 0;
    // palette.read(reinterpret_cast<char*>(&colorCount), 2);

    // // Skip 2
    // palette.seekg(2, std::ios::cur);
    
    // // Validate color count
    // if (colorCount < 0 || colorCount > 256) {
    //     palette.close();
    //     return isValid;
    // }

    isValid = true;
    palette.close();
    return isValid;
}

int ApeCore::hasBackgroundFrame(std::string fileName) 
{
    std::ifstream graphic(fileName, std::ios::binary);
    if (!graphic.is_open()) {
        return 0;
    }

    // if has magic bytes FATZ
    if (hasMagic(graphic)) {
        // skip 9 bytes
        graphic.seekg(8, std::ios::cur);
    }
    else {
        graphic.close();
        return 0;
    }
    bool hasBackground = false;
    graphic.read((char*)&hasBackground, 1);
    graphic.close();
    return hasBackground;
}

int ApeCore::exportToPNG(std::string fileName, OutputBuffer output)
{
    if (!output.pixels) {
        std::cerr << "No pixels to write" << std::endl;
        return -1;
    }

    int stride = output.width * output.channels;
    if (!stbi_write_png(fileName.c_str(), output.width, output.height, output.channels, output.pixels, stride)) {
        std::cerr << "Failed to write image" << std::endl;
        return -2;
    } else {
        std::cout << "Wrote image" << std::endl;
    }

    return 1;
}

Header ApeCore::getHeader(std::string fileName) 
{
    std::ifstream graphic(fileName, std::ios::binary);
    Header hdr;
    if (!graphic.is_open()) {
        return hdr;
    }

    // if has magic bytes FATZ
    if (hasMagic(graphic)) {
        // skip 9 bytes
        graphic.seekg(9, std::ios::cur);
    }

    graphic.read((char*)&hdr.speed, 4); // speed in ms
    graphic.read((char*)&hdr.palNameSize, 4); // size of palette name
    hdr.palName.resize(hdr.palNameSize); // resize to size
    graphic.read(hdr.palName.data(), hdr.palNameSize); // read palette name
    graphic.read((char*)&hdr.frameCount, 4); // frame count

    graphic.close();
    return hdr;
}

