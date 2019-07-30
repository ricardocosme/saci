#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtGui/QTreeWidget>
#include <coruja/container/list.hpp>
#include <coruja/object/object.hpp>
#include <saci/tree/model/tree.hpp>

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
  st::leaves<persons_t::value_type, st::Checkable> 
>;

namespace saci { namespace tree {
coruja::object<std::string>& node_label(person_t& o)
{ return o.name; }
}}

#include <saci/tree/view/qt/tree.hpp>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setMinimumSize(400, 400);
    
    persons_t persons;
    model_tree_t model(persons);
    
    persons.emplace_back("joao");
    persons.emplace_back("diego");
    
    QTreeWidget treeWidget(&window);
    st::view::qt::tree tree(model, treeWidget);
    tree.widget().adjustSize();
    
    persons.emplace_back("maria");
    persons.erase(std::prev(persons.end()));

    persons.front().name = "JOAO";

    model.children.front().check = true;
    
    model.children.front().check.after_change([](bool v)
    {std::cout << "front element visibility=" << v << std::endl;});
    
    window.show();
    return app.exec();
}
