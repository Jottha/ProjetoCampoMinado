//Header de Perdeu
int perdeu(int screenwidth, int screenheight, ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *backgr, ALLEGRO_BITMAP *bomba)
{
  //Desenha o PLano de Fundo
  al_draw_bitmap(backgr, 0,0, NULL);
  al_draw_bitmap(bomba, screenwidth,screenheight , NULL);
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_primitives_addon();
  //Definindo as cores a serem usadas
  ALLEGRO_COLOR Electric = al_map_rgb(255, 229, 32);
  ALLEGRO_COLOR BrightRed = al_map_rgb(250, 28, 3);
  ALLEGRO_COLOR Branco = al_map_rgb(255, 255, 255);
  ALLEGRO_COLOR Branco1, Branco2, Branco3, Branco4;
  //Carregando sons
  al_stop_sample(id_music);
  game_over = al_load_sample("songs/game_over.wav");
  clique_jogo = al_load_sample("songs/clique.wav");
  music = 1;
  id_game_over = malloc(sizeof(ALLEGRO_SAMPLE_ID));
  al_play_sample(game_over, 0.7, 0, 1, ALLEGRO_PLAYMODE_LOOP, id_game_over);
  //Definindo o Bitmaps
  ALLEGRO_BITMAP *area_facil = 0, *area_menu_fundo = NULL;
  area_facil = al_create_bitmap(360, 72);
  area_menu_fundo =   al_create_bitmap(360, 72);
  int  na_area_facil = 0;
  //Carregando fontes
  ALLEGRO_FONT *font2 = al_load_ttf_font("fonts/retro.ttf", 35, NULL);
  ALLEGRO_FONT *font1 = al_load_ttf_font("fonts/retro3d.ttf", 150, NULL);
  //Carregando eventos
  ALLEGRO_EVENT_QUEUE *backpress = al_create_event_queue();
  al_register_event_source(backpress, al_get_display_event_source(display));
  al_register_event_source(backpress, al_get_mouse_event_source());
  ALLEGRO_EVENT eventback;
  //Variáveis de controle
  int voltar = 0;
  int done = 0;
  float altura = 2.1;
  float distancia = 72;
  //Inicia o loop de eventos
  while(!done)
  {
    al_wait_for_event(backpress, &eventback); //Espera por algum evento
    int mousex = eventback.mouse.x; //Pega a posição do mouse
    int mousey = eventback.mouse.y; //Pega a posição do mouse
    if(eventback.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //Ao clicar
    {
      //Clique em "Voltar"
      if((400 < mousex) && (mousex < screenwidth / 1.7 + 65)
      && (mousey > screenheight / 1.8) && (mousey < screenheight / 1.8 + 72))
      {
        al_play_sample(clique_jogo, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_stop_sample(id_game_over);
        done = 1; //Acaba com o loop
      }
    }
    else if (eventback.type == ALLEGRO_EVENT_MOUSE_AXES) //Apontando para
    {
      //Sobre "Voltar"
      if((400 < mousex) && (mousex < screenwidth / 1.7 + 65) && (mousey > screenheight / 1.8) && (mousey < screenheight / 1.8 + 72))
        na_area_facil = 1;
      else
        na_area_facil = 0;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0)); //Limpa a tela
    //Desenha o Plano de Fundo
    al_draw_bitmap(backgr, 0, 0, NULL);
    al_draw_bitmap(bomba, screenwidth / 2 - 256, screenheight / 2 - 256, NULL);

    al_set_target_bitmap(area_menu_fundo);
    al_clear_to_color(al_map_rgba_f(0, 0, 0, 0.5)); //Limpa a tela
    //Modificando o Bitmap "area_voltar" com o efeito "Apontar"
    al_set_target_bitmap(area_facil);
    if (!na_area_facil)
    {
      al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
      Branco2 = al_map_rgb(255,255,255);
    }
    else
    {
      al_clear_to_color(al_map_rgba_f(0.2, 0.2, 0.2, 0.5));
      Branco2 = al_map_rgb(255,229,32);
    }
    //Usando a tela
    al_set_target_bitmap(al_get_backbuffer(display));
    //Desenha os elementos na tela
    al_draw_bitmap(area_menu_fundo, screenwidth / 2.8  , screenheight / 1.8 , 0.5);
    al_draw_rectangle(screenwidth/2 - 180, screenheight / 1.8, screenwidth/2 + 180, screenheight / 1.8 + 72, al_map_rgb(255, 255, 255), 6.0);
    al_draw_rectangle(screenwidth/2 - 186, screenheight / 1.8 - 6, screenwidth/2 + 186, screenheight / 1.8 + 78, al_map_rgb(0, 0, 0), 6.0);
    al_draw_bitmap(area_facil, screenwidth/2.8  , screenheight / 2.2 + distancia, 0.5);

    al_draw_text(font1,al_map_rgb(250,31,3),screenwidth/2,screenheight/4 - 100,ALLEGRO_ALIGN_CENTRE,"VOCE PERDEU");

    al_draw_text(font2, Branco2, screenwidth/2, screenheight/altura + distancia, ALLEGRO_ALIGN_CENTER, "VOLTAR AO MENU");
    //Atualiza o frame
    al_flip_display();
  }
  //Limpa a memória RAM
  al_destroy_font(font1);
  al_destroy_font(font2);
  al_destroy_bitmap(area_facil);
  al_destroy_bitmap(area_menu_fundo);
  al_destroy_sample(game_over);
  al_destroy_event_queue(backpress);
}