#include "main.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
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
    tmp_surface = SDL_LoadBMP("assets/menu/game_over.bmp");
    SDL_Texture* game_over = SDL_CreateTextureFromSurface(renderer,tmp_surface);
    SDL_FreeSurface(tmp_surface);
    // pour l'interactivité avec les boutons du menu
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
    // game_over
    SDL_Rect clickZoneRestart = {814,782,1106,887};
    SDL_Rect clickZoneGOMenu = {864,915,1057,1020};
    // positions du curseur
    SDL_Rect positionCurseur = {910,487,1461,587};
    // initialisation de l'état du jeu
    short int SELECTED = DEFAULT;
    short int MENU = ACCUEIL;
    short int JEU_TOURNE = 0;
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
    //Chargement des textures des maps/véhicules/autres...
    SDL_Texture* TEXTURES[sizeof(TEXTURE_FILES)/sizeof(char*)];
    for (unsigned short int i=0; i<sizeof(TEXTURE_FILES)/sizeof(char*); ++i) {
        tmp_surface = SDL_LoadBMP(TEXTURE_FILES[i]);
        TEXTURES[i] = SDL_CreateTextureFromSurface(renderer, tmp_surface);
        SDL_SetTextureBlendMode(TEXTURES[i],SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(tmp_surface);
    }

    // initialisation ttf font
    TTF_Init();
    // chargement police
    TTF_Font* font48 = TTF_OpenFont("assets/police/game_over.ttf", 120);
    if (!font48) {
        printf("Erreur: impossible de charger la police de caractères.\n");
        return 1;
    }

    // initialisation sdl mixer
    Mix_Init(FORMATS_AUDIO);
    if (Mix_OpenAudio(FREQ_ECHANT, MIX_DEFAULT_FORMAT, 1, T_AUDIO_BUFFER) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Erreur initialisation SDL_mixer : %s", Mix_GetError());
        SDL_Quit();
    }
    // allocations channels (+1 pour la musique)
    Mix_AllocateChannels(NB_CHANNELS+1);
    // initialisation du volume
    Mix_Volume(0, MIX_MAX_VOLUME*VOL_INIT_MUS);
    for (unsigned short int i=1; i<=NB_CHANNELS; ++i) Mix_Volume(i, MIX_MAX_VOLUME*VOL_INIT_CHAN[i]);
    // chargement des sons
    Mix_Chunk* SONS[sizeof(SON_FILES)/sizeof(char*)];
    for (unsigned short int i=0; i<sizeof(SON_FILES)/sizeof(char*); ++i)
        SONS[i] = Mix_LoadWAV(SON_FILES[i]);

    // initialisation du monde physique
    CONTEXTE_SDL contexte;
    contexte.param_cam.renderer = renderer;
    contexte.param_cam.tmp_text = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,8000,4000);
    SDL_SetTextureBlendMode(contexte.param_cam.tmp_text,SDL_BLENDMODE_BLEND);
    contexte.param_cam.tmp_cible = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,1.5*TX,1.5*TX);
    SDL_SetTextureBlendMode(contexte.param_cam.tmp_cible,SDL_BLENDMODE_BLEND);
    contexte.param_cam.dimension_cible.x = contexte.param_cam.dimension_cible.y = 0;
    contexte.param_cam.dimension_cible.w = TX;
    contexte.param_cam.dimension_cible.h = TY;
    contexte.param_cam.cible = NULL;
    contexte.param_cam.N_MAX = 10000;
    contexte.tableau_textures = TEXTURES;
    contexte.police = font48;
    contexte.nb_channels = NB_CHANNELS;
    contexte.tableau_sons = SONS;
    contexte.minimap_rect = MINIMAP_RECT;
    contexte.timer_position.x = contexte.timer_position.y = 10;
    contexte.timer_color = TIMER_COLOR;
    MONDE_PHYSIQUE monde;

    // initialisation de la boucle principale
    short int INPUT[16] = { 0 };
    SDL_Event EVENT;
    int loop = 1;
    long long temps_frame,temps_frame2;
    long long mesure_dt = -1;
    long long mesure_dt2;
#ifdef MONTRER_FPS
    int compteur_fps = 0;
    long long temps_ecoule_fps = SDL_GetTicks();
