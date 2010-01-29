
#include "jack.hpp"

#include <gtkmm.h>
#include <assert.h>
#include <stdio.h>

#ifdef max
#undef max
#endif

namespace Jacker {

class Client : public Jack::Client {
public:
    Jack::MIDIPort *midi_omni_out;

    Client()
        : Jack::Client("jacker") {
        midi_omni_out = new Jack::MIDIPort(*this, "omni", Jack::MIDIPort::IsOutput);
    }
    
    ~Client() {
        delete midi_omni_out;
    }

    virtual void on_process(Jack::NFrames size) {
    }
};

class PatternColumns : public Gtk::TreeModelColumnRecord {
public:
    Gtk::TreeModelColumn<int> dummy_column;

    PatternColumns() {
        add(dummy_column);
    }
};

class App {
public:
    Gtk::Main kit;
    Jacker::Client client;

    Glib::RefPtr<Gtk::Builder> builder;
    Glib::RefPtr<Gtk::ListStore> list_store;
    PatternColumns pattern_columns;

    Gtk::TreeView *pattern_view;

    App(int argc, char **argv)
        : kit(argc,argv) {
        pattern_view = NULL;
    }
    
    ~App() {
    }
    
    void init_ui() {
        list_store = Gtk::ListStore::create(pattern_columns);
        for (int i = 0; i < 4096; ++i) {
            Gtk::TreeModel::iterator iter = list_store->append();
            //Gtk::TreeModel::Row row = *iter;
        }
        
        builder->get_widget("patternview", pattern_view);
        
        pattern_view->set_model(list_store);
        
        // TODO: create columns (with sub-columns?)
        // write custom cell renderer for representing ints as hex/note values
        // use set_cell_data_func of TreeViewColumn to feed directly from
        // the source data.
    }

    void run() {
        //~ if (!client.init())
            //~ return;
        builder = Gtk::Builder::create_from_file("jacker.glade");
        
        Gtk::Window* window = 0;
        builder->get_widget("main", window);
        
        init_ui();
        
        window->show_all();
        
        //~ client.activate();
        
        kit.run(*window);
            
        //~ client.deactivate();
        //~ client.shutdown();
    }
};
    
} // namespace Jacker

int main(int argc, char **argv) {
    Jacker::App app(argc, argv);
    app.run();
    
    return 0;
}

#ifdef WIN32
int WINAPI WinMain(      
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    return main(__argc, __argv);
}
#endif