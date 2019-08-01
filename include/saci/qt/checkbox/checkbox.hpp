
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/qt/base_widget.hpp"
#include "saci/qt/checkbox/detail/view_to_model.hpp"

#include <QCheckBox>

namespace saci { namespace qt {   

struct checkbox
    : base_widget<
          QCheckBox,
          bool,
          detail::checkbox_to_model
    >
{
    using base = base_widget<QCheckBox,
                             bool,
                             detail::checkbox_to_model>;
    
    checkbox() = default;

    template<typename ObservableObject>
    checkbox(ObservableObject& model, QCheckBox& widget)
        : base(model,
               widget,
               SIGNAL(stateChanged(int)),
               SLOT(propagates(int)),
               [](QCheckBox& widget, bool v)
               { widget.setCheckState(v ? Qt::Checked : Qt::Unchecked); })
    {}
};

}}
