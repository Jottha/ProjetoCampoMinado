//Header do Ranking
int ranking(int screenwidth, int screenheight, ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *backgr, ALLEGRO_BITMAP *bomba)
{
  //Pega o ranking
  char archive[100];
  if (dificuldade == 1)
  	strcpy(archive, "rankingFACIL.txt");
  else if (dificuldade == 2)
  	strcpy(archive, "rankingMEDIO.txt");
  else if (dificuldade == 3)
  	strcpy(archive, "rankingDIFICIL.txt");
  //Salva no struct
  score loadRanking[100];
  getRanking(loadRanking, archive);

  if (dificuldade == 1)
    strcpy(archive, "rankingNomeFACIL.txt");
  else if (dificuldade == 2)
    strcpy(archive, "rankingNomeMEDIO.txt");
  else if (dificuldade == 3)
    strcpy(archive, "rankingNomeDIFICIL.txt");
  //Salva no struct
  score loadNome[100];
  getRanking(loadNome, archive);

  //Desenha o PLano de Fundo
  al_draw_bitmap(backgr, 0,0, NULL);
  al_draw_bitmap(bomba, screenwidth,screenheight , NULL);
  //Inicia dependências do Allegro
  al_init_font_addon();
  al_init_ttf_addon();
  al_init_primitives_addon();
  //Definindo as cores a serem usadas
  ALLEGRO_COLOR Electric = al_map_rgb(236, 242, 22);
  ALLEGRO_COLOR BrightRed = al_map_rgb(168, 200, 249);
  ALLEGRO_COLOR Branco = al_map_rgb(255, 255, 255);
  ALLEGRO_COLOR Branco1, Branco2, Branco3, Branco4;
  //Definindo o bitmaps
  ALLEGRO_BITMAP *area_menu_fundo = NULL, *area_voltar = 0, *area_medio = 0, *area_facil = 0, *area_dificil = 0;
  area_voltar = al_create_bitmap(360, 76);
  int na_area_voltar = 0;
  //Carregando fontes
  ALLEGRO_FONT *font2 = al_load_ttf_font("fonts/retro.ttf", 40, NULL);
  ALLEGRO_FONT *font1 = al_load_ttf_font("fonts/retro.ttf", 110, NULL);
  ALLEGRO_FONT *font3 = al_load_ttf_font("fonts/retro.ttf", 45, NULL);
  ALLEGRO_FONT *font4 = al_load_ttf_font("fonts/retro.ttf", 30, NULL);
  //Carregando sons
  clique_jogo = al_load_sample("songs/clique.wav");
  //Carregando eventos
  al_install_keyboard();
  ALLEGRO_EVENT_QUEUE *backpress = al_create_event_queue();
  al_register_event_source(backpress, al_get_display_event_source(display));
  al_register_event_source(backpress, al_get_mouse_event_source());
  al_register_event_source(backpress, al_get_keyboard_event_source());
  ALLEGRO_EVENT eventback;
  //Variáveis de controle
  float altura = 2.1;
  float distancia = 75;
  char searchField[4];
  int voltar = 0;
  int i;
  //Limpando a string
  for (i = 0; i <= 4; i ++)
  {
    searchField[i] = '\0';
  }
  int done = 0, letras = 0, move, find = -1;
  //Inicia o loop de eventos
  while(!done)
  {
    al_wait_for_event(backpress, &eventback); //Espera por um event
    int mousex = eventback.mouse.x; //Pega a posição do mouse
    int mousey = eventback.mouse.y; //Pega a posição do mouse
    if(eventback.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) //Ao clicar
    {
      //Clique em "Voltar"
      if((screenwidth / 1.2 - 50 < mousex) && (mousex < screenwidth / 1.2 + 220) && (mousey > screenheight / 1.2 + 10 ) && (mousey < screenheight / 1.2  + 91))
      {
        al_play_sample(clique_jogo, 1.4, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
        done = 1; //Acaba com o loop
      }
    }
    else if (eventback.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
      //Sobre "Voltar"
      if((screenwidth / 1.2 - 50 < mousex) && (mousex < screenwidth / 1.2 + 220)
      && (mousey > screenheight / 1.2 + 10 ) && (mousey < screenheight / 1.2  + 91))
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
    //Desenha os elementos na tela
    al_draw_bitmap(area_voltar, screenwidth / 1.2 - 50  , screenheight / 1.2 + 13 , 0.5);
    al_draw_text(font1, Branco, screenwidth/2 - 350, screenheight/10 - 50, ALLEGRO_ALIGN_CENTER, "RANKING");
    al_draw_text(font4, Branco, screenwidth/2 - 350, screenheight/10 + 60, ALLEGRO_ALIGN_CENTER, "Digite seu nome aqui");
    al_draw_text(font4, Branco, screenwidth/2 - 350, screenheight/10 + 100, ALLEGRO_ALIGN_CENTER, "para saber sua colocacao");
    //Desenha o nome das pessoas no Top-5
    int i;
    for (i = 0; i < 5; i ++)
    {
      char stringTempo[50];
      char colocacao[50];
      sprintf(stringTempo, "%.4lf", loadRanking[i].tempo);
      sprintf(colocacao, "%d", i + 1); // <---- Esse aqui é o tempo do cara
      i ++;
      al_draw_text(font3, Branco, screenwidth/2 - 290, screenheight/4 + (distancia*i) + 80, ALLEGRO_ALIGN_CENTER, colocacao);
      al_draw_text(font3, BrightRed, screenwidth/2 - 190, screenheight/4 + (distancia*i) + 80, ALLEGRO_ALIGN_CENTER, loadRanking[i - 1].nome);
      al_draw_text(font3, Branco, screenwidth/2 + 20, screenheight/4 + (distancia*i) + 80, ALLEGRO_ALIGN_CENTER, "TEMPO:");
      al_draw_text(font3, Electric, screenwidth/2 + 200, screenheight/4 + (distancia*i) + 80, ALLEGRO_ALIGN_CENTER, stringTempo);
      al_draw_rectangle(screenwidth /2 - 350, screenheight /4 + (distancia*i) - 10 + 80, screenwidth /2 + 300, screenheight /4 + (distancia*i) + 145, al_map_rgb(255, 255, 255), 6.0);
      i --;
    }

    al_draw_filled_rectangle(screenwidth/3 + 98 , screenheight /3 - 100, screenwidth /3 + 250, screenheight /3 - 40, al_map_rgba_f(0, 0, 0, 0.5));
    al_draw_rectangle(screenwidth /3 + 96, screenheight / 3 - 100, screenwidth / 3 + 250, screenheight / 3 - 40, al_map_rgb(0, 0, 0), 6.0);

    al_draw_text(font2, Branco, screenwidth /3 + 200, screenheight / 3 - 90, ALLEGRO_ALIGN_CENTER, searchField);

    //Eventos do teclado
    if (eventback.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      //Convertendo de Allegro para string
      if (eventback.keyboard.keycode != 63)
      {
        if (eventback.keyboard.keycode < 27)
        {
          searchField[letras] = eventback.keyboard.keycode + 'A' - 1;
        }
        else
        {
          searchField[letras] = eventback.keyboard.keycode + 21;
        }
        letras ++;
      }
      else
      {
        searchField[letras] = ' ';
        letras --;
      }
      if (letras < 0)
      {
        letras = 0;
      }
      if (letras >= 3)
      {
        letras = 2;
      }
      searchField[3] = '\0';
      move = letras;
      while (move <= 4)
      {
        move ++;
        searchField[move] = '\0';
      }
      printf("%s\n", searchField);
      //Pesquisando pelo nome
      find = buscaBinaria(0, 100, searchField, loadNome);
      while (move > letras + 1)
      {
        searchField[move] = '\0';
        move --;
      }
    }
    //Se achado o nome, desenhe-o
    if (find >= 0)
    {
      char stringTempo[50];
      char colocacao[50];
      sprintf(stringTempo, "%.4lf", loadRanking[find].tempo);
      sprintf(colocacao, "%d", find + 1); // <---- Esse aqui Ã© o tempo do cara
      al_draw_text(font3, Branco, screenwidth/2 - 290, screenheight/4 + 80, ALLEGRO_ALIGN_CENTER, colocacao);
      al_draw_text(font3, BrightRed, screenwidth/2 - 190, screenheight/4 + 80, ALLEGRO_ALIGN_CENTER, loadRanking[find].nome);
      al_draw_text(font3, Branco, screenwidth/2 + 20, screenheight/4 + 80, ALLEGRO_ALIGN_CENTER, "TEMPO:");
      al_draw_text(font3, Electric, screenwidth/2 + 200, screenheight/4 + 80, ALLEGRO_ALIGN_CENTER, stringTempo);
      al_draw_rectangle(screenwidth /2 - 350, screenheight /4 - 10 + 80, screenwidth /2 + 300, screenheight /4 + 145, al_map_rgb(246, 0, 33), 6.0);
    }

    al_draw_rectangle(screenwidth / 1.2 - 50, screenheight / 1.2 + 10, screenwidth / 1.2 + 220, screenheight / 1.2 + 91, al_map_rgb(255, 255, 255), 6.0);
    al_draw_rectangle(screenwidth / 1.2 - 56, screenheight / 1.2 + 4, screenwidth / 1.2 + 226, screenheight / 1.2 + 97, al_map_rgb(0, 0, 0), 6.0);

    al_draw_text(font2, Branco1, 19*screenwidth / 20, 21*screenheight / 24, ALLEGRO_ALIGN_RIGHT, "Voltar");
    //Atualiza o frame
    al_flip_display();
  }
  //Limpa a memória RAM
  al_destroy_font(font1);
  al_destroy_font(font2);
  al_destroy_font(font3);
  al_destroy_font(font4);
  al_destroy_bitmap(area_voltar);
  al_destroy_bitmap(area_facil);
  al_destroy_bitmap(area_medio);
  al_destroy_bitmap(area_dificil);
  al_destroy_bitmap(area_menu_fundo);
  al_destroy_event_queue(backpress);
}