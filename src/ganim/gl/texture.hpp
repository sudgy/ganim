#ifndef GANIM_GL_TEXTURE_H
#define GANIM_GL_TEXTURE_H

namespace ganim::gl {
    class Texture {
        public:
            Texture();
            ~Texture();
            Texture(Texture&&) noexcept;
            Texture(const Texture&)=delete;
            Texture& operator=(Texture&&) noexcept;
            Texture& operator=(const Texture&)=delete;
            operator unsigned() const {return M_id;} //NOLINT

        private:
            unsigned M_id {0};
    };
}

#endif
