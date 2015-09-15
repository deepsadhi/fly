#include <gtk/gtk.h>                                                            // For GTK+
#include <string.h>                                                             // For strlen()
#include <stdint.h>

GtkBuilder      *builder;
GObject         *windowMain;
enum {

  LIST_ITEM = 0,
  N_COLUMNS
};


void init_list(GtkWidget *list) {

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes("List Items",
          renderer, "text", LIST_ITEM, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);

  gtk_tree_view_set_model(GTK_TREE_VIEW(list),
      GTK_TREE_MODEL(store));

  g_object_unref(store);
}

void add_to_list(GtkWidget *list, const gchar *str) {

  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);
}

void on_changed(GtkWidget *widget, gpointer label) {

  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, LIST_ITEM, &value,  -1);
    gtk_label_set_text(GTK_LABEL(label), value);
    g_free(value);
  }
}


int main(int argc, char *argv[])
{
    char            *gladeFile;
    gladeFile       = "gui.glade";


    gtk_init(&argc, &argv);

 /* Construct a GtkBuilder instance and load UI description */
    builder =  gtk_builder_new();
    gtk_builder_add_from_file(builder, gladeFile, NULL);

    windowMain = gtk_builder_get_object(builder, "windowMain");                 // Main window
    g_signal_connect(windowMain, "destroy", G_CALLBACK(gtk_main_quit), NULL);   // Action on close button

  GtkWidget *list;
    GtkTreeSelection *selection;
    GtkWidget *label;

    label = GTK_LABEL(gtk_builder_get_object(builder, "label1"));

    list = GTK_TREE_VIEW(gtk_builder_get_object(builder, "treeview1"));
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

    init_list(list);
    add_to_list(list, "Aliens");
    add_to_list(list, "Leon");
    add_to_list(list, "The Verdict");
    add_to_list(list, "North Face");
    add_to_list(list, "Der Untergang");
    add_to_list(list, "Der Untergang");
    add_to_list(list, "Der Untergang");
    add_to_list(list, "Der Untergang");
    add_to_list(list, "Der Untergang");
    add_to_list(list, "Der Untergang");
    add_to_list(list, "Der Untergang");
    add_to_list(list, "Der Untergang");

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

    g_signal_connect(selection, "changed",
        G_CALLBACK(on_changed), label);





   g_object_unref(windowMain);


    gtk_main();


    return 0;
}

