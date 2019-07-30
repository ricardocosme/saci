
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QDoubleSpinBox>

#include <coruja/object/object.hpp>
#include <saci/qt/spinbox/spinbox.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow window;

    coruja::object<double> model{1.f};
    
    model.for_each([](double v)
    { std::cout << "model is " << v << std::endl; });

    QDoubleSpinBox widget(&window);
    
    saci::qt::spinbox spinbox(model, widget);
        
    window.show();

    model = 15.7f;
    
    return app.exec();
}
