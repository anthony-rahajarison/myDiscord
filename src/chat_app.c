#include <gtk/gtk.h>
#include <time.h>
#include "chat_app.h"

void send_message(GtkWidget *widget, gpointer data) {
    ChatWidgets *widgets = (ChatWidgets *)data;
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(widgets->entry));
    
    if (strlen(text) > 0) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char time_str[9];
        strftime(time_str, sizeof(time_str), "%H:%M:%S", t);
        
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

void add_channel(GtkWidget *channel_list, const char *channel_name, gpointer widgets) {
    GtkWidget *channel_btn = gtk_button_new_with_label(channel_name);
    gtk_list_box_insert(GTK_LIST_BOX(channel_list), channel_btn, -1);
    g_signal_connect(channel_btn, "clicked", G_CALLBACK(send_message), widgets);
    
    GtkStyleContext *context = gtk_widget_get_style_context(channel_btn);
    gtk_style_context_add_class(context, "channel_select");
    gtk_widget_show(channel_btn);
}

static void dialog_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    DialogData *data = (DialogData *)user_data;
    if (response_id == GTK_RESPONSE_OK) {
        GtkWidget *entry = gtk_dialog_get_content_area(dialog);
        GList *children = gtk_container_get_children(GTK_CONTAINER(entry));
        entry = g_list_nth_data(children, 1);
        
        if (GTK_IS_ENTRY(entry)) {
            const gchar *channel_name = gtk_entry_get_text(GTK_ENTRY(entry));
            if (channel_name && *channel_name) {
                add_channel(data->channel_list, channel_name, data->widgets);
            }
        }
        g_list_free(children);
    }
    g_free(data);
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

void create_channel(GtkWidget *button, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Nouveau canal",
        NULL,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "_Annuler",
        GTK_RESPONSE_CANCEL,
        "_Créer",
        GTK_RESPONSE_OK,
        NULL);
    
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new("Entrez le nom du nouveau canal:");
    GtkWidget *entry = gtk_entry_new();
    
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_container_add(GTK_CONTAINER(content_area), entry);
    
    DialogData *dialog_data = g_new(DialogData, 1);
    dialog_data->channel_list = app_data->chat.channel_list;
    dialog_data->widgets = &app_data->chat;
    
    g_signal_connect(dialog, "response", G_CALLBACK(dialog_response), dialog_data);
    gtk_widget_show_all(dialog);
}

void apply_css_from_file(const char *filepath) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    
    if (!gtk_css_provider_load_from_path(provider, filepath, &error)) {
        g_printerr("Error loading CSS: %s\n", error->message);
        g_error_free(error);
    }
    
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void activate(GtkApplication* app, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkWidget *entry;
    GtkWidget *send_button;
    GtkWidget *channel_list;
    GtkWidget *add_channel_btn;
    
    // Initialize ChatWidgets in AppData
    app_data->chat = (ChatWidgets){0};
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Chat Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    // Left Sidebar
    channel_list = gtk_list_box_new();
    gtk_widget_set_vexpand(channel_list, TRUE);
    gtk_widget_set_size_request(channel_list, 200, -1);
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(channel_list), GTK_SELECTION_SINGLE);
    gtk_grid_attach(GTK_GRID(grid), channel_list, 0, 0, 1, 2);
    app_data->chat.channel_list = channel_list;
    
    add_channel_btn = gtk_button_new_with_label("+ Ajouter un canal");
    g_signal_connect(add_channel_btn, "clicked", G_CALLBACK(create_channel), app_data);
    gtk_list_box_insert(GTK_LIST_BOX(channel_list), add_channel_btn, 0);
    
    // Right Chat Section
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_hexpand(scrolled_window, TRUE);
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 1, 0, 2, 1);
    
    text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
    
    app_data->chat.buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    app_data->chat.view = text_view;
    
    // Add default channel
    add_channel(channel_list, "Canal Général", &app_data->chat);
    
    entry = gtk_entry_new();
    gtk_widget_set_hexpand(entry, TRUE);
    gtk_grid_attach(GTK_GRID(grid), entry, 1, 1, 1, 1);
    app_data->chat.entry = entry;
    
    send_button = gtk_button_new_with_label("Envoyer");
    g_signal_connect(send_button, "clicked", G_CALLBACK(send_message), &app_data->chat);
    gtk_grid_attach(GTK_GRID(grid), send_button, 2, 1, 1, 1);
    
    apply_css_from_file("./../assets/style.css");
    g_signal_connect(entry, "activate", G_CALLBACK(send_message), &app_data->chat);
    gtk_widget_show_all(window);
}