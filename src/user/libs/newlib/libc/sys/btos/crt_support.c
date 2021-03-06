#include <btos.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "crt_support.h"

#define ARGS_MAX 4096
#define ARGC_MAX 512

static char argsbuffer[ARGS_MAX]={0};
static char *argsptr[ARGC_MAX]={0};

static virtual_handle **filehandles=NULL;
static size_t fh_count=0;

struct dup{
	virtual_handle *vh;
	size_t count;
	bool pend_close;
};

static struct dup **dups = NULL;
static size_t dups_count = 0;

static bool std_streams_open = false;

static virtual_handle null_virt_handle = {HANDLE_NULL};

virtual_handle *btos_dup(virtual_handle *vh){
	size_t idx = -1;
	size_t zidx = -1;
	for(size_t i = 0; i < dups_count; ++i){
		if(!dups[i]) zidx = i;
		else if(dups[i]->vh == vh){
			idx = i;
			break;
		}
	}
	if(idx == (size_t)-1){
		if(zidx == (size_t)-1){
			idx = dups_count++;
			dups = realloc(dups, dups_count * sizeof(struct dup*));
		}else{
			idx = zidx;
		}
		dups[idx] = malloc(sizeof(struct dup));
		dups[idx]->vh = vh;
		dups[idx]->count = 0;
		dups[idx]->pend_close = false;
	}
	dups[idx]->count++;
	virtual_handle *ret = malloc(sizeof(virtual_handle));
	ret->dup.parent = vh;
	ret->type = HANDLE_DUP;
	return ret;
}

int btos_close(virtual_handle *vh){
	if(vh->type == HANDLE_NULL){
    	return 0;
    }else if(vh->type != HANDLE_DUP){
    	bool can_close = true;
		for(size_t i = 0; i < dups_count; ++i){
			if(dups[i] && dups[i]->vh == vh){
				can_close = false;
				dups[i]->pend_close = true;
				break;
			}
		}
		if(can_close){
		    if(vh->type == HANDLE_OS){
				bt_handle_t h = vh->os.handle;
				int ret=bt_fclose(h);
				return ret;
			}else{
				int ret=vh->virt.close(vh->virt.data);
				return ret;
			}
		}else{
			return 0;
		}
    }else{
    	for(size_t i = 0; i < dups_count; ++i){
    		if(dups[i]->vh == vh->dup.parent){
    			dups[i]->count--;
    			if(dups[i]->count == 0){
    				struct dup *ptr = dups[i];
    				dups[i] = NULL;
    				if(ptr->pend_close) btos_close(ptr->vh);
    				free(ptr);
    			}
    			return 1;
    		}
    	}
    }
    return 0;
}

static void *memdup(void *ptr, size_t size){
	void *ret = malloc(size);
	memcpy(ret, ptr, size);
	return ret;
}

void btos_open_std_streams(){
	if(std_streams_open) return;
    char buffer[BT_MAX_PATH];
    size_t s=bt_getenv("STDIN", buffer, BT_MAX_PATH);
    if(s){
        bt_handle input=bt_fopen(buffer, FS_Read);
        btos_set_specific_filenum(0, input, buffer);
    }else{
    	btos_set_specific_filenum_virt(0, &null_virt_handle);
    }
    memset(buffer, 0, BT_MAX_PATH);
    s=bt_getenv("STDOUT", buffer, BT_MAX_PATH);
    if(s){
        bt_handle output=bt_fopen(buffer, FS_Write | FS_AtEnd);
        btos_set_specific_filenum(1, output, buffer);
    }else{
    	btos_set_specific_filenum_virt(1, &null_virt_handle);
    }
    memset(buffer, 0, BT_MAX_PATH);
    s=bt_getenv("STDERR", buffer, BT_MAX_PATH);
    if(s){
        bt_handle output=bt_fopen(buffer, FS_Write | FS_AtEnd);
        btos_set_specific_filenum(2, output, buffer);
    }else{
    	btos_set_specific_filenum_virt(2, &null_virt_handle);
    }
    std_streams_open = true;
}

int btos_set_filenum_virt(virtual_handle *vh){
    size_t i;
    for(i=0; i<fh_count; ++i){
        if(filehandles[i] == NULL){
            filehandles[i]=vh;
            return (int)i;
        }
    }
    ++fh_count;
    filehandles=realloc(filehandles, fh_count * sizeof(virtual_handle*));
    filehandles[fh_count-1]=vh;
    return (int)fh_count-1;
}

int btos_set_filenum(bt_handle fh, const char *path){
	virtual_handle *vh = calloc(1, sizeof(virtual_handle));
	vh->type = HANDLE_OS;
	vh->os.handle = fh;
	strncpy(vh->os.path, path, BT_MAX_PATH);
	return btos_set_filenum_virt(vh);
}

virtual_handle *btos_get_handle_virt(int fd){
	btos_open_std_streams();
    if((size_t)fd < fh_count) return filehandles[fd];
    else return 0;
}

