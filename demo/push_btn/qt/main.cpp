
// Copyright Ricardo Calheiros de Miranda Cosme 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>

#include <saci/qt/push_btn/push_btn.hpp>

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow window;

    QPushButton qpushbutton(&window);
    saci::qt::experimental::push_btn push_btn("click me", qpushbutton);
        
    window.show();

    push_btn.clicked([]{std::cout << "clicked" << std::endl;});
    
    return app.exec();
}
