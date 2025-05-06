#pragma once
#include <GL/glew.h>
#include <stb_truetype.h>
#include <fstream>
#include <vector>
#include <string>

struct FontChar {
    float ax; // advance.x
    float bx; // bitmap left
    float by; // bitmap top
    float bw; // bitmap w
    float bh; // bitmap h
    float tx; // x offset in texture
    float ty; // y offset in texture
};

class TextRenderer {
public:
    stbtt_bakedchar cdata[96]; // ASCII 32..126
    GLuint fontTex;

    bool init(const std::string& font_path, int font_size = 24) {
        std::ifstream fontFile(font_path, std::ios::binary | std::ios::ate);
        if (!fontFile.is_open()) {
            std::cerr << "Failed to open font file: " << font_path << std::endl;
            return false;
        }
    
        std::streamsize size = fontFile.tellg();
        fontFile.seekg(0, std::ios::beg);
    
        std::vector<unsigned char> fontBuffer(size);
        if (!fontFile.read((char*)fontBuffer.data(), size)) {
            std::cerr << "Failed to read font file: " << font_path << std::endl;
            return false;
        }
    
        std::vector<unsigned char> bitmap(512 * 512);
        int result = stbtt_BakeFontBitmap(fontBuffer.data(), 0, font_size, bitmap.data(), 512, 512, 32, 96, cdata);
        if (result <= 0) {
            std::cerr << "Failed to bake font bitmap." << std::endl;
            return false;
        }
    
        glGenTextures(1, &fontTex);
        glBindTexture(GL_TEXTURE_2D, fontTex);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 512, 512, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, bitmap.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
        return true;
    }
    

    void draw_text(float ndc_x, float ndc_y, const std::string& text, float screen_w, float screen_h) {
        float px = (ndc_x + 1.0f) * 0.5f * screen_w;
        float py = (ndc_y + 1.0f) * 0.5f * screen_h;
    
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, fontTex);
        glBegin(GL_QUADS);
        for (char c : text) {
            if (c < 32 || c > 126) continue;
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &px, &py, &q, 1);
    
            float x0 =  q.x0 / screen_w  * 2.0f - 1.0f;
            float y0 = 1.0f - (q.y0 / screen_h * 2.0f);
            float x1 =  q.x1 / screen_w  * 2.0f - 1.0f;
            float y1 = 1.0f - (q.y1 / screen_h * 2.0f);
    
            glTexCoord2f(q.s0, q.t0); glVertex2f(x0, y0);
            glTexCoord2f(q.s1, q.t0); glVertex2f(x1, y0);
            glTexCoord2f(q.s1, q.t1); glVertex2f(x1, y1);
            glTexCoord2f(q.s0, q.t1); glVertex2f(x0, y1);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }


    
};
