#include "Image.h"
#include "Config.h"
#include <iostream>

ImageBase::ImageBase(): m_texture(nullptr) {}

ImageBase::~ImageBase() {
    DEBUG("ImageBase Destructor\n");
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}

// Should be called By Game::init() 
bool ImageBase::load(SDL_Renderer* renderer) {
    std::string filePath = getFilePath();
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }

    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!m_texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

SDL_Texture* ImageBase::getTexture() const {
    return m_texture;
}


Image::Image(const std::string& filePath): m_filePath(filePath) {}

std::string Image::getFilePath() const {
    return m_filePath;
}