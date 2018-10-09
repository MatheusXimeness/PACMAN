#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const float FPS = 5;
const int SCREEN_W = 500;
const int SCREEN_H = 550;

enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

//matriz definindo mapa do jogo:  0 representa corredor, 1 representa paredes, 2 representa moeda, 4 represemta t�nel, 5 representa estrela
char MAPA[26][26] =
{
    "1111111111111111111111111",
    "1222222221222221222222221",
    "1211212121211121212121121",
    "1222215122211122215122221",
    "1121112121222221212111211",
    "1122112121111111212112211",
    "1112122222221222222212111",
    "1222121121121211211212221",
    "1212222221221221222222121",
    "1212112111211121112112121",
    "1222112122222222212112221",
    "1112112221121211222112111",
    "4212111121121211211112124",
    "1222222221122211222222221",
    "1211111121112111211111121",
    "1211122222222222222211121",
    "1222221211112111121222221",
    "1111121212222222121211111",
    "1111121212112112121211111",
    "1225222222112112222225221",
    "1212121111112111111212121",
    "1212121112222222111212121",
    "1212122222111112222212121",
    "1222221112222222111222221",
    "1111111111111111111111111",
};

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_BITMAP *mario = NULL;//figura de uma das 4 definicoes de estado do mario
ALLEGRO_BITMAP *mario2 = NULL;//figura de uma das 4 definicoes de estado do mario
ALLEGRO_BITMAP *mario3 = NULL;//figura de uma das 4 definicoes de estado do mario
ALLEGRO_BITMAP *mario4 = NULL;//figura de uma das 4 definicoes de estado do mario
ALLEGRO_BITMAP *boo = NULL;//figura do personagem vilao boo
ALLEGRO_BITMAP *bowser = NULL;//figura do personagem vilao bowser
ALLEGRO_BITMAP *goomba = NULL;//figura do personagem vilao goomba
ALLEGRO_BITMAP *wario = NULL;//figura do personagem vilao wario
ALLEGRO_BITMAP *moeda = NULL;//figura da "pilula" do nosso jogo, no caso a moeda.
ALLEGRO_BITMAP *star = NULL;//figura da estrela que garante imunidade momentanea
ALLEGRO_BITMAP *tunel1 = NULL;//figura da passagem por tunel do mapa
ALLEGRO_BITMAP *tunel2 = NULL;//figura da passagem por tunel do mapa
ALLEGRO_BITMAP *derrota = NULL;//tela de game over-final do jogo devido ao contato com um dos viloes
ALLEGRO_BITMAP *win = NULL;//tela de vitoria do jogador(adquiriu todas as moedas).
ALLEGRO_FONT *placar = NULL;//
ALLEGRO_FONT *pontos = NULL;//
ALLEGRO_AUDIO_STREAM *musica = NULL;//trilha sonora do jogo
ALLEGRO_SAMPLE *sample = NULL;//efeito sonoro(aquisicao de moeda)do jogo
ALLEGRO_AUDIO_STREAM *starsong = NULL;//musica tocada quando e pego uma das 4 estrelas do jogo(equivalente as pilulas de forca)

int i = 15, j = 12; //posicao inicial do Mario na matriz
int q = 20; //tamanho de cada celula no mapa

int posy = i*q;
int posx = j*q;

int a = 1, b = 1;//posicao dos viloes do jogo(boo,bowser,goomba,wario)
int c = 1, d = 23;
int e = 23, f = 1;
int g = 23, h = 23;

int posay = a*q, posax = b*q; //posicao do boo
int posly = c*q, poslx = d*q; //posicao do bowser
int posry = e*q, posrx = f*q; //posicao do goomba
int posvy = g*q, posvx = h*q; //posicao do wario

int ra = 1; //numero randomico do boo
int rl = 1; //numero randomico do bowser
int rr = 2; //numero randomico do goomba (para quando ele não conseguir chegar até o pacman)
int rv = 3; //numero randomico do wario

