#include "renderer.h"
#include "image.h"
#include "config.h"
#include <iostream>

Renderer::Renderer(SDL_Window* window)
    : m_window(window), m_renderer(nullptr) {}

Renderer::~Renderer() {
    DEBUG("Renderer Destructor\n");
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

bool Renderer::init() {
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::present() {
    SDL_RenderPresent(m_renderer);
}

void Renderer::renderCopyImage(const Image& image, int x, int y, int w, int h) {
    SDL_Rect destRect = { x, y, w, h };
    SDL_RenderCopy(m_renderer, image.getTexture(), nullptr, &destRect);
}

SDL_Renderer* Renderer::getSDLRenderer() const {
    return m_renderer;
}