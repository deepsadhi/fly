/* COMPILE WITH:  gcc -Wall -o cssbutton `pkg-config --cflags --libs gtk+-3.0` cssbutton.c */

#include <gtk/gtk.h>
#include <string.h>

int main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *button;
/*-- CSS ------------------*/
  GtkCssProvider *provider;
  GdkDisplay *display;
  GdkScreen *screen;
/*---------------------------*/
  gtk_init(&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  gtk_window_set_position (GTK_WINDOW(window),
                           GTK_WIN_POS_CENTER);

  gtk_widget_set_size_request (window,
                               180,160);

  gtk_window_set_title (GTK_WINDOW(window),
                        "CSS Button");

  g_signal_connect (G_OBJECT(window),
                    "destroy",
                    G_CALLBACK(gtk_main_quit),
                    NULL);

  button = gtk_button_new_with_label ("CssButton");

  gtk_widget_set_name (GTK_WIDGET(button),
                       "mybutton");        // name button so we can apply css to it later

  gtk_widget_set_halign (GTK_WIDGET(button),
                         GTK_ALIGN_CENTER);

  gtk_widget_set_valign (GTK_WIDGET(button),
                         GTK_ALIGN_CENTER);

  gtk_widget_set_size_request (GTK_WIDGET(button),
                               150,100);

  gtk_container_add (GTK_CONTAINER(window),
                     GTK_WIDGET(button));

/*---------------- CSS ----------------------------------------------------------------------------------------------------*/
  provider = gtk_css_provider_new ();
  display = gdk_display_get_default ();
  screen = gdk_display_get_default_screen (display);
  gtk_style_context_add_provider_for_screen (screen,
                       GTK_STYLE_PROVIDER(provider),
                       GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gsize bytes_written, bytes_read;

  const gchar* home = "cssbutton.css";   // for example:  /home/zerohour/gtk/cssbutton.css 

  GError *error = 0;

   gtk_css_provider_load_from_path (provider,
                                    g_filename_to_utf8(home, strlen(home), &bytes_read, &bytes_written, &error),
                                    NULL);
  g_object_unref (provider);
/*-------------------------------------------------------------------------------------------------------------------------*/

  gtk_widget_show_all(window);
  gtk_main ();
  return 0;
}
