
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/qt/base_widget.hpp"
#include "saci/qt/spinbox/detail/view_to_model.hpp"

#include <QDoubleSpinBox>

namespace saci { namespace qt { 

struct spinbox
    : base_widget<
          QDoubleSpinBox,
          double,
          detail::spinbox_to_model
    >
{
    using base = base_widget<QDoubleSpinBox,
                             double,
                             detail::spinbox_to_model>;
    
    spinbox() = default;

    template<typename ObservableObject>
    spinbox(ObservableObject& model, QDoubleSpinBox& widget)
        : base(model,
               widget,
               SIGNAL(valueChanged(double)),
               SLOT(propagates(double)),
               [](QDoubleSpinBox& widget, double v)
               { widget.setValue(v); })
    {}
};

}}
