#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class ImageBase {
public:
    ImageBase();
    virtual ~ImageBase();
    bool load(SDL_Renderer* renderer);
    SDL_Texture* getTexture() const;

protected:
    virtual std::string getFilePath() const = 0;

private:
    SDL_Texture* m_texture;
};

class Image : public ImageBase {
public:
    Image(const std::string& filePath);
    std::string getFilePath() const override;

private:
    std::string m_filePath;
};

#endif // IMAGE_H