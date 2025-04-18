#include <gtk/gtk.h>

static void print_hello (GtkWidget *widget, gpointer   data) {
  g_print ("Hello World\n");
}

void apply_css_to_button(GtkWidget *button) {
  GtkCssProvider *provider = gtk_css_provider_new();
  const char *css = "button { background-color:#212531; color: #FFFFFF; }";

  gtk_css_provider_load_from_data(provider, css, -1, NULL);
  GtkStyleContext *context = gtk_widget_get_style_context(button);
  gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref(provider);
}

static void activate (GtkApplication* app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 1000, 800);
  gtk_container_set_border_width(GTK_CONTAINER (window), 10);

  grid = gtk_grid_new();

  gtk_container_add (GTK_CONTAINER (window), grid);

  button = gtk_button_new_with_label("Je suis un bouton");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  gtk_grid_attach(GTK_GRID (grid), button, 0, 0, 1, 1);
  apply_css_to_button(button);

  button = gtk_button_new_with_label("Je suis un autre bouton");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  gtk_grid_attach(GTK_GRID (grid), button, 1, 0, 1, 1);
  apply_css_to_button(button);

  button = gtk_button_new_with_label("Quitter");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_grid_attach(GTK_GRID (grid), button, 0, 1, 2, 1);
  apply_css_to_button(button);

  gtk_widget_show_all(window);
}

int main (int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new ("myDiscord.gtk.app", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}