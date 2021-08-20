//
// Created by Will Chambers on 4/15/21.
//
#include "willow/rendering/Texture.hpp"
#define  STB_IMAGE_IMPLEMENTATION
#include"willow/external/stb_image.h"
#include"willow/external/stb_truetype.h"
namespace wlo{


    Texture::Texture(std::string loadFile) {
        int texWidth,texHeight,texChannels;
        stbi_uc* pixels = stbi_load(loadFile.c_str(), &texWidth, &texHeight, &texChannels, 0);
        m_width = texWidth;
        m_height = texHeight;
        m_depth = texChannels;
        if(pixels==nullptr){
            throw std::runtime_error("Failed to load image from file: "+loadFile);
        }

        switch(texChannels) {
            case 4 : {
                m_storage = CreateUniquePointer<Storage<wlo::Pixel4>>(texWidth, texHeight, 1);
                format = data::typeOf<wlo::Pixel4 >();
                m_storage->fill(pixels);
                break;
            }
            case 3 : {
                m_storage = CreateUniquePointer<Storage<wlo::Pixel4>>(texWidth, texHeight, 1);
                format = data::typeOf<wlo::Pixel4 >();
                m_depth =4;
                std::vector<wlo::Pixel3> truePixels(texWidth * texHeight);
                memcpy(truePixels.data(), pixels, truePixels.size() * sizeof(truePixels[0]));
                size_t i = 0;
                std::vector<wlo::Pixel4> castedPixels(texWidth * texHeight);
                for (wlo::Pixel3 pixel : truePixels) {
                    castedPixels[i++] = {pixel.r,pixel.g,pixel.b,1};
                }
                m_storage->fill(castedPixels.data());
                break;
            }
            case 2 : {
                m_storage = CreateUniquePointer<Storage<wlo::Pixel2>>(texWidth, texHeight, 1);
                format = data::typeOf<wlo::Pixel2 >();
                m_storage->fill(pixels);
                break;
            }
            case 1 : {
                m_storage = CreateUniquePointer<Storage<wlo::Pixel1>>(texWidth, texHeight, 1);
                format = data::typeOf<wlo::Pixel1 >();
                m_storage->fill(pixels);
                break;
            }
            default:
                throw std::runtime_error("Unsupported Pixel Format! supported pixel format have between 1 and 4 8bit channels, the texture from "+loadFile +" had "+std::to_string(texChannels));
        }


        //free the original copy of the pixels
        stbi_image_free(pixels);

    }

    Texture::Texture(data::Type t, size_t x, size_t y, size_t z):m_width(x),m_height(y),m_depth(z) {
        if(t==data::typeOf<wlo::Pixel4>())
            m_storage = CreateUniquePointer<Storage<wlo::Pixel4>>(x,y,z);
        else if(t==data::typeOf<wlo::Pixel3>())
            m_storage = CreateUniquePointer<Storage<wlo::Pixel3>>(x,y,z);
        else if(t==data::typeOf<wlo::Pixel2>())
            m_storage = CreateUniquePointer<Storage<wlo::Pixel2>>(x,y,z);
        else if(t==data::typeOf<wlo::Pixel1>())
            m_storage = CreateUniquePointer<Storage<wlo::Pixel1>>(x,y,z);
        else
            throw std::runtime_error("Unsupported Pixel Format! for "+t.name());
    }

    View Texture::view()const {
        return m_storage->view();
    }

    void* Texture::data() {
        return m_storage->data();
    }

    data::Type Texture::texelFormat() const{
        return m_storage->view().layout;
    }

    size_t Texture::size() const{
        return m_storage->size();
    }

    size_t Texture::width() const{
        return m_width;
    }

    size_t Texture::height() const{
        return m_height;
    }

    size_t Texture::depth() const{
        return m_depth;
    }
   

}
