#ifndef GANIM_SCRIPT_EXCEPTION_HPP
#define GANIM_SCRIPT_EXCEPTION_HPP

#include <stdexcept>
#include <format>

namespace ganim {
    class ScriptException : public std::exception {
        public:
            ScriptException(int line, int column, std::string_view actual_what)
            {
                if (line == -1) {
                    M_what = std::format(
                            "Error at end of file: {}",
                            actual_what);
                }
                else {
                    M_what = std::format(
                            "Error on line {} column {}: {}",
                            line + 1, column + 1, actual_what);
                }
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
