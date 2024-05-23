#include "main.h"

/* typedef struct {
    float x, y;
    SDL_Texture* texture;
    SDL_Rect rect;
} Car; */

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ultimate Car Racing 80 : Retour vers le passé", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TX, TY, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // charge les textures du menu UNE FOIS POUR TOUTES
    SDL_Surface* tmp_surface = SDL_LoadBMP("assets/menu/page_accueil.bmp");
    SDL_Texture* page_accueil = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    tmp_surface = SDL_LoadBMP("assets/menu/select_map.bmp");
    SDL_Texture* select_map = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    tmp_surface = SDL_LoadBMP("assets/menu/options.bmp");
    SDL_Texture* options = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    tmp_surface = SDL_LoadBMP("assets/menu/pause.bmp");
    SDL_Texture* pause = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    tmp_surface = SDL_LoadBMP("assets/menu/curseur.bmp");
    SDL_Texture* curseur = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    tmp_surface = SDL_LoadBMP("assets/menu/chargement.bmp");
    SDL_Texture* chargement = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    tmp_surface = SDL_LoadBMP("assets/menu/titre.bmp");
    SDL_Texture* titre = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    // pour l'interactivité avec les boutons du menu
#define EST_DANS_CLICKZONE(chose,clickZone) (chose.x >= clickZone.x && chose.y >= clickZone.y && chose.x <= clickZone.w && chose.y <= clickZone.h)
    // page accueil
    SDL_Rect clickZoneStart = { .x = 827, .y = 138, .w = 1115, .h = 271 };
    SDL_Rect clickZoneSelectMap = {719,304,1219,438};
    SDL_Rect clickZoneLeaderboard = {698,473,1241,607};
    SDL_Rect clickZoneOptions = {782,640,1156,774};
    SDL_Rect clickZoneLeave = { .x = 827, .y = 813, .w = 1115, .h = 945 };
    // select map
    SDL_Rect clickZoneMap1 = { .x = 236, .y = 695, .w = 383, .h = 754 };
    SDL_Rect clickZoneMap2 = { .x = 670, .y = 695, .w = 817, .h = 754 };
    SDL_Rect clickZoneSelectBack = {1648,940,1794,999};
    // leaderboard
    // options
    SDL_Rect clickZoneSoundsM = {816,470,850,504};
    SDL_Rect clickZoneSoundsP = {1521,470,1555,504};
    SDL_Rect clickZoneMusicsM = {816,570,850,604};
    SDL_Rect clickZoneMusicsP = {1521,570,1555,604};
    SDL_Rect clickZoneOptionsBack = {1648,940,1794,999};
    // pause
    SDL_Rect clickZoneContinue = {798,356,1123,460};
    SDL_Rect clickZonePauseOptions = {814,488,1106,593};
    SDL_Rect clickZonePauseMenu = {864,620,1057,725};
    // positions du curseur
    SDL_Rect positionCurseur = {910,487,1461,587};
    // initialisation de l'état du jeu
    enum { ACCUEIL = 0, SELECTION, LEADERBOARD, OPTIONS, PAUSE, JEU };
    short int MENU = ACCUEIL;
    short int JEU_TOURNE = 0;
    enum { DEFAULT, MAP1, MAP2};
    short int SELECTED = DEFAULT;
    const short int MAX_VOLUME = 10;
    short int SOUNDS_VOLUME = MAX_VOLUME;
    short int MUSICS_VOLUME = MAX_VOLUME;
    SDL_Rect menu_source;
    choix_page_menu(3, 0, &menu_source);
    SDL_Rect menu_dest = {0,0,TX,TY};
    SDL_Texture* menu_texture = page_accueil;
    SDL_Rect curseur_dest;
    SDL_QueryTexture(curseur,NULL,NULL,&curseur_dest.w,&curseur_dest.h);

    SDL_Rect loading_source;
    SDL_Rect titre_source;

    tmp_surface = SDL_LoadBMP("assets/maps/map.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);
    tmp_surface = SDL_LoadBMP("assets/sprites/delorean.bmp");
    SDL_Texture* car_texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);

    CAMERA cam;
    cam.N_MAX = 10000;
    cam.tableau_z = calloc(cam.N_MAX,sizeof(Z_SPRITE));
    cam.tableau_p = calloc(cam.N_MAX,sizeof(SPRITE_PROJETE));
    cam.position.x = 0.;
    cam.position.y = 0.;
    cam.position.z = 50;
    cam.longitude = 0.;
    cam.latitude = -1.;
    cam.roulis = 0.;
    cam.renderer = renderer;
    cam.tmp_text = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,4000,4000);
    cam.tmp_cible = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,1.5*TX,1.5*TX);
    cam.cible = NULL;
    cam.dimension_cible.x = 0;
    cam.dimension_cible.y = 0;
    cam.dimension_cible.w = TX;
    cam.dimension_cible.h = TY;
    cam.echelle_ecran = 0.025;
    cam.distance_ecran = 10.;
    cam.offset_horizontal = cam.offset_vertical = 0.;

    PLAN_HORIZONTAL plan;
    plan.texture = texture;
    plan.rotation = 0.;
    plan.echelle = 1.;
    plan.position.x = plan.position.z = 0.;
    plan.position.y = 0;
    plan.au_dessus = plan.en_dessous = NULL;
    plan.source.x = plan.source.y = 0;
    plan.source.w = 1600;
    plan.source.h = 3000;

    SCENE scene;
    scene.sprites_tout_en_bas = NULL;
    scene.tout_en_bas = scene.tout_en_haut = &plan;

    SPRITE sprite[50];
    for (unsigned int i=0; i<sizeof(sprite)/sizeof(SPRITE); ++i) {
        sprite[i].texture = car_texture;
        sprite[i].echelle = 2.;
        sprite[i].source.x = sprite[i].source.y = 0;
        sprite[i].source.w = 800;
        sprite[i].source.h = 500;
        sprite[i].position.z = 20.;
        sprite[i].position.x = (15*i)%500;
        sprite[i].position.y = (10*i*i)%500;
        sprite[i].speed = 0;
        sprite[i].max_speed = 4;
    }

    TABLEAU_SPRITES tab_sprites;
    tab_sprites.N = sizeof(sprite)/sizeof(SPRITE);
    tab_sprites.suivant = NULL;
    tab_sprites.sprites = sprite;
    plan.sprites_au_dessus = &tab_sprites;

    VECTEUR2D deplacement_zs,deplacement_qd,deplacement_zs2,deplacement_qd2;
    float speed_coef = 5.;

    /*Car player_car;
    player_car.x = 0;
    player_car.y = 0;
    player_car.texture = car_texture;
    SDL_QueryTexture(car_texture, NULL, NULL, &player_car.rect.w, &player_car.rect.h);
    player_car.rect.x = (int)player_car.x;
    player_car.rect.y = (int)player_car.y;*/

    enum { UP = 0, DOWN, LEFT, RIGHT, Z, Q, S, D, A, E, W, X, O, K, L, M}; 
    short int INPUT[16] = { 0 };
    SDL_Event EVENT;
    int loop = 1;
    long long temps_frame,temps_frame2;
