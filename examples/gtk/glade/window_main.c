 #include <gtk/gtk.h>
#include <string.h>

    GtkBuilder  *builder;
int main(int argc, char *argv[])
{
    GtkWidget   *window;
   GtkCssProvider *provider;
    GdkDisplay *display;
   GdkScreen *screen;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "window_main.glade", NULL);

/*---------------- CSS ----------------------------------------------------------------------------------------------------*/
  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen,
                       GTK_STYLE_PROVIDER(provider),
                       GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "main.css";   // for example:  /home/zerohour/gtk/cssbutton.css

  GError *error = 0;

   gtk_css_provider_load_from_path (provider,
                                    g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
                                    NULL);
  g_object_unref (provider);
/*-------------------------------------------------------------------------------------------------------------------------*/

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

  //  g_object_unref(builder);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}

void on_window_main_destroy()
{
    gtk_main_quit();
}

void on_button1_clicked()
{
    GtkWidget *message;
    message = GTK_WIDGET(gtk_builder_get_object(builder, "messagedialog1"));
    gtk_widget_show(message);
}
