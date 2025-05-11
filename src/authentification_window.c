#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "chat_app.h"
#include "authentification_window.h"

static void show_main_app(GtkWidget *widget, gpointer data) {
    AppData *app_data = (AppData *)data;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(app_data->auth.login_username));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(app_data->auth.login_password));



    //CHECK INFO HERE

    if (strlen(username) > 0 && strlen(password) > 0) {
        printf("Launching main app\n");
        
        gtk_widget_hide(app_data->auth.main_window);
        gtk_widget_destroy(app_data->auth.main_window);
        
        activate(app_data->app, app_data);
    }
}

static void create_account(GtkWidget *widget, gpointer data) {
    AuthWidgets *widgets = (AuthWidgets *)data;
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(widgets->register_username));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(widgets->register_password));
    const gchar *password_confirm = gtk_entry_get_text(GTK_ENTRY(widgets->register_confirm));

    if (strlen(username) == 0 || strlen(password) == 0) {
        printf("Username or password is empty");
        return;
    }

    if (strcmp(password, password_confirm) != 0) {
        printf("Passwords do not match");
    }

    //REGISTER ACCOUNT HERE

    gtk_stack_set_visible_child_name(GTK_STACK(widgets->stack), "login");
}

static void switch_to_register(GtkWidget *widget, gpointer data) {
    AuthWidgets *widgets = (AuthWidgets *)data;
    gtk_stack_set_visible_child_name(GTK_STACK(widgets->stack), "register");
}

static void switch_to_login(GtkWidget *widget, gpointer data) {
    AuthWidgets *widgets = (AuthWidgets *)data;
    gtk_stack_set_visible_child_name(GTK_STACK(widgets->stack), "login");
}

void activate_auth(GtkApplication *app, gpointer user_data) {
    AppData *app_data = (AppData *)user_data;
    AuthWidgets *widgets = &app_data->auth;
 
    //Main Window
    widgets->main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(widgets->main_window), "Authentification");
    gtk_window_set_default_size(GTK_WINDOW(widgets->main_window), 600, 400);
    gtk_container_set_border_width(GTK_CONTAINER(widgets->main_window), 10);

    //New Stack to switch between login/register
    widgets->stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(widgets->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_container_add(GTK_CONTAINER(widgets->main_window), widgets->stack);

    //Connexion Page
    GtkWidget *login_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(login_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(login_box, GTK_ALIGN_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(login_box), 10);

    //Login Grid setup
    GtkWidget *login_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(login_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(login_grid), 5);

    gtk_box_pack_start(GTK_BOX(login_box), login_grid, FALSE, FALSE, 0);

    widgets->login_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->login_username), "Nom d'utilisateur");
    gtk_grid_attach(GTK_GRID(login_grid), widgets->login_username, 0, 0, 1, 1);

    widgets->login_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->login_password), "Mot de passe");
    gtk_grid_attach(GTK_GRID(login_grid), widgets->login_password, 0, 1, 1, 1);

    GtkWidget *login_button = gtk_button_new_with_label("Se connecter");
    g_signal_connect(login_button, "clicked", G_CALLBACK(show_main_app), widgets);
    gtk_grid_attach(GTK_GRID(login_grid), login_button, 0, 2, 1, 1);

    GtkWidget *register_link = gtk_button_new_with_label("Créer un compte");
    g_signal_connect(register_link, "clicked", G_CALLBACK(switch_to_register), widgets);
    gtk_grid_attach(GTK_GRID(login_grid), register_link, 0, 3, 1, 1);
    gtk_widget_set_name(register_link, "register_link");

    //Register Page
    GtkWidget *register_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_halign(register_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(register_box, GTK_ALIGN_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(register_box), 10);

    GtkWidget *register_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(register_grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(register_grid), 5);
    gtk_box_pack_start(GTK_BOX(register_box), register_grid, FALSE, FALSE, 0);
    
    widgets->register_username = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->register_username), "Nom d'utilisateur");
    gtk_grid_attach(GTK_GRID(register_grid), widgets->register_username, 0, 0, 1, 1);

    widgets->register_password = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->register_password), "Mot de passe");
    gtk_grid_attach(GTK_GRID(register_grid), widgets->register_password, 0, 1, 1, 1);

    widgets->register_confirm = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(widgets->register_confirm), "Confirmer le mot de passe");
    gtk_grid_attach(GTK_GRID(register_grid), widgets->register_confirm, 0, 2, 1, 1);

    GtkWidget *register_button = gtk_button_new_with_label("S'inscrire");
    g_signal_connect(register_button, "clicked", G_CALLBACK(create_account), widgets);
    gtk_grid_attach(GTK_GRID(register_grid), register_button, 0, 3, 1, 1);
    
    GtkWidget *login_link = gtk_button_new_with_label("Déjà un compte? Se connecter");
    g_signal_connect(login_link, "clicked", G_CALLBACK(switch_to_login), widgets);
    gtk_grid_attach(GTK_GRID(register_grid), login_link, 0, 4, 1, 1);
    gtk_widget_set_name(login_link, "login_link");

    //Adding grids to stacks
    gtk_stack_add_named(GTK_STACK(widgets->stack), login_box, "login");
    gtk_stack_add_named(GTK_STACK(widgets->stack), register_box, "register");
    
    gtk_widget_show_all(widgets->main_window);
    apply_css_from_file("./assets/auth.css");
}

int main(int argc, char **argv) {
    GtkApplication *app;
    AppData *app_data = g_new0(AppData, 1);
    
    app = gtk_application_new("org.gtk.chatapp", G_APPLICATION_DEFAULT_FLAGS);
    app_data->app = app;
    
    g_signal_connect(app, "activate", G_CALLBACK(activate_auth), app_data);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_free(app_data);
    g_object_unref(app);
    return status;
}