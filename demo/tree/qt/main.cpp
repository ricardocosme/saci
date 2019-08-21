#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <coruja/container/list.hpp>
#include <coruja/object/object.hpp>
#include <saci/tree/model/tree.hpp>
#include <saci/qt/push_btn/push_btn.hpp>

#include <iostream>
#include <string>

namespace st = saci::tree;

struct person_t {
    person_t(std::string s) : name(std::move(s)) {}
    coruja::object<std::string> name;
};
using persons_t = coruja::list<person_t>;
    
using model_tree_t =
st::root<persons_t, st::UnCheckable, 
  st::leaves<persons_t, st::Checkable> 
>;

namespace saci { namespace tree {
coruja::object<std::string>& node_label(person_t& o)
{ return o.name; }
}}

#include <saci/tree/view/qt/tree.hpp>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QDialog window;
    QVBoxLayout layout(&window);
    
    persons_t persons;
    model_tree_t model(persons);
    
    QTreeWidget treeWidget(&window);
    treeWidget.setSelectionMode(QAbstractItemView::MultiSelection);
    QPushButton qpushbtn(&window);

    layout.addWidget(&treeWidget);
    layout.addWidget(&qpushbtn);
    
    persons.emplace_back("joao");
    persons.emplace_back("josefina");
    persons.emplace_back("diego");
   
    st::view::qt::tree<model_tree_t> tree(model, treeWidget);
    
    persons.emplace_back("maria");
    persons.erase(std::prev(persons.end()));

    persons.front().name = "JOAO";

    model.children.front().check = true;
    
    model.children.front().check.after_change([](bool v)
    {std::cout << "front element visibility=" << v << std::endl;});

    saci::qt::experimental::push_btn remove("remove selected nodes", qpushbtn);
    remove.clicked([&]{ tree.remove_selected_nodes(); });
    
    window.show();
    return app.exec();
}