#ifdef MONTRER_FPS
    int compteur_fps = 0;
    long long temps_ecoule_fps = SDL_GetTicks();
#endif


    //Ecran titre
    for (int i = 0; i <= 255; i+=5){
        choix_page_menu(1, 0, &titre_source);
        SDL_SetTextureAlphaMod(titre, i);
        SDL_RenderCopy(renderer, titre, &titre_source, &menu_dest);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    
        //SDL_SetTextureAlphaMod(SDL_Texture, ValeurAlpha);
    }
    while (loop) {
        // gestion des fps (partie 1)
        temps_frame = SDL_GetTicks();
        
        if (MENU < JEU) {
        // gestion des différents menus
            while (SDL_PollEvent(&EVENT) != 0) {
                if (EVENT.type == SDL_QUIT) {
                    loop = 0;
                }
                else if (EVENT.type == SDL_MOUSEBUTTONDOWN) {
                    switch (MENU) {
                        case ACCUEIL:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneStart){
                                choix_page_menu(3, 1, &menu_source);
                            }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneSelectMap)
                                choix_page_menu(3, 2, &menu_source);
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneLeaderboard)
                                choix_page_menu(3, 3, &menu_source);
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneOptions)
                                choix_page_menu( 3, 4, &menu_source);
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneLeave)
                                choix_page_menu(3, 5, &menu_source);
                            break;
                        case SELECTION:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneMap1) {
                                choix_page_menu(4, 4, &menu_source);
                                SELECTED = MAP1; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneMap2) {
                                choix_page_menu(4, 6, &menu_source);
                                SELECTED = MAP2; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneSelectBack)
                                switch (SELECTED) {
                                    case DEFAULT: choix_page_menu(4, 1, &menu_source); break;
                                    case MAP1: choix_page_menu(4, 2, &menu_source); break;
                                    case MAP2: choix_page_menu(4, 3, &menu_source); break; }
                            break;
                        case LEADERBOARD:
                            break;
                        case OPTIONS:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneSoundsM) {
                                choix_page_menu(3,2,&menu_source); 
                                SOUNDS_VOLUME = SOUNDS_VOLUME-1 < 0 ? 0 : SOUNDS_VOLUME-1; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneSoundsP) {
                                choix_page_menu(3,3,&menu_source);
                                SOUNDS_VOLUME = SOUNDS_VOLUME+1 > MAX_VOLUME ? MAX_VOLUME : SOUNDS_VOLUME+1; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneMusicsM) {
                                choix_page_menu(3,4,&menu_source);
                                MUSICS_VOLUME = MUSICS_VOLUME-1 < 0 ? 0 : MUSICS_VOLUME-1; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneMusicsP) {
                                choix_page_menu(3,5,&menu_source);
                                MUSICS_VOLUME = MUSICS_VOLUME+1 > MAX_VOLUME ? MAX_VOLUME : MUSICS_VOLUME+1; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneOptionsBack)
                                choix_page_menu(3,1,&menu_source);
                            break;
                        case PAUSE:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneContinue)
                                choix_page_menu(2,1,&menu_source);
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZonePauseOptions)
                                choix_page_menu(2,2,&menu_source);
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZonePauseMenu)
                                choix_page_menu(2,3,&menu_source);
                            break;
                    }
                }
                else if (EVENT.type == SDL_MOUSEBUTTONUP) {
                    switch (MENU) {
                        case ACCUEIL:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneStart) {
                                //Loading screen
                                for (int i = 0; i <= 10; i++){
                                    choix_page_menu(4, 0, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(100);

                                    choix_page_menu(4, 1, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(100);
                                    
                                    choix_page_menu(4, 2, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(100);

                                    choix_page_menu(4, 3, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(100);
                                    // SDL_RenderCopy la bonne zone de l'image
                                    // SDL_RenderPresent (pour updater la fenetre)
                                    // SDL_Delay(le temps nécessaire)
                                    // répéter
                                }
                                MENU = JEU;
                                JEU_TOURNE = 1;
                                for (unsigned int i=0; i<sizeof(INPUT)/sizeof(short int); ++i) INPUT[i] = 0; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneSelectMap) {
                                switch (SELECTED) {
                                    case DEFAULT: choix_page_menu(4, 0, &menu_source); break;
                                    case MAP1: choix_page_menu(4, 5, &menu_source); break;
                                    case MAP2: choix_page_menu(4, 7, &menu_source); break; }
                                menu_texture = select_map;
                                MENU = SELECTION; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneLeaderboard)
                                choix_page_menu(3, 0, &menu_source);
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneOptions) {
                                choix_page_menu(3, 0, &menu_source);
                                menu_texture = options;
                                MENU = OPTIONS; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneLeave)
                                loop = 0;
                            else
                                choix_page_menu(3, 0, &menu_source);
                            break;
                        case SELECTION:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneSelectBack) {
                                choix_page_menu(3, 0, &menu_source);
                                menu_texture = page_accueil;
                                MENU = ACCUEIL; }
                            else switch (SELECTED) {
                                case DEFAULT: choix_page_menu(4, 0, &menu_source); break;
                                case MAP1: choix_page_menu(4, 5, &menu_source); break;
                                case MAP2: choix_page_menu(4, 7, &menu_source); break; }
                            break;
                        case LEADERBOARD:
                            break;
                        case OPTIONS:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneOptionsBack) {
                                if (JEU_TOURNE) {
                                    choix_page_menu(2,0,&menu_source);
                                    menu_texture = pause;
                                    MENU = PAUSE; }
                                else {
                                    choix_page_menu(3, 0, &menu_source);
                                    menu_texture = page_accueil;
                                    MENU = ACCUEIL; }
                                break;
                            }
                            choix_page_menu(3,0,&menu_source);
                            break;
                        case PAUSE:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneContinue) {
                                MENU = JEU;
                                for (unsigned int i=0; i<sizeof(INPUT)/sizeof(short int); ++i) INPUT[i] = 0; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZonePauseOptions) {
                                choix_page_menu(3,0,&menu_source);
                                menu_texture = options;
                                MENU = OPTIONS; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZonePauseMenu) {
                                choix_page_menu(3,0,&menu_source);
                                menu_texture = page_accueil;
                                MENU = ACCUEIL; }
                            else
                                choix_page_menu(2,0,&menu_source);
                            break;
                    }
                }
            }

            SDL_RenderClear(renderer);
            switch (MENU) {
                case PAUSE:
                    AFFICHAGE_CAMERA(&cam, &scene);
                    SDL_RenderCopy(renderer,menu_texture,&menu_source,&menu_dest);
                    break;
                case OPTIONS:
                    SDL_RenderCopy(renderer,menu_texture,&menu_source,&menu_dest);
                    curseur_dest.x = positionCurseur.x + (positionCurseur.w - positionCurseur.x)*((float)SOUNDS_VOLUME)/MAX_VOLUME - curseur_dest.w/2;
                    curseur_dest.y = positionCurseur.y - curseur_dest.h/2;
                    SDL_RenderCopy(renderer,curseur,NULL,&curseur_dest);
                    curseur_dest.x = positionCurseur.x + (positionCurseur.w - positionCurseur.x)*((float)MUSICS_VOLUME)/MAX_VOLUME - curseur_dest.w/2;
                    curseur_dest.y = positionCurseur.h - curseur_dest.h/2;
                    SDL_RenderCopy(renderer,curseur,NULL,&curseur_dest);
                    break;
                default:
                    SDL_RenderCopy(renderer,menu_texture,&menu_source,&menu_dest);
                    break;
            }
            SDL_RenderPresent(renderer);
        }
        else {
        // le jeu est lancé
            while (SDL_PollEvent(&EVENT) != 0) {
                if (EVENT.type == SDL_QUIT) {
                    loop = 0;
                } else if (EVENT.type == SDL_KEYDOWN) {
                    switch (EVENT.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            MENU = PAUSE;
                            menu_texture = pause;
                            choix_page_menu(2,0,&menu_source);
                            break;
                        case SDLK_UP:
                            INPUT[UP] = 1;
                            break;
                        case SDLK_DOWN:
                            INPUT[DOWN] = 1;
                            break;
                        case SDLK_LEFT:
                            INPUT[LEFT] = 1;
                            break;
                        case SDLK_RIGHT:
                            INPUT[RIGHT] = 1;
                            break;
                        case SDLK_z:
                            INPUT[Z] = 1;
                            break;
                        case SDLK_q:
                            INPUT[Q] = 1;
                            break;
                        case SDLK_s:
                            INPUT[S] = 1;
                            break;
                        case SDLK_d:
                            INPUT[D] = 1;
                            break;
                        case SDLK_a:
                            INPUT[A] = 1;
                            break;
                        case SDLK_e:
                            INPUT[E] = 1;
                            break;
                        case SDLK_w:
                            INPUT[W] = 1;
                            break;
                        case SDLK_x:
                            INPUT[X] = 1;
                            break;
                        case SDLK_o:
                            INPUT[O] = 1;
                            break;
                        case SDLK_k:
                            INPUT[K] = 1;
                            break;
                        case SDLK_l:
                            INPUT[L] = 1;
                            break;
                        case SDLK_m:
                            INPUT[M] = 1;
                            break;
                        default:
                            break;
                    }
                } else if (EVENT.type == SDL_KEYUP) {
                    switch (EVENT.key.keysym.sym) {
                        case SDLK_UP:
                            INPUT[UP] = 0;
                            break;
                        case SDLK_DOWN:
                            INPUT[DOWN] = 0;
                            break;
                        case SDLK_LEFT:
                            INPUT[LEFT] = 0;
                            break;
                        case SDLK_RIGHT:
                            INPUT[RIGHT] = 0;
                            break;
                        case SDLK_z:
                            INPUT[Z] = 0;
                            break;
                        case SDLK_q:
                            INPUT[Q] = 0;
                            break;
                        case SDLK_s:
                            INPUT[S] = 0;
                            break;
                        case SDLK_d:
                            INPUT[D] = 0;
                            break;
                        case SDLK_a:
                            INPUT[A] = 0;
                            break;
                        case SDLK_e:
                            INPUT[E] = 0;
                            break;
                        case SDLK_w:
                            INPUT[W] = 0;
                            break;
                        case SDLK_x:
                            INPUT[X] = 0;
                            break;
                        case SDLK_o:
                            INPUT[O] = 0;
                            break;
                        case SDLK_k:
                            INPUT[K] = 0;
                            break;
                        case SDLK_l:
                            INPUT[L] = 0;
                            break;
                        case SDLK_m:
                            INPUT[M] = 0;
                            break;
                        default:
                            break;
                    }
                }
            }

            if (INPUT[UP]) cam.latitude += 0.05;
            if (INPUT[DOWN]) cam.latitude -= 0.05;
            if (INPUT[LEFT]) cam.longitude += 0.05;
            if (INPUT[RIGHT]) cam.longitude -= 0.05;
            if (INPUT[W]) plan.rotation += 0.05;
            if (INPUT[X]) plan.rotation -= 0.05;
            cam.latitude = cam.latitude > M_PI / 2 ? M_PI / 2 : cam.latitude; //limite la latitude
            cam.latitude = cam.latitude < -M_PI / 2 ? -M_PI / 2 : cam.latitude; //limite la latitude
            sincosf(cam.longitude,&deplacement_qd.y,&deplacement_qd.x);
            deplacement_qd.x *= speed_coef;     //déplacement de la caméra
            deplacement_qd.y *= speed_coef;
           
            deplacement_zs.x = -deplacement_qd.y;
            deplacement_zs.y = deplacement_qd.x;

            deplacement_qd2.x = deplacement_qd.x;
            deplacement_qd2.y = deplacement_qd.y;
            deplacement_qd2.x *= sprite[0].speed/2; //déplacement du joueur
            deplacement_qd2.y *= sprite[0].speed/2;

            deplacement_zs2.x = -deplacement_qd2.y;
            deplacement_zs2.y = deplacement_qd2.x;


            if (INPUT[Z]) {cam.position.x += deplacement_zs.x; cam.position.y += deplacement_zs.y;}
            if (INPUT[S]) {cam.position.x -= deplacement_zs.x; cam.position.y -= deplacement_zs.y;}
            if (INPUT[D]) {cam.position.x += deplacement_qd.x; cam.position.y += deplacement_qd.y;}
            if (INPUT[Q]) {cam.position.x -= deplacement_qd.x; cam.position.y -= deplacement_qd.y;}
            if (INPUT[E]) cam.position.z += speed_coef;
            if (INPUT[A]) cam.position.z -= speed_coef;

            cam.position.x += deplacement_zs2.x; cam.position.y += deplacement_zs2.y; //déplacements du joueur synchronisé avec la caméra
            sprite[0].position.x += deplacement_zs2.x;sprite[0].position.y += deplacement_zs2.y;

            if (INPUT[O] && sprite[0].speed < sprite[0].max_speed && (SDL_GetTicks() - temps_ecoule_fps >= 995)) {sprite[0].speed += 1;printf("Speed up\n");}
            if (INPUT[L] && sprite[0].speed > 0 && (SDL_GetTicks() - temps_ecoule_fps >= 995)) {sprite[0].speed -= 1;}
            if (INPUT[M]) {cam.longitude -= 0.02;}
            if (INPUT[K]) {cam.longitude += 0.02;}

            SDL_RenderClear(renderer);
            AFFICHAGE_CAMERA(&cam, &scene);
            SDL_RenderPresent(renderer);
        }

        // gestion des FPS partie 2
#ifdef MONTRER_FPS
        compteur_fps++;
        if (SDL_GetTicks() - temps_ecoule_fps > 1000) {
            printf("FPS: %d\n",compteur_fps);
            temps_ecoule_fps = SDL_GetTicks();
            compteur_fps = 0;
        }
#endif
        while ((temps_frame2 = SDL_GetTicks() - temps_frame) <= DUREE_FRAME) SDL_Delay(DUREE_FRAME - temps_frame2);
    }

    free(cam.tableau_p);
    free(cam.tableau_z);
    SDL_DestroyTexture(select_map);
    SDL_DestroyTexture(page_accueil);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(car_texture);
    SDL_DestroyTexture(cam.tmp_text);
    SDL_DestroyTexture(cam.tmp_cible);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
