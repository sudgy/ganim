#ifndef GANIM_GEX_MACRO_HPP
#define GANIM_GEX_MACRO_HPP

#include <memory>
#include <vector>
#include <unordered_map>

#include "token.hpp"

namespace ganim::gex {
    class Preprocessor;

    struct Macro {
        TokenList delimiters;
        TokenList replacement_text;
        bool output_directly = false;
    };

    class MacroStack {
        public:
            MacroStack();
            void push();
            void pop();
            const Macro& get_macro(const std::u32string& name);
            void add_macro(const std::u32string& name, Macro macro);

            static void add_base_macros(std::string_view input);

        private:
            using Frame = std::unordered_map<std::u32string, Macro>;
            std::vector<Frame> M_frames;

            inline static Frame S_base_frame;
            inline static bool S_making_base_frame = false;
            static std::unique_ptr<Preprocessor> S_base_preprocessor;
    };
}

#endif
