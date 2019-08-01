
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QTextEdit>

#include <coruja/object/object.hpp>
#include <saci/qt/textedit/textedit.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow window;

    coruja::object<std::string> model{"string"};
    
    model.for_each([](std::string v)
    { std::cout << "model is " << v << std::endl; });

    QTextEdit widget(&window);
    
    saci::qt::textedit textedit(model, widget);
        
    window.show();

    model = "another string";
    
    return app.exec();
}
