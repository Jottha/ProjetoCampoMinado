//Fun�oes auxiliares para limitar o fps
void iniciarTimer()
{
  tempoInicial = al_get_time();
}

double obterTempoTimer()
{
  return al_get_time() - tempoInicial;
}

void drawTempo() //Fun��o para desenhar o cronometro
{
	cronometro = al_get_time() - newTempo;
	sprintf(stringTempo, "%.3lf", cronometro);
  al_draw_filled_rectangle( 25 , 170, 300, 266, al_map_rgba_f(0, 0, 0, 1));
  al_draw_filled_rectangle( 25 , 290, 300, 386, al_map_rgba_f(0, 0, 0, 1));
	al_draw_text(font, branco, 160, 180, ALLEGRO_ALIGN_CENTER, "TEMPO:");
	al_draw_text(font, branco, 160, 230, ALLEGRO_ALIGN_CENTER, stringTempo);
	sprintf(stringFlags, "%d", flags);
	al_draw_text(font, branco, 160, 300, ALLEGRO_ALIGN_CENTER, "BOMBAS:");
	al_draw_text(font, branco, 160, 350, ALLEGRO_ALIGN_CENTER, stringFlags);
}

void load_images()
{
	imagem = al_load_bitmap("imgs/bl.png");
	vazio = al_load_bitmap("imgs/vazio.png");
	um = al_load_bitmap("imgs/1.png");
	dois = al_load_bitmap("imgs/2.png");
	tres = al_load_bitmap("imgs/3.png");
	quatro = al_load_bitmap("imgs/4.png");
	cinco = al_load_bitmap("imgs/5.png");
	seis = al_load_bitmap("imgs/6.png");
	sete = al_load_bitmap("imgs/7.png");
	oito = al_load_bitmap("imgs/8.png");
	bomba = al_load_bitmap("imgs/bomba.png");
	flag = al_load_bitmap("imgs/flag.png");
}

/////Alocar o tabuleiro 1 que � o tabuleiro com o conteudo
int** alocaMatriz()
{
	int i;
	int** matriz = (int**)calloc(altura, sizeof(int*));
	for (i = 0; i < altura; i++)
  {
		matriz[i] = (int*)calloc(largura, sizeof(int));
	}
	return matriz;
}

//alocar o tabuleiro 2 que � o tabuleiro que o usuario v�
char** alocaMatrizChar()
{
  int i;
  char** matriz = (char**)calloc(altura, sizeof(char*));
  for (i = 0; i < altura; i++)
  {
    matriz[i] = (char*)calloc(largura, sizeof(char));
  }
  return matriz;
}

 ALLEGRO_BITMAP *screenshot = NULL;