bt_handle btos_get_handle(int fd){
	btos_open_std_streams();
    if((size_t)fd < fh_count){
		if(filehandles[fd] && filehandles[fd]->type == HANDLE_OS) return filehandles[fd]->os.handle;
		else return 0;
	}
    else return 0;
}

void btos_set_specific_filenum_virt(int fd, virtual_handle *vh){
    if((size_t)fd < fh_count) filehandles[fd]=vh;
    else{
        size_t oldsize=fh_count;
        fh_count=fd+1;
        filehandles=realloc(filehandles, fh_count * sizeof(virtual_handle*));
        memset(&filehandles[oldsize], 0, (fh_count-oldsize) * sizeof(virtual_handle*));
        filehandles[fd]=vh;
    }
}

void btos_set_specific_filenum(int fd, bt_handle_t fh, const char *path){
	virtual_handle *vh = calloc(1, sizeof(virtual_handle));
	vh->type = HANDLE_OS;
	vh->os.handle = fh;
	strncpy(vh->os.path, path, BT_MAX_PATH);
	btos_set_specific_filenum_virt(fd, vh);
}

void btos_remove_filenum(int fd){
    if((size_t)fd < fh_count){
		free(filehandles[fd]);
		filehandles[fd] = 0;
	}
}

size_t get_argc(){
	return bt_get_argc();
}

char **get_argv(){
	size_t bufferused=0;
	size_t argc=get_argc();
	if(argc >  ARGC_MAX) argc=ARGC_MAX;
	size_t i;
	for(i=0; i < argc; ++i){
		if(bufferused >= ARGS_MAX) break;
		size_t psize=bt_get_arg(i, &argsbuffer[bufferused], ARGS_MAX-bufferused);
		argsptr[i]=&argsbuffer[bufferused];
		bufferused+=psize+1;
	}
	return argsptr;
}

bool btos_path_is_root(const char *path){
	if(!path) return false;
	size_t len = strlen(path);
	if(len < 3) return false;
	if(path[len - 1] == '/' && path[len - 2] == ':') return true;
	else return false;
}

bool btos_path_is_absolute(const char *path){
	if(!path) return false;
	const char *c;
	for(c=path; *c; ++c){
		if(*c==':') return true;
		if(*c=='/') return false;
	}
	return false;
}

size_t btos_get_drive_end(const char *path){
	size_t i;
	for(i=0; i<strlen(path); ++i){
		if(path[i]==':') return i;
	}
	return 0;
}

bool btos_path_parse(const char *opath, char *buffer, size_t size){
	char *path = calloc(BT_MAX_PATH, 1);
	if(!path) return false;
	if(btos_path_is_absolute(opath)){
		strncpy(path, opath, BT_MAX_PATH);
	}else{
		if(opath[0]=='/'){
			char cwd[BT_MAX_PATH];
			bt_getenv("CWD", cwd, BT_MAX_PATH);
			size_t drive_end=btos_get_drive_end(cwd);
			strncpy(path, cwd, drive_end);
			strncpy(&path[strlen(path)], ":", BT_MAX_PATH-strlen(path));
			strncpy(&path[strlen(path)], opath, BT_MAX_PATH-strlen(path));
		}else{
			bt_getenv("CWD", path, BT_MAX_PATH);
			strncpy(&path[strlen(path)], "/", BT_MAX_PATH-strlen(path));
			strncpy(&path[strlen(path)], opath, BT_MAX_PATH-strlen(path));
		}
	}
	char *c;
	bool has_drive=false;
	memset(buffer, 0, size);
	size_t bpos=0, ipos=0, cstart=0;
	for(c=path; (ipos==0 || *(c - 1)) && bpos < size; ++c, ++ipos){
		if(*c==':'){
			if(!has_drive && ipos){
				buffer[bpos++]=':';
				buffer[bpos++]='/';
				cstart=bpos;
				has_drive=true;
			}else{
				free(path);
				return false;
			}
		}else if(*c=='/' || *c=='\n' || *c=='\0'){
			char cstr[BT_MAX_SEGLEN]={0};
			if((int)(bpos-cstart) > 0) strncpy(cstr, &buffer[cstart], bpos-cstart);
			if(strlen(cstr) && strcmp(cstr, "..")==0){
				buffer[--bpos]='\0';
				buffer[--bpos]='\0';
				if(bpos-2 > 0 && buffer[bpos-2] != ':'){
					buffer[--bpos]='\0';
					while(bpos && buffer[bpos]!='/') buffer[bpos--]='\0';
					buffer[bpos++]='/';
				}
				cstart=bpos;
			}else if(strlen(cstr) && strcmp(cstr, ".")==0){
				buffer[--bpos]='\0';
				cstart=bpos;
			}else if(strlen(cstr) && *c!='\n' && *c!='\0'){
				buffer[bpos++]='/';
				cstart=bpos;
			}
		}else buffer[bpos++]=*c;
	}
	if(bpos > 1 && buffer[bpos-1]=='/' && buffer[bpos-2] != ':') buffer[bpos-1]='\0';
	free(path);
	return true;
}
