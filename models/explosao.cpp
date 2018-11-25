/////////////////EXPLOSAO//////////////////////////////////////////////
const int NUM_EXPLOSIONS = 5;

class Explosion {

	int x;
	int y;
	bool live;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *image;


        //Inicia a animação das primeiras explosões
    void InitExplosions(class Explosion explosions[], int size, ALLEGRO_BITMAP *image)
    {
        int i;
        for(i = 0; i < size; i++)
        {
            explosions[i].live = false;

            explosions[i].maxFrame = 31;
            explosions[i].curFrame = 0;
            explosions[i].frameCount = 0;
            explosions[i].frameDelay = 1;
            explosions[i].frameWidth = 104;
            explosions[i].frameHeight = 100;
            explosions[i].animationColumns = 9;
            explosions[i].animationDirection = 1;

            explosions[i].image = image;
        }
    }

    //Desenha as explosões
    void DrawExplosions(class Explosion explosions[], int size)
    {
        int i;
        for(i = 0; i < size; i++)
        {
            if(explosions[i].live)
            {
                int fx = (explosions[i].curFrame % explosions[i].animationColumns) * explosions[i].frameWidth;
                int fy =  (explosions[i].curFrame / explosions[i].animationColumns) * explosions[i].frameHeight;

                al_draw_bitmap_region(explosions[i].image, fx, fy, explosions[i].frameWidth,
                    explosions[i].frameHeight, explosions[i].x - explosions[i].frameWidth /2,
                    explosions[i].y - explosions[i].frameHeight / 2, 0);
            }
        }
    }

    //Inicia as explosões
    void StartExplosions(class Explosion explosions[], int size, int x, int y)
    {
        int i;
        for(i = 0; i < size; i++)
        {
            if(!explosions[i].live)
            {
                explosions[i].live = true;
                explosions[i].x = x;
                explosions[i].y = y;
                break;
            }
        }
    }

    //Atualiza a sprite da explosão
    void UpdateExplosions(class Explosion explosions[], int size)
    {
        int i;
        for(i = 0; i < size; i++)
        {
            if(explosions[i].live)
            {
                if(++explosions[i].frameCount >= explosions[i].frameDelay)
                {
                    explosions[i].curFrame += explosions[i].animationDirection;
                    if(explosions[i].curFrame >= explosions[i].maxFrame)
                    {
                        explosions[i].curFrame = 0;
                        explosions[i].live = false;
                    }

                    explosions[i].frameCount = 0;
                }
            }
        }
    }
};