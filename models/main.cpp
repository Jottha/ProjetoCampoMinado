#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#define all_directions jogada_coluna >= 0 && jogada_coluna < largura && jogada_linha < altura && jogada_linha >= 0

typedef struct ALLEGRO_MOUSE_STATE ALLEGRO_MOUSE_STATE;

//Constantes de tela
const int LARGURA_TELA = 1280;//Largura
const int ALTURA_TELA = 720;//Altura

int largura;
int altura;
int bombas;
int compensacao;
int flags;
char stringFlags[100];
int **tabuleiro;
char **tabuleiro2;
FILE *campo;

int FRAMES_POR_SEGUNDO = 120;
double tempoInicial = 0;
double cronometro;
double newTempo;
char stringTempo[100];
int distanciax;
int distanciay;
float multiplicador_tamanho;
int firstI = -1, firstJ = -1;
int win, lose, aux;
int dificuldade;

//Declara eventos
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_MOUSE_STATE state;

//Declara BITMAPS
ALLEGRO_BITMAP *botao_sair = NULL;

//////////////////////SPRITES DAS CASAS////////////////////////////////////////
ALLEGRO_BITMAP *imagem = NULL, *vazio = NULL, *um = NULL, *dois = NULL, *tres = NULL;
ALLEGRO_BITMAP *quatro = NULL, *cinco = NULL, *seis = NULL, *sete = NULL, *oito = NULL;
ALLEGRO_BITMAP *bomba = NULL, *flag = NULL;

//Declara fontes e cores
ALLEGRO_FONT *font;
ALLEGRO_COLOR branco;

//Declara sons
ALLEGRO_SAMPLE *musica = NULL;//Musica de fundo
ALLEGRO_SAMPLE *clique_jogo = NULL;//Som de clique no menu
ALLEGRO_SAMPLE *clique_menu = NULL;//Som de clique durante a gameplay
ALLEGRO_SAMPLE *explosao = NULL;//Som de explosão
ALLEGRO_SAMPLE *floodfill_som = NULL;//Som usado no floodfield
ALLEGRO_SAMPLE *inicio = NULL;//Som de inicio
ALLEGRO_SAMPLE *set_flag = NULL;//Som de colocar bandeira
ALLEGRO_SAMPLE *take_flag = NULL;//Som de tirar bandeira
ALLEGRO_SAMPLE *game_over = NULL;//Som de fim de jogo
ALLEGRO_SAMPLE *victory = NULL;//som de vitória
ALLEGRO_SAMPLE_ID *id_music = NULL;//ID da musica de fundo
ALLEGRO_SAMPLE_ID *id_game_over = NULL;//ID do som de fim de jogo
ALLEGRO_SAMPLE_ID *id_victory = NULL;//ID do som de vitória

//Declara a janela
ALLEGRO_DISPLAY *janela = 0;

int music = 0;
int menu = 1;


/////////////NOSAS HEADERS////////////////////////////////////////////////////////

# include "RankingFunctions.h"
score array[100];
# include "Ranking.h"
# include "Credits.h"
# include "Jogar.h"
# include "Ganhou.h"
# include "Perdeu.h"
# include "Escolher_Dificuldade.h"
# include "informacao.h"

