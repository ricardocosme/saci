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

    template<typename Model>
    radio_btn(Model& model, QRadioButton& widget)
        : base(model,
               widget,
               SIGNAL(toggled(bool)),
               SLOT(propagates(bool)),
               [](QRadioButton& widget, bool v)
               { widget.setChecked(v); })
    {}
};

}}
