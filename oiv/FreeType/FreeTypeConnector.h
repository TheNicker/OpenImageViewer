#pragma once
#include "FreeTypeHeaders.h"


#if OIV_BUILD_FREETYPE == 1
#include "FreeTypeFont.h"
#include <map>
#include <cstdint>
#include <string>
#include <vector>
#include <Color.h>
#include <Buffer.h>
#include <Singleton.h>


class FreeTypeConnector : public LLUtils::Singleton<FreeTypeConnector>
{
    friend class LLUtils::Singleton<FreeTypeConnector>;
public:
    ~FreeTypeConnector();

    struct Bitmap
    {
        uint32_t width;
        uint32_t height;
        LLUtils::Buffer buffer;
        uint32_t PixelSize;
        uint32_t rowPitch;
    };
    
    enum class RenderMode
    {
          Default
        , Antialiased
        , SubpixelAntiAliased
    };

    struct TextCreateParams
    {
        std::string fontPath;
        std::string text;
        uint16_t fontSize;
        LLUtils::Color backgroundColor;
        LLUtils::Color outlineColor;
        uint32_t outlineWidth;
        RenderMode renderMode;
        uint16_t DPIx;
        uint16_t DPIy;
    };

    void CreateBitmap(const TextCreateParams& textCreateParams, Bitmap & out_bitmap);


private:
       // private types
    struct TextMesureParams
    {
        TextCreateParams createParams;
        FreeTypeFont* font;
    };

    struct TextMesureResult
    {
        uint32_t width;
        uint32_t height;
        uint32_t rowHeight;
        uint32_t baselineHeight;
    };
    //private member methods

    FreeTypeConnector();
    FreeTypeFont* GetOrCreateFont(std::string fontPath);
    std::string GenerateFreeTypeErrorString(std::string userMessage, FT_Error error);
    void MesaureText(const TextMesureParams& measureParams, TextMesureResult& out_result);
    

private:
    FT_Library  fLibrary;
    std::map<std::string, FreeTypeFontUniquePtr> fFontNameToFont;
    
};

#endif