#endif

    //Ecran titre
    /*for (int i = 0; i <= 255; i+=5){
        choix_page_menu(1, 0, &titre_source);
        SDL_SetTextureAlphaMod(titre, i);
        SDL_RenderCopy(renderer, titre, &titre_source, &menu_dest);
        SDL_RenderPresent(renderer);
        SDL_Delay(70);
    }*/

    // Jouer la musique indéfiniment
    Mix_PlayChannel(0, SONS[0], -1);

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
                        case GAME_OVER:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneRestart)
                                choix_page_menu(2,1,&menu_source);
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneGOMenu)
                                choix_page_menu(2,2,&menu_source);
                            break;
                    }
                }
                else if (EVENT.type == SDL_MOUSEBUTTONUP) {
                    switch (MENU) {
                        case ACCUEIL:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneStart) {
                                //Loading screen
                                /*for (int i = 0; i <= 10; i++){
                                    choix_page_menu(4, 0, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(50);

                                    choix_page_menu(4, 1, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(50);
                                    
                                    choix_page_menu(4, 2, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(50);

                                    choix_page_menu(4, 3, &loading_source);
                                    SDL_RenderCopy(renderer, chargement, &loading_source, &menu_dest);
                                    SDL_RenderPresent(renderer);
                                    SDL_Delay(50);
                                }*/
                                if (SELECTED == MAP2) Charger_Monde_Physique(&monde,&lvl_neon_city,&contexte);
                                else Charger_Monde_Physique(&monde,&lvl_ferme,&contexte);
                                MENU = JEU;
                                JEU_TOURNE = 1;
                                mesure_dt = -1;
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
                                for (unsigned int i=0; i<sizeof(INPUT)/sizeof(short int); ++i) INPUT[i] = 0; 
                                mesure_dt = -1; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZonePauseOptions) {
                                choix_page_menu(3,0,&menu_source);
                                menu_texture = options;
                                MENU = OPTIONS; }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZonePauseMenu) {
                                choix_page_menu(3,0,&menu_source);
                                menu_texture = page_accueil;
                                MENU = ACCUEIL; 
                                JEU_TOURNE = 0; 
                                Decharger_Monde_Physique(&monde);}
                            else
                                choix_page_menu(2,0,&menu_source);
                            break;
                        case GAME_OVER:
                            if EST_DANS_CLICKZONE(EVENT.button,clickZoneRestart) {
                                if (SELECTED == MAP2) Charger_Monde_Physique(&monde,&lvl_neon_city,&contexte);
                                else Charger_Monde_Physique(&monde,&lvl_ferme,&contexte);
                                MENU = JEU;
                                JEU_TOURNE = 1;
                                mesure_dt = -1;
                                for (unsigned int i=0; i<sizeof(INPUT)/sizeof(short int); ++i) { INPUT[i] = 0; }
                            }
                            else if EST_DANS_CLICKZONE(EVENT.button,clickZoneGOMenu) {
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
                    Afficher_Monde_Physique(&monde);
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

            mesure_dt2 = SDL_GetTicks();
            if (mesure_dt < 0) mesure_dt = mesure_dt2;
            long long int score = Calculer_Monde_Physique(&monde, INPUT, mesure_dt2 - mesure_dt); // calcule du monde physique
            if (score > 0) {
                // fin du jeu (menu game over)
                Decharger_Monde_Physique(&monde);
                choix_page_menu(2,0,&menu_source);
                menu_texture = game_over;
                MENU = GAME_OVER; 
                JEU_TOURNE = 0;
                continue;
            }
            mesure_dt = mesure_dt2;

            SDL_RenderClear(renderer);
            Afficher_Monde_Physique(&monde);
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

    // tout décharger
    if (JEU_TOURNE)
        Decharger_Monde_Physique(&monde);
    for (unsigned short int i=0; i<sizeof(TEXTURE_FILES)/sizeof(char*); ++i)
        SDL_DestroyTexture(TEXTURES[i]);
    Mix_HaltChannel(-1);
    for (unsigned short int i=0; i<sizeof(SON_FILES)/sizeof(char*); ++i)
        Mix_FreeChunk(SONS[i]);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_CloseFont(font48);
    TTF_Quit();
    SDL_DestroyTexture(game_over);
    SDL_DestroyTexture(titre);
    SDL_DestroyTexture(chargement);
    SDL_DestroyTexture(select_map);
    SDL_DestroyTexture(page_accueil);
    SDL_DestroyTexture(options);
    SDL_DestroyTexture(pause);
    SDL_DestroyTexture(curseur);
    SDL_DestroyTexture(contexte.param_cam.tmp_text);
    SDL_DestroyTexture(contexte.param_cam.tmp_cible);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
