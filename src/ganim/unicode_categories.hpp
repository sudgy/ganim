#ifndef GANIM_SCRIPT_UNICODE_CATEGORIES_HPP
#define GANIM_SCRIPT_UNICODE_CATEGORIES_HPP

#include <unordered_set>
#include <cstdint>

namespace ganim {
extern const std::unordered_set<std::uint32_t> C_xid_start;
extern const std::unordered_set<std::uint32_t> C_xid_continue;
extern const std::unordered_set<std::uint32_t> unicode_letter;
extern const std::unordered_set<std::uint32_t> C_tex_op;
extern const std::unordered_set<std::uint32_t> C_tex_bin;
extern const std::unordered_set<std::uint32_t> C_tex_rel;
extern const std::unordered_set<std::uint32_t> C_tex_open;
extern const std::unordered_set<std::uint32_t> C_tex_close;
extern const std::unordered_set<std::uint32_t> C_tex_punct;
}

#endif
