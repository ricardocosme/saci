
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <QtGui/QApplication>
#include <QtGui/QCheckBox>
#include <QtGui/QMainWindow>

#include <coruja/object/object.hpp>
#include <saci/qt/checkbox/checkbox.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow window;

    coruja::object<bool> model{false};

    model.for_each([](bool v)
    { std::cout << "model is " << v << std::endl; });
    
    QCheckBox qcheckbox(&window);
    saci::qt::checkbox checkbox(model, qcheckbox);
        
    window.show();

    model = true;
    
    return app.exec();
}
