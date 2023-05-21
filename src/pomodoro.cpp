#include <gtk/gtk.h>
#include <iostream>

static int work_duration = 25;
static int break_duration = 5;
static int minutes_left = work_duration;
static int seconds_left = 0;
static gboolean is_break = false;

GtkWidget *window;
GtkWidget *timer_label;
GtkWidget *start_button;
GtkWidget *reset_button;

static gboolean timer_callback(gpointer data) {
    if (minutes_left == 0 && seconds_left == 0) {
        if (is_break) {
            is_break = false;
            minutes_left = work_duration;
        } else {
            is_break = true;
            minutes_left = break_duration;
        }
        seconds_left = 0;
    } else if (seconds_left == 0) {
        minutes_left--;
        seconds_left = 59;
    } else {
        seconds_left--;
    }

    gchar *time_text = g_strdup_printf("%02d:%02d", minutes_left, seconds_left);
    gtk_label_set_text(GTK_LABEL(timer_label), time_text);
    g_free(time_text);

    return G_SOURCE_CONTINUE;
}

static void start_timer(GtkWidget *widget, gpointer data) {
    g_timeout_add_seconds(1, timer_callback, NULL);
    gtk_widget_set_sensitive(start_button, FALSE);
    gtk_widget_set_sensitive(reset_button, TRUE);
}

static void reset_timer(GtkWidget *widget, gpointer data) {
    minutes_left = work_duration;
    seconds_left = 0;
    is_break = false;

    gchar *time_text = g_strdup_printf("%02d:%02d", minutes_left, seconds_left);
    gtk_label_set_text(GTK_LABEL(timer_label), time_text);
    g_free(time_text);

    gtk_widget_set_sensitive(start_button, TRUE);
    gtk_widget_set_sensitive(reset_button, FALSE);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Pomodoro Timer");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_box = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    timer_label = gtk_label_new("25:00");
    gtk_label_set_selectable(GTK_LABEL(timer_label), TRUE);
    gtk_label_set_justify(GTK_LABEL(timer_label), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(main_box), timer_label, TRUE, TRUE, 0);

    GtkWidget *button_box = gtk_hbutton_box_new();
    gtk_box_pack_start(GTK_BOX(main_box), button_box, TRUE, TRUE, 0);

    start_button = gtk_button_new_with_label("Start");
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_timer), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), start_button);

    reset_button = gtk_button_new_with_label("Reset");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_timer), NULL);
    gtk_widget_set_sensitive(reset_button, FALSE);
    gtk_container_add(GTK_CONTAINER(button_box), reset_button);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}