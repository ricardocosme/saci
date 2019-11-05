
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "saci/qt/base_widget.hpp"
#include "saci/qt/slider/detail/view_to_model.hpp"

#include <cmath>
#include <QSlider>

namespace saci { namespace qt { 

struct slider
    : base_widget<
          QSlider,
          double,
          detail::slider_to_model
    >
{
    using base = base_widget<QSlider,
                             double,
                             detail::slider_to_model>;
    
    slider() = default;

    template<typename ObservableObject>
    slider(ObservableObject& model,
           QSlider& widget,
           double max,
           double step = 0.05f)
        : base(model,
               widget,
               SIGNAL(valueChanged(int)),
               SLOT(propagates(int)),
               [step](QSlider& widget, double v)
               { widget.setValue(std::round(v/step)); })
    {
        //TODO: Minimum value
        widget.setMaximum(std::round(max/step));
        _view_to_model.step = step;
    }
};

}}
