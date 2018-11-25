//Header de Ganhou
int ganhou(int screenwidth, int screenheight, ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *backgr, ALLEGRO_BITMAP *bomba, score atual)
{
  //Desenha o Plano de Fundo
  al_draw_bitmap(backgr, 0,0, NULL);
  al_draw_bitmap(bomba, screenwidth,screenheight , NULL);
  //Inicia dependências do Allegro
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_primitives_addon();
  //Definindo as cores a serem usadas
  ALLEGRO_COLOR Electric = al_map_rgb(255, 229, 32);
  ALLEGRO_COLOR BrightRed = al_map_rgb(250, 28, 3);
  ALLEGRO_COLOR Branco = al_map_rgb(255, 255, 255);
  ALLEGRO_COLOR Branco1, Branco2, Branco3, Branco4;
  //Carrega sons
  al_stop_sample(id_music);
  victory = al_load_sample("songs/victory.wav");
  music = 1;
  id_victory = malloc(sizeof(ALLEGRO_SAMPLE_ID));
  al_play_sample(victory, 0.7, 0, 1, ALLEGRO_PLAYMODE_LOOP, id_victory);
  //Definindo Bitmaps
  ALLEGRO_BITMAP *area_menu_fundo = NULL, *area_voltar = 0, *area_medio = 0, *area_facil = 0, *area_dificil = 0;
  area_voltar = al_create_bitmap(360, 76);
  int na_area_voltar = 0;
  //Carregando fontes
  ALLEGRO_FONT *font2 = al_load_ttf_font("fonts/retro.ttf", 35, NULL);
  ALLEGRO_FONT *font1 = al_load_ttf_font("fonts/retro3d.ttf", 100, NULL);
  //Carregando eventos
  al_install_keyboard();
  ALLEGRO_EVENT_QUEUE *backpress = al_create_event_queue();
  al_register_event_source(backpress, al_get_display_event_source(display));
  al_register_event_source(backpress, al_get_mouse_event_source());
  al_register_event_source(backpress, al_get_keyboard_event_source());
  ALLEGRO_EVENT eventback;
  //Variáveis de controle
  int done = 0;
  float altura = 2.1;
  float distancia = 72;
  int move;
  int voltar = 0; int letras = 0;
  //Inicia o loop de eventos
  while(!done)
  {
    al_wait_for_event(backpress, &eventback); //Espera por algum evento
    //Eventos com o teclado
    if (eventback.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      //Transformando a entrada do Allegro para string
      if (eventback.keyboard.keycode != 63)
      {
        if (eventback.keyboard.keycode < 27)
        {
          atual.nome[letras] = eventback.keyboard.keycode + 'A' - 1;
        }
        else
        {
          atual.nome[letras] = eventback.keyboard.keycode + 21;
        }
        letras ++;
      }
      else
      {
        atual.nome[letras] = ' ';
        letras --;
      }
      atual.nome[3] = ' ';
      if (letras < 0)
      {
        letras = 0;
      }
      if (letras >= 3)
      {
        letras = 2;
      }
    }
    //Eventos com o mouse
    int mousex = eventback.mouse.x; //Pega a posição do mouse
    int mousey = eventback.mouse.y; //Pega a posição do mouse
    if(eventback.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
      //Clique em "Voltar"
      if((screenwidth / 1.2 - 50 < mousex) && (mousex < screenwidth / 1.2 + 220)
      && (mousey > screenheight / 1.2 + 10 ) && (mousey < screenheight / 1.2  + 91))
      {
        move = letras;
        while (move < 4)
        {
          move ++;
          atual.nome[move] = ' ';
        }
        changeRankingData(atual); //Salva a pontuação
        al_stop_sample(id_victory);
        done = 1; //Acaba com o loop
      }
    }
    else if (eventback.type == ALLEGRO_EVENT_MOUSE_AXES) //Apontando para
    {
      //Sobre "Voltar"
      if((screenwidth / 1.2 - 50 < mousex) && (mousex < screenwidth / 1.2 + 220) && (mousey > screenheight / 1.2 + 10 ) && (mousey < screenheight / 1.2  + 91))
        na_area_voltar = 1;
      else
        na_area_voltar = 0;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0)); //Limpa a tela
    //Desenha o PLano de Fundo
    al_draw_bitmap(backgr, 0, 0, NULL);
    al_draw_bitmap(bomba, screenwidth / 2 - 256, screenheight / 2 - 256, NULL);
    //Modificando o Bitmap "area_voltar" com o efeito "Apontar"
    al_set_target_bitmap(area_voltar);
    if (!na_area_voltar)
    {
      al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
      Branco1 = al_map_rgb(255,255,255);
    }
    else
    {
      al_clear_to_color(al_map_rgba_f(0.2, 0.2, 0.2, 0.5));
      Branco1 = al_map_rgb(255,229,32);
    }
    //Usando a tela
    al_set_target_bitmap(al_get_backbuffer(display));
    //Desenha elementos na tela
    al_draw_bitmap(area_voltar, screenwidth / 1.2 - 50  , screenheight / 1.2 + 13 , 0.5);

    al_draw_text(font1,al_map_rgb(102,241,10),screenwidth/2,screenheight/4 - 150,ALLEGRO_ALIGN_CENTRE,"Parabens! Voce Ganhou.");
    al_draw_text(font2,al_map_rgb(255,229,32),screenwidth/2 - 40,screenheight/4 + 100,ALLEGRO_ALIGN_CENTRE,"Digite seu nome");
    al_draw_filled_rectangle(screenwidth/3 + 98 , screenheight /3  + 90, screenwidth /3 + 250, screenheight /3 + 150, al_map_rgba_f(0, 0, 0, 0.5));
    al_draw_rectangle(screenwidth /3 + 96, screenheight / 3 + 90, screenwidth / 3 + 250, screenheight / 3 + 150, al_map_rgb(0, 0, 0), 6.0);

    al_draw_rectangle(screenwidth / 1.2 - 50, screenheight / 1.2 + 10, screenwidth / 1.2 + 220, screenheight / 1.2 + 91, al_map_rgb(255, 255, 255), 6.0);
    al_draw_rectangle(screenwidth / 1.2 - 56, screenheight / 1.2 + 4, screenwidth / 1.2 + 226, screenheight / 1.2 + 97, al_map_rgb(0, 0, 0), 6.0);

    al_draw_text(font2, Branco1, 19*screenwidth / 20, 21*screenheight / 24, ALLEGRO_ALIGN_RIGHT, "Voltar");

    al_draw_text(font2, Branco, screenwidth/2 + distancia - 100, screenheight/altura, ALLEGRO_ALIGN_CENTER, atual.nome);
    //Atualiza o frame
    al_flip_display();
  }
  //Limpa a memória RAM
  al_destroy_event_queue(backpress);
  al_destroy_font(font1);
  al_destroy_font(font2);
  al_destroy_bitmap(area_voltar);
  al_destroy_bitmap(area_facil);
  al_destroy_bitmap(area_medio);
  al_destroy_bitmap(area_dificil);
  al_destroy_bitmap(area_menu_fundo);
  al_destroy_sample(victory);
}