int ultimo_ax, ultimo_ay;
int ultimo_lx, ultimo_ly;
int ultimo_rx, ultimo_ry;
int ultimo_vx, ultimo_vy;

int tun1 = 0  * q; //posicao dos tuneis
int tun2 = 24 * q;
int tun3 = 12 * q;

int n = -1; //confere o uso da estrela

int k1, k2; //confere por qual eixo o Mario esta mais distante

int estado = 1; //estado do Mario, definindo a figura a ser exibida (1 representa direita, -1 representa esquerda, 2 representa cima, -2 representa baixo)

//Auxiliares da pontuacao
const char score[10] = {"Score: "};		//string que aparece no display indicando o score
int pontuacao = 0;		//contador da pontuacao do jogo, incrementada a cada moeda pegada

const char aux1[4] = {"0"};   const char aux60[4] = {"60"};       const char aux180[4] = {"180"};
const char aux10[4] = {"10"}; const char aux70[4] = {"70"};       const char aux190[4] = {"190"};
const char aux20[4] = {"20"}; const char aux80[4] = {"80"};       const char aux210[4] = {"210"};
const char aux30[4] = {"30"}; const char aux90[4] = {"90"};       const char aux220[4] = {"220"};
const char aux40[4] = {"40"}; const char aux110[4] = {"110"};     const char aux230[4] = {"230"};
const char aux50[4] = {"50"};  const char aux120[4] = {"120"};    const char aux240[4] = {"240"};
const char aux100[4] = {"100"}; const char aux130[4] = {"130"};   const char aux260[4] = {"260"};
const char aux160[4] = {"160"}; const char aux140[4] = {"140"};	  const char aux270[4] = {"270"};
const char aux200[4] = {"200"}; const char aux150[4] = {"150"};   const char aux280[4] = {"280"};
const char aux250[4] = {"250"}; const char aux170[4] = {"170"};   const char aux290[4] = {"290"};
const char aux300[4] = {"300"};		//string corresponde a intervalos de dez em dez pontos obtidos no jogo


bool key[4] = { false, false, false, false };
bool redraw = true;
bool sair = false;

