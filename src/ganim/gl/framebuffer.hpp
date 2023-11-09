#ifndef GANIM_GL_FRAMEBUFFER_H
#define GANIM_GL_FRAMEBUFFER_H

namespace ganim::gl {
    class Framebuffer {
        public:
            Framebuffer();
            ~Framebuffer();
            Framebuffer(Framebuffer&&) noexcept;
            Framebuffer(const Framebuffer&)=delete;
            Framebuffer& operator=(Framebuffer&&) noexcept;
            Framebuffer& operator=(const Framebuffer&)=delete;
            operator unsigned() const {return M_id;}

        private:
            unsigned M_id {0};
    };
}

#endif
