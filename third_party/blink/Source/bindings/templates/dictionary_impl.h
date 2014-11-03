// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by {{code_generator}}. DO NOT MODIFY!

#ifndef {{cpp_class}}_h
#define {{cpp_class}}_h

{% for filename in header_includes %}
#include "{{filename}}"
{% endfor %}

namespace blink {

class {{cpp_class}} final {
    ALLOW_ONLY_INLINE_ALLOCATION();
public:
    {{cpp_class}}();

    {% for member in members %}
    bool {{member.has_method_name}}() const { return {{member.has_method_expression}}; }
    {{member.rvalue_cpp_type}} {{member.cpp_name}}() const { return {{member.getter_expression}}; }
    void {{member.setter_name}}({{member.rvalue_cpp_type}} value) { m_{{member.cpp_name}} = value; }

    {% endfor %}
    void trace(Visitor*);

private:
    {% for member in members %}
    {{member.member_cpp_type}} m_{{member.cpp_name}};
    {% endfor %}

    friend class V8{{cpp_class}};
};

} // namespace blink

#endif // {{cpp_class}}_h
