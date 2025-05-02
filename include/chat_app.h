#ifndef CHAT_APP_H
#define CHAT_APP_H

#include <gtk/gtk.h>

typedef struct AuthWidgets AuthWidgets;
typedef struct ChatWidgets ChatWidgets;
typedef struct AppData AppData;
typedef struct DialogData DialogData;

struct AuthWidgets {
    GtkWidget *main_window;
    GtkWidget *stack;
    GtkWidget *login_username;
    GtkWidget *login_password;
    GtkWidget *register_username;
    GtkWidget *register_password;
    GtkWidget *register_confirm;
};

struct DialogData {
    GtkWidget *channel_list;
    ChatWidgets *widgets;
};

struct ChatWidgets {
    GtkWidget *entry;
    GtkWidget *view;
    GtkTextBuffer *buffer;
    GtkWidget *channel_list;
};

struct AppData {
    AuthWidgets auth;
    ChatWidgets chat;
    GtkApplication *app;
};

void send_message(GtkWidget *widget, gpointer data);
void add_channel(GtkWidget *channel_list, const char *channel_name, gpointer widgets);
void create_channel(GtkWidget *button, gpointer user_data);
void apply_css_from_file(const char *filepath);
void activate(GtkApplication* app, gpointer user_data);

#endif