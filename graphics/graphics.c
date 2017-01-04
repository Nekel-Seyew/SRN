#include "graphics.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "../jsmn/jsmn.h"
#include "../asprintf.c/asprintf.h"
#include "../Collections-C/src/array.h"

static struct graphics_settings global_settings;

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 1;
	}
	return 0;
}

void init_graphics(char* json){
	if(settings != NULL){
		settings_from_json(&global_settings,json);
		return;
	}else{
		//struct graphics_settings* ret = 
		global_settings.border.up_left  = '+';
		global_settings.border.up_right = '+';
		global_settings.border.dn_left  = '+';
		global_settings.border.dn_right = '+';
		global_settings.border.down     = '-';
		global_settings.border.up       = '-';
		global_settings.border.left     = '|';
		global_settings.border.right    = '|';
		global_settings.color           =  7 ;
		global_settings.width           =  80;
		global_settings.height          =  24;
		global_settings.color_off       =  0 ;
	}
}

void init_ncurses(){
	//printf("%d\n",COLOR_PAIRS);
	int cresp;
	initscr();
	keypad(stdscr, 1);
	if(has_colors()){
		start_color();
		int i=1, j=1;
		for(i=0; i<8; ++i){
			for(j=0; j<8; ++j){
				cresp = init_pair(8*i+j+1,j,i);
				assert(cresp == OK);//making sure it's fine
			}
		}
	}
	noecho();
	raw();
	timeout(0);
	curs_set(0);
}
void draw_char(WINDOW* win, char a, int color, int x, int y){
	if(has_colors()){
		wattron(win,COLOR_PAIR(color));
	}
	mvwprintw(win,y,x,"%c",a);
	if(has_colors()){
		wattroff(win,COLOR_PAIR(color));
	}
}
void draw_sprite(WINDOW* win, struct sprite* spr, int x, int y){
	int i=0, j=0;
	int color=0;
	char a;
	for(i=0; i<spr->height; ++i){
		for(j=0; j<spr->width; ++j){
			color = spr->color[spr->width*i+j];
			a = spr->sprite[spr->width*i+j];
			if(has_colors()){
				wattron(win,COLOR_PAIR(color));
				mvwprintw(win,y+i,x+j,"%c",a);
				wattroff(win,COLOR_PAIR(color));
			}else{
				mvwprintw(win,y+i,x+j,"%c",a);
			}
		}
	}
}
void draw_window(WINDOW* win, int x, int y, struct graphics_settings* stng){
	struct graphics_settings* settings =((stng==NULL) ? &global_settings : stng);
	int i=0;
	int j=0;
	if(!settings->color_off){
		wattron(win,COLOR_PAIR(settings->color));
	}
	mvwin(win,y,x);
	//4 corners
	mvwprintw(win,0,0,"%c",settings->border.up_left);
	mvwprintw(win,0,settings->width-1,"%c",settings->border.up_right);
	mvwprintw(win,settings->height-1,0,"%c",settings->border.dn_left);
	mvwprintw(win,settings->height-1,settings->width-1,"%c",settings->border.dn_right);
	//top,bottom
	for(i=0; i<settings->width; i++){
		mvwprintw(win,0,i,"%c",settings->border.up);
		mvwprintw(win,settings->height-1,i,"%c",settings->border.down);
	}
	//left,right
	for(i=0; i<settings->height; i++){
		mvwprintw(win,i,0,"%c",settings->border.left);
		mvwprintw(win,i,settings->width-1,"%c",settings->border.right);
	}	
	if(!settings->color_off){
		wattroff(win,COLOR_PAIR(settings->color));
	}
}

