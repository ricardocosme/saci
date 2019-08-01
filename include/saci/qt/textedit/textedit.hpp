
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/qt/base_widget.hpp"
#include "saci/qt/textedit/detail/view_to_model.hpp"

#include <QTextEdit>

namespace saci { namespace qt {   

struct textedit
    : base_widget<
          QTextEdit,
          std::string,
          detail::textedit_to_model
    >
{
    using base = base_widget<QTextEdit,
                             std::string,
                             detail::textedit_to_model>;
    
    textedit() = default;

    template<typename ObservableObject>
    textedit(ObservableObject& model, QTextEdit& widget)
        : base(model,
               widget,
               SIGNAL(textChanged()),
               SLOT(propagates()),
               [](QTextEdit& widget, std::string s)
               { widget.setText(s.c_str()); })
    {}
};

}}
