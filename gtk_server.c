#include <gtk/gtk.h>

typedef struct _Chat chat;
struct _Chat 
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *label;
	GtkWidget *textView;
	GtkWidget *textBuffer;
	GtkWidget *textField;
};

// 메세지 전송 버튼 클릭 시
G_MODULE_EXPORT void on_btnSend_clicked (GtkWidget *button, Chat *chat)
{
	// text view 버퍼 가져오기
	GtkWidget *buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (chat->textView));
	
	gchar *message = gtk_entry_get_text (GTK_ENTRY (chat->textField));	

	// 버퍼초기화
	//gtk_text_buffer_set_text (buf, "good!", -1);


	gtk_text_buffer_insert_at_cursor (buf, message, -1);
	gtk_text_buffer_insert_at_cursor (buf, "\n", -1);

	gtk_label_set_text (GTK_LABEL (chat->label), "send");
	
}

G_MODULE_EXPORT void on_btnOpen_clicked (GtkButton *button, Chat *chat)
{
	gtk_label_set_text (GTK_LABEL (chat->label), "open");
}

int main (int argc, char *argv[])
{
	GtkBuilder *builder;
	GError *error;
	Chat *chat;
	gtk_init (&argc, &argv);
	
	// 빌더 생성 및 UI 파일 열기
	builder = gtk_builder_new ();

	chat = g_slice_new (Chat);
	chat->window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
	chat->button = GTK_WIDGET(gtk_builder_get_object (builder, "btnSend"));
	chat->button = GTK_WIDGET(gtk_builder_get_object (builder, "btnOpen"));
	chat->label = GTK_WIDGET(gtk_builder_get_object (builder, "label"));
	chat->textView = GTK_WIDGET(gtk_builder_get_object (builder, "textViewLog"));
	chat->textBuffer = GTK_WIDGET(gtk_builder_get_object (builder, "textBuffer"));
	chat->textField = GTK_WIDGET(gtk_builder_get_object (builder, "textFieldMessage"));

	gtk_builder_connect_signals (builder, chat);
	g_object_unref (G_OBJECT (builder));
	gtk_widget_show_all (chat->window);
	gtk_main ();
	g_slice_free (Chat, chat);
	return (0);
}
