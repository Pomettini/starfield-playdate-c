#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"

typedef struct
{
    float x;
    float y;
    float z;
    float pz;
} star_t;

#define STARS 10000

#define WIDTH 400
#define HEIGHT 240

static inline void star_new(star_t *star);
static inline void star_update(star_t *star, float speed);
static inline void star_show(star_t *star, PlaydateAPI *pd);
static inline float map(float value, float start1, float stop1, float start2, float stop2);
static inline int randomf(int value, int min, int max);
static int update(void *userdata);

static star_t starfield[STARS];

static inline void star_new(star_t *star)
{
    float z = randomf(rand(), 0, WIDTH);
    star->x = randomf(rand(), -WIDTH, WIDTH);
    star->y = randomf(rand(), -HEIGHT, HEIGHT);
    star->z = z;
    star->pz = star->pz;
}

static inline void star_update(star_t *star, float speed)
{
    star->z -= speed;
    if (star->z < 1.0f)
    {
        star->z = WIDTH;
        star->x = randomf(rand(), -WIDTH, WIDTH);
        star->y = randomf(rand(), -HEIGHT, HEIGHT);
        star->pz = star->z;
    }
}

static inline void star_show(star_t *star, PlaydateAPI *pd)
{
    float sx = map((star->x / star->z) + 0.5f, 0.0f, 1.0f, 0.0f, WIDTH);
    float sy = map((star->y / star->z) + 0.5f, 0.0f, 1.0f, 0.0f, HEIGHT);

    float r = map(star->z, 0.0f, WIDTH, 4.0f, 0.0);

    float px = map((star->x / star->pz) + 0.5f, 0.0f, 1.0f, 0.0f, WIDTH);
    float py = map((star->y / star->pz) + 0.5f, 0.0f, 1.0f, 0.0f, HEIGHT);

    star->pz = star->z;

    pd->graphics->drawLine(px, py, sx, sy, r, kColorWhite);
}

static inline float map(float value, float start1, float stop1, float start2, float stop2)
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

static inline int randomf(int value, int min, int max)
{
    return value % (max - min) + min;
}

int eventHandler(PlaydateAPI *pd, PDSystemEvent event, uint32_t arg)
{
    if (event == kEventInit)
    {
        for (int i = 0; i < STARS; i++)
        {
            star_new(&starfield[i]);
        }

        pd->display->setRefreshRate(50.0f);
        pd->system->setUpdateCallback(update, pd);
    }

    return 0;
}

static int update(void *userdata)
{
    PlaydateAPI *pd = userdata;

    pd->graphics->clear(kColorBlack);

    float crank_change = pd->system->getCrankChange();

    if (crank_change < 0.0f)
    {
        crank_change = 0.0f;
    }

    for (int i = 0; i < STARS; i++)
    {
        star_update(&starfield[i], crank_change);
        star_show(&starfield[i], pd);
    }

    // pd->system->drawFPS(0, 0);

    return 1;
}
