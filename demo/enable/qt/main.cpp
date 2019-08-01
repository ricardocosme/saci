
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>

#include <coruja/object/object.hpp>
#include <coruja/object/transform.hpp>
#include <saci/qt/checkbox/checkbox.hpp>
#include <saci/qt/textedit/textedit.hpp>

#include <iostream>
#include <string>

using namespace coruja;

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QDialog window;
    QVBoxLayout layout(&window);

    QCheckBox qcheckbox(&window);
    QTextEdit qtextedit(&window);
    
    layout.addWidget(&qcheckbox);
    layout.addWidget(&qtextedit);
    
    object<bool> model{false};

    model.for_each([](bool v)
    { std::cout << "checkbox is " << (v ? "true" : "false") << std::endl; });
    
    saci::qt::checkbox checkbox(model, qcheckbox);

    object<std::string> name{"Clear this text to disable the checkbox"};
    saci::qt::textedit textedit(name, qtextedit);
    
    window.show();

    checkbox.enable(transform(name, [](std::string s){ return !s.empty(); }));
        
    model = true;

    return app.exec();
}
