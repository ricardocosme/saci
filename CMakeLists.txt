cmake_minimum_required (VERSION 3.13)
project (saci_qt)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_compile_options(
    -Wno-strict-aliasing
)

include_directories(
    include
    ${CORUJA_ROOT}/include
    ${RANGE_V3_ROOT}/include
)

find_package(Boost 1.67)
include_directories(${Boost_INCLUDE_DIRS})

set(QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake)
find_package(Qt4 4.8.0 REQUIRED)
include(${QT_USE_FILE})

qt4_generate_moc(
    include/saci/qt/push_btn/detail/clicked.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/push_btn_clicked.cpp
)
add_executable(
    qt_push_btn
    demo/push_btn/qt/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/push_btn_clicked.cpp
)
list(APPEND MOCS ${CMAKE_CURRENT_BINARY_DIR}/push_btn_clicked.cpp)
target_link_libraries(qt_push_btn ${QT_LIBRARIES})

add_executable(
  qt_tree
  demo/tree/qt/main.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/push_btn_clicked.cpp
)
target_link_libraries(qt_tree ${QT_LIBRARIES})

macro(add_qt_demo NAME)
    qt4_generate_moc(
        include/saci/qt/${NAME}/detail/view_to_model.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_view_to_model.cpp
    )
    add_executable(
        qt_${NAME}
        demo/${NAME}/qt/main.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_view_to_model.cpp
    )
    target_link_libraries(qt_${NAME} ${QT_LIBRARIES})
    list(APPEND MOCS ${CMAKE_CURRENT_BINARY_DIR}/${NAME}_view_to_model.cpp)
endmacro(add_qt_demo)

add_qt_demo(checkbox)
add_qt_demo(radio_btn)
add_qt_demo(slider)
add_qt_demo(spinbox)
add_qt_demo(textedit)

add_executable(
    qt_enable
    demo/enable/qt/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/checkbox_view_to_model.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/textedit_view_to_model.cpp
)

target_link_libraries(qt_enable ${QT_LIBRARIES})
list(APPEND MOCS ${CMAKE_CURRENT_BINARY_DIR}/checkbox_view_to_model.cpp
${CMAKE_CURRENT_BINARY_DIR}/textedit_view_to_model.cpp)
    
add_executable(
    qt_label
    demo/label/qt/main.cpp
)
target_link_libraries(qt_label ${QT_LIBRARIES})

add_library(saci_qt ${MOCS})
target_include_directories(saci_qt PUBLIC include)
