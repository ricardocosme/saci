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

    template<typename Model>
    checkbox(Model& model, QCheckBox& widget)
        : base(model,
               widget,
               SIGNAL(stateChanged(int)),
               SLOT(propagates(int)),
               [](QCheckBox& widget, bool v)
               { widget.setCheckState(v ? Qt::Checked : Qt::Unchecked); })
    {}
};

}}
