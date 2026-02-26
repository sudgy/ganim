#ifndef GANIM_SCRIPT_BYTECODE_INTERPRETER_HPP
#define GANIM_SCRIPT_BYTECODE_INTERPRETER_HPP

#include <vector>
#include <variant>
#include <cstdint>

namespace ganim {
    class Interpreter {
        public:
            explicit Interpreter(std::vector<std::byte> code);
            void execute();
            // There is going to be a lot more stuff in the future related to
            // jumping around in the state and executing only certain sections

            using TestType = std::variant<
                std::byte,
                std::int64_t,
                std::uint64_t,
                double
            >;
            const std::vector<TestType>& get_test_output() const
                {return M_test_output;}

        private:
            std::vector<std::byte> M_code;
            std::vector<std::uint64_t> M_stack;
            std::size_t M_program_counter = 0;
            std::size_t M_stack_frame = 0;
            std::size_t M_stack_top = 0;

            std::byte read_byte_parameter();
            std::int64_t read_int_parameter();
            std::uint64_t read_uint_parameter();
            double read_double_parameter();

            std::vector<TestType> M_test_output;
    };
}

#endif