void drawCampo(char espaco, int j, int i)
{
	iniciarTimer();
	if (espaco == '0' || espaco == '9')
		al_draw_bitmap(vazio, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '1')
		al_draw_bitmap(um, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '2')
		al_draw_bitmap(dois, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '3')
		al_draw_bitmap(tres, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '4')
		al_draw_bitmap(quatro, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '5')
		al_draw_bitmap(cinco, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '6')
		al_draw_bitmap(seis, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '7')
		al_draw_bitmap(sete, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '8')
		al_draw_bitmap(oito, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else if (espaco == '/')
    {
		  al_draw_bitmap(bomba, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
      //Animação de explosão
      explosao = al_load_sample("songs/explosao.wav");
      al_play_sample(explosao, 0.7, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
      struct Explosion explosions[NUM_EXPLOSIONS];
      ALLEGRO_BITMAP *expImage;
      expImage = al_load_bitmap("imgs/explosion.png");
      InitExplosions(explosions, NUM_EXPLOSIONS, expImage);
      StartExplosions(explosions, NUM_EXPLOSIONS,(distanciax + i * 25) * multiplicador_tamanho + 20 , (distanciay + j * 25) * multiplicador_tamanho + 20) ;  ////////////EXPLOSAO//////////////
      int l;
      al_save_bitmap("arquivo.png", al_get_backbuffer(janela));
      screenshot = al_load_bitmap("arquivo.png");
      for(l = 0; l < 10; l++)
      {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(screenshot, 0, 0, 0);
        DrawExplosions(explosions, NUM_EXPLOSIONS);
        UpdateExplosions(explosions, NUM_EXPLOSIONS);
        al_rest(0.01);
        al_flip_display();
      }
      al_destroy_bitmap(expImage);
      al_destroy_bitmap(screenshot);
    }
	else if (espaco == 'f')
		al_draw_bitmap(flag, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	else
		al_draw_bitmap(imagem, (distanciax + i * 25) * multiplicador_tamanho, (distanciay + j * 25) * multiplicador_tamanho, 0);
	drawTempo();

    //Verifica��o para controle de fps
	if ((obterTempoTimer() < 1.0 / FRAMES_POR_SEGUNDO))
		al_rest((1.0 / FRAMES_POR_SEGUNDO) - obterTempoTimer());
	al_flip_display();
}

void floodfill(int jogada_linha, int jogada_coluna)
{
	iniciarTimer();
  if(tabuleiro[jogada_linha][jogada_coluna] == 0)
  {
    tabuleiro[jogada_linha][jogada_coluna] = 9;
    tabuleiro2[jogada_linha][jogada_coluna] = '0';

		win --;

		al_draw_bitmap(vazio, (distanciax + jogada_coluna * 25) * multiplicador_tamanho, (distanciay + jogada_linha * 25) * multiplicador_tamanho, 0);
		drawTempo();
		if ((obterTempoTimer() < 1.0 / FRAMES_POR_SEGUNDO))
			al_rest((1.0 / FRAMES_POR_SEGUNDO) - obterTempoTimer());
		al_flip_display();


  //verifica�oes para nao fazer o floodfill fora do tabuleiro
    if(jogada_linha > 0 && jogada_coluna > 0)
      floodfill(jogada_linha - 1, jogada_coluna - 1);

    if(jogada_linha > 0 && jogada_coluna < largura - 1)
      floodfill(jogada_linha - 1, jogada_coluna + 1);

    if(jogada_linha < altura - 1 && jogada_coluna > 0)
      floodfill(jogada_linha + 1, jogada_coluna - 1);

    if(jogada_linha > 0 && jogada_coluna < largura - 1)
      floodfill(jogada_linha - 1, jogada_coluna + 1);

    if(jogada_linha < altura - 1)
      floodfill(jogada_linha + 1, jogada_coluna);

    if(jogada_coluna < largura - 1)
      floodfill(jogada_linha, jogada_coluna + 1);

    if(jogada_linha > 0)
      floodfill(jogada_linha - 1, jogada_coluna);

    if(jogada_coluna > 0)
      floodfill(jogada_linha , jogada_coluna - 1);
  }
  else
    return;
}

//Revelar os numeros em volta do floodfill para completa-lo
void completarFloodfill()
{
  int i;
  int j;
  for(i = 0; i < altura; i++)
  {
    for(j = 0; j < largura; j++)
    {
      if(tabuleiro2[i][j] == '0')
      {
        if(i > 0 && j > 0 && tabuleiro2[i - 1][j - 1] == '#')
        {
        	tabuleiro2[i - 1][j - 1] = '0' + tabuleiro[i - 1][j - 1];
					drawCampo(tabuleiro2[i - 1][j - 1], i - 1, j - 1);
					win --;
        }
        if(i > 0 && tabuleiro2[i - 1][j] == '#')
        {
          tabuleiro2[i - 1][j] = '0' + tabuleiro[i - 1][j];
					drawCampo(tabuleiro2[i - 1][j], i - 1, j);
					win --;
        }
        if(i > 0 && j < largura - 1 && tabuleiro2[i - 1][j + 1] == '#')
        {
          tabuleiro2[i - 1][j + 1] = '0' + tabuleiro[i - 1][j + 1];
					drawCampo(tabuleiro2[i - 1][j + 1], i - 1, j + 1);
					win --;
        }

        if(j > 0 && tabuleiro2[i][j - 1] == '#')
        {
          tabuleiro2[i][j - 1] = '0' + tabuleiro[i][j - 1];
					drawCampo(tabuleiro2[i][j - 1], i, j - 1);
					win --;
        }
        if(j < altura - 1 && tabuleiro2[i][j + 1] == '#')
        {
          tabuleiro2[i][j + 1] = '0' + tabuleiro[i][j + 1];
					drawCampo(tabuleiro2[i][j + 1], i, j + 1);
					win --;
        }

        if(i < altura - 1 && j > 0 && tabuleiro2[i + 1][j - 1] == '#')
        {
          tabuleiro2[i + 1][j - 1] = '0' + tabuleiro[i + 1][j - 1];
					drawCampo(tabuleiro2[i + 1][j - 1], i + 1, j - 1);
					win --;
        }
        if(i < altura - 1 && tabuleiro2[i + 1][j] == '#')
        {
          tabuleiro2[i + 1][j] = '0' + tabuleiro[i + 1][j];
					drawCampo(tabuleiro2[i + 1][j], i + 1, j);
					win --;
        }
        if(i < altura - 1 && j < largura - 1 && tabuleiro2[i + 1][j + 1] == '#')
        {
          tabuleiro2[i + 1][j + 1] = tabuleiro[i + 1][j + 1] + '0';
					drawCampo(tabuleiro2[i + 1][j + 1], i + 1, j + 1);
					win --;
        }
			}
    }
  }
}

//revelar todas as bombas quando o usuario perder
void explodir_tudo(int jogada_linha, int jogada_coluna)
{
	FRAMES_POR_SEGUNDO = 60;
	int i, j, iFim = 2, jFim = 2;
	while (bombas > 1)
	{
		for (i = 1; i < iFim; i ++)
		{
			jogada_linha --;
			if (all_directions)
			{
				if (tabuleiro[jogada_linha][jogada_coluna] == -1)
				{
					bombas --;
					jogada(jogada_linha, jogada_coluna);
				}
			}
		}
		for (j = 1; j < jFim; j ++)
		{
			jogada_coluna ++;
			if (all_directions)
			{
				if (tabuleiro[jogada_linha][jogada_coluna] == -1)
				{
					bombas --;
					jogada(jogada_linha, jogada_coluna);
				}
			}
		}
		iFim ++; jFim ++;
		for (i = 1; i < iFim; i ++)
		{
			jogada_linha ++;
			if (all_directions)
			{
				if (tabuleiro[jogada_linha][jogada_coluna] == -1)
				{
					bombas --;
					jogada(jogada_linha, jogada_coluna);
				}
			}
		}
		for (j = 1; j < jFim; j ++)
		{
			jogada_coluna --;
			if (all_directions)
			{
				if (tabuleiro[jogada_linha][jogada_coluna] == -1)
				{
					bombas --;
					jogada(jogada_linha, jogada_coluna);
				}
			}
		}
		iFim ++; jFim ++;
	}
}

////////fun�ao para quando o jogador clicar em um bloco ja revelado, revelar oque esta em volta
void contorno(int jogada_linha, int jogada_coluna)
{
	int i, j;
	for (i = jogada_linha - 1; i < jogada_linha + 2; i ++)
	{
		for (j = jogada_coluna - 1; j < jogada_coluna + 2; j ++)
		{
			if (i >= 0 && j >= 0 && i < altura && j < largura && tabuleiro2[i][j] == '#')
			{
				jogada(i, j);
			}
		}
	}
}

void gerarTabuleiro()
{
  int i, j;
  for(i = 0; i < altura; i++)
  {
    for(j = 0; j < largura; j++)
    {
      tabuleiro2[i][j] = '#';
    }
  }
}

void jogada(int jogada_linha, int jogada_coluna)
{
  ALLEGRO_SAMPLE_ID *id_sample = NULL;
  id_sample = malloc(sizeof(ALLEGRO_SAMPLE_ID));

  //tabuleiro visivel(tabuleiro2) se torna o tabuleiro com o conteudo(tabuleiro) pois o jogador clicou nele
  tabuleiro2[jogada_linha][jogada_coluna] = '0' + tabuleiro[jogada_linha][jogada_coluna];
	drawCampo(tabuleiro2[jogada_linha][jogada_coluna], jogada_linha, jogada_coluna);

	//se for -1, o jogador perdeu
  if(tabuleiro[jogada_linha][jogada_coluna] == -1)
  {
    lose = 1;
  }
  //se for 0 o espa�o � vazio e chamara a floodfill
  else if(tabuleiro[jogada_linha][jogada_coluna] == 0)
  {
    al_play_sample(floodfill_som, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, id_sample);
    floodfill(jogada_linha, jogada_coluna);
    completarFloodfill();
    al_rest(0.01);
    al_stop_sample(id_sample);
  }
  //caso contrario, apenas revela o bloco
  else
  {
    al_play_sample(clique_menu, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		win --;
  }
}

void liberaMatriz()
{
	int i;
	for (i = 0; i < altura; i++)
  {
		free(tabuleiro[i]);
	}
	free(tabuleiro);
}

//armazena o campo no bloco de notas
void fprintaMatriz()
{
	int i, j;
	for (i = 0; i < altura; i++)
  {
		for (j = 0; j < largura; j++)
    {
			if (tabuleiro[i][j] >= 0)
      {
				fprintf(campo, " ");
			}
			fprintf(campo, "%d ", tabuleiro[i][j]);
		}
		fprintf(campo, "\n");
	}
	fprintf(campo, "\n");
}

int EstaNoCampo(int x, int y)
{
	if (x >= 0 && x < altura)
  {
		if (y >= 0 && y < largura)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

//preencher o tabuleiro com os numeros de acordo com quantas bombas existem adjacentes
void Checar(int x, int y)
{
	int aux = 0, i, j;
	for (i = x - 1; i <= x + 1; i++)
  {
		for (j = y - 1; j <= y + 1; j++)
    {
			if (EstaNoCampo(i, j))
      {
				if (tabuleiro[i][j] == -1)
        {
					aux++;
				}
			}
		}
	}
	tabuleiro[x][y] = aux;
}


void fazerTabuleiro()
{
	int x, y, random = 1, i, j;
	if (!bombas){
		for (i = 0; i < altura; i++)
    {
			for (j = 0; j < largura; j++)
      {
				if (tabuleiro[i][j] == 0)
        {
					Checar(i, j);
				}
			}
		}
		fprintaMatriz();
		for (i = 0; i < altura; i++)
    {
			for (j = 0; j < largura; j++)
      {
				if (tabuleiro[i][j] > 0)
        {
					tabuleiro[i][j] = 0;
				}
			}
		}
	}
	else
  {
		srand((time(NULL)) / bombas);
		while(random)
    {
			y = rand() % largura;
			x = rand() % altura;
			random--;
      //Não preenche lugares que já têm bomba ou onde foi o primeiro clique
			if (tabuleiro[x][y] == -1 || (x == firstI && y == firstJ))
				random++;
		}
		tabuleiro[x][y] = -1;
		bombas --;
		fazerTabuleiro();
	}
}

int GdT()
{
	tabuleiro = alocaMatriz();
	campo = fopen("campo.txt", "w");
	fazerTabuleiro();
	liberaMatriz();
	fclose(campo);
	return 0;
}

void leMatriz()
{
	int i, j;
	for (i = 0; i < altura; i++)
  {
		for (j = 0; j < largura; j++)
    {
			fscanf(campo, "%d", &tabuleiro[i][j]);
		}
	}
}

int jogar(ALLEGRO_BITMAP *backgr, ALLEGRO_BITMAP *bomba)
{
    //carregar fontes e cores
	font = al_load_ttf_font("fonts/retro.ttf", 35, NULL);
  ALLEGRO_FONT *font2 = al_load_ttf_font("fonts/retro.ttf", 35, NULL);
	branco = al_map_rgb(255, 255, 255);

  //stuct do score atual para ser salvo no ranking
	score atual;
  double cronometroInicial, cronometroFinal;

  //limpar a tela e desenhar o background
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_bitmap(backgr, 0, 0, NULL);
  al_draw_bitmap(bomba, LARGURA_TELA / 2 - 256, ALTURA_TELA / 2 - 256, NULL);

  int i, j;
	int temp, mousex, mousey;
	lose = 0; win = 1;

	//aonde o tabuleiro vai ficar dependendo da dificuldade
  if (dificuldade == 1)
  {
    largura = 8; altura = 8; bombas = 9;
		distanciax = 220;
		distanciay = 80;
		multiplicador_tamanho = 2;
		compensacao = 200;
  }
  else if (dificuldade == 2)
  {
    largura = 10; altura = 10; bombas = 15;
		distanciax = 195;
		distanciay = 55;
		multiplicador_tamanho = 2;
		compensacao = 250;
  }
  else
  {
    largura = 12; altura = 12; bombas = 30;
		distanciax = 170;
		distanciay = 30;
		multiplicador_tamanho = 2;
		compensacao = 300;
  }
  //calculo para quantos blocos voce ter� que revelar para ganhar
	win = (largura * altura) - bombas;

	load_images();

	//carregar os sons
  clique_menu = al_load_sample("songs/clique_menu.wav");
  explosao = al_load_sample("songs/explosao.wav");
  floodfill_som = al_load_sample("songs/floodfill.wav");
  inicio = al_load_sample("songs/inicio.wav");
  set_flag = al_load_sample("songs/set_flag.wav");
  take_flag = al_load_sample("songs/take_flag.wav");


  //desenhar o tabuleiro com anima��o
	for (i = 0; i < altura; i ++)
	{
		for (j = 0; j < largura; j ++)
		{
			iniciarTimer();
			al_draw_bitmap(imagem, (distanciax + j * 25) * multiplicador_tamanho, (distanciay + i * 25) * multiplicador_tamanho, 0);
			if ((obterTempoTimer() < 1.0 / FRAMES_POR_SEGUNDO))
			{
				al_rest((1.0 / FRAMES_POR_SEGUNDO) - obterTempoTimer());
			}
			al_flip_display();
		}
	}

  al_play_sample(inicio, 1.4, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);

  //tamanho do retangulo que � a borda do tabuleiro
  if(dificuldade == 1)
    al_draw_rectangle(438, 160, 843, 563, al_map_rgb(0, 0, 0), 5.0);
  if(dificuldade == 2)
    al_draw_rectangle(390, 110, 890, 610, al_map_rgb(0, 0, 0), 5.0);
  if(dificuldade == 3)
    al_draw_rectangle(339, 60, 943, 665, al_map_rgb(0, 0, 0), 5.0);

  //desenhar os retangulos da parte esquerda da tela do jogo
  ALLEGRO_COLOR Branco1;
  Branco1 = al_map_rgb(255,255,255);
  al_draw_rectangle( 20 , -10, 305, 805, al_map_rgb(0, 0, 0), 7.0);
  al_draw_rectangle( 22 , -10, 303, 803, al_map_rgb(253, 152, 0), 7.0);
  al_draw_filled_rectangle( 25 , -10, 300, 800, al_map_rgba_f(0, 0, 0, 0.5));
  al_draw_text(font2, Branco1, 160, 500, ALLEGRO_ALIGN_CENTER, "Reiniciar");
  al_draw_text(font2, Branco1, 160 , 600, ALLEGRO_ALIGN_CENTER, "Voltar");

  int sair = 0, first = 0;

  //declarar a struct explosao carregar a imagem e inicializar a explosao
  struct Explosion explosions[NUM_EXPLOSIONS];
  ALLEGRO_BITMAP *expImage;
  expImage = al_load_bitmap("imgs/explosion.png");
  InitExplosions(explosions, NUM_EXPLOSIONS, expImage);

  while (!sair)
  {
    iniciarTimer();
    if (lose == 1)
    {
      explodir_tudo(i, j);
      FRAMES_POR_SEGUNDO = 120;
      al_rest(0.6);
      al_destroy_event_queue(fila_eventos);
      perdeu(LARGURA_TELA, ALTURA_TELA, janela, backgr, bomba);
      fila_eventos = al_create_event_queue();
      sair = 1; continue;
    }

    if (win == 0)
    {
      cronometroFinal = al_get_time() - cronometroInicial;
      atual.tempo = cronometroFinal;
      al_clear_to_color(al_map_rgb(255,255,255));
      al_destroy_event_queue(fila_eventos);
      ganhou(LARGURA_TELA, ALTURA_TELA, janela, backgr, bomba, atual);
      fila_eventos = al_create_event_queue();
      sair = 1; continue;
    }

    //enquanto tiver eventos na fila de eventos
    while (!al_is_event_queue_empty(fila_eventos))
    {
      ALLEGRO_EVENT evento;
      al_wait_for_event(fila_eventos, &evento);

      if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
      {
				al_get_mouse_state(&state);
				mousex = evento.mouse.x;
				mousey = evento.mouse.y;

        //caso o jogador clique em reiniciar
				if((25 < mousex) && (mousex < 290) && (mousey > 490 ) && (mousey < 550))
        {
          al_play_sample(clique_jogo, 1.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          al_destroy_sample(clique_menu);
          al_destroy_sample(explosao);
          al_destroy_sample(floodfill_som);
          al_destroy_sample(inicio);
          al_destroy_sample(set_flag);
          al_destroy_sample(take_flag);
          al_destroy_font(font);
          al_destroy_font(font2);
          al_destroy_bitmap(imagem);
          al_destroy_bitmap(vazio);
          al_destroy_bitmap(um);
          al_destroy_bitmap(dois);
          al_destroy_bitmap(tres);
          al_destroy_bitmap(quatro);
          al_destroy_bitmap(cinco);
          al_destroy_bitmap(seis);
          al_destroy_bitmap(sete);
          al_destroy_bitmap(oito);
          al_destroy_bitmap(flag);
          al_destroy_bitmap(expImage);
          jogar(backgr, bomba);
          return;
        }

        //caso o jogador clique em voltar ao menu
        if((25 < mousex) && (mousex < 290) && (mousey > 575 ) && (mousey < 660))
        {
          al_play_sample(clique_jogo, 1.2, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          sair = 1; continue;
        }

                //calculo para identificar em qual lugar da matriz o jogador clicou
				temp = mousex - LARGURA_TELA/2 + compensacao;
				j = temp / 50;
				temp = mousey - ALTURA_TELA/2 + compensacao;
				i = temp / 50;

				//se for o botao esquerdo ele vai revelar a casa
				if (state.buttons & 1)
				{
					if(i >= 0 && i < altura && j >= 0 && j < largura)
					{
            /*FEATURE!!!!!!!!!!!!!!
            O mapa só é gerado depois do primeiro clique,
            e ele é gerado de forma que nunca mate você no começo!
            */
						if (first == 0)
						{
							aux = bombas;
							first = 1; firstI = i; firstJ = j;
							GdT();
							campo = fopen("campo.txt", "r");
							tabuleiro = alocaMatriz();
							tabuleiro2 = alocaMatrizChar();
							leMatriz();
							gerarTabuleiro();
							bombas = aux; flags = bombas;
							cronometroInicial = al_get_time();
							newTempo = al_get_time();
						}
						if (tabuleiro2[i][j] == '#')
						{
							jogada(i, j);
						}
						else if (tabuleiro2[i][j] != '/' && tabuleiro2[i][j] != 'f')
						{
              contorno(i, j);
						}
					}
				}
				//se for o botao direito o jogador vai tirar ou colocar uma flag
				if (state.buttons & 2)
				{
					if(i >= 0 && i < altura && j >= 0 && j < largura && first == 1)
					{
						if (tabuleiro2[i][j] == '#')
						{
              al_play_sample(set_flag, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							tabuleiro2[i][j] = 'f';
							flags --;
							drawCampo(tabuleiro2[i][j], i, j);
						}
						else if (tabuleiro2[i][j] == 'f')
						{
              al_play_sample(take_flag, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
							tabuleiro2[i][j] = '#';
							flags ++;
							drawCampo(tabuleiro2[i][j], i, j);
						}
					}
          else
          {
            al_play_sample(take_flag, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
          }
				}
			}
		}
		if (first != 0)
		{
			drawTempo();
		}

    // Atualiza a tela
    if ((obterTempoTimer() < 1.0 / FRAMES_POR_SEGUNDO))
    {
    	al_rest((1.0 / FRAMES_POR_SEGUNDO) - obterTempoTimer());
    }
		al_flip_display();
  }
  //liberar a ram
  al_destroy_font(font);
  al_destroy_font(font2);
  al_destroy_sample(clique_menu);
  al_destroy_sample(explosao);
  al_destroy_sample(floodfill_som);
  al_destroy_sample(inicio);
  al_destroy_sample(set_flag);
  al_destroy_sample(take_flag);
  al_destroy_bitmap(imagem);
  al_destroy_bitmap(vazio);
  al_destroy_bitmap(um);
  al_destroy_bitmap(dois);
  al_destroy_bitmap(tres);
  al_destroy_bitmap(quatro);
  al_destroy_bitmap(cinco);
  al_destroy_bitmap(seis);
  al_destroy_bitmap(sete);
  al_destroy_bitmap(oito);
  al_destroy_bitmap(flag);
  al_destroy_bitmap(expImage);
}