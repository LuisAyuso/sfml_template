



namespace myapp {

struct no_copy {
    no_copy()                = default;
    no_copy(const no_copy &) = delete;
    no_copy(no_copy &&)      = default;
};
}