int main(void)
{
	ALLEGRO_BITMAP *botao_sair = NULL;
    ALLEGRO_BITMAP *area_jogar = 0;
    ALLEGRO_BITMAP *area_informacao = 0;
    ALLEGRO_BITMAP *area_ver_recordes = 0;
    ALLEGRO_BITMAP *area_creditos = 0;
    ALLEGRO_BITMAP *area_sair_do_jogo = 0;
    ALLEGRO_BITMAP *area_menu_fundo = NULL;

  // Flag que condicionar� nosso looping
	int sair = 0;
	int creditos = 0;
	int escolher = 0;

  //Inicia o Allegro
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

  //Cria a janela
	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);

	if (!janela)
	{
		fprintf(stderr, "Falha ao criar janela.\n");
		return -1;
	}

  // Configura o título da janela
	al_set_window_title(janela, "Campo Minado");

  // Torna apto o uso de mouse na aplição
	if (!al_install_mouse())
	{
		fprintf(stderr, "Falha ao inicializar o mouse.\n");
		al_destroy_display(janela);
		return -1;
	}
	fila_eventos = al_create_event_queue();

  // Atribui o cursor padrão do sistema para ser usado
	if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
	{
		fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
		al_destroy_display(janela);
		return -1;
	}

  // Alocamos os bitmaps que serão desenhados na tela
	area_menu_fundo =   al_create_bitmap(360, 360);//Criação do bitmap do fundo do menu
	area_jogar =        al_create_bitmap(360, 72);//Criação do bitmap de fundo da opção jogar
	area_informacao =   al_create_bitmap(360, 72);//Criação do bitmap de fundo da opção informações
	area_ver_recordes = al_create_bitmap(360, 72);//Criação do bitmap de fundo da opção ver recordes
	area_creditos =     al_create_bitmap(360, 72);//Criação do bitmap de fundo da opção creditos
	area_sair_do_jogo = al_create_bitmap(360, 72);//Criação do bitmap de fundo da opção sair do jogo

  // Alocamos o bitmap da area do botão para fechar a aplicação
	botao_sair = al_create_bitmap(100, 50);
	if (!botao_sair)
	{
		fprintf(stderr, "Falha ao criar botãoo de saída.\n");
		al_destroy_display(janela);
		return -1;
	}

  // Flags indicando se o mouse está sobre alguma das opção do menu
	int na_area_jogar = 0;//Diz se o mouse esta sobre a opção jogar
	int na_area_informacao = 0;//Diz se o mouse esta sobre a opção informação
	int na_area_ver_recordes = 0;//Diz se o mouse esta sobre a opção ver recordes
	int na_area_creditos = 0;//Diz se o mouse esta sobre a opção creditos
	int na_area_sair_do_jogo = 0;//Diz se o mouse esta sobre a opção sair do jogo

  //Inicializando
	al_init_image_addon();
	ALLEGRO_BITMAP *backgr;
	ALLEGRO_BITMAP *bomba;

	//Carregando arquivos de imagem do background
	backgr = al_load_bitmap("imgs/background.png");
	bomba = al_load_bitmap("imgs/bombaBG.png");

  //Carregando fontes
	al_init_font_addon();
	al_init_ttf_addon();
	//Carregando arquivos de fonte
	ALLEGRO_FONT *font1 = al_load_ttf_font("fonts/retro3d.ttf",150,NULL),*font2 = al_load_ttf_font("fonts/retro.ttf",35,NULL);

  //Carregando cores
	al_init_primitives_addon();
	//Iniciando cada cor que sera usada
	ALLEGRO_COLOR Azul = al_map_rgb(44,117,255);
	ALLEGRO_COLOR VermelhoClaro = al_map_rgb(250,28,3);
	ALLEGRO_COLOR Preto = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR Cinza = al_map_rgb(172, 166, 166);
	ALLEGRO_COLOR Branco1, Branco2, Branco3, Branco4, Branco5;

	//Declarando a variável para eventos
	ALLEGRO_EVENT evento;
  //Carregando sons
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);//Reservando canais de audio no mixer principal
	id_music = malloc(sizeof(ALLEGRO_SAMPLE_ID));//Variável de ID para a musica de fundo

	//Carregando arquivos de som
	musica = al_load_sample("songs/background.wav");//Musica de fundo
	clique_jogo = al_load_sample("songs/clique.wav");//Som de clique
	al_play_sample(musica, 0.7, 0, 1, ALLEGRO_PLAYMODE_LOOP, id_music);//Tocando musica de fundo

	al_draw_filled_rectangle( 25 , 100, 300, 600, al_map_rgba_f(1, 1, 1, 0.5));

	//Variáveis que controlam onde ficam os desenhos
	float altura = 2.1;
	float distancia = 72;
	int play = 0;

	while (true)
	{
		//Toca a musica de fundo
		if(music == 1)
		{
			al_play_sample(musica, 0.7, 0, 1, ALLEGRO_PLAYMODE_LOOP, id_music);
			music = 0;
		}

    // Dizemos que vamos tratar os eventos vindos do mouse
		al_register_event_source(fila_eventos, al_get_mouse_event_source());

    // Limpamos a tela
		al_clear_to_color(al_map_rgb(0, 0, 0));

		//Desenhamos o backgound
		al_draw_bitmap(backgr, 0, 0, NULL);
		al_draw_bitmap(bomba, LARGURA_TELA / 2 - 256, ALTURA_TELA / 2 - 256, NULL);

    // Desenhamos os bitmaps das opções do menu
		al_draw_bitmap(area_menu_fundo, LARGURA_TELA / 2.8  , ALTURA_TELA / 2.2 , 0.5);//Desenha o bitmap do fundo do menu
		al_draw_bitmap(area_jogar, LARGURA_TELA/2.8  , ALTURA_TELA / 2.2 , 0.5);//Desenha o bitmap do fundo da opção jogar
		al_draw_bitmap(area_informacao, LARGURA_TELA / 2.8  , ALTURA_TELA / 2.2 + distancia , 0.5);//Desenha o bitmap do fundo da opção informação
		al_draw_bitmap(area_ver_recordes, LARGURA_TELA / 2.8  , ALTURA_TELA / 2.2 + distancia*2, 0.5);//Desenha o bitmap do fundo da opção ver reordes
		al_draw_bitmap(area_creditos, LARGURA_TELA / 2.8  , ALTURA_TELA / 2.2 + distancia*3 , 0.5);//Desenha o bitmap do fundo da opção creditos
		al_draw_bitmap(area_sair_do_jogo, LARGURA_TELA / 2.8  , ALTURA_TELA / 2.2 + distancia*4 , 0.5);//Desenha o bitmap do fundo da opção sair do jogo

		//Desenha o texto da opções do menu
		al_draw_text(font1,al_map_rgb(255,229,32),LARGURA_TELA/2,ALTURA_TELA/4 - 100,ALLEGRO_ALIGN_CENTRE,"CAMPO MINADO");//Desenha o texto do titulo
		//Desenha restangulos que fazem o contorno do menu
		al_draw_rectangle(LARGURA_TELA/2 - 180, ALTURA_TELA/altura - 15, LARGURA_TELA/2 + 180, ALTURA_TELA/altura + distancia*5 - 10, al_map_rgb(255, 255, 255), 6.0);
		al_draw_rectangle(LARGURA_TELA/2 - 186, ALTURA_TELA/altura - 21, LARGURA_TELA/2 + 186, ALTURA_TELA/altura + distancia*5 - 4, al_map_rgb(0, 0, 0), 6.0);
		//Desenha o texto das opções do menu
		al_draw_text(font2, Branco1, LARGURA_TELA/2, ALTURA_TELA/altura + 5, ALLEGRO_ALIGN_CENTER, "JOGAR");//Desenha o texto da opção jogar
		al_draw_text(font2, Branco2, LARGURA_TELA/2, ALTURA_TELA/altura + distancia, ALLEGRO_ALIGN_CENTER, "INFORMACOES");//Desenha o texto da opção  informações
		al_draw_text(font2, Branco3, LARGURA_TELA/2, ALTURA_TELA/altura + distancia*2, ALLEGRO_ALIGN_CENTER, "VER RECORDES");//Desenha o texto da opção ver recordes
		al_draw_text(font2, Branco4, LARGURA_TELA/2, ALTURA_TELA/altura + distancia*3, ALLEGRO_ALIGN_CENTER, "CREDITOS");//Desenha o texto da opção creditos
		al_draw_text(font2, Branco5, LARGURA_TELA/2, ALTURA_TELA/altura + distancia*4, ALLEGRO_ALIGN_CENTER, "SAIR DO JOGO");//Desenha o texto da opção sair do jogo
		al_flip_display();

		//Muda a cor da area onde esta a opção jogar dependendo se a flag na_area_jogar está "levantada" ou não
		al_set_target_bitmap(area_jogar);
		if (!na_area_jogar)
		{
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
			Branco1 = al_map_rgb(255,255,255);
		}
		else
		{
			al_clear_to_color(al_map_rgba_f(0.2, 0.2, 0.2, 0.5));
			Branco1 = al_map_rgb(255,229,32);
		}

		//Muda a cor da area onde esta a opção informação dependendo se a flag na_area_informacao está "levantada" ou não
		al_set_target_bitmap(area_informacao);
		if (!na_area_informacao)
		{
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
			Branco2 = al_map_rgb(255,255,255);
		}
		else
		{
			al_clear_to_color(al_map_rgba_f(0.2, 0.2, 0.2, 0.5));
			Branco2 = al_map_rgb(255,229,32);
		}

		//Muda a cor da area onde esta a opção recordes dependendo se a flag area_informacao está "levantada" ou não
		al_set_target_bitmap(area_ver_recordes);
		if (!na_area_ver_recordes)
		{
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
			Branco3 = al_map_rgb(255,255,255);
		}
		else
		{
			al_clear_to_color(al_map_rgba_f(0.2, 0.2, 0.2, 0.5));
			Branco3 = al_map_rgb(255,229,32);
		}

		//Muda a cor da area onde esta a opção creditos dependendo se a flag está "levantada" ou não
		al_set_target_bitmap(area_creditos);
		if (!na_area_creditos)
		{
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
			Branco4 = al_map_rgb(255,255,255);
		}
		else
		{
			al_clear_to_color(al_map_rgba_f(0.2, 0.2, 0.2, 0.5));
			Branco4 = al_map_rgb(255,229,32);
		}

		//Muda a cor da area onde esta a opção sair do jogo dependendo se a flag está "levantada" ou não
		al_set_target_bitmap(area_sair_do_jogo);
		if (!na_area_sair_do_jogo)
		{
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
			Branco5 = al_map_rgb(255,255,255);
		}
		else
		{
			al_clear_to_color(al_map_rgba_f(0.2, 0.2, 0.2, 0.5));
			Branco5 = al_map_rgb(255,229,32);
		}

		//Dizemos que agora vamos desenhar no bitmap do botão de sair da aplicação
    // Colorimos o bitmap do botão de sair
		al_set_target_bitmap(botao_sair);
		al_clear_to_color(al_map_rgb(255, 0, 0));

		//Dizemos que agora vamos desenhar no bitmap do fundo do menu
		//Colorimos o bitmap do fundo do menu
		al_set_target_bitmap(area_menu_fundo);
		al_clear_to_color(al_map_rgba_f(0, 0, 0, 0.5));

		//Dizemos que agora vamos desenhar a partir do fundo da janela
		al_set_target_bitmap(al_get_backbuffer(janela));

		//Esperamos o primeiro evento
		al_wait_for_event(fila_eventos, &evento);

		//Checamos se o mouse se moveu para cima de alguma das opções do menu
		if (evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			//Se estiver sobre alguma das opções levantamos a devida flag
			if((400 < evento.mouse.x) && (evento.mouse.x < LARGURA_TELA / 1.7 + 65) && (evento.mouse.y > ALTURA_TELA / 2.21) && (evento.mouse.y < ALTURA_TELA / 2.2 + 72))
				na_area_jogar = 1;//Levatamos a flag que diz que o mouse está sobre a opção jogar
			//Se não
			else
				na_area_jogar = 0;//Abaixamos a flag que diz que o mouse está sobre a opção jogar

			if((400 < evento.mouse.x) && (evento.mouse.x < LARGURA_TELA / 1.7 + 65) && (evento.mouse.y > ALTURA_TELA / 1.8) && (evento.mouse.y < ALTURA_TELA / 1.8 + 72))
				na_area_informacao = 1;//Levatamos a flag que diz que o mouse está sobre a opção informação
			//Se não
			else
				na_area_informacao = 0;//Abaixamos a flag que diz que o mouse está sobre a opção informação

			if((400 < evento.mouse.x) && (evento.mouse.x < LARGURA_TELA / 1.7 + 65) && (evento.mouse.y > ALTURA_TELA / 1.53 + 1) && (evento.mouse.y < ALTURA_TELA / 1.53 + 72))
				na_area_ver_recordes = 1;//Levatamos a flag que diz que o mouse está sobre a opção ver recordes
			//Se não
			else
				na_area_ver_recordes = 0;//Abaixamos a flag que diz que o mouse está sobre a opção ver recordes

			if((400 < evento.mouse.x) && (evento.mouse.x < LARGURA_TELA / 1.7 + 65) && (evento.mouse.y > ALTURA_TELA / 1.33 + 1) && (evento.mouse.y < ALTURA_TELA / 1.33 + 72))
				na_area_creditos = 1;//Levatamos a flag que diz que o mouse está sobre a opção creditos
			//Se não
			else
				na_area_creditos = 0;//Abaixamos a flag que diz que o mouse está sobre a opção creditos

			if((400 < evento.mouse.x) && (evento.mouse.x < LARGURA_TELA / 1.7 + 65) && (evento.mouse.y > ALTURA_TELA / 1.17 - 2) && (evento.mouse.y < ALTURA_TELA / 1.17 + 72))
				na_area_sair_do_jogo = 1;//Levatamos a flag que diz que o mouse está sobre a opção sair do jogo
			//Se não
			else
				na_area_sair_do_jogo = 0;//Abaixamos a flag que diz que o mouse está sobre a opção sair do jogo
		}

    // Ou se o evento foi um clique do mouse
		else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && menu == 1)
		{
			int mousex = evento.mouse.x;
			int mousey = evento.mouse.y;

			//Verificamos se o clique ocorreu sobre alguma das opções do menu
			if((350 < mousex) && (mousex < LARGURA_TELA / 1.5) && (mousey > ALTURA_TELA / 1.17 - 2) && (mousey < ALTURA_TELA / 1.17 + 72))
			{
				//Quando sobre o botão Sair do Jogo
				al_play_sample(clique_jogo, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);//Tocamos o som de clique
				return 0;
			}
			else if((400 < mousex) && (mousex < LARGURA_TELA / 1.7 + 65) && (mousey > ALTURA_TELA / 1.33 + 1) && (mousey < ALTURA_TELA / 1.33 + 72) )
			{
				//Quando sobre a opção Creditos
				menu = 0;//Mudamos a variável de controle
				al_play_sample(clique_jogo, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);//Tocamos o som de clique
				al_clear_to_color(al_map_rgb(255,255,255));//Limpamos a tela
				al_destroy_event_queue(fila_eventos);//Destruimos a fila de evntos para que não haja conflito
				credits(LARGURA_TELA, ALTURA_TELA, janela, backgr, bomba);//Desenhamos a tela de creditos
				fila_eventos = al_create_event_queue();//Quando o usuário volta da tela de creditos recriamos a fila de evntos
				menu = 1;//Mudamos a variável de controle//Mudamos a variável de controle
			}
			else if((465 < mousex) && (mousex < 815) && (mousey > 400) && (mousey< 472))
			{
				//Quando sobere a opção Informação
				menu = 0;//Mudamos a variável de controle//Mudamos a variável de controle
				al_play_sample(clique_jogo, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);//Tocamos o som de clique
				al_clear_to_color(al_map_rgb(255,255,255));//Limpamos a tela
				al_destroy_event_queue(fila_eventos);//Destruimos a fila de evntos para que não haja conflito
				informacao(LARGURA_TELA, ALTURA_TELA, janela, backgr, bomba);//Desenhamos a tela de informação
				fila_eventos = al_create_event_queue();//Quando o usuário volta da tela de informação recriamos a fila de evntos
				menu = 1;//Mudamos a variável de controle//Mudamos a variável de controle
			}
			else if((400 < mousex) && (mousex < LARGURA_TELA / 1.7 + 65) && (mousey > ALTURA_TELA / 2.21) && (mousey < ALTURA_TELA / 2.21 + 72 ))
			{
				//Quando sobre a opção Jogar
				al_play_sample(clique_jogo, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);//Tocamos o som de clique
				play = choose(LARGURA_TELA, ALTURA_TELA, janela, backgr, bomba);//Selecionamos a dificuldade atravez da tela de escolha de dificuldade
				dificuldade = play;//Setamos a dificuldade
			}
			else if((400 < mousex) && (mousex < LARGURA_TELA / 1.7 + 65) && (mousey > ALTURA_TELA / 1.53 + 1) && (mousey< ALTURA_TELA / 1.53 + 72))
			{
				//quando sobre a opção ver ranking
				menu = 0;//Mudamos a variável de controle
				al_play_sample(clique_jogo, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);//Tocamos o som de clique
				al_clear_to_color(al_map_rgb(255,255,255));//Limpamos a tela
				al_destroy_event_queue(fila_eventos);//Destruimos a fila de evntos para que não haja conflito
				dificuldade = choose(LARGURA_TELA, ALTURA_TELA, janela, backgr, bomba);//Selecionamos a dificuldade atravez da tela de escolha de dificuldade
				fila_eventos = al_create_event_queue();//Quando o usuário volta da tela de escolha de dificuldade recriamos a fila de evntos
				//Se o usuário não clicou no botão voltar
				if (dificuldade > 0)
				{
					al_destroy_event_queue(fila_eventos);//Destruimos a fila de evntos para que não haja conflito
					ranking(LARGURA_TELA, ALTURA_TELA, janela, backgr, bomba);//Desenhamos a tela do ranking
					fila_eventos = al_create_event_queue();//Quando o usuário volta da tela de ranking recriamos a fila de evntos
				}
				menu = 1;//Mudamos a variável de controle
			}
		}

		//se o usuário escolheu a opção jogar e uma dificuldade
		if(play > 0)
		{
			jogar(backgr, bomba);
			play = 0;//Mudamos a variável de controle
			menu = 1;//Mudamos a variável de controle
		}

	}

  // Desaloca os recursos utilizados na aplicação
	al_destroy_bitmap(botao_sair);//Bitmap do botão sair
	al_destroy_event_queue(fila_eventos);//Fila de evntos
	al_destroy_font(font1);//Arquivo de fonte
	al_destroy_font(font2);//Arquivo de fonte
	al_destroy_bitmap(backgr);//Imagem de fundo
	al_destroy_bitmap(bomba);//Imagem de fundo
	al_destroy_display(janela);//a janela em si

	return 0;
}