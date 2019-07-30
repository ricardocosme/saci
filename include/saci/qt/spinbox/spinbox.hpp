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

    template<typename Model>
    spinbox(Model& model, QDoubleSpinBox& widget)
        : base(model,
               widget,
               SIGNAL(valueChanged(double)),
               SLOT(propagates(double)),
               [](QDoubleSpinBox& widget, double v)
               { widget.setValue(v); })
    {}
};

}}