int marios() {	 //funcao responsavel pelo carregamento do mario corresponde a cada estado

    mario = al_load_bitmap("mario.tga");
    if(!mario)
    {
        cout << "Falha ao carregar o mario!" << endl;
        al_destroy_display(display);
        return 0;
    }

    mario2 = al_load_bitmap("mario2.tga");
    if(!mario2)
    {
        cout << "Falha ao carregar o mario!" << endl;
        al_destroy_display(display);
        return 0;
    }

    mario3 = al_load_bitmap("mario3.tga");
    if(!mario3)
    {
        cout << "Falha ao carregar o mario!" << endl;
        al_destroy_display(display);
        return 0;
    }
    mario4 = al_load_bitmap("mario4.tga");
    if(!mario4)
    {
        cout << "Falha ao carregar o mario!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mario,posx,posy,0);

    return 1;
}

int tuneis() {	 //funcao responsavel pelo carregamento dos tuneis do mapa

    tunel1 = al_load_bitmap("tuneldireita.png");
    if(!tunel1)
    {
	cout << "Falha ao carregar o tunel esquerdo!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(tunel1,tun1,tun3,0);

    tunel2 = al_load_bitmap("tunelesquerda.png");
    if(!tunel2)
    {
	cout << "Falha ao carregar o tunel direito!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(tunel2,tun2,tun3,0);

    return 1;
}

int viloes() {	 //funcao responsavel pelo carregamento dos viloes do jogo

    boo = al_load_bitmap("boo.png");
    if(!boo)
    {
        cout << "Falha ao carregar o boo!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(boo,posax,posay,0);

    bowser = al_load_bitmap("bowser.png");
    if(!bowser)
    {
        cout << "Falha ao carregar o bowser!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(bowser,poslx,posly,0);

    goomba = al_load_bitmap("goomba.png");
    if(!goomba)
    {
        cout << "Falha ao carregar o goomba!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(goomba,posrx,posly,0);

    wario = al_load_bitmap("wario.png");
    if(!wario)
    {
        cout << "Falha ao carregar o wario!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(wario,posvx,posvy,0);

	return 1;
}

int itens() {	//funcao responsavel pelo carregamento das moedas (correspondente a pilula) e da estrela do mapa, respectivamente

	for(int as=25;as>=0;as--){
        for(int bs=25;bs>=0;bs--){
            if(MAPA[as][bs]=='2'){
                moeda = al_load_bitmap("moeda.png");
                if(!moeda)
                {
                    cout << "Falha ao carregar a moeda!" << endl;
                    al_destroy_display(display);
                    return 0;
                }
                al_draw_bitmap(moeda,as*q,bs*q,0);
            }
        }
    }

	for(int as=25;as>=0;as--){
        for(int bs=25;bs>=0;bs--){
            if(MAPA[as][bs]=='5'){
                star = al_load_bitmap("star.png");
                if(!moeda)
                {
                    cout << "Falha ao carregar a estrela!" << endl;
                    al_destroy_display(display);
                    return 0;
                }
                al_draw_bitmap(star,as*q,bs*q,0);
            }
        }
    }

	return 1;
}

int inicializa() {	//inicializa funcoes relativas ao jogo

    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout << "Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        cout << "Falha ao inicializar allegro_ttf" << endl;
        return 0;
    }

    if (!al_install_audio())
    {
        cout << "Falha ao inicializar audio." << endl;
        return 0;
    }

    if (!al_init_acodec_addon())
    {
        cout << "Falha ao inicializar codecs de audio." << endl;
        return 0;
    }

    if (!al_reserve_samples(1))
    {
        cout << "Falha ao alocar canais de audio." << endl;
        return 0;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    mapa = al_load_bitmap("map.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

    derrota = al_load_bitmap("derrota.png");	//carregamento da tela de game over
    if (!derrota)
    {
      cout << "Falha na imagem da tela de derrota!" << endl;
      return 0;
    }

    win = al_load_bitmap("vitoria.png");	//carregamento da tela de vitoria do jogador
    if (!win)
    {
      cout << "Falha na imagem da tela de vitoria!" << endl;
      return 0;
    }

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }

    placar = al_load_font("Super Mario Bros..ttf", 40, 0);	//carregamento da fonte do placar
    if(!placar)
    {
        cout << "Falha ao carregar a fonte para o placar!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }

    pontos = al_load_font("Super Mario Bros..ttf", 40, 0);	//carregamento da fonte do numero de pontos obtidos
    if(!pontos)
    {
        cout << "Falha ao carregar a fonte para pontos!" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }

    sample = al_load_sample("mario-coin.wav");	//carregamento do efeito sonoro
    if (!sample)
    {
        cout << "Falha ao carregar sample." << endl;
        al_destroy_display(display);
        return 0;
    }

    starsong =al_load_audio_stream("Super Mario Star Theme.wav", 4, 1024);	//carregamento do efeito sonoro da estrela
    if(!starsong)
    {
        cout << "Falha ao carregar starsong." << endl;
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        al_destroy_sample(sample);
        return 0;
    }

    musica = al_load_audio_stream("Super Mario Bros Official Theme Song.ogg", 4, 1024);	//carregamento da trilha sonora do jogo
    if (!musica)
    {
        cout << "Falha ao carregar audio." << endl;
        al_destroy_event_queue(event_queue);
        al_destroy_display(display);
        al_destroy_sample(sample);
        return 0;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

void testa (int &m, int &n, int &posxx, int &posyy, int rm) {

		if (rm==0 && MAPA[m-1][n] != '1') {
			m--;
			posyy = m*q;
		}
		if (rm==1 && MAPA[m][n+1] != '1') {
			n++;
			posxx = n*q;
		}
		if (rm==2 && MAPA[m+1][n] != '1') {
			m++;
			posyy = m*q;
		}
		if (rm==3 && MAPA[m][n-1] != '1') {
			n--;
			posxx = n*q;
		}
}

int main(int argc, char **argv)
{
    if(!inicializa()) return -1;
    if(!itens()) return -1;		
    if(!viloes()) return -1;
    if(!marios()) return -1;
    if(!tuneis()) return -1;

    al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());	//criacao do vinculo da stream ao mixer principal da trilha sonora
    al_set_audio_stream_playing(musica, true);	 //declaracao que a musica da trilha sonora deve começar a tocar ao iniciar o jogo

    bool dois;	//booleano que verifica se ainda há moedas no mapa e controla se é para terminar ou continuar a executar o jogo

	srand(time(0));

    while(!sair)
    {
	dois=false;

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
			//Os codigos de movimentacoes aleatorias dos viloes verificam todos os movimentos possiveis que ele pode fazer, e se nao houver parede, escolher um aleatoriamente
			if (ra==0 && MAPA[a-1][b] != '1')
			{
				if (MAPA[a][b-1] != '1' && MAPA[a][b+1] != '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a][b-1] != '1' && MAPA[a][b+1] == '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a][b-1] == '1' && MAPA[a][b+1] != '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else {
					a--;
					posay = a*q;
				}
			}

			else if (ra==1 && MAPA[a][b+1] != '1')
			{
				if (MAPA[a-1][b] != '1' && MAPA[a+1][b] != '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a-1][b] != '1' && MAPA[a+1][b] == '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a-1][b] == '1' && MAPA[a+1][b] != '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else {
					b++;
					posax = b*q;
				}
			}

			else if (ra==2 && MAPA[a+1][b] != '1')
			{
				if (MAPA[a][b-1] !='1' && MAPA[a][b+1] != '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a][b-1] != '1' && MAPA[a][b+1] == '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a][b-1] == '1' && MAPA[a][b+1] != '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else {
					a++;
					posay = a*q;
				}
			}

			else if (ra==3 && MAPA[a][b-1] != '1')
			{
				if (MAPA[a-1][b] != '1' && MAPA[a+1][b] != '1' )
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a-1][b] != '1' && MAPA[a+1][b] == '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else if (MAPA[a-1][b] == '1' && MAPA[a+1][b] != '1')
				{
					ra = rand()%4;
					testa(a,b,posax,posay,ra);
				}

				else {
					b--;
					posax = b*q;
				}
			}

			else  {
			ra = rand()%4;
			testa(a,b,posax,posay,ra);
			}

			//Caso o vilao entre no tunel sai do outro lado
			if (b==-1) {
                b = 24;
				posax = b*q;
			}

			if (b==25) {
                b = 0;
				posax = b*q;
			}
			//Fim da movimentacao do boo

			if (rl==0 && MAPA[c-1][d] != '1')
			{
				if (MAPA[c][d-1] != '1' && MAPA[c][d+1] != '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c][d-1] != '1' && MAPA[c][d+1] == '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c][d-1] == '1' && MAPA[c][d+1] != '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else {
					c--;
					posly = c*q;
				}
			}

			else if (rl==1 && MAPA[c][d+1] != '1')
			{
				if (MAPA[c-1][d] != '1' && MAPA[c+1][d] != '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c-1][d] != '1' && MAPA[c+1][d] == '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c-1][d] == '1' && MAPA[c+1][d] != '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else {
					d++;
					poslx = d*q;
				}
			}

			else if (rl==2 && MAPA[c+1][d] != '1')
			{
				if (MAPA[c][d-1] !='1' && MAPA[c][d+1] != '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c][d-1] != '1' && MAPA[c][d+1] == '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c][d-1] == '1' && MAPA[c][d+1] != '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else {
					c++;
					posly = c*q;
				}
			}

			else if (rl==3 && MAPA[c][d-1] != '1')
			{
				if (MAPA[c-1][d] != '1' && MAPA[c+1][d] != '1' )
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c-1][d] != '1' && MAPA[c+1][d] == '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else if (MAPA[c-1][d] == '1' && MAPA[c+1][d] != '1')
				{
					rl = rand()%4;
					testa(c,d,poslx,posly,rl);
				}

				else {
					d--;
					poslx = d*q;
				}
			}

			else {
			rl = rand()%4;
			testa(c,d,poslx,posly,rl);
			}

			if (d==-1) {
                d = 24;
				poslx = d*q;
			}

			if (d==25) {
                d = 0;
				poslx = d*q;
			}
			//Fim da movimentacao do bowser

			//O codigo do fantasma inteligente verifica a posicao do pacman e vai em direcao a ele
			//Quando nao tem como segui-lo ele anda em um movimento aleatorio ate encontra-lo de novo
			if (i<e && j<f) {
				k1 = e-i; //Compara qual das duas distancias e maior, a do do eixo x ou a do eixo y
				k2 = f-j;
				if ((k2>=k1 && MAPA[e][f-1] != '1') || (k2<k1 && MAPA[e-1][f] == '1' && MAPA[e][f-1] != '1')) {
					f--;
					posrx = f*q;
				}
				if ((k2<k1 && MAPA[e-1][f] != '1') || (k2>=k1 && MAPA[e][f-1] == '1' && MAPA[e-1][f] != '1')) {
					e--;
					posry = e*q;
				}

			}
			
			else if (i<e && j>f) {
				k1 = e-i;
				k2 = j-f;
				if ((k2>=k1 && MAPA[e][f+1] != '1') || (k2<k1 && MAPA[e-1][f] == '1' && MAPA[e][f+1] != '1')) {
					f++;
					posrx = f*q;
				}
				if ((k2<k1 && MAPA[e-1][f] != '1') || (k2>=k1 && MAPA[e][f+1] == '1' && MAPA[e-1][f] != '1')) {
					e--;
					posry = e*q;
				}
			}
			
			else if (i>e && j>f) {
				k1 = i-e;
				k2 = j-f;
				if ((k2>=k1 && MAPA[e][f+1] != '1') || (k2<k1 && MAPA[e+1][f] == '1' && MAPA[e][f+1] != '1')) {
					f++;
					posrx = f*q;
				}
				if ((k2<k1 && MAPA[e+1][f] != '1') || (k2>=k1 && MAPA[e][f+1] == '1' && MAPA[e+1][f] != '1')) {
					e++;
					posry = e*q;
				}
			}
			
			else if (i>e && j<f) {
				k1 = i-e;
				k2 = f-j;
				if ((k2>=k1 && MAPA[e][f-1] != '1') || (k2<k1 && MAPA[e+1][f] == '1' && MAPA[e][f-1] != '1')) {
					f--;
					posrx = f*q;
				}
				if ((k2<k1 && MAPA[e+1][f] != '1') || (k2>=k1 && MAPA[e][f-1] == '1' && MAPA[e+1][f] != '1')) {
					e++;
					posry = e*q;
				}
			}
			
			else if (i==e && j<f && MAPA[e][f-1] != '1') {
				f--;
				posrx = f*q;
			}
			
			else if (i==e && j>f && MAPA[e][f+1] != '1') {
				f++;
				posrx = f*q;
			}
			
			else if (i<e && j==f && MAPA[e-1][f] != '1') {
				e--;
				posry = e*q;
			}
			
			else if (i>e && j==f && MAPA[e+1][f] != '1') {
				e++;
				posry = e*q;
			}
			
			else if (MAPA[e][f+1] == '1')
			{
				if (MAPA[e-1][f] != '1' && MAPA[e+1][f] != '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}

				else if (MAPA[e-1][f] != '1' && MAPA[e+1][f] == '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}

				else if (MAPA[e-1][f] == '1' && MAPA[e+1][f] != '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}
			}
			
			else if (MAPA[e+1][f] == '1')
			{
				if (MAPA[e-1][f] != '1' && MAPA[e+1][f] != '1' ) {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}

				else if (MAPA[e-1][f] != '1' && MAPA[e+1][f] == '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}
 
				else if (MAPA[e-1][f] == '1' && MAPA[e+1][f] != '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}
			}
			
			else if (MAPA[e-1][f] == '1')
			{
				if (MAPA[e-1][f] != '1' && MAPA[e+1][f] != '1' ) {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}

				else if (MAPA[e-1][f] != '1' && MAPA[e+1][f] == '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}

				else if(MAPA[e-1][f] == '1' && MAPA[e+1][f] != '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}
			}
			
			else if (MAPA[e][f-1] == '1')
			{
				if (MAPA[e-1][f] != '1' && MAPA[e+1][f] != '1' ) {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}

				else if (MAPA[e-1][f] != '1' && MAPA[e+1][f] == '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}

				else if(MAPA[e-1][f] == '1' && MAPA[e+1][f] != '1') {
					rr = rand()%4;
					testa(e,f,posrx,posry,rr);
				}
			}
			//Fim da movimentacao do goomba
			
			if (rv==0 && MAPA[g-1][h] != '1')
			{
				if (MAPA[g][h-1] != '1' && MAPA[g][h+1] != '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g][h-1] != '1' && MAPA[g][h+1] == '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g][h-1] == '1' && MAPA[g][h+1] != '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else {
					g--;
					posvy = g*q;
				}
			}

			else if (rv==1 && MAPA[g][h+1] != '1')
			{
				if (MAPA[g-1][h] != '1' && MAPA[g+1][h] != '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g-1][h] != '1' && MAPA[g+1][h] == '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g-1][h] == '1' && MAPA[g+1][h] != '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else {
					h++;
					posvx = h*q;
				}
			}

			else if (rv==2 && MAPA[g+1][h] != '1')
			{
				if (MAPA[g][h-1] !='1' && MAPA[g][h+1] != '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g][h-1] != '1' && MAPA[g][h+1] == '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g][h-1] == '1' && MAPA[g][h+1] != '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else {
					g++;
					posvy = g*q;
				}
			}

			else if (rv==3 && MAPA[g][h-1] != '1')
			{
				if (MAPA[g-1][h] != '1' && MAPA[g+1][h] != '1' )
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g-1][h] != '1' && MAPA[g+1][h] == '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else if (MAPA[g-1][h] == '1' && MAPA[g+1][h] != '1')
				{
					rv = rand()%4;
					testa(g,h,posvx,posvy,rr);
				}

				else {
					h--;
					posvx = h*q;
				}
			}

			else  {
			rv = rand()%4;
			testa(g,h,posvx,posvy,rr);
			}

			if (h==-1) {
                h = 24;
				posvx = h*q;
			}

			if (h==25) {
                h = 0;
				posvx = h*q;
			}
			//Fim da movimentação do Wario

			if (n<0){
				if ((posx==posax && posy==posay) || (posx==poslx && posy==posly) || (posx==posrx && posy==posry) || (posx==posvx && posy==posvy) || (posx==ultimo_ax && posy==ultimo_ay) || (posx==ultimo_lx && posy==ultimo_ly) || (posx==ultimo_rx && posy==ultimo_ry) || (posx==ultimo_vx && posy==ultimo_vy)){ //Mario esta na mesma posicao de um dos viloes e o jogo termina
					 al_draw_bitmap(derrota, 1, 1, 0); //jogo exibe uma tela de game over no display
    					 al_flip_display();
    					 al_rest(3.0);//tela exibida durante tres segundos
    					 exit(0);
					 break;
			     }
			}
			
            if(key[KEY_UP] && MAPA[i-1][j] != '1')
            {
                i--;
                posy = i*q;
                estado = 2; //definicao do estado de subida do Mario no mapa
				if(MAPA[i][j] == '2')
                {
                    pontuacao++; //aumento na pontuacao pois o Mario esta numa posicao do mapa que possui moeda (posicao 2 da matriz)
	                  al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); //execucao do efeito sonoro condicionado a aquisicao de uma moeda
                }
            }

            if(key[KEY_DOWN] && MAPA[i+1][j] != '1')
            {
                i++;
                posy = i*q;
                estado = -2; //definicao do estado de descida do Mario no mapa
				if(MAPA[i][j] == '2')
                {
                    pontuacao++;
		                al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }

            if(key[KEY_LEFT] && MAPA[i][j-1] != '1')
            {
                j--;
                //Para o caso do Mario sair da tela pelo tunel a esquerda.
                if (j == -1){
                    j = 24;
                }
                posx = j*q;
                estado = -1; //definicao do estado de ida para a esquerda do Mario no mapa
				if(MAPA[i][j] == '2')
                {
                    pontuacao++;
		                al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }

            }

            if(key[KEY_RIGHT] && MAPA[i][j+1] != '1')
            {
                j++;
                //Para o caso do Mario sair da tela pelo tunel a direita.
                if (j == 25){
                    j = 0;
                }
                posx = j*q;
                estado = 1; //definicao do estado de ida para a direita do Mario no mapa
				if(MAPA[i][j] == '2')
                {
                    pontuacao++;
		                al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }

            }

            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                key[KEY_UP] = true;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = true;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = true;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                key[KEY_UP] = false;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = false;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_ESCAPE:
                sair = true;
                break;
            }
        }

	if (MAPA[i][j]=='5'){	//Mario esta na posicao do mapa que possui estrela
		n=120;	//Condiciona o tempo de vulnerabilidade dos viloes
	}

	//Quando o Mario pega estrela
	if (n>=0) {
  	al_attach_audio_stream_to_mixer(starsong, al_get_default_mixer());	//Criacao do vinculo da stream ao mixer principal do efeito sonoro quando uma estrela e pega
		al_set_audio_stream_playing(starsong, true);	//Declaracao da musica da estrela
		if ((posx==posax && posy==posay) || (posx==ultimo_ax && posy==ultimo_ay)) {
			a=1;
			b=1;
			posay=a*q;
			posax=b*q;
		}
		if ((posx==poslx && posy==posly) || (posx==ultimo_lx && posy==ultimo_ly)) {
			c=1;
			d=23;
			posly=c*q;
			poslx=d*q;
		}
		if ((posx==posrx && posy==posry) || (posx==ultimo_rx && posy==ultimo_ry)) {
			e=23;
			f=1;
			posry=e*q;
			posrx=f*q;
		}
		if ((posx==posvx && posy==posvy) || (posx==ultimo_vx && posy==ultimo_vy)) {
			g=23;
			h=23;
			posvy=g*q;
			posvx=h*q;
		}
		n--;

		if(n==-1){
		al_set_audio_stream_playing(starsong, false);	//Declaracao que a musica da estrela deve parar de tocar quando acabar a imunidade provisoria do Mario
	        }
	}

		if (MAPA[i][j]=='2' || MAPA[i][j]=='5')
			MAPA[i][j]=false;	//Mario passou sobre uma moeda ou estrela e elas sao apagadas do mapa (posicoes 2 ou 5 viram posicao 0)

		for (int as=0;as<26;as++)
			for (int bs=0;bs<26;bs++)
				if (MAPA[as][bs]=='2')	//Confere se ainda ha elementos no mapa)
					dois=true;

		if (dois==false){	//Nao ha mais moeda no mapa e o jogo encerra com a vitoria do jogador
			al_draw_bitmap(win, 1, 1, 0);	//Tela "you win" da vitoria do jogador
				al_flip_display();
				al_rest(3.0);	//Tela fica disponivel por 3 segundos
				exit(0);
			break;
		}
	
		//Salva os valores das ultimas posicoes dos viloes
		ultimo_ax = posax;
		ultimo_ay = posay;
		ultimo_lx = poslx;
		ultimo_ly = posly;
		ultimo_rx = posrx;
		ultimo_ry = posry;
		ultimo_vx = posvx;
		ultimo_vy = posvy;

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap(mapa,0,0,0);

            switch (estado){	//Mario desenhado referente a cada caso de seu estado
            case 1:
                al_draw_bitmap(mario,posx,posy,0);
                break;
            case -1:
                al_draw_bitmap(mario,posx,posy,ALLEGRO_FLIP_HORIZONTAL);
                break;
            case 2:
                al_draw_rotated_bitmap(mario,0,20,posx,posy,M_PI/2,ALLEGRO_FLIP_HORIZONTAL);
                break;
            case -2:
                al_draw_rotated_bitmap(mario,0,20,posx,posy,M_PI/2,0);
                break;
            }

            for(int as=25;as>=0;as--)
                for(int bs=25;bs>=0;bs--)
                    if(MAPA[as][bs]=='2')	//celulas vazias do mapa preenchidas com moedas
                        al_draw_bitmap(moeda,bs*q,as*q,0);

			      for(int as=25;as>=0;as--)
                for(int bs=25;bs>=0;bs--)
                    if(MAPA[as][bs]=='5') 	//celulas vazias do mapa preenchidas com estrelas
                        al_draw_bitmap(star,bs*q,as*q,0);

            al_draw_bitmap(boo,posax,posay,0);
            al_draw_bitmap(bowser,poslx,posly,0);
            al_draw_bitmap(goomba,posrx,posry,0);
            al_draw_bitmap(wario,posvx,posvy,0);
		    al_draw_bitmap(tunel1,tun1,tun3,0);
            al_draw_bitmap(tunel2,tun2,tun3,0);

			al_draw_text(placar, al_map_rgb(255, 255, 255), 130, 507, ALLEGRO_ALIGN_LEFT, score);	//Escreve o texto referente a palavra "score" do jogo
            if (pontuacao >= 0 && pontuacao < 10)		//A partir deste ponto, escreve a pontuacao atual do jogo e atualiza a cada intervalo de dez pontos
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux1);
            else if (pontuacao < 20)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux10);
            else if (pontuacao < 30)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux20);
            else if (pontuacao < 40)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux30);
            else if (pontuacao < 50)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux40);
            else if (pontuacao < 60)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux50);
            else if (pontuacao < 70)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux60);
            else if (pontuacao < 80)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux70);
            else if (pontuacao < 90)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux80);
            else if (pontuacao < 100)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux90);
            else if (pontuacao < 110)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux100);
            else if (pontuacao < 120)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux110);
            else if (pontuacao < 130)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux120);
            else if (pontuacao < 140)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux130);
            else if (pontuacao < 150)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux140);
            else if (pontuacao < 160)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux150);
            else if (pontuacao < 170)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux160);
            else if (pontuacao < 180)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux170);
            else if (pontuacao < 190)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux180);
            else if (pontuacao < 200)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux190);
            else if (pontuacao < 210)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux200);
            else if (pontuacao < 220)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux210);
            else if (pontuacao < 230)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux220);
            else if (pontuacao < 240)
                al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux230);
            else if (pontuacao < 250)
				al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux240);
            else if (pontuacao < 260)
				al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux250);
			else if (pontuacao < 270)
				al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux260);
            else if (pontuacao < 280)
				al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux270);
            else if (pontuacao < 290)
				al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux280);
			else if (pontuacao < 300)
				al_draw_text(pontos, al_map_rgb(255, 255, 255), 295, 507, ALLEGRO_ALIGN_LEFT, aux290);

            al_flip_display();

        }
    }

    al_destroy_bitmap(mapa);
    al_destroy_bitmap(mario);
    al_destroy_bitmap(moeda);
    al_destroy_bitmap(star);
    al_destroy_bitmap(boo);
    al_destroy_bitmap(bowser);
    al_destroy_bitmap(goomba);
    al_destroy_bitmap(wario);
    al_destroy_bitmap(tunel1);
    al_destroy_bitmap(tunel2);
    al_destroy_bitmap(derrota);
    al_destroy_bitmap(win);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(pontos);
    al_destroy_font(placar);
    al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_audio_stream(starsong);


    return 0;
}
