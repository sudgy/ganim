#ifndef GANIM_SCRIPT_EXCEPTION_HPP
#define GANIM_SCRIPT_EXCEPTION_HPP

#include <stdexcept>
#include <format>

namespace ganim {
    inline std::string get_compile_error_message(
            int line, int column, std::string_view actual_what)
    {
        if (line == -1) {
            return std::format(
                    "Error at end of file: {}",
                    actual_what);
        }
        else {
            return std::format(
                    "Error on line {} column {}: {}",
                    line + 1, column + 1, actual_what);
        }
    }
    class CompileError : public std::exception {
        public:
            CompileError(int line, int column, std::string_view actual_what)
            {
                M_what = get_compile_error_message(line, column,actual_what);
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
