
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <coruja/object/any_object.hpp>
#include <coruja/support/signal/scoped_blocked_connection.hpp>
#include <QObject>
#include <QTextEdit>

#include <string>
#include <utility>

namespace saci { namespace qt {  namespace detail {

class textedit_to_model : public QObject {
    Q_OBJECT
public:
    textedit_to_model() = default;
    textedit_to_model(coruja::any_object<std::string> pmodel,
                      QTextEdit& pwidget)
        : model(std::move(pmodel))
        , widget(pwidget)
    {}
    virtual ~textedit_to_model() = default;
    coruja::any_object<std::string> model;
    coruja::any_connection conn;
    QTextEdit& widget;
public Q_SLOTS:
    void propagates()
    {
        auto blk = coruja::make_scoped_blocked_connection(conn);
        model = widget.toPlainText().toStdString();
    }
};

}}}
