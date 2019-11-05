
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QSlider>

#include <coruja/object/object.hpp>
#include <saci/qt/slider/slider.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow window;

    coruja::object<double> model{0};
    
    model.for_each([](double v)
    { std::cout << "model is " << v << std::endl; });

    QSlider widget(Qt::Horizontal, &window);
    
    saci::qt::slider slider(model, widget, 255.f, 0.5f);
        
    window.show();

    model = 150.5;
    
    return app.exec();
}
