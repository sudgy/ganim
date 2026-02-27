
using namespace $NAMESPACE;

$CLASS_NAME::$CLASS_NAME()
{
    M_states.emplace_back(0, std::monostate());
}

bool $CLASS_NAME::push(token_type token)
{
    if (M_finished) return false;
    auto s = M_states.back().state_id;
    auto a = token.token_id;
    auto encoded = (std::uint64_t(s) << 32) + std::uint64_t(a);
    switch (encoded) {
        $STATE_ACTION
        default:
            if (token.token_id == -1) {
                // Do not let this exception bubble up
                throw std::invalid_argument(
                    "This exception should have been caught and a new one thrown "
                    "with a better error message.  There was a syntax error "
                    "somewhere but I have no idea where it is."
                );
            }
            else return push(end_token());
    }
}

$CLASS_NAME::result_type $CLASS_NAME::take_result()
{
    return std::move(get<result_type>(M_states.back().value));
}

$TOKEN_FUNC_DEFS

$SYMBOL_TYPE $CLASS_NAME::convert_to_symbol_type($TOKEN_TYPE token)
{
    return std::visit([](auto&& token) {return $SYMBOL_TYPE(token);}, token);
}
