#include <nanobind/trampoline.h>
#include "internals.h"

NAMESPACE_BEGIN(NB_NAMESPACE)
NAMESPACE_BEGIN(detail)

void implicitly_convertible(const std::type_info *src,
                            const std::type_info *dst) noexcept {
    internals &internals = internals_get();

    auto it = internals.type_c2p.find(std::type_index(*dst));
    if (it == internals.type_c2p.end())
        fail("nanobind::detail::implicitly_convertible(src=%s, dst=%s): "
             "destination type unknown!", type_name(src),
             type_name(dst));

    type_data *t = it->second;

    size_t size = 0;
    if (t->flags & (uint16_t) type_flags::has_implicit_conversions) {
        while (t->implicit && t->implicit[size])
            size++;
    } else {
        t->implicit = nullptr;
        t->implicit_py = nullptr;
        t->flags |= (uint16_t) type_flags::has_implicit_conversions;
    }

    const std::type_info **data = (const std::type_info **) malloc(
        sizeof(const std::type_info *) * (size + 2));

    memcpy(data, t->implicit, size * sizeof(const std::type_info *));
    data[size] = src;
    data[size + 1] = nullptr;
    free(t->implicit);
    t->implicit = data;
}

void implicitly_convertible(bool (*predicate)(PyObject *, PyObject **),
                            const std::type_info *dst) noexcept {
    internals &internals = internals_get();

    auto it = internals.type_c2p.find(std::type_index(*dst));
    if (it == internals.type_c2p.end())
        fail("nanobind::detail::implicitly_convertible(src=<predicate>, dst=%s): "
             "destination type unknown!", type_name(dst));

    (void) predicate;
#if 0
    size_t size = 0;
    if (t->flags & (uint16_t) type_flags::has_implicit_conversions) {
        while (t->implicit_py && t->implicit_py[size])
            size++;
    } else {
        t->implicit = nullptr;
        t->implicit_py = nullptr;
        t->flags |= (uint16_t) type_flags::has_implicit_conversions;
    }

    const void **data = (const void **) malloc(sizeof(void *) * (size + 2));
    memcpy(data, t->implicit_py, size * sizeof(const void *));
    data[size] = predicate;
    data[size + 1] = nullptr;
    free(t->implicit_py);
    t->implicit_py = data;
#endif
}

NAMESPACE_END(detail)
NAMESPACE_END(NB_NAMESPACE)