char* sprite_to_JSON(struct sprite* spr){
	int len = spr->height*spr->width;
	//"{"height":num,"width":num,"sprite":"SPRITE SIZE OF H*W","color":[H*W]}
	char* ret = malloc(sizeof(char)*(49+len+1)+sizeof(char)*4*len);
	memset(ret,0,sizeof(char)*(49+2*(len))+1);
	char h[4], w[4];
	h[3]=0; w[3]=0;
	sprintf(h,"%03d",spr->height);
	sprintf(w,"%03d",spr->width);
	char* hc = "\"height\":";
	char* wc = "\"width\":";
	char* sc = "\"sprite\":";
	char* cc = "\"color\":";
	int hcl = strlen(hc);
	int wcl = strlen(wc);
	int scl = strlen(sc);
	int ccl = strlen(cc);

	ret[0] = '{';
	memcpy(ret+1,hc,hcl);
	memcpy(ret+1+hcl,h,3);
	ret[1+hcl+3] = ',';
	memcpy(ret+1+hcl+3+1,wc,wcl);
	memcpy(ret+1+hcl+3+1+wcl,w,3);
	ret[1+hcl+3+1+wcl+3] = ',';
	memcpy(ret+1+hcl+3+1+wcl+3+1,sc,scl);
	ret[1+hcl+3+1+wcl+3+1+scl] = '"';
	memcpy(ret+1+hcl+3+1+wcl+3+1+scl+1,spr->sprite,len*sizeof(char));
	ret[1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))] = '"';
	ret[1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1] = ',';
	memcpy(ret+1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1+1,cc,ccl);
	ret[1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1+1+ccl] = '[';
	//need to copy over everything.
	int k =0;
	for(k=0; k<len; ++k){
		sprintf(h,"%03d",spr->color[k]);
		h[3]=',';
		memcpy(ret+1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1+1+ccl+1+(k*4*sizeof(char)),h,4);
	}
		ret[1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1+1+ccl+1+(len*sizeof(char)*4)-1]=' ';
	//memcpy(ret+1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1+1+ccl+1,spr->color,len*sizeof(char)*3);
	ret[1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1+1+ccl+1+(len*sizeof(char)*4)] = ']';
	ret[1+hcl+3+1+wcl+3+1+scl+1+(len*sizeof(char))+1+1+ccl+1+(len*sizeof(char)*4)+1] = '}';
	return ret;
}

void sprite_from_JSON(struct sprite* spr, char* jsn){
	//jsn="{"height":num,"width":num,"sprite":"SPRITE SIZE OF H*W","color":[H*W]}
	jsmn_parser parser;
	jsmntok_t tok[300];
	jsmn_init(&parser);
	//ArrayConf vc;
	//array_conf_init(&vc);
	//vc.capacity = 100;
	//Array* wa;
	//array_new_conf(&vc, &wa);
	int r;
	char* work;
	r = jsmn_parse(&parser, jsn, strlen(jsn), tok, sizeof(tok)/sizeof(tok[0]));
	if(r < 1 || tok[0].type != JSMN_OBJECT){
		//BAD
	}
	int i;
	int j;
	for(i=0; i<r; i++){
		if(jsoneq(jsn, &tok[i], "height")){
			//asprintf(&work,"%.*s",tok[i+1].end-tok[i+1].start,jsn+tok[i+1].start);
			//spr->height = atoi(work);
			//free(work);
			spr->height = strtol(jsn+tok[i+1].start,NULL,10);
			i++;
		}else if(jsoneq(jsn, &tok[i],"width")){
			//asprintf(&work,"%.*s",tok[i+1].end-tok[i+1].start, jsn+tok[i+1].start);
			//spr->width=atoi(work);
			//free(work);
			spr->width = strtol(jsn+tok[i+1].start,NULL,10);
			i++;
		}else if(jsoneq(jsn, &tok[i],"color")){
			if(tok[i+1].type != JSMN_ARRAY){
				//BAD
			}
			spr->color=malloc(sizeof(int)*tok[i+1].size);
			for(j=0; j<tok[i+1].size; j++){
				jsmntok_t* g = &tok[i+j+2];
				int val = strtol(jsn+g->start,NULL,10);
				spr->color[j] = val;
				//array_add(wa,(void*)val);//gross, but ints fit inside void*
			}
			//int cap = array_capacity(wa);
			//spr->color=malloc(sizeof(int)*cap);
			//for(j=0; j<cap; j++){
			//	array_get_at(wa,j,(void*)&(spr->color[j]));
			//}
			i++;
		}else if(jsoneq(jsn, &tok[i],"sprite")){
			spr->sprite = strndup(jsn+tok[i+1].start, tok[i+1].end-tok[i+1].start);
			i++;
		}
	}
	//array_destroy(wa);

}

void settings_from_JSON(struct graphics_settings* st, char* jsn){
}
void settings_to_JSON(struct graphics_settings* st){
}

