
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/qt/base_widget.hpp"
#include "saci/qt/slider/detail/view_to_model.hpp"

#include <QSlider>

namespace saci { namespace qt { 

struct slider
    : base_widget<
          QSlider,
          int,
          detail::slider_to_model
    >
{
    using base = base_widget<QSlider,
                             int,
                             detail::slider_to_model>;
    
    slider() = default;

    template<typename ObservableObject>
    slider(ObservableObject& model, QSlider& widget)
        : base(model,
               widget,
               SIGNAL(valueChanged(int)),
               SLOT(propagates(int)),
               [](QSlider& widget, int v)
               { widget.setValue(v); })
    {}
};

}}
