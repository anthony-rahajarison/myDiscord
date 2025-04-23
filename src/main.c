#include <gtk/gtk.h>
#include <time.h>

typedef struct {
    GtkWidget *entry;
    GtkWidget *view;
    GtkTextBuffer *buffer;
} ChatWidgets;

static void send_message(GtkWidget *widget, gpointer data) {
    ChatWidgets *widgets = (ChatWidgets *)data;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(widgets->entry));
    
    if (strlen(text) > 0) {
        // Get Time
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char time_str[9]; // HH:MM:SS + null terminator
        strftime(time_str, sizeof(time_str), "%H:%M:%S", t);
        
        // Formatting message
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter(widgets->buffer, &iter);
        
        gtk_text_buffer_insert(widgets->buffer, &iter, "[User] ", -1);
        gtk_text_buffer_insert(widgets->buffer, &iter, time_str, -1);
        gtk_text_buffer_insert(widgets->buffer, &iter, ": ", -1);
        gtk_text_buffer_insert(widgets->buffer, &iter, text, -1);
        gtk_text_buffer_insert(widgets->buffer, &iter, "\n", -1);
        
        gtk_entry_set_text(GTK_ENTRY(widgets->entry), "");
        
        GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(
            gtk_widget_get_parent(widgets->view)));
        gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
    }
}

void apply_css(GtkWidget *widget, const char *css) {
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_data(provider, css, -1, NULL);
  
  GtkStyleContext *context = gtk_widget_get_style_context(widget);
  gtk_style_context_add_provider(context, 
                               GTK_STYLE_PROVIDER(provider), 
                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                          GTK_STYLE_PROVIDER(provider),
                                          GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkTextBuffer *text_buffer;
    GtkWidget *entry;
    GtkWidget *button;
    ChatWidgets *widgets = g_new(ChatWidgets, 1);
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 0, 2, 1);
    
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    widgets->buffer = text_buffer;
    widgets->view = text_view;
    
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(text_buffer, &iter);
    gtk_text_buffer_insert(text_buffer, &iter, "Chat Public\n", -1);
    gtk_text_buffer_insert(text_buffer, &iter, "[Admin] 12:00:00: Tapez votre message ci-dessous et cliquez sur Envoyer.\n", -1);
    
    entry = gtk_entry_new();
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 1, 1, 1);
    widgets->entry = entry;
    
    button = gtk_button_new_with_label("Envoyer");
    g_signal_connect(button, "clicked", G_CALLBACK(send_message), widgets);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);
    
    // CSS
    const char *window_css = 
    "window {"
        "background-color: #36393f;"
    "}"
    "grid {"
        "background-color: #36393f;"
    "}"
    "textview, textview text {"
        "background-color: #2f3136;"
        "color: #dcddde;"
        "font-family: Arial;"
        "font-size: 20px;"
        "border-radius: 10px;"
        "padding: 10px;"
    "}"
    "entry {"
        "background-color: #40444b;"
        "color: #dcddde;"
        "font-family: Arial;"
        "font-size: 14px;"
        "padding: 10px;"
        "border-radius: 5px;"
        "border: none;"
    "}"
    "button {"
        "font-family: Arial;"
        "font-size: 14px;"
        "padding: 0px;"
        "border-radius: 5px;"
        "border: none;"
    "}"
    "button > label {"
        "padding: 15px 32px;"
        "background-color: #5056a0;"
        "color: #dcddde;"
        "border-radius: 5px;"
    "}";
    
    apply_css(window, window_css);
    
    g_signal_connect(entry, "activate", G_CALLBACK(send_message), widgets);
    
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    
    app = gtk_application_new("org.gtk.chatapp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}