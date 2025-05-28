#ifndef GANIM_GEX_ERROR_HPP
#define GANIM_GEX_ERROR_HPP

#include <format>

namespace ganim::gex {
    class GeXError : public std::exception {
        public:
            GeXError(int group, int index, std::string_view actual_what)
            {
                M_what = std::format(
                    "GeX compilation error in group {} index {}: {}",
                    group, index, actual_what
                );
            }
            virtual const char* what() const noexcept
            {
                return M_what.c_str();
            }
        private:
            std::string M_what;
    };
}

#endif
