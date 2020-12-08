#include <gtk/gtk.h>

typedef struct _Chat Chat;
struct _Chat {
  GtkWidget *window;
  GtkWidget *PortEntry; //포트번호의 텍스트 엔트리 위젯 포인터
  GtkWidget *ConBtn; //접속 버튼 위젯 포인터
  GtkWidget *SendBtn; //전송 버튼 위젯 포인터
  GtkWidget *ChatTextView; //채팅이 나타날 텍스트 뷰 위젯 포인터
  GtkWidget *msgEntry; //메시지가 적힐 텍스트 엔트리 위젯 포인터
};

G_MODULE_EXPORT void quit (GtkWidget *window,gpointer data){
	gtk_main_quit ();
}

G_MODULE_EXPORT void on_ConBtn_clicked(GtkButton *button, Chat *chat){
 GtkTextBuffer* buffer =gtk_text_view_get_buffer(GTK_TEXT_VIEW(chat->ChatTextView));
 GtkTextIter end;
//해당 텍스트뷰의 텍스트뷰버퍼 받아오기
 gtk_text_buffer_get_end_iter(buffer,&end);
//텍스트뷰 버퍼의 마지막에 해당하는 위치 가져오기.
 gtk_text_buffer_insert(buffer,&end,"Hello, this is some text\n", -1);
//마지막에 해당하는 위치에 계속 뿌리기.
 
}

int main (int argc, char *argv[]){
  GtkBuilder *builder;
  GError *error;
  Chat *chat;
  gtk_init (&argc, &argv);
  // 빌더 생성 및 UI 파일 열기
  builder = gtk_builder_new ();
 
  data = g_slice_new (Chat); //메모리 버퍼 초기화

  chat->window = GTK_WIDGET(gtk_builder_get_object (builder, "GtkWindow"));
  chat->PortEntry = GTK_WIDGET(gtk_builder_get_object (builder, "PortEntry"));
  chat->ConBtn = GTK_WIDGET(gtk_builder_get_object (builder, "ConBtn"));
  chat->SendBtn = GTK_WIDGET(gtk_builder_get_object (builder, "SendBtn"));
  chat->msgEntry = GTK_WIDGET(gtk_builder_get_object (builder, "msgEntry"));
  chat->ChatTextView = GTK_WIDGET(gtk_builder_get_object (builder, "ChatTextView"));

  gtk_builder_connect_signals (builder, chat);
  g_object_unref (G_OBJECT (builder));
  gtk_widget_show_all (chat->window);
  gtk_main ();
  g_slice_free (Chat, chat);
return (0);
}
