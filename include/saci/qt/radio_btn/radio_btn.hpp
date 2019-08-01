
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/qt/base_widget.hpp"
#include "saci/qt/radio_btn/detail/view_to_model.hpp"

#include <QRadioButton>

namespace saci { namespace qt { 

struct radio_btn
    : base_widget<
          QRadioButton,
          bool,
          detail::radio_btn_to_model
    >
{
    using base = base_widget<QRadioButton,
                             bool,
                             detail::radio_btn_to_model>;
    
    radio_btn() = default;

    template<typename ObservableObject>
    radio_btn(ObservableObject& model, QRadioButton& widget)
        : base(model,
               widget,
               SIGNAL(toggled(bool)),
               SLOT(propagates(bool)),
               [](QRadioButton& widget, bool v)
               { widget.setChecked(v); })
    {}
};

}}
