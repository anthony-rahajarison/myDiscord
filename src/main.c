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
        
        gtk_text_buffer_insert(widgets->buffer, &iter, "[User] ", -1); //Username
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

void apply_css_from_file(const char *filepath) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;

    gtk_css_provider_load_from_path(provider, filepath, &error);

    if (error) {
        g_printerr("Erreur lors du chargement du CSS : %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
}


static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *scrolled_window;
    GtkWidget *channel_list;
    GtkWidget *text_view;
    GtkTextBuffer *text_buffer;
    GtkWidget *entry;
    GtkWidget *send_button;
    ChatWidgets *widgets = g_new(ChatWidgets, 1);
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    //Left Sidebar
    channel_list = gtk_list_box_new();
    gtk_widget_set_vexpand(channel_list, TRUE);
    gtk_widget_set_hexpand(channel_list, FALSE);
    gtk_widget_set_size_request(channel_list, 200, -1);
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(channel_list), GTK_SELECTION_SINGLE);
    gtk_grid_attach(GTK_GRID(grid), channel_list, 0, 0, 1, 2);

    GtkWidget *chan1 = gtk_button_new_with_label("chan1");
    gtk_list_box_insert(GTK_LIST_BOX(channel_list), chan1, -1);
    g_signal_connect(chan1, "clicked", G_CALLBACK(send_message), widgets);
    // gtk_style_context_add_class(cancel_ctx, "cancel-btn");

    
    //Right Chat Section
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 1, 0, 2, 1);
    
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
    
    entry = gtk_entry_new();
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 1, 1);
    widgets->entry = entry;
    
    send_button = gtk_button_new_with_label("Envoyer");
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), widgets);
    gtk_grid_attach(GTK_GRID(grid), send_button, 2, 1, 1, 1);
    GtkStyleContext *context = gtk_widget_get_style_context(send_button);
    gtk_style_context_add_class(context, "send_button");

    apply_css_from_file("assets/style.css");
    
